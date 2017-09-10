//
// Created by eustace on 25/05/17.
//

#include <iostream>
#include <sstream>

#include "Model.h"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <stb/stb_image.h>
#include <glm/glm.hpp>

#include "../resourcing/Texture.h"

Model::Model(std::shared_ptr<Shader> shader, const glm::vec3 &pos, 
    const GLchar *path, const std::string &label)
    : Renderable(shader, pos, label)
{
    if (std::strcmp(path, "") != 0)
        this->loadModel(path);
}

void Model::render()
{
    for (unsigned int i = 0; i < m_Meshes.size(); i++) {
        m_Meshes[i]->render(m_Shader);
    }
}

void Model::loadModel(std::string path)
{
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
        return;
    }
    this->m_Directory = path.substr(0, path.find_last_of('/'));

    this->processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode *node, const aiScene *scene)
{
    for (GLuint i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
        this->processMesh(mesh, scene);
    }
    for (GLuint i = 0; i < node->mNumChildren; i++)
        processNode(node->mChildren[i], scene);
}

void Model::processMesh(aiMesh *mesh, const aiScene *scene)
{
    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;
    std::vector <std::shared_ptr<Texture>> textures;
    // Process vertices
    for (GLuint i = 0; i < mesh->mNumVertices; i++)
    {
        Vertex v;
        v.position = glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
        v.normal = glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
        if(mesh->mTextureCoords[0]) // Check if mesh has tex coords
            v.texCoords = glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
        else
            v.texCoords = glm::vec2(0.0f, 0.0f);
        vertices.push_back(v);
    }
    // Faces
    for(GLuint i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        for(GLuint j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }
    if (scene->mNumMaterials > 0)
    {
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
        std::vector <std::shared_ptr<Texture>> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
        std::vector <std::shared_ptr<Texture>> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
        textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
    }
    m_Meshes.push_back(new Mesh(vertices, indices, textures));
}

std::vector<std::shared_ptr<Texture>> Model::loadMaterialTextures(aiMaterial *material, aiTextureType type, std::string typeName)
{
    std::vector <std::shared_ptr<Texture>> textures;
    for (GLuint i = 0; i < material->GetTextureCount(type); i++)
    {
        aiString path;
        material->GetTexture(type, i, &path);

        // Check if tex already registered
        GLboolean skip = false;
        for (auto it = m_TexturesLoaded.begin(); it != m_TexturesLoaded.end(); it++)
        {
            if (std::strcmp(path.C_Str(), (*it)->getPath().c_str()) == 0)
            {
                textures.push_back(*it);
                skip = true;
                break;
            }
        }
        if (!skip)
        {
            std::string fullPath = std::string(m_Directory + "/" + path.C_Str());
            auto tex = std::make_shared<Texture>(fullPath, typeName, GL_RGB, GL_RGB);
            textures.push_back(tex);
            m_TexturesLoaded.push_back(tex);
        }
    }
    return textures;
}

Model::~Model()
{
    for (unsigned int i = 0; i < m_Meshes.size(); i++)
        delete m_Meshes[i];
}

void Model::addMesh(Mesh* m)
{
    m_Meshes.push_back(m);
}


