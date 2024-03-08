#pragma once
#include <string>
#include <memory>
#include <vector>
#include <PapayaEngine/Engine/Resource.hpp>
#include <PapayaEngine/Engine/ResourceModel.hpp>
#include <PapayaEngine/Engine/Texture.hpp>
#include <PapayaEngine/Engine/Character.hpp>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

namespace pap{

class ResourceManager {
public:
    ResourceManager();
    ~ResourceManager();

    pap::ResourceModel *getResourceModel (std::string p);
    pap::ResourceModel *getGeometryModel (std::string p, std::string texture, std::string texType = "texture");
	Texture 	   getResourceTexture (std::string p, std::string typeName);

private:
    bool createTexture(const std::string path, std::string typeName);
    const aiScene *fileExist(std::string p, Assimp::Importer &import);


    std::vector< std::unique_ptr<ResourceModel> > resources;
    std::vector<Texture> textures;
};

}