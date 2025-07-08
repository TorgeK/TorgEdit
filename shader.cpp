#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <GL/glew.h>
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
        std::cerr << "Error: failed to read shader" << std::endl; 
    }

    rawShaderCode = shaderCode.c_str();
}

void Shader::compileShader() {
    int success;
    char infoLog[512];

    shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(shader, 1, &rawShaderCode, NULL);
    glCompileShader(shader);

    // print compile errors
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        std::cerr << "Error: failed to compile shader" << infoLog << std::endl;
    }

}

void Shader::createShaderProgram() {
    int success;
    char infoLog[512];
    ID = glCreateProgram();

    glAttachShader(ID, shader);
    glLinkProgram(ID);
    
    // print linking errors
    glGetProgramiv(ID, GL_LINK_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(ID, 512, NULL, infoLog);
        std::cerr << "Error: failed to link shader program" << infoLog << std::endl;
    }

    // delete shaders once linked to program
    glDeleteShader(shader);
}