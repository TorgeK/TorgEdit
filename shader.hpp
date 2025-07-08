#ifndef SHADER_HPP
#define SHADER_HPP

class Shader {
public:
    Shader();

    void loadShaderFromFile(const char* shaderPath);

private:
    void compileShader();

    void createShaderProgram();

private:
    const char* rawShaderCode;
    unsigned int shader;
    unsigned int ID;
};

#endif
