//
// Created by jzy99 on 2021/1/21.
//

#include<phoenix/core/model.h>
#include<spdlog/spdlog.h>

PHOENIX_NAMESPACE_BEGIN


void Model::Load(const string& path) {
  string dir = path.substr(0, path.find_last_of('/'));

  Assimp::Importer importer;
  const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate|aiProcess_GenNormals);
  if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
  {
    spdlog::error("load file faild {}",path);
    return;
  }
  processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode* node, const aiScene* scene)
{
  for (unsigned int i = 0; i < node->mNumMeshes; i++)
  {
    aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
    meshes.push_back(processMesh(mesh, scene));
  }
  for (unsigned int i = 0; i < node->mNumChildren; i++)
  {
    processNode(node->mChildren[i], scene);
  }
}

shared_ptr<Mesh> Model::processMesh(aiMesh* mesh, const aiScene* scene)
{
  auto result = make_shared<Mesh>();
  std::vector<uint32_t> indices;
  std::vector<Vertex> vertices;
  bool hasTexture=false;



  for (unsigned int i = 0; i < mesh->mNumVertices; i++)
  {
    Vertex vertex;
    vertex.position = Point3f(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
    vertex.normal = Vector3f(mesh->mNormals[i].x,mesh->mNormals[i].y,mesh->mNormals[i].z);


    if(mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
    {

      // a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't
      // use models where a vertex can have multiple texture coordinates so we always take the first set (0).
      vertex.texcoord = Vector2f(mesh->mTextureCoords[0][i].x,mesh->mTextureCoords[0][i].y);
      // tangent
//            vector.x = mesh->mTangents[i].x;
//            vector.y = mesh->mTangents[i].y;
//            vector.z = mesh->mTangents[i].z;
//            vertex.Tangent = vector;
//            // bitangent
//            vector.x = mesh->mBitangents[i].x;
//            vector.y = mesh->mBitangents[i].y;
//            vector.z = mesh->mBitangents[i].z;
//            vertex.Bitangent = vector;
    }
    else
      vertex.texcoord = Vector2f(0.0f, 0.0f);
    vertices.push_back(vertex);
  }
  result->vertices = vertices;

  for (unsigned int i = 0; i < mesh->mNumFaces; i++)
  {
    aiFace face = mesh->mFaces[i];

    for (unsigned int j = 0; j < face.mNumIndices; j++)
      indices.push_back(face.mIndices[j]);
  }
  result->indices = indices;
  return result;
}






PHOENIX_NAMESPACE_END