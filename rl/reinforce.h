#ifndef REINFORCE_H
#define REINFORCE_H

#include <armadillo>
#include <vector>
#include <random>
#include <tuple>

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
    frowvec policyForward(vector<float> observation);

    /*
     * Pass observations through neural network and choose an action from the forward pass's probability distribution
     */
    int selectAction(vector<float> observation);

    /*
    * Neural network backward pass (calculate gradient of all parameters with respect to output ("backpropagation"))
    */
    void policyBackward(frowvec dout);
    //update network weights (hopefully in a good way)
    void optimize();


private:
    // softmax function (turns a list of numbers into probabilities)
    frowvec softmax_(frowvec logp);
    // discrete sample from probability list
    int sample_(frowvec probs);

    vector<float> cacheLogProbs;
    fmat cacheObservations;
    fmat cacheHiddenStates;
    fmat cacheSoftmaxProbs;



    // Network shape parameters
    fvec O; // observation shape
    int A; // action shape
    static const int H = 400; // number of hidden units (higher = more numbers the network can use to learn)

    // Neural network parameters (weights and biases)
    fmat W1;
    fvec b1;
    fmat W2;
    fvec b2;
};

#endif // REINFORCE_H
