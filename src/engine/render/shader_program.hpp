#pragma once
#include <filesystem>
#include "config.hpp"

namespace fs = std::filesystem;

class ShaderProgram {
    unsigned int programID = 0;
public:
    ShaderProgram(const fs::path vertexPath, const fs::path fragmentPath);
    ~ShaderProgram();
    //
    void use() const;
    unsigned int getID() const { return programID; }
    //
    void setUniformMat4 (const char* name, const float* mat4_column_major) const;
    void setUniformVec4 (const char* name, const float* vec4) const;
    void setUniformInt  (const char* name, int value) const;
    void setUniformFloat(const char* name, float value) const;
private:
    int location(const char* name) const;
    t1_disable_copy_and_move(ShaderProgram)
};
