#ifndef REINFORCE_H
#define REINFORCE_H

#include <string>
#include <vector>
#include <random>
#include <map>
#include <armadillo>

using namespace arma;
using namespace std;
class REINFORCE
{
public:
    REINFORCE(const int obsSpace, const int actionChoices);

    /*
     * Neural network forward pass (matrix multiply observations through weights and biases)
     *
     * (affine --[relu]--> affine --[softmax]--> output)
     *
     * Returns a vector of numbers (0,1) of length actionChoices that represent the probability
     * of choosing each of the different actions.  These will start nearly equally distributed and as
     * the network learns, the probabilities will be higher for better actiosn
     */
    fvec policyForward(vector<float> observation);

    /*
     * Pass observations through neural network and choose an action from the forward pass's probability distribution
     */
    int selectAction(vector<float> observation);

    /*
    * Neural network backward pass (calculate gradient of all parameters with respect to output ("backpropagation"))
    */
    void policyBackward(fmat dout);
    //update network weights (hopefully in a good way)
    void optimizePolicy(vector<float> rewards, bool doit);

    void zeroGradients();


private:
    // softmax function (turns a list of numbers into probabilities)
    fvec softmax_(fvec logp);
    // discrete sample from probability list
    int sample_(fvec probs);

    // See Sutton Book for math
    // Calculating discounted returns
    // $G_t = r_{t+1} + \gamma*r_{t+2} + \gamma^2*r_{t=3} + ....$
    // $G_i = r_{i+1} + \gamma * G_{i+1}$
    fvec calculateReturns_(fvec rewards);

    vector<float> cacheNegLogProbs;
    vector<float> cacheActions;
    fmat cacheLogits;
    fmat cacheObservations;
    fmat cacheHiddenStates;
    fmat cacheSoftmaxProbs;

    // Network shape parameters
    fvec O; // observation shape
    int A; // action shape
    static constexpr int H = 400; // number of hidden units (higher = more numbers the network can use to learn)
    static constexpr float GAMMA = 0.99; // discount factor gamma (close to 1 means more long-sighted, 0 only optimizes current reward)
    // adam optimizer params
    static constexpr float ADAM_B1 = 0.9;
    static constexpr float ADAM_B2 = 0.999;
    static constexpr float ADAM_LEARNING_RATE = 4e-3;
    long adamCount; // count up
    map<string, fmat> adamCacheM;
    map<string, fmat> adamCacheV;

    // Neural network parameters (weights and biases)
    map<string, fmat> params;
    map<string, fmat> gradients;

    //fmat dW1;
    //fvec db1;
    //fmat dW2;
    //fvec db2;
};

#endif // REINFORCE_H
