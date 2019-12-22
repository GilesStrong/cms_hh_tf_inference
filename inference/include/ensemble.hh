#ifndef ENSEMBLE_HH_
#define ENSEMBLE_HH_

// C++
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <assert.h>
#include <experimental/filesystem>

// TensorFlow
#include "PhysicsTools/TensorFlow/interface/TensorFlow.h"

// Local
#include "nn.hh"

class Ensemble {
	/* Class to handle loading and inference of ensembling of models*/

private:
	unsigned int _n_models, _n_threads;
    std::vector<NN> _models;
    std::vector<float> _weights;
    bool _verbose;

public:
    // Methods
	Ensemble(std::string, unsigned int, bool);
	~Ensemble();
	float predict(tensorflow::Tensor);
    bool load_ensemble(std::string);
};

#endif /* ENSEMBLE_HH_ */