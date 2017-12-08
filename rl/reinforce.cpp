#include "reinforce.h"

// Inspired by:
// https://gist.github.com/karpathy/a4166c7fe253700972fcbc77e4ea32c5
// https://github.com/pytorch/examples/blob/master/reinforcement_learning/reinforce/py
// See Karpathy Blog and Sutton Book for theory

REINFORCE::REINFORCE(int obsSpace, int actionChoices)
{
    arma_rng::set_seed_random();  // set the seed to a random value
    // random initialization for the weights. all zeros intialization for the biases
    fmat W1 = randn<fmat>(obsSpace, H) / sqrt(H);
    fmat b1 = zeros<frowvec>(H);
    fmat W2 = randn<fmat>(H, actionChoices) / sqrt(H);
    fmat b2 = zeros<frowvec>(actionChoices);
    params["W1"] = W1;
    params["b1"] = b1;
    params["W2"] = W2;
    params["b2"] = b2;

    adamCount = 1;
    // create gradient and adamCache element for every parameter (used to train network)
    for (auto it=params.begin(); it!=params.end(); ++it) {
        string name = it->first;
        fmat param = it->second;

        fmat grad =  zeros<fmat>(size(param));
        fmat m =  zeros<fmat>(size(param));
        fmat v =  zeros<fmat>(size(param));
        gradients[name] = grad;
        adamMCache[name] = m;
        adamVCache[name] = v;
    }
}

frowvec REINFORCE::softmax_(frowvec logp) {
    logp -= logp.max(); // subtract max for numerical stability (exp can be very large)
    frowvec p = exp(logp) / sum(exp(logp));
    return p;
}

int REINFORCE::sample_(frowvec probs) {
    std::random_device rd;
    std::mt19937 gen(rd());

    vector<float> fprobs = conv_to<vector<float>>::from(probs);
    //vector<int> iprobs(fprobs.begin(), fprobs.end());
    std::discrete_distribution<int> d(fprobs.begin(), fprobs.end());

    return d(gen);
}



fvec REINFORCE::calculateReturns_(fvec rewards) {
    // See Sutton Book for math
    // Calculating discounted returns
    // $G_t = r_{t+1} + \gamma*r_{t+2} + \gamma^2*r_{t=3} + ....$
    // $G_i = r_{i+1} + \gamma * G_{i+1}$

    fvec returns(rewards);
    returns.zeros();

    // nextReturn is the return of the next state. It is used in the recursive calculation of
    // return (if you know the current reward at state i and the return at state i+1, you can
    // calculate the return at state i. read that again)
    float nextReturn = 0.0; // starts at 0 because we start at the last state (there's no return after we are done)

    for (int i = (int) rewards.size()-1; i >= 0; i--) {
        nextReturn = rewards[i] + GAMMA*nextReturn;
        returns[i] = nextReturn;
    }

    // normalize returns (this tends to help the network train (something something statistics))
    returns = (returns - mean(returns)) / (stddev(returns));
    return returns;
}

void REINFORCE::zeroGradients() {
    for (auto it=params.begin(); it!=params.end(); ++it) {
        string name = it->first;
        gradients[name].zeros();
    }
}


frowvec REINFORCE::policyForward(vector<float> observation) {
    fmat W1 = params["W1"];
    fmat b1 = params["b1"];
    fmat W2 = params["W2"];
    fmat b2 = params["b2"];

    frowvec obs(observation); // convert to arma vec

    // affine 1
    fmat h = obs *  W1 + b1;
    // ReLU (make all elements < 0 = zero)
    h.elem( find(h < 0) ).zeros();
    // affine 2
    fmat logp = h * W2 + b2; // log (unnormalized) probabilities

    //logp.print();

    frowvec softmaxProbs = softmax_(logp);
    //cout << "softmaxProbs" << softmaxProbs << endl;

    // cache variables for computing analytic gradient in backward pass
    cacheObservations = join_vert(cacheObservations, obs);
    cacheSoftmaxProbs = join_vert(cacheSoftmaxProbs, softmaxProbs);
    cacheHiddenStates = join_vert(cacheHiddenStates, h);
    // weights and biases are also needed, but they are implicitly cached (they do not change during forward pass)

    return softmaxProbs;
}

int REINFORCE::selectAction(vector<float> observation) {
    frowvec probs = policyForward(observation);
    int action = sample_(probs);
    cacheActions.push_back(action);
    // this is for multiplying by the rewards in optimize (line 72 pytorch reinforce)
    cacheNegLogProbs.push_back(-log(probs[action]));

    return action;
}

