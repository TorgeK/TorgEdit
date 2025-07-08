#ifndef SHADER_HPP
#define SHADER_HPP

class Shader {
public:
    Shader();

    void loadShaderFromFile(const char* shaderPath);

    void compileShader();

private:
    const char* rawShaderCode;
    unsigned int shader;
};

#endif
