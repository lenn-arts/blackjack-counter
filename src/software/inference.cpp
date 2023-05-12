#include <torch/script.h> // include the PyTorch C++ API
#include <iostream>
#include <fstream>
#include <vector>

int infer(std::vector<double>* vec) {
    // Load the PyTorch model
    torch::jit::script::Module module = torch::jit::load("path/to/your/model.pth");

    //input = torch::from_blob(vec->data(), {vec->size()}, torch::dtype(torch::kFloat64))
    //

    // Prepare input data
    torch::Tensor input = torch::ones({1, 3, 224, 224});
    std::cout << input.sizes() << std::endl;

    // Run the model
    at::Tensor output = module.forward({input}).toTensor();

    // Print the output
    std::cout << output << std::endl;

    return 0;
}



std::vector<double>* read() {
    // Open the text file for reading
    std::ifstream input_file("rgbimg888");

    // Create a vector to store the data
    std::vector<double> data;

    // Read the data from the text file and store it in the vector
    double value;
    while (input_file >> value) {
        data.push_back(value);
    }

    // Print the data
    for (double d : data) {
        std::cout << d << " ";
    }
    std::cout << std::endl;

    return &data;
}

int main() {
    //vec_ptr = read();
    infer(vec_ptr);
}