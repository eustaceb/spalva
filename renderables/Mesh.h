#ifndef _Mesh_H_
#define _Mesh_H_

#include <memory>
#include <string>
#include <vector>
#include <glad/glad.h>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <assimp/types.h>

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoords;
};

struct Texture {
    GLuint id;
    std::string type;
    aiString path;
};

class Shader;

class Mesh {
public:
    std::vector<Vertex> m_Vertices;
    std::vector<GLuint> m_Indices;
    std::vector <Texture> m_Textures;

    Mesh(std::vector<Vertex> vertices, std::vector<GLuint> indices, std::vector<Texture> textures);
    ~Mesh();

    GLuint getVAO();

    void render(std::shared_ptr<Shader> shader);
private:
    GLuint m_VBO, m_VAO, m_EBO;
    void setupMesh();
};


#endif //_Mesh_H_
