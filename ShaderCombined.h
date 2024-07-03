#ifndef SHADER_COMBINED_H
#define SHADER_COMBINED_H

#include <glad/glad.h>

#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

enum ShaderType {
    VERTEX_FRAGMENT,  // Traditional vertex and fragment shader pair
    COMPUTE           // Compute shader
};

class ShaderCombined {
private:
    unsigned int ID;
    ShaderType type;
    int workgroupSizeX; // Only relevant for compute shaders
    int workgroupSizeY; // Only relevant for compute shaders

    std::string loadShaderCode(const char* path) {
        std::string code;
        std::ifstream file;
        file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

        try {
            file.open(path);
            std::stringstream stream;
            stream << file.rdbuf();
            file.close();
            code = stream.str();
        }
        catch (std::ifstream::failure e) {
            std::cerr << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: " << path << std::endl;
        }
        return code;
    }

    unsigned int compileShader(const char* shaderCode, GLenum shaderType) {
        unsigned int shader = glCreateShader(shaderType);
        glShaderSource(shader, 1, &shaderCode, NULL);
        glCompileShader(shader);

        int success;
        char infoLog[512];
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shader, 512, NULL, infoLog);
            std::cerr << "ERROR::SHADER::COMPILATION_FAILED ("
                << (shaderType == GL_VERTEX_SHADER ? "Vertex" :
                    shaderType == GL_FRAGMENT_SHADER ? "Fragment" : "Compute")
                << ")\n" << infoLog << std::endl;
        }
        return shader;
    }

public:
    ShaderCombined(const char* path1, const char* path2 = nullptr, ShaderType type = VERTEX_FRAGMENT, int sizeX = 0, int sizeY = 0)
        : type(type), workgroupSizeX(sizeX), workgroupSizeY(sizeY) {

        if (type == VERTEX_FRAGMENT) {
            std::string vertexCode = loadShaderCode(path1);
            std::string fragmentCode = loadShaderCode(path2);

            unsigned int vertexShader = compileShader(vertexCode.c_str(), GL_VERTEX_SHADER);
            unsigned int fragmentShader = compileShader(fragmentCode.c_str(), GL_FRAGMENT_SHADER);

            ID = glCreateProgram();
            glAttachShader(ID, vertexShader);
            glAttachShader(ID, fragmentShader);
            glLinkProgram(ID);

            glDeleteShader(vertexShader);
            glDeleteShader(fragmentShader);
        }
        else if (type == COMPUTE) {
            std::string computeCode = loadShaderCode(path1);
            unsigned int computeShader = compileShader(computeCode.c_str(), GL_COMPUTE_SHADER);

            ID = glCreateProgram();
            glAttachShader(ID, computeShader);
            glLinkProgram(ID);

            glDeleteShader(computeShader);
        }

        // Check linking errors for both shader types
        int success;
        char infoLog[512];
        glGetProgramiv(ID, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(ID, 512, NULL, infoLog);
            std::cerr << "ERROR::SHADER::PROGRAM::LINK_ERROR\n" << infoLog << std::endl;
        }
    }

    ~ShaderCombined() {
        glDeleteProgram(ID);
    }

    void use() {
        glUseProgram(ID);
    }

    // Combined compute shader execution
    void dispatchCompute() {
        if (type != COMPUTE) {
            std::cerr << "ERROR: Cannot dispatch non-compute shader." << std::endl;
            return;
        }
        glUseProgram(ID);
        glDispatchCompute(workgroupSizeX, workgroupSizeY, 1);
        glMemoryBarrier(GL_ALL_BARRIER_BITS); // Ensure memory operations are complete
    }


    // ... (rest of the uniform setting functions remain the same)
};

#endif
