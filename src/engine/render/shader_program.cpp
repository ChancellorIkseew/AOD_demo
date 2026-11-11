#include "shader_program.hpp"
//
#include "engine/debug/logger.hpp"
#include "engine/io/io.hpp"
#include "glad/glad.h"

static debug::Logger logger("shaders");

static unsigned int compileShader(unsigned int type, const fs::path path) {
    std::string source = io::readFile(path, io::Log::only_error);
    if (source.empty()) {
        logger.error("Shader source is empty: {}", path.string());
        return 0;
    }
    const char* sourcePtr = source.c_str();
    const unsigned int shaderID = glCreateShader(type);
    glShaderSource(shaderID, 1, &sourcePtr, nullptr);
    glCompileShader(shaderID);

    int success;
    glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
    if (!success) {
        int logLen = 0;
        glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &logLen);
        std::string infoLog(logLen, '\0');
        glGetShaderInfoLog(shaderID, logLen, nullptr, infoLog.data());
        logger.error("Shader Compilation Error ({}):\n{}", path.string(), infoLog);
    }
    return shaderID;
}

ShaderProgram::ShaderProgram(const fs::path vertexPath, const fs::path fragmentPath) {
    const unsigned int vertexShader   = compileShader(GL_VERTEX_SHADER,   vertexPath);
    const unsigned int fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentPath);
    programID = glCreateProgram();
    glAttachShader(programID, vertexShader);
    glAttachShader(programID, fragmentShader);
    glLinkProgram(programID);

    int success;
    glGetProgramiv(programID, GL_LINK_STATUS, &success);
    if (!success) {
        int logLen = 0;
        glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &logLen);
        std::string infoLog(logLen, '\0');
        glGetProgramInfoLog(programID, logLen, nullptr, infoLog.data());
        logger.error("Shader Linking Error ({} + {}):\n{}",
                     vertexPath.string(), fragmentPath.string(), infoLog);
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    logger.info("Shader program built: {} + {}", vertexPath.string(), fragmentPath.string());
}

ShaderProgram::~ShaderProgram() {
    glDeleteProgram(programID);
}

void ShaderProgram::use() const {
    glUseProgram(programID);
}

int ShaderProgram::location(const char* name) const {
    int loc = glGetUniformLocation(programID, name);
    if (loc < 0)
        logger.warning("Uniform not found or optimized out: \"{}\"", name);
    return loc;
}

void ShaderProgram::setUniformMat4(const char* name, const float* mat4) const {
    glUniformMatrix4fv(location(name), 1, GL_FALSE, mat4);
}
void ShaderProgram::setUniformVec4(const char* name, const float* vec4) const {
    glUniform4fv(location(name), 1, vec4);
}
void ShaderProgram::setUniformInt(const char* name, int value) const {
    glUniform1i(location(name), value);
}
void ShaderProgram::setUniformFloat(const char* name, float value) const {
    glUniform1f(location(name), value);
}
