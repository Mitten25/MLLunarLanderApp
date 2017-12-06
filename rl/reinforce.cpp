#include "reinforce.h"

// Inspired by:
// https://gist.github.com/karpathy/a4166c7fe253700972fcbc77e4ea32c5
// https://github.com/pytorch/examples/blob/master/reinforcement_learning/reinforce/py
// See Karpathy Blog and Sutton Book for theory

REINFORCE::REINFORCE(int obsSpace, int actionChoices)
{
    arma_rng::set_seed_random();  // set the seed to a random value
    // random initialization for the weights. all zeros intialization for the biases
    W1.randn(obsSpace, H) / sqrt(H);
    b1.zeros(H);
    W2.randn(H, actionChoices) / sqrt(actionChoices);
    b2.zeros(actionChoices);
}

frowvec REINFORCE::softmax_(frowvec logp) {
    logp -= logp.max(); // subtract max for numerical stability (exp can be very large)
    frowvec p = exp(logp) / sum(exp(logp));
    return p;
}

int REINFORCE::sample_(frowvec probs) {
    int n = probs.size();
    std::random_device rd;
    std::mt19937 gen(rd());

    vector<float> fprobs = conv_to<vector<float>>::from(probs);
    //vector<int> iprobs(fprobs.begin(), fprobs.end());
    std::discrete_distribution<int> d(fprobs.begin(), fprobs.end());

    return d(gen);
}

frowvec REINFORCE::policyForward(vector<float> observation) {
    frowvec obs(observation); // convert to arma vec

    // affine 1
    fmat h = obs *  W1 + trans(b1);
    // ReLU (make all elements < 0 = zero)
    h.elem( find(h < 0) ).zeros();
    // affine 2
    fmat logp = h * W2 + trans(b2); // log (unnormalized) probabilities

    //logp.print();

    frowvec softmaxProbs = softmax_(logp);

    // cache variables for computing analytic gradient in backward pass
    join_vert(cacheObservations, obs);
    join_vert(cacheSoftmaxProbs, softmaxProbs);
    join_vert(cacheHiddenStates, h);
    // weights and biases are also needed, but they are implicitly cached (they do not change during forward pass)

    return softmaxProbs;
}

int REINFORCE::selectAction(vector<float> observation) {
    frowvec probs = policyForward(observation);
    int action = sample_(probs);

    // this is for multiplying by the rewards in optimize (line 72 pytorch reinforce)
    cacheLogProbs.push_back(probs[action]);

    return action;
}

void REINFORCE::policyBackward(frowvec dout) {
    // compute gradients and multiply them back through the network to update
    // weights. This method does "backpropagation" by computing the analytic
    // gradient for each layer of the network, where the gradient is the
    // extension of the derivative to matrices and vectors. So this function
    // was derived using calculus.


    // TODO: need to go through and make sure everything is the right dimension

    // TODO: verify this gradient somehow
    fmat dSoftmax(cacheSoftmaxProbs);
    dSoftmax.elem(cacheActions) -= 1;

    // forward pass was softmax = h*W2 + b2, so derivative w.r.t. W2 is dsoftmax * h from the forward pass
    fmat dW2 = dSoftmax * cacheHiddenStates;
    // derivative of softmax w.r.t. h is similar because they are multiplied
    fmat dh = dSoftmax * W2;
    // derivate of softmax w.r.t. b2 is 1 so gradient gets passed through
    fmat db2 = dSoftmax;//.sum() along some axis or something

    // analytic gradient of relu. if anything was less than 0 in the forward pass, the gradient is 0
    dh.elem( find(cacheHiddenStates < 0) ).zeros();
    // forward pass was h = obs*W1 + b1, so derivative w.r.t. W1 is dh * obs from the forward pass
    fmat dW1 = dh * cacheObservations;
    // derivate of h w.r.t. b2 is 1 so gradient gets passed through
    fmat db1 = dh; //.sum() along some axis or something


}


void REINFORCE::optimize() {
    //frowvec logProbs(cacheLogProbs);

}
