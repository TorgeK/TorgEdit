#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include "shader.hpp"

void Shader::loadShaderFromFile(const char* shaderPath) {

    std::string shaderCode;

    std::ifstream shaderFile;

    shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try{
        shaderFile.open(shaderPath);
        std::stringstream shaderStream;

        // read file contents into stream
        shaderStream << shaderFile.rdbuf();

        // close file handler
        shaderFile.close();

        shaderCode = shaderStream.str();
    }
    catch(std::ifstream::failure e) {
        std::cout << "Error: failed to read shader" << std::endl; 
    }

    const char* rawShaderCode = shaderCode.c_str();
}