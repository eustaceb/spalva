//
// Created by eustace on 25/05/17.
//
#pragma once
#ifndef _Model_H_
#define _Model_H_


#include <glad/glad.h>
#include <vector>
#include <string>
#include <map>
#include <memory>
#include <assimp/scene.h>

#include "../Shader.h"
#include "Renderable.h"
#include "Mesh.h"

class Shader;

class Model : public Renderable {
public:
    /*  Functions   */
    Model(std::shared_ptr<Shader> shader, const glm::vec3 &pos, const GLchar *path, 
        const std::string &label = "Model");
    ~Model();

    void render();
    void addMesh(Mesh* m);
protected:
    std::vector<Mesh*> m_Meshes;
    std::string m_Directory;
    std::vector<Texture> m_TexturesLoaded;

    void loadModel(std::string path);
private:
    void processNode(aiNode* node, const aiScene* scene);
    void processMesh(aiMesh* mesh, const aiScene* scene);
    std::vector<Texture> loadMaterialTextures(aiMaterial* material, aiTextureType type,
                                         std::string typeName);
    GLuint load_texture(const char *filename, std::string directory);
};

#endif //_Model_H_
