#include <vector>
#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <SFML/Graphics.hpp>

class MLP {

    public:

    int ***W;
    int *d;
    float **X;
    float **deltas;

    MLP(int ***W, int *d, float **X, float **deltas) {
        this->W = W;
        this->d = d;
        this->X = X;
        this->deltas = deltas;
    }

    void forward_pass(float *sample_inputs, bool is_classification) {
        int L = sizeof(this->d)/sizeof(this->d[0]) - 1;
        for (int j = 0; j < this->d[0] + 1; j++) {
            this->X[0][j] = sample_inputs[j - 1];
        }
        for (int l = 1; l < L + 1; l++) {
			for (int j = 1; j < this->d[l] + 1; j++) {
                float sum_result = 0.0;
                for (int i = 0; i < this->d[l - 1]; i++) {
                    sum_result += this->W[l][i][j] * this->X[l - 1][i];
                }
                this->X[l][j] = sum_result;
                if (is_classification or l < L) {
                    this->X[l][j] = tanh(this->X[l][j]);
                }
            }
        }
    }

	float *get_sample_input(float *flattened_dataset_inputs, int input_dim, int k) {
		float *sample_input;
		sample_input = (float*)malloc(sizeof(sample_input) * 255);
		for (int i = 0; i < (k + 1) * input_dim; i++) {
			sample_input[i] = flattened_dataset_inputs[i + k * input_dim];
		}
		return sample_input;
	}

	float *get_sample_output(float *flattened_dataset_expected_outputs, int output_dim, int k) {
		float *sample_ouput;
		sample_ouput = (float*)malloc(sizeof(sample_ouput) * 255);
		for (int i = 0; i < (k + 1) * output_dim; i++) {
			sample_ouput[i] = flattened_dataset_expected_outputs[i + k * output_dim];
		}
		return sample_ouput;
	}

    void train_stochastic_gradient_backpropagation(float *flattened_dataset_inputs, float *flattened_dataset_expected_outputs, bool is_classification, float alpha = 0.001, int iterations_count = 100000) {
        int input_dim = this->d[0];
        int output_dim = this->d[sizeof(this->d) / sizeof(this->d[0]) - 1];
        int samples_count = sizeof(flattened_dataset_inputs) / sizeof(flattened_dataset_inputs[0]) / input_dim;
        int L = sizeof(this->d) / sizeof(d[0]) - 1;
        for (int it = 0; it < iterations_count; it++) {
            int k = rand() % samples_count - 1;
            float *sample_input = get_sample_input(flattened_dataset_inputs, input_dim, k);
            float *sample_expected_output = get_sample_output(flattened_dataset_expected_outputs, output_dim, k);
            this->forward_pass(sample_input, is_classification);
            for (int j = 1; j < this->d[L]; j++) {
                this->deltas[L][j] = (this->X[L][j] - sample_expected_output[j - 1]);
                if (is_classification) {
                    this->deltas[L][j] *= (1 - this->X[L][j] * this->X[L][j]);
                }
            }
            for (int l = L; l > 0; l--) {
                for (int i = 1; i < this->d[l - 1]; i++) {
                    float sum_result = 0.0;
                    for (int j = 1; j < this->d[l]; j++) {
                        sum_result += this->W[l][i][j] * this->deltas[l][j];
                    }
                    this->deltas[l - 1][i] = (1 - this->X[l - 1][i] * this->X[l - 1][i])* sum_result;
                }
            }
            for (int l = 1; l < L; l++) {
                for (int i = 0; i < this->d[l - 1]; i++) {
                    for (int j = 1; j < this->d[l]; j++) {
                        this->W[l][i][j] -= alpha * this->X[l - 1][i] * this->deltas[l][j];
                    }
                }
            }
        }
    }
};

MLP create_mlp_model(int *npl) {
    int ***W;
    W = (int***)malloc(sizeof(int**) * 255);
    for (int l = 0; l < sizeof(npl) / sizeof(npl[0]); l++) {
		W[l] = (int**)malloc(sizeof(int*) * 255);
        if (l != 0) {
            for (int i = 0; i < npl[l - 1]; i++) {
				W[l][i] = (int*)malloc(sizeof(int) * 255);
                for (int j = 0; j < npl[l]; j++) {
                    W[l][i][j] = rand() % 3 - 1;
                }
            }
        }
    }
    int *d = npl;
    float **X;
	X = (float**)malloc(sizeof(float*) * 255);
    for (int l = 0; l < sizeof(npl) / sizeof(npl[0]); l++) {
		X[l] = (float*)malloc(sizeof(float) * 255);
        for (int j = 0; j < npl[l]; j++) {
            if (j == 0) {
                X[l][j] = 1.0;
            } else {
                X[l][j] = 0.0;
            }
        }
    }
    float **deltas;
    deltas = (float**)malloc(sizeof(float*) * 255);
    for (int l = 0; l < sizeof(npl) / sizeof(npl[0]); l++) {
		deltas[l] = (float*)malloc(sizeof(float) * 255);
        for (int j = 0; j < npl[l]; j++) {
            deltas[l][j] = 0.0;
        }
    }
    return MLP(W, d, X, deltas);
}

float* predict_mlp_model_regression(MLP model, float *sample_inputs) {
    model.forward_pass(sample_inputs, false);
    return model.X[sizeof(model.X) / sizeof(model.X[0])];
}

float* predict_mlp_model_classification(MLP model, float *sample_inputs) {
    model.forward_pass(sample_inputs, true);
    return model.X[sizeof(model.X) / sizeof(model.X[0])];
}

void train_classification_stochastic_gradient_backpropagation_mlp_model(MLP model, float *flattened_dataset_inputs, float *flattened_dataset_expected_outputs, float alpha = 0.001, int iterations_count = 100000) {
    model.train_stochastic_gradient_backpropagation(flattened_dataset_inputs, flattened_dataset_expected_outputs, true, alpha, iterations_count);
}

void train_regression_stochastic_gradient_backpropagation_mlp_model(MLP model, float *flattened_dataset_inputs, float *flattened_dataset_expected_outputs, float alpha = 0.001, int iterations_count = 100000) {
    model.train_stochastic_gradient_backpropagation(flattened_dataset_inputs, flattened_dataset_expected_outputs, false, alpha, iterations_count);
}

int main() {
	srand (time(NULL));
	float dataset_inputs[3];
	dataset_inputs[0] = -5;
	dataset_inputs[1] = 4;
	dataset_inputs[2] = 6;
	float dataset_expected_outputs[3];
	dataset_expected_outputs[0] = 1.2;
	dataset_expected_outputs[1] = 7;
	dataset_expected_outputs[2] = 8.3;
	int datas[5];
	datas[0] = 1;
	datas[1] = 3;
	datas[2] = 1;
	MLP model = create_mlp_model(datas);
	train_regression_stochastic_gradient_backpropagation_mlp_model(model, dataset_inputs, dataset_expected_outputs);
    return 0;
}
