//
// Created by jzy99 on 2021/1/21.
//

#include<phoenix/core/model.h>
#include<spdlog/spdlog.h>
#include<filesystem>
#define STB_IMAGE_IMPLEMENTATION
#include<stb/stb_image.h>
#include<phoenix/core/texture.h>

PHOENIX_NAMESPACE_BEGIN

void Model::Load(const string &path) {
  string dir = path.substr(0, path.find_last_of('/'));
  dir_ = std::filesystem::path(dir);
  area_ = 0.0f;
  dpdf_.Clear();

  Assimp::Importer importer;
  const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenNormals);
  if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
    spdlog::error("load file faild {}", path);
    return;
  }
  processNode(scene->mRootNode, scene);
  dpdf_.normalize();

}

void Model::processNode(aiNode *node, const aiScene *scene) {
  for (unsigned int i = 0; i < node->mNumMeshes; i++) {
    aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
    auto mesh_res = processMesh(mesh, scene);
    area_ += mesh_res->area_;
    dpdf_.Append(mesh_res->area_);
    meshes_.push_back(mesh_res);
  }
  for (unsigned int i = 0; i < node->mNumChildren; i++) {
    processNode(node->mChildren[i], scene);

  }
}

shared_ptr<Mesh> Model::processMesh(aiMesh *mesh, const aiScene *scene) {
  auto result = make_shared<Mesh>();
  std::vector<uint32_t> indices;
  std::vector<Vertex> vertices;
  bool hasTexture = false;

  for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
    Vertex vertex;
    vertex.position = Point3f(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
    vertex.normal = Vector3f(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);

    if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
    {

      // a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't
      // use models where a vertex can have multiple texture coordinates so we always take the first set (0).
      vertex.texcoord = Vector2f(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
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
    } else
      vertex.texcoord = Vector2f(0.0f, 0.0f);
    vertices.push_back(vertex);
  }
  result->vertices = vertices;

  for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
    aiFace face = mesh->mFaces[i];
    for (unsigned int j = 0; j < face.mNumIndices; j++)
      indices.push_back(face.mIndices[j]);
  }
  result->indices = indices;
  result->CalcArea();

  if (mesh->mMaterialIndex >= 0) {
    auto material = scene->mMaterials[mesh->mMaterialIndex];
    if (material->GetTextureCount(aiTextureType_DIFFUSE) > 0) {
      hasTexture = true;
      auto diffuseMaps = LoadMaterialTextures(material,
                                              aiTextureType_DIFFUSE, "texture_diffuse", scene);
      result->texture_ = diffuseMaps;
    }
  }
  return result;
}
SampleData Model::SampleSurface(Point2f sample) const {
  float pdf;
  auto index = dpdf_.Sample(sample.x(), pdf);
  auto res = meshes_[index]->SamplePoint(sample);
  res.pdf *= pdf;
  return meshes_[index]->SamplePoint(sample);
}
shared_ptr<Texture> Model::LoadMaterialTextures(aiMaterial *mat,
                                                aiTextureType type,
                                                std::string typeName,
                                                const aiScene *scene) {

  int c = mat->GetTextureCount(type);
  aiString name;
  mat->GetTexture(type, 0, &name);
  std::filesystem::path now_path(name.C_Str());
  auto file_path = dir_;
  if (!now_path.is_absolute()) {
    file_path /= now_path;

  } else {
    file_path = now_path;
  }
  file_path = absolute(file_path);
  if (exists(file_path)) {
    spdlog::info("texture path {}", file_path.string());
  }
  auto aitexture = scene->GetEmbeddedTexture(name.C_Str());
  unsigned char *image_data = nullptr;
  int width, height, nrChannels;
  if (aitexture != nullptr) {
    if (aitexture->mHeight == 0) {
      image_data = stbi_load_from_memory(reinterpret_cast<unsigned char *>(aitexture->pcData),
                                         aitexture->mWidth,
                                         &width,
                                         &height,
                                         &nrChannels,
                                         3);
    } else {
      image_data = stbi_load_from_memory(reinterpret_cast<unsigned char *>(aitexture->pcData),
                                         aitexture->mWidth * aitexture->mHeight,
                                         &width,
                                         &height,
                                         &nrChannels,
                                         3);
    }
  } else {
    image_data = stbi_load(file_path.string().c_str(), &width, &height, &nrChannels, 3);
  }
  if (image_data != nullptr) {
    spdlog::info("load texture");
  }
  return make_shared<ImageTexture>(image_data, width, height, 3 * width);

}
Color3f Model::GetTextureColor(unsigned int geoid, unsigned int priid, Point2f uv) const {
  auto mesh = meshes_[geoid];
  if (mesh->texture_ == nullptr) {
    return Color3f(0.3, 0.4, 0.5);
  }
  Point2f now_uv = mesh->getTexcood(priid, uv.x(), uv.y());
  return mesh->texture_->GetColor(now_uv);
}

PHOENIX_NAMESPACE_END