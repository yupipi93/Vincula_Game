#include "ResourceManager.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
//console.log
#include <ft2build.h> // Freetype
#include FT_FREETYPE_H


const uint16_t MAX_RESOURCES = 10000;

pap::ResourceManager::ResourceManager() {
    resources.reserve(MAX_RESOURCES);
    textures.reserve(MAX_RESOURCES);
}

pap::ResourceManager::~ResourceManager() {

}

const aiScene *pap::ResourceManager::fileExist(std::string p, Assimp::Importer &import) {
    const aiScene *scene = import.ReadFile(p, aiProcess_Triangulate | aiProcess_FlipUVs);

    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        return nullptr;
    }

    return scene;
}

pap::ResourceModel *pap::ResourceManager::getResourceModel (std::string p) {
    for (auto &resource : resources) {
        if(p == resource.get()->path) {
            // if found, return resource
            return resource.get();
        }
    }
    Assimp::Importer import;
    const aiScene *scene = fileExist(p, import);

    if(scene != nullptr) {
        resources.emplace_back( std::make_unique<ResourceModel>(p, scene, *this) );
        return resources.back().get();
    }
    return nullptr;
}

pap::ResourceModel *pap::ResourceManager::getGeometryModel (std::string p, std::string texture, std::string texType) {
    std::string filename = p + "?" + texture;
    for (auto &resource : resources) {
        if(filename == resource.get()->path) {
            // if found, return resource
            return resource.get();
        }
    }

    resources.emplace_back( std::make_unique<pap::ResourceModel>(filename, p, *this, texture, texType) );
    return resources.back().get();
}

Texture pap::ResourceManager::getResourceTexture (std::string p, std::string typeName) {
    for (auto &texture : textures) {
        if(p == texture.path) {
            // if found, return resource
            return texture;
        }
    }

    if(!createTexture(p, typeName)) { // if error
        return getResourceTexture("./media/error/ErrorTexture.png", typeName);
    }

    return textures.back();
}


bool pap::ResourceManager::createTexture(const std::string path, std::string typeName) {

    unsigned int textureprogramID;
    glGenTextures(1, &textureprogramID);

    if(typeName == "texture2D") {
        stbi_set_flip_vertically_on_load(true);
    } else {
        stbi_set_flip_vertically_on_load(false);
    }

    int width, height, nrComponents;
    unsigned char *data = stbi_load(path.c_str(), &width, &height, &nrComponents, 0);

    if (data) {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureprogramID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        stbi_image_free(data);

        // Create the texture
        Texture texture;
        texture.id = textureprogramID;
        texture.type = typeName;
        texture.path = path;
        textures.push_back(texture); // add to loaded textures

        return true;

    } else {
        stbi_image_free(data);
    }

    return false;
}