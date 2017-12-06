#ifndef REINFORCE_H
#define REINFORCE_H

#include <armadillo>
#include "vector"


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

    void policyBackward();
    void optimize();

    fmat W1;
    fvec b1;
    fmat W2;
    fvec b2;

    fvec O;
    int A;
    static const int H = 10;
private:
    frowvec softmax_(frowvec logp);
    int sample_(frowvec probs);
};

#endif // REINFORCE_H
