#include "cms_hh_tf_inference/inference/interface/inf_wrapper.hh"
#include <iostream>
#include <string>
#include <map>
#include <sstream>
#include <fstream>
#include <assert.h>
#include <vector>
#include <cmath>

std::string root_dir = "../../src/cms_hh_tf_inference/testing_setup/python/";

void show_help() {
    /* Show help for input arguments */

    std::cout << "-i : model root name, default " << root_dir << "models/ensemble\n";
    std::cout << "-d : data root name, default " << root_dir << "data/set\n";
    std::cout << "-n : number of threads, default 1\n";
    std::cout << "-v : Run in verbose point, default off\n";
}

std::map<std::string, std::string> get_options(int argc, char* argv[]) {
    /*Interpret input arguments*/

    std::map<std::string, std::string> options;
    options.insert(std::make_pair("-i", root_dir + "models/ensemble")); // model root name
    options.insert(std::make_pair("-d", root_dir + "data/example")); // data root name
    options.insert(std::make_pair("-n", "1")); // number of threads
    options.insert(std::make_pair("-v", "false")); // verbose mode

    if (argc >= 2) { //Check if help was requested
        std::string option(argv[1]);
        if (option == "-h" || option == "--help") {
            show_help();
            options.clear();
            return options;
        }
    }

    for (int i = 1; i < argc; i = i+2) {
        std::string option(argv[i]);
        std::string argument(argv[i+1]);
        if (option == "-h" || option == "--help" || argument == "-h" || argument == "--help") { // Check if help was requested
            show_help();
            options.clear();
            return options;
        }
        options[option] = argument;
    }
    return options;
}

bool run_test_loop(std::string file_name, InfWrapper* wrapper, unsigned long int event_id) {
    std::vector<float> row;
    std::string line, val;
    float pred, targ, diff;
    if (!boost::filesystem::exists(file_name)) {
        throw std::invalid_argument("File: " + file_name + " not found");
        return false;
    }
    std::cout << "Opening " << file_name << "\n";
    std::ifstream infile(file_name);
    int l = -1;
    while (std::getline(infile, line)) {  // Read in event
        l++;
        if (l == 0) continue;  // Skip header
        std::istringstream iss(line);
        row.clear();
        std::cout << "\nRow " << l << " [";
        while (std::getline(iss, val, ',')) {   // Row to vector
            if (val == "") {
                row.push_back(std::nanf("1"));
            } else {
                row.push_back(std::stof(val));
            }
            std::cout << val << ",";
        }
        std::cout << "]\n";
        pred = wrapper->predict(std::vector<float>(row.begin(), row.end()-1), event_id);
        targ = row.back();
        diff = std::abs(targ-pred);
        std::cout << "prediction:" << pred << " expected " << targ << " difference " << diff << "\n";
        assert(diff < 1e-5);
    }
    infile.close();
    return true;
}

int main(int argc, char *argv[]) {
    std::map<std::string, std::string> options = get_options(argc, argv); // Parse arguments
    if (options.size() == 0) {
        return 1;
    }
    bool verbose = options["-v"] == "true";
    unsigned int n_threads = std::stoi(options["-n"]);

    std::cout << "Instantiating wrapper\n";
    InfWrapper* wrapper = new InfWrapper(options["-i"], n_threads, verbose);
    std::cout << "Wrapper instantiated\n";

    std::cout << "\nBeginning test loop for ensemble 0\n";
    assert(run_test_loop(options["-d"]+"_0.csv", wrapper, 0));
    std::cout << "Test loop for ensemble 0 complete\n";

    std::cout << "\nBeginning test loop for ensemble 1\n";
    assert(run_test_loop(options["-d"]+"_1.csv", wrapper, 1));
    std::cout << "Test loop for ensemble 1 complete\n";
    std::cout << "\nAll tests completed sucessfully\n";
    return 0;
}