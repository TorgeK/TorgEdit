#ifndef SHADER_HPP
#define SHADER_HPP

class Shader {
public:
    Shader();

    void loadShaderFromFile(const char* shaderPath);

private:
    char* rawShaderCode;
};

#endif
