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

class Texture;
class Shader;

class Mesh {
public:
    std::vector<Vertex> m_Vertices;
    std::vector<GLuint> m_Indices;
    std::vector <std::shared_ptr<Texture>> m_Textures;

    Mesh(std::vector<Vertex> vertices, std::vector<GLuint> indices, 
        std::vector<std::shared_ptr<Texture>> textures);
    ~Mesh();

    GLuint getVAO() const;

    void render(std::shared_ptr<Shader> shader);
private:
    GLuint m_VBO, m_VAO, m_EBO;
    void setupMesh();
};


#endif //_Mesh_H_
