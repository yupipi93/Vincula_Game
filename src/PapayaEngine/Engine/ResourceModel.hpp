#pragma once
#include <PapayaEngine/Engine/Resource.hpp>
#include <PapayaEngine/Engine/Mesh.hpp>
#include <PapayaEngine/Engine/Shader.hpp>
#include <PapayaEngine/Engine/Texture.hpp>
#include <vector>
#include <string>
#include <assimp/scene.h>           // Output data structure

namespace pap {
class ResourceManager;
} // NAMESPACE


namespace pap {

class ResourceModel : public pap::Resource {
public:
    /*  Functions   */
    ResourceModel(string p, string geometry, pap::ResourceManager &resManager, string texture, string texType = "texture");
    ResourceModel(string p, const aiScene *scene, pap::ResourceManager &resManager);
    ~ResourceModel();
    void draw(pap::Shader &shader) const ;
    void draw2D(pap::Shader &shader, glm::vec2 translation, glm::vec2 scale, glm::mat4 projection2d, glm::vec4 color)const ;
    vector<Mesh> &getMeshes();

private:
    /*  Model Data  */
    vector<Mesh> meshes;
    string directory;
    //vector<Texture> textures_loaded;
    /*  Functions   */
    void loadModel(string p, pap::ResourceManager &resManager);
    void processNode(aiNode *node, const aiScene *scene, pap::ResourceManager &resManager);
    Mesh processMesh(aiMesh *mesh, const aiScene *scene, pap::ResourceManager &resManager);
    void createNode(string geometry, string texture, pap::ResourceManager &resManager, string texType);
    Mesh createMesh(float aVertices[], int sAVertices, unsigned int aIndices[], int sAIndices, string texture, pap::ResourceManager &resManager, string texType);
    vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type,
                                         string typeName, pap::ResourceManager &resManager);
    //unsigned int textureFromFile(const char *p, const string &directory);
};

} // NAMESPACE