// TODO: verify this gradient somehow
void REINFORCE::policyBackward(fmat dout) {
    // compute gradients and multiply them back through the network to update
    // weights. This method does "backpropagation" by computing the analytic
    // gradient for each layer of the network, where the gradient is the
    // extension of the derivative to matrices and vectors. So this function
    // was derived using calculus.
    fmat W1 = params["W1"];
    fmat b1 = params["b1"];
    fmat W2 = params["W2"];
    fmat b2 = params["b2"];

    //cacheSoftmaxProbs.print();
    //dout.print();

    fmat stretchDout = join_horiz(dout, dout);
    stretchDout = join_horiz(stretchDout, dout);
    stretchDout = join_horiz(stretchDout, dout);
    //cout << "size1 " << size(cacheSoftmaxProbs) << endl;
    //cout << "size2 " << size(dout) << endl;
    //cout << "size2 " << size(cacheHiddenStates) << endl;
    //cout << "dout " << dout << endl;
    //fmat dSoftmax = stretchDout % (cacheSoftmaxProbs % (1 - cacheSoftmaxProbs));
    //fmat dSoftmax = stretchDout;
    //dSoftmax.print();

    fmat inner = sum(stretchDout % cacheSoftmaxProbs, 1);
    //cout << "inner " << inner << endl;
    inner = join_horiz(inner, inner);
    inner = join_horiz(inner, inner);
    fmat dSoftmax = cacheSoftmaxProbs % (stretchDout - inner);

    //// zero out gradients for actions not chosen (similar to an arg max)
    //fvec vecActions(cacheActions);
    //fmat z = zeros<fmat>(size(stretchDout));

    //for (size_t i = 0; i < vecActions.size(); i++) {
    //    //cout << "action " << vecActions(i) << endl;
    //    z(i, vecActions(i)) = 1;
    //}

    ////cout << "z " << z << endl;

    //dSoftmax %= z;
    //cout << "dSoftmax " << dSoftmax << endl;

    //for (size_t i = 0; i < vecActions.size(); i++) {
    //    dSoftmax *= cacheSoftmaxProbs * (vecActions[i])
    //}


    // TODO: next try deciphering this (using jacobian):
    // https://stats.stackexchange.com/questions/79454/softmax-layer-in-a-neural-network


    // TODO: we need something like this, but this may be wrong
    //fvec vecActions(cacheActions);
    //for (size_t i = 0; i < vecActions.size(); i++) {
    //    dSoftmax(i, vecActions[i]) -= 1;
    //}

    // forward pass was softmax = h*W2 + b2, so derivative w.r.t. W2 is dsoftmax * h from the forward pass
    fmat curr_dW2 = cacheHiddenStates.t() * dSoftmax;

    // derivative of softmax w.r.t. h is similar because they are multiplied
    fmat curr_dh = dSoftmax * W2.t();
    // analytic gradient of relu. if anything was less than 0 in the forward pass, the gradient is 0
    curr_dh.elem( find(cacheHiddenStates < 0) ).zeros();

    // derivate of softmax w.r.t. b2 is 1 so gradient gets passed through
    frowvec curr_db2 = sum(dSoftmax, 0);

    // forward pass was h = obs*W1 + b1, so derivative w.r.t. W1 is dh * obs from the forward pass
    fmat curr_dW1 = cacheObservations.t() * curr_dh;
    // derivate of h w.r.t. b2 is 1 so gradient gets passed through
    frowvec curr_db1 = sum(curr_dh, 0);

    cacheActions.clear();
    cacheObservations.clear();
    cacheHiddenStates.clear();
    cacheSoftmaxProbs.clear();
    gradients["W2"] += curr_dW2;
    gradients["b2"] += curr_db2;
    gradients["W1"] += curr_dW1;
    gradients["b1"] += curr_db1;
}


void REINFORCE::optimizePolicy(vector<float> rewards, bool doit) {
    fvec returns = calculateReturns_(fvec(rewards));
    fvec negLogProbs(cacheNegLogProbs);
    cacheNegLogProbs.clear();

    policyBackward(returns % negLogProbs);

    if (doit) {
        // Adam Optimizer
        // (http://cs231n.github.io/neural-networks-3/)
        for (auto it=params.begin(); it!=params.end(); ++it) {
            string name = it->first;
            fmat param = it->second;

            adamMCache[name] = ADAM_B1*adamMCache[name] + (1-ADAM_B1)*gradients[name];
            //cout << "mCache " << adamMCache[name] << endl;
            fmat mt = adamMCache[name] / (1.-pow(ADAM_B1, adamCount));
            //cout << "mt " << mt << endl;

            //cout << "power " << pow(ADAM_B1, (float) adamCount) << endl;
            //cout << "power " << 1.-pow(ADAM_B2, adamCount) << endl;

            adamVCache[name] = ADAM_B2*adamVCache[name] + (1-ADAM_B2)*pow(gradients[name], 2);
            fmat vt = adamVCache[name] / (1-pow(ADAM_B2, adamCount));
            //cout << "vt " << vt << endl;

            fmat update = -ADAM_LEARNING_RATE * mt / (sqrt(vt) + 1e-9);
            //cout << "update " << name << " " << update << endl;

            params[name] += update;
        }
        adamCount++;
        zeroGradients();
    }
}



