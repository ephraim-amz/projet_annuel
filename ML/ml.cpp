#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#define DLLEXPORT extern "C" __declspec(dllexport)
#else
#define DLLEXPORT extern "C"
#endif
#include <cstdlib>
#include <iostream>
#include <random>
#include "Eigen/Dense"


using namespace Eigen;
using namespace std;

class Model {

public:
    int size;
    float *value;

};

/**
 * FUNCTIONS PART
 */
Model *createModel(int size){
    Model *model = (Model *) (malloc( sizeof(Model)));
    model -> size = size+1;
    model->value = (float *) (malloc(sizeof(float) * (size+1)));


    return model;
}

float *array_copy(float *array, int size){
    float *a = (float*) malloc(sizeof(float)*size);

    for(int i = 0; i < size; i++){
        a[i] = array[i];
    }
    return a;
}

float *insert_value(int n, float arr[], int x, int pos)
{
    int i;
    // increase the size by 1
    n++;
    // shift elements forward
    for (i = n; i >= pos; i--)
        arr[i] = arr[i - 1];
    // insert x at pos
    arr[pos - 1] = x;

    return arr;
}

float* arrayOf_flattened_dataset_inputs(float* array, int start, int end){
    int size = end-start;

    float* inputs = (float*) malloc(sizeof (float ) * size);

    for(int i = 0; i < size; i++)
    {
        inputs[i] = array[start+i];
    }
    return inputs;
}

/**
 * END OF FUNCTIONS PART
 */

DLLEXPORT Model *create_linear_model(int input_dim){

    Model *m1 = createModel(input_dim);
    std::random_device r;
    // std::seed_seq ssq{r()};
    // and then passing it to the engine does the same
    default_random_engine generator{r()};
    std::uniform_real_distribution<float> distribution(-1.0, 1.0);

    for (int i = 0; i < input_dim+1; i++) {

        m1->value[i] = distribution(generator);
    }

    return m1;
}

DLLEXPORT float predict_linear_model_regression_unefficient_but_more_readable(Model *m1, float *sample_inputs, int length) {
    float *sample_inputs_copy = array_copy(sample_inputs, length);
    sample_inputs_copy = insert_value(length, sample_inputs_copy, 0, 1.0);

    float result = 0.0;

    for(int i=0; i< m1 -> size ; i++){
        result+= m1 -> value[i] * sample_inputs_copy[i];
    }

    return result;
}


DLLEXPORT float predict_linear_model_regression(Model *m1, float* sample_inputs){
    float result = m1->value[0] * 1.0;

    for (int i = 1; i < m1->size; i++){
        result += m1->value[i] * sample_inputs[i - 1];
    }
    return result;
}


DLLEXPORT float predict_linear_model_classification(Model* m1, float *sample_inputs){
    float prediction = predict_linear_model_regression(m1, sample_inputs);
    if (prediction >= 0.0) {
        return 1.0;
    }
    else {
        return -1.0;
    }
}


DLLEXPORT void train_classification_rosenblatt_rule_linear_model(Model *m1, float* flattened_dataset_inputs, float *flattened_dataset_expected_outputs, float alpha = 0.001, int iterations_count = 50, int len = 0) {
    int input_dim = m1->size - 1;
    int samples_count = len / input_dim; //pas sizeOF

    for (int it = 0; it < iterations_count; it++) {

        int k = std::rand() % samples_count;

        float* Xk = arrayOf_flattened_dataset_inputs(flattened_dataset_inputs,k * input_dim,(k + 1) * input_dim);
        float Yk = flattened_dataset_expected_outputs[k];
        float gXk = predict_linear_model_classification(m1, Xk);
        m1->value[0] += alpha * (Yk - gXk) * 1.0;

        for (int i = 1; i < m1->size; i++) {
            m1->value[i] += alpha * (Yk - gXk) * Xk[i - 1];
        }
    }
}


DLLEXPORT void train_regression_pseudo_inverse_linear_model(Model *m1, float *flattened_dataset_inputs, float *flattened_dataset_expected_outputs, int len_input, int len_output){
    int input_dim = m1->size - 1;
    int samples_count = len_input / input_dim;

    MatrixXf X(len_input,1); // define matrix
    VectorXf vX (len_input);

    for(int i = 0; i<  len_input; i++){
        vX[i] = flattened_dataset_inputs[i];
    }

    X << vX;

    MatrixXf Y(len_output,1); // define matrix
    VectorXf vY (len_output);

    for(int i = 0; i<  len_output; i++){
        vY[i] = flattened_dataset_expected_outputs[i];
    }

    Y << vY;

    MatrixXf ones(samples_count, 1);
    ones.setOnes();

    MatrixXf Xi(X.rows(), X.cols()+ones.cols());
    Xi << ones, X ; // X = np.hstack((ones, X))

    MatrixXf W = Xi.transpose() * Xi;

    MatrixXf W_inv = W.inverse();

    MatrixXf W2 = W_inv * Xi.transpose();

    MatrixXf W3 = W2 * Y;

    ArrayXf Ww = W3.array();

    for(int i=0; i<m1->size-1; i++){
        m1->value[i] = Ww[i];

    }
}

DLLEXPORT void destroy_linear_model(Model *m1) {
    delete[] m1;
}