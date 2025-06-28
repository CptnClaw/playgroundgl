#include <memory>   
#include <iostream>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <glm/gtc/matrix_transform.hpp>
#include "mesh.h"
#include "model.h"

#define PI 3.14159f

Model::Model(const std::string &filepath) 
{ 
    Assimp::Importer importer;
    const aiScene *scene = importer.ReadFile(filepath, aiProcess_Triangulate | aiProcess_FlipUVs);
    if (!scene || !scene->mRootNode || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE)
    {
        std::cout << "Error importing model: " << filepath << std::endl;
        std::cout << importer.GetErrorString() << std::endl;
    }
    else
    {
        directory = filepath.substr(0, filepath.find_last_of('/') + 1);
        processNode(scene->mRootNode, scene);
    }
    world_transform = glm::mat4(1.f); 
    print_debug_stats(filepath);
}

void Model::print_debug_stats(const std::string &filepath)
{
    std::cout << "Model " << filepath << " loaded successfully with ";
    int cnt_diffuse = 0;
    int cnt_specular = 0;
    for (const std::unique_ptr<Texture> &tex : texture_pool)
    {
        switch (tex->type)
        {
        case TextureType::Diffuse:
            cnt_diffuse++;
            break;
        case TextureType::Specular:
            cnt_specular++;
            break;
        default:
            break;
        }
    }
    std::cout << cnt_diffuse << " diffuse textures and ";
    std::cout << cnt_specular << " specular textures" << std::endl;
}

void Model::processNode(aiNode *node, const aiScene *scene)
{
    // Process all meshes in this node
    for (uint i = 0; i < node->mNumMeshes; i++)
    {
        processMesh(scene->mMeshes[node->mMeshes[i]], scene);
    }
    
    // Recursively process this node's children
    for (uint i = 0; i < node->mNumChildren; i++)
    {
        processNode(node->mChildren[i], scene);
    }
}

void Model::processMesh(aiMesh *mesh, [[maybe_unused]] const aiScene *scene)
{
    // Read off vertices
    std::vector<Vertex> vertices;
    for (uint i = 0; i < mesh->mNumVertices; i++)
    {
        aiVector3D aiPosition = mesh->mVertices[i];
        glm::vec3 position(aiPosition.x, aiPosition.y, aiPosition.z);

        aiVector3D aiNormal = mesh->mNormals[i];
        glm::vec3 normal(aiNormal.x, aiNormal.y, aiNormal.z);

        aiVector3D * aiTextureCoord = mesh->mTextureCoords[0];
        glm::vec2 texture_coord(0.f, 0.f);
        if (aiTextureCoord)
        {
            texture_coord.x = aiTextureCoord[i].x;
            texture_coord.y = aiTextureCoord[i].y;
        }
        vertices.push_back({position, normal, texture_coord});
    }

    // Read off indices
    std::vector<uint> indices;
    indices.reserve(mesh->mNumFaces * 3);
    for (uint i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        indices.insert(indices.end(), face.mIndices, face.mIndices + face.mNumIndices);
    }
    
    // Read off textures
    std::vector<TextureHandle> textures;
    if (mesh->mMaterialIndex >= 0)
    {
        aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
        aiString str;
        for (uint i = 0; i < material->GetTextureCount(aiTextureType_DIFFUSE); i++)
        {
            material->GetTexture(aiTextureType_DIFFUSE, i, &str);
            std::string texture_path = directory + std::string(str.C_Str());
            uint texture_id = lazy_add_to_pool(texture_path, TextureType::Diffuse);
            textures.emplace_back(TextureHandle{texture_id, TextureType::Diffuse});
        }
        for (uint i = 0; i < material->GetTextureCount(aiTextureType_SPECULAR); i++)
        {
            material->GetTexture(aiTextureType_SPECULAR, i, &str);
            std::string texture_path = directory + std::string(str.C_Str());
            uint texture_id = lazy_add_to_pool(texture_path, TextureType::Specular);
            textures.emplace_back(TextureHandle{texture_id, TextureType::Specular});
        }
    }

    // Create a mesh object in-place
    meshes.emplace_back(std::make_unique<Mesh>(vertices, indices, std::move(textures)));
}

uint Model::lazy_add_to_pool(const std::string &texture_path, TextureType type)
{
    // Check if texture already exists in pool
    for (const std::unique_ptr<Texture> &texture : texture_pool)
    {
        if (texture_path == texture->filepath)
        {
            // Already exists, do not load again
            return texture->id;
        }
    }
    
    // Texture needs to actually load from file
    std::unique_ptr<Texture> newtexture = std::make_unique<Texture>(texture_path, type);
    uint texture_id = newtexture->id;
    texture_pool.emplace_back(std::move(newtexture));
    return texture_id;
}

void Model::spin(float delta_time)
{
    float speed = 1.0;
    float angle = delta_time * speed;
    rotate(angle * -PI / 3, 1.f, 2.f, 0.f);
}

void Model::translate(float x, float y, float z)
{
    world_transform = glm::translate(world_transform, glm::vec3(x, y, z));
}

void Model::scale(float amount)
{
    world_transform = glm::scale(world_transform, glm::vec3(amount));
}

void Model::rotate(float angle, float axis_x, float axis_y, float axis_z)
{
    world_transform = glm::rotate(world_transform, angle, glm::vec3(axis_x, axis_y, axis_z));
}

void Model::draw(const Shaders &program, const glm::mat4 &view, const glm::mat4 &projection) const
{
    // Set up transform uniforms
    program.use();
    glm::mat4 mv = view * world_transform;
    glm::mat4 mvp = projection * mv;
    program.uniform_mat4("mv", mv);
    program.uniform_mat4("mvp", mvp);
    program.uniform_mat3("mv_for_normals", glm::transpose(glm::inverse(mv)));

    // Draw all meshes
    for (const std::unique_ptr<Mesh> &m : meshes)
    {
        m->draw(program);
    }
}