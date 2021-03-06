#include "cms_hh_tf_inference/inference/interface/pipeline.hh"

Pipeline::Pipeline(Preproc* preproc, Ensemble* ensemble, bool verbose) {
    _preproc = preproc;
    _ensemble = ensemble;
    _verbose = verbose;
}

Pipeline::~Pipeline() {}

float Pipeline::predict(std::vector<float> input) {
    if (_verbose) std::cout << "Preprocessing input\n";
    tensorflow::Tensor x = _preproc->process(input);
    if (_verbose) std::cout << "Input processed\n";
    float y = _ensemble->predict(x);
    if (_verbose) std::cout << "Prediction is " << y << "\n";
    return y;
}