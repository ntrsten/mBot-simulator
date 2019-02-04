#include "drawable.h"
#include <iostream>


void Drawable::LoadOBJFile(
            const char *OBJpath,
            std::vector<Vertex> & vertices,
            std::vector<unsigned int> & indices)
{
    std::vector<float> verticesInput;
    std::vector<float> textureCoordsInput;

    std::ifstream infile(OBJpath);
    if (!infile){
//        std::cout << OBJpath << " file missing!" << std::endl;
        return;
    }
    std::string line;

    int indicesIndex = 0;

    while(std::getline(infile, line))
    {
        float coord;
        unsigned int vertexIndex;
        char idWord[64];

        std::istringstream lineInput(line);
        lineInput >> idWord;

        if(strcmp(idWord, "v")==0)
        {
            for(int i = 0; i < 3; i++)
            {
                lineInput >> coord;
                verticesInput.push_back(coord);
            }
        }
        if(strcmp(idWord, "vt")==0)
        {
            for(int i = 0; i < 2; i++)
            {
                lineInput >> coord;
                textureCoordsInput.push_back(coord);
            }
        }
        else if(strcmp(idWord, "f")==0)
        {
            char charWaste;
            int intWaste;
            unsigned int texIndex;
            for(int i = 0; i < 3; i++)
            {
                Vertex vertex;
                lineInput >> vertexIndex >> charWaste >> texIndex >> charWaste >> intWaste;
                vertex.Position = glm::vec3(verticesInput[(vertexIndex - 1) * 3],
                                            verticesInput[(vertexIndex - 1) * 3 + 1],
                                            verticesInput[(vertexIndex - 1) * 3 + 2]);
                vertex.TexCoords = glm::vec2(textureCoordsInput[(texIndex - 1) * 2],
                                             textureCoordsInput[(texIndex - 1) * 2 + 1]);
                vertices.push_back(vertex);
                indices.push_back(indicesIndex++);
            }
        }
    }
    infile.close();
}

unsigned int Drawable::LoadBMPFile(const char *BMPpath)
{
    std::ifstream infile(BMPpath, std::ifstream::binary);
//    if (!infile) std::cout << BMPpath << " file missing!" << std::endl;
    //BMP header consists of 54 bytes
    char *bmpHeader = new char [54];
    unsigned int textureID;
    infile.read(bmpHeader, 54);
    if(bmpHeader[0] != 'B' && bmpHeader[1] != 'M')
    {
//        std::cout << "Wrong texture file encoding. Use BMP file." << std::endl;
    }
    else
    {
        unsigned int width = *(int*)&(bmpHeader[0x12]);
        unsigned int height = *(int*)&(bmpHeader[0x16]);
        unsigned int numberOfPixels = width * height;

        char* buffer = new char[numberOfPixels * 3];
        infile.read(buffer, numberOfPixels * 3);

        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, (unsigned char*)buffer);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    }
    infile.close();
    return textureID;
}

void Drawable::Draw(Shader *shader,
                    glm::vec3 translate_vector,
                    float yaw_angle,
                    float pitch_angle,
                    glm::vec3 rotate_vector,
                    glm::vec3 scale_vector){
    glBindVertexArray(VAO);
    glm::mat4 model;
    model = glm::translate(model, translate_vector);
    model = glm::rotate(model, glm::radians(pitch_angle), rotate_vector);
    model = glm::rotate(model, glm::radians(yaw_angle), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::scale(model, scale_vector);
    shader->setMat4("model", model);

    glBindTexture(GL_TEXTURE_2D, texture);

    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

    glBindVertexArray(0);
}

Drawable::Drawable(const char *OBJpath, const char *BMPpath)
{
    this->LoadOBJFile(OBJpath, vertices, indices);
    texture = this->LoadBMPFile(BMPpath);

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

Drawable::Drawable(unsigned int textureID )
{
    Vertex v;
    v.Position = glm::vec3(0.0f, 0.0f, 0.1f);
    v.TexCoords = glm::vec2(1.0f, 1.0f);
    vertices.push_back(v);
    v.Position = glm::vec3(0.0f, 0.0f, 0.0f);
    v.TexCoords = glm::vec2(0.0f, 1.0f);
    vertices.push_back(v);
    v.Position = glm::vec3(0.0f, 0.065f, 0.1f);
    v.TexCoords = glm::vec2(1.0f, 0.0f);
    vertices.push_back(v);
    v.Position = glm::vec3(0.0f, 0.065f, 0.0f);
    v.TexCoords = glm::vec2(0.0f, 0.0f);
    vertices.push_back(v);

    indices.push_back(1); indices.push_back(2); indices.push_back(0);
    indices.push_back(1); indices.push_back(3); indices.push_back(2);

    texture = textureID;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

Drawable::~Drawable()
{
    glDeleteTextures(1, &texture);
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}
