#include "reinforce.h"
// Inspired by:
// https://gist.github.com/karpathy/a4166c7fe253700972fcbc77e4ea32c5
// https://github.com/pytorch/examples/blob/master/reinforcement_learning/reinforce/py
// (see Karpathy Blog and Sutton Book for theory)

REINFORCE::REINFORCE(int obsSpace, int actionChoices)
{
    //arma_rng::set_seed_random();  // set the seed to a random value
    // random initialization for the weights. all zeros intialization for the biases
    fmat W1 = randn<fmat>(H, obsSpace) / sqrt(obsSpace);
    fmat b1 = zeros<fvec>(H);
    fmat W2 = randn<fmat>(actionChoices, H) / sqrt(H);
    fmat b2 = zeros<fvec>(actionChoices);
    params["W1"] = W1;
    params["b1"] = b1;
    params["W2"] = W2;
    params["b2"] = b2;

    adamCount = 0;
    // create gradient and adamCache element for every parameter (used to train network)
    for (auto it=params.begin(); it!=params.end(); ++it) {
        string name = it->first;
        fmat param = it->second;

        fmat grad =  zeros<fmat>(size(param));
        fmat m =  zeros<fmat>(size(param));
        fmat v =  zeros<fmat>(size(param));
        gradients[name] = grad;
        adamCacheM[name] = m;
        adamCacheV[name] = v;
    }
}

fvec REINFORCE::softmax_(fvec logp) {
    logp -= logp.max(); // subtract max for numerical stability (exp can be very large)
    fvec p = exp(logp) / sum(exp(logp));
    return p;
}

int REINFORCE::sample_(fvec probs) {
    std::random_device rd;
    std::mt19937 gen(rd());

    vector<float> fprobs = conv_to<vector<float>>::from(probs);
    std::discrete_distribution<int> d(fprobs.begin(), fprobs.end());

    return d(gen);
}

fvec REINFORCE::calculateReturns_(fvec rewards) {
    fvec returns = zeros<fvec>(size(rewards));

    // nextReturn is the return of the next state. It is used in the recursive calculation of
    // return (if you know the current reward at state i and the return at state i+1, you can
    // calculate the return at state i. read that again)
    float nextReturn = 0.0; // starts at 0 because we start at the last state (there's no return after we are done)

    for (int i = (int) rewards.size()-1; i >= 0; i--) {
        nextReturn = rewards[i] + GAMMA*nextReturn;
        returns[i] = nextReturn;
    }
    // normalize returns (this tends to help the network train (something something statistics))
    returns = (returns - mean(returns)) / (stddev(returns) + 1e-8);
    return returns;
}

void REINFORCE::zeroGradients() {
    for (auto it=params.begin(); it!=params.end(); ++it) {
        string name = it->first;
        gradients[name].zeros();
    }
}


fvec REINFORCE::policyForward(vector<float> observation) {
    fmat W1 = params["W1"];
    fmat b1 = params["b1"];
    fmat W2 = params["W2"];
    fmat b2 = params["b2"];
    fvec obs(observation); // convert to arma vec

    // affine 1
    fmat h = W1 * obs + b1;
    // ReLU (make all elements < 0 = zero)
    h.elem( find(h < 0) ).zeros();
    // affine 2
    fmat logits = W2 * h + b2; // (unnormalized) probabilities
    fvec softmaxProbs = softmax_(logits);
    //cout << "softmaxProbs" << softmaxProbs << endl;

    // cache variables for computing analytic gradient in backward pass
    //cacheLogits = join_horiz(cacheLogits, logits);
    cacheObservations = join_horiz(cacheObservations, obs);
    cacheSoftmaxProbs = join_horiz(cacheSoftmaxProbs, softmaxProbs);
    cacheHiddenStates = join_horiz(cacheHiddenStates, h);
    // weights and biases are also needed, but they are implicitly cached (they do not change during forward pass)

    return softmaxProbs;
}

int REINFORCE::selectAction(vector<float> observation) {
    fvec probs = policyForward(observation);
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
    //cout << "size dout " << size(dout) << endl;

    // TODO: dive deep into this to see if weights and everything seem reasonable

    fmat stretchDout = join_horiz(dout, dout);
    stretchDout = join_horiz(stretchDout, dout);
    stretchDout = join_horiz(stretchDout, dout);

    fmat inner = sum(stretchDout % cacheSoftmaxProbs.t(), 1);
    inner = join_horiz(inner, inner);
    inner = join_horiz(inner, inner);
    fmat dSoftmax =  (stretchDout - inner) % cacheSoftmaxProbs.t();
    //cout << "size dSoftmax " << size(dSoftmax) << endl;

    // forward pass was softmax = h*W2 + b2, so derivative w.r.t. W2 is dsoftmax * h from the forward pass
    fmat curr_dW2 = dSoftmax.t() * cacheHiddenStates.t();

    // derivative of softmax w.r.t. h is similar because they are multiplied
    fmat curr_dh = W2.t() * dSoftmax.t();
    //cout << "W2 " << size(W2) << "  dSoftmax " << size(dSoftmax) << endl;

    // analytic gradient of relu. if anything was less than 0 in the forward pass, the gradient is 0

    curr_dh.elem( find(cacheHiddenStates < 0) ).zeros();
    //cout << "curr_dh size = " << size(curr_dh) << endl;

    // derivate of softmax w.r.t. b2 is 1 so gradient gets passed through
    //cout << size(dSoftmax) << endl;
    fvec curr_db2 = sum(dSoftmax, 0).t();

    // forward pass was h = obs*W1 + b1, so derivative w.r.t. W1 is dh * obs from the forward pass
    fmat curr_dW1 = curr_dh * cacheObservations.t();
    // derivate of h w.r.t. b2 is 1 so gradient gets passed through
    fvec curr_db1 = sum(curr_dh, 1);

    //cacheLogits.clear();
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
    policyBackward(returns % negLogProbs);
    cacheNegLogProbs.clear();

    if (doit) {
        adamCount++;
        float lr = ADAM_LEARNING_RATE * sqrt(1 - pow(ADAM_B2, adamCount)) / (1 - pow(ADAM_B1, adamCount));
        // Adam Optimizer
        // (http://cs231n.github.io/neural-networks-3/)
        for (auto it=params.begin(); it!=params.end(); ++it) {
            string name = it->first;
            fmat param = it->second;

            //adamCacheM[name] = ADAM_B1*adamCacheM[name] + (1-ADAM_B1)*gradients[name];
            ////cout << "mCache " << adamCacheM[name] << endl;
            //fmat mt = adamCacheM[name] / (1.-pow(ADAM_B1, adamCount));
            ////cout << "mt " << mt << endl;

            ////cout << "power " << pow(ADAM_B1, (float) adamCount) << endl;
            ////cout << "power " << 1.-pow(ADAM_B2, adamCount) << endl;

            //adamCacheV[name] = ADAM_B2*adamCacheV[name] + (1-ADAM_B2)*pow(gradients[name], 2);
            //fmat vt = adamCacheV[name] / (1-pow(ADAM_B2, adamCount));
            ////cout << "vt " << vt << endl;
            ///

            adamCacheM[name] = ADAM_B1 * adamCacheM[name] + (1 - ADAM_B2) * gradients[name];
            adamCacheV[name] = ADAM_B2 * adamCacheV[name] + (1 - ADAM_B2) * pow(gradients[name], 2);
            fmat update = -lr * adamCacheM[name] / (sqrt(adamCacheV[name]) + 1e-8);
            //cout << "update " << name << " " << update << endl;
            params[name] += update;
        }
        zeroGradients();
    }
}



