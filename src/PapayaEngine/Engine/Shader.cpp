#include <glew.h>
#include "Shader.hpp"
#include <string>
#include <fstream>
#include <sstream>
//console.log

/*
----------------------------------------------------------

    Shader class based on shader class from learnOpenGL

----------------------------------------------------------
*/

pap::Shader::Shader(const char *vertexPath, const char *fragmentPath) {

    // Text of the shaders Vertex and Fragment
    std::string vertexCode;
    std::string fragCode;
    // File path of the shaders
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;

    // No exeption catch for optimization
    //try {
    // open files
    vShaderFile.open(vertexPath);
    fShaderFile.open(fragmentPath);
    std::stringstream vShaderStream, fShaderStream;
    // read files
    vShaderStream << vShaderFile.rdbuf();
    fShaderStream << fShaderFile.rdbuf();
    // close files
    vShaderFile.close();
    fShaderFile.close();

    // set to stream the data
    vertexCode   = vShaderStream.str();
    fragCode = fShaderStream.str();
    const char *vShaderCode = vertexCode.c_str();
    const char *fShaderCode = fragCode.c_str();



    glewInit();

    unsigned int vertex, fragment;
    int success;
    char infoLog[512];

    // vertex Shader
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);
    // check for shader compile errors
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if(!success) {
        glGetShaderInfoLog(vertex, 512, NULL, infoLog);
    };

    // fragment shader
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);

    // check for shader compile errors
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragment, 512, NULL, infoLog);
    }

    // shader Program
    programID = glCreateProgram();
    glAttachShader(programID, vertex);
    glAttachShader(programID, fragment);
    glLinkProgram(programID);
    // check for shader link errors
    glGetProgramiv(programID, GL_LINK_STATUS, &success);
    if(!success) {
        glGetProgramInfoLog(programID, 512, NULL, infoLog);
    }

    // delete the shaders and clean memory
    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

pap::Shader::~Shader() {
    // Proper way to delete shader program
    glUseProgram(0); // don't use an existant program

    //delete this program
    glDeleteProgram(programID);
}

void pap::Shader::use() {

    glUseProgram(programID);
}



void pap::Shader::setBool(const std::string &name, bool value) const {
    glUniform1i(glGetUniformLocation(programID, name.c_str()), (int)value);
}

void pap::Shader::setInt(const std::string &name, int value) const {
    glUniform1i(glGetUniformLocation(programID, name.c_str()), value);
}

void pap::Shader::setFloat(const std::string &name, float value) const {
    glUniform1f(glGetUniformLocation(programID, name.c_str()), value);
}

void pap::Shader::setVec2(const std::string &name, const glm::vec2 &value) const {
    glUniform2fv(glGetUniformLocation(programID, name.c_str()), 1, &value[0]);
}

void pap::Shader::setVec2(const std::string &name, float x, float y) const {
    glUniform2f(glGetUniformLocation(programID, name.c_str()), x, y);
}

void pap::Shader::setVec3(const std::string &name, const glm::vec3 &value) const {
    glUniform3fv(glGetUniformLocation(programID, name.c_str()), 1, &value[0]);
}

void pap::Shader::setVec3(const std::string &name, float x, float y, float z) const {
    glUniform3f(glGetUniformLocation(programID, name.c_str()), x, y, z);
}

void pap::Shader::setVec4(const std::string &name, const glm::vec4 &value) const {
    glUniform4fv(glGetUniformLocation(programID, name.c_str()), 1, &value[0]);
}

void pap::Shader::setVec4(const std::string &name, float x, float y, float z, float a) const {
    glUniform4f(glGetUniformLocation(programID, name.c_str()), x, y, z, a);
}

void pap::Shader::setMat4(const std::string &name, const glm::mat4 &mat) const {
    glUniformMatrix4fv(glGetUniformLocation(programID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}