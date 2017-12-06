#include "reinforce.h"

// Inspired by:
// https://gist.github.com/karpathy/a4166c7fe253700972fcbc77e4ea32c5
// https://github.com/pytorch/examples/blob/master/reinforcement_learning/reinforce/py
// See Karpathy Blog and Sutton Book for theory

REINFORCE::REINFORCE(int obsSpace, int actionChoices)
{
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
    vec p = randu<vec>(1);
    p.

}

frowvec REINFORCE::policyForward(vector<float> observation) {
    frowvec obs(observation); // convert to arma vec

    // affine 1
    fmat h = obs *  W1 + b1;
    // ReLU (all elements < 0 = zero)
    h.elem( find(h < 0) ).zeros();
    // affine 2
    fmat logp = h * W2; // log (unnormalized) probabilities
    return softmax_(logp);
}

int REINFORCE::selectAction(vector<float> observation) {
    frowvec probs = forward(observation);



    return 0;
}


void REINFORCE::backward() {

}


void REINFORCE::optimize() {

}
