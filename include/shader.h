#ifndef SHADER_H
#define SHADER_H

#include<GL/glew.h>
#include<glm/glm.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader
{
    public:
        unsigned int ID;

        Shader(const char *vertexPath, const char *fragmentPath);
        void Use();

        void setMat4(const std::string &name, const glm::mat4 &mat) const;
        void setInt(const std::string &name, int value) const;

        virtual ~Shader();

    protected:

    private:
};

#endif // SHADER_H
