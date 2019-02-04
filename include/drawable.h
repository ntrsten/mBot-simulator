#ifndef DRAWABLE_H
#define DRAWABLE_H

#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include <cstring>

#include <GL/glew.h>
#include <SDL/SDL.h>
#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>

#include "shader.h"

struct Vertex {
    glm::vec3 Position;
    glm::vec2 TexCoords;
};

class Drawable
{
    public:
        Drawable(const char *OBJpath, const char *BMPpath);
        Drawable(unsigned int textureID);

        unsigned int VAO, VBO, EBO, texture;

        void Draw(Shader *shader,
                  glm::vec3 translate_vector,
                  float yaw_angle,
                  float pitch_angle,
                  glm::vec3 rotate_vector,
                  glm::vec3 scale_vector);

        virtual ~Drawable();

    protected:

    private:
        void LoadOBJFile(
            const char *OBJpath,
            std::vector<Vertex> & vertices,
            std::vector<unsigned int> & indices
        );

        unsigned int LoadBMPFile(
            const char *BMPpath
        );

        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;

};

#endif // DRAWABLE_H
