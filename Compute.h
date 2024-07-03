#ifndef COMPUTE_H
#define COMPUTE_H

#include <glad/glad.h>

#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

class Compute
{
private:
    int sizeX;
    int sizeY;

public:
    unsigned int id;

    Compute(const char* path, int sizeX, int sizeY) : sizeX(sizeX), sizeY(sizeY) {

        // read in shader code
        std::string compute_code;
        std::ifstream file;

        file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

        try {
            file.open(path);
            std::stringstream file_stream;
            file_stream << file.rdbuf();
            file.close();

            compute_code = file_stream.str();
        }
        catch (std::ifstream::failure e) {
            std::cerr << "failed to read compute shader file" << std::endl;
        }

        const char* c_shader_code = compute_code.c_str();

        // compile shader
        unsigned int shader;

        shader = glCreateShader(GL_COMPUTE_SHADER);
        glShaderSource(shader, 1, &c_shader_code, NULL);
        glCompileShader(shader);

        // Create success flag and array for info log
        int success;
        char infoLog[512];

        // Check the compile status of the compute shader
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            // Get info log and print it out if fail
            glGetShaderInfoLog(shader, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::COMPUTE::COMPILATION_ERROR\n" << infoLog << std::endl;
        }

        // create program
        id = glCreateProgram();
        glAttachShader(id, shader);
        glLinkProgram(id);

        // Check the link status of the compute program
        glGetProgramiv(id, GL_LINK_STATUS, &success);
        if (!success)
        {
            // Get info log and print it out if fail
            glGetProgramInfoLog(id, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::PROGRAM::LINK_ERROR\n" << infoLog << std::endl;
        }

        // cleanup
        glDeleteShader(shader);
    }

    ~Compute() {
        glDeleteProgram(id);
    }

    void use() {
        glUseProgram(id);
    }

    void dispatch() {
        // just keep it simple, 2d work group
        glDispatchCompute(sizeX, sizeY, 1);
    }

    void wait() {
        glMemoryBarrier(GL_ALL_BARRIER_BITS);
    }
};

#endif