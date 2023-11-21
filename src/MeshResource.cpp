#include "MeshResource.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

static const std::vector<std::string> supportedExtentions = {
    ".obj"
};

void* ModelResource::getResourceData() const
{
    return (void*) & m_meshes;
}

void* ModelResource::getResourceExtra() const
{
    return (void*) & m_textures;
}

std::vector<std::string> MeshLoader::getLoaderSupportedExtentions() const
{
    return supportedExtentions;
}

std::shared_ptr<IResource> MeshLoader::loadResource(const ResourceHandle& handle) 
{
    m_filename = handle.getResourceName();
    m_resource = std::make_shared<ModelResource>();
    loadModel();
    return m_resource;
}

bool MeshLoader::supportSaving() const
{
    return false;
}

void MeshLoader::saveResource(const std::shared_ptr<IResource>& resource, const std::string& filename) const
{
    return;
}

void MeshLoader::loadModel()
{
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(m_filename, 
        aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) 
    {
        return;
    }
    processNode(scene->mRootNode, scene);
}

void MeshLoader::processNode(aiNode* node, const aiScene* scene)
{
    for (unsigned int i = 0; i < node->mNumMeshes; ++i)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        std::shared_ptr<ModelResource> resource = std::dynamic_pointer_cast<ModelResource>(m_resource);
        resource->m_meshes.push_back(processMesh(mesh, scene));
    }
    for (unsigned int i = 0; i < node->mNumChildren; i++)
    {
        processNode(node->mChildren[i], scene);
    }
}

ModelResource::Mesh MeshLoader::processMesh(aiMesh* mesh, const aiScene* scene)
{
    ModelResource::Mesh outMesh;

    for (int i = 0; i < mesh->mNumVertices; ++i)
    {
        Vec3 position;
        Vec3 normal;
        Vec2 texCoords;

        position = { mesh->mVertices[i].x,
            mesh->mVertices[i].y,
            mesh->mVertices[i].z };

        outMesh.geometry.vertexArray.push_back(position);

        if (mesh->HasNormals())
        {
            normal = { mesh->mNormals[i].x,
                mesh->mNormals[i].y,
                mesh->mNormals[i].z };
            outMesh.geometry.normals.push_back(normal);
        }

        if (mesh->mTextureCoords[0])
        {
            texCoords = { mesh->mTextureCoords[0][i].x,
                mesh->mTextureCoords[0][i].y };
            outMesh.geometry.texCoords.push_back(texCoords);
        }
    }

    for (unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++)
        {
            outMesh.geometry.indices.push_back(face.mIndices[j]);
        }
    }

    aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
    aiColor4D color{ 0.0f, 0.0f, 0.0f, 0.0f };

    aiGetMaterialColor(material, AI_MATKEY_COLOR_AMBIENT, &color);
    outMesh.material.ambientColor = { (unsigned char)(255 * color.r),
        (unsigned char)(255 * color.g),
        (unsigned char)(255 * color.b),
        (unsigned char)(255 * color.a) };

    aiGetMaterialColor(material, AI_MATKEY_COLOR_DIFFUSE, &color);
    outMesh.material.diffuseColor = { (unsigned char)(255 * color.r),
        (unsigned char)(255 * color.g),
        (unsigned char)(255 * color.b),
        (unsigned char)(255 * color.a) };

    aiGetMaterialColor(material, AI_MATKEY_COLOR_SPECULAR, &color);
    outMesh.material.specularColor = { (unsigned char)(255 * color.r),
        (unsigned char)(255 * color.g),
        (unsigned char)(255 * color.b),
        (unsigned char)(255 * color.a) };

    return outMesh;
}
