#include "ResourceModel.hpp"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
//console.log
#include <PapayaEngine/Engine/ResourceManager.hpp>


pap::ResourceModel::ResourceModel(string p, string geometry, pap::ResourceManager &resManager, string texture, string texType) {
    path = p;
    createNode(geometry, texture, resManager, texType);
}

pap::ResourceModel::ResourceModel(string p, const aiScene *scene, pap::ResourceManager &resManager) {
    path = p;
    directory = p.substr(0, p.find_last_of('/'));

    processNode(scene->mRootNode, scene, resManager);
    //loadModel(p, resManager);
}


pap::ResourceModel::~ResourceModel(){

}

void pap::ResourceModel::draw(pap::Shader &shader) const {
    for(unsigned int i = 0; i < meshes.size(); i++) {
        meshes[i].Draw(shader);
    }
}

void pap::ResourceModel::draw2D(pap::Shader &shader, glm::vec2 translation, glm::vec2 scale, glm::mat4 projection2d, glm::vec4 color) const{
    for(unsigned int i = 0; i < meshes.size(); i++) {
        meshes[i].Draw2D(shader, translation, scale, projection2d, color);
    }
}


void pap::ResourceModel::loadModel(string p, pap::ResourceManager &resManager) {

    Assimp::Importer import;
    const aiScene *scene = import.ReadFile(p, aiProcess_Triangulate | aiProcess_FlipUVs);

    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        return;
    }

    directory = p.substr(0, p.find_last_of('/'));

    processNode(scene->mRootNode, scene, resManager);

}

void pap::ResourceModel::createNode(string geometry, string texture, pap::ResourceManager &resManager, string texType) {

    if(geometry == "cube") {
        float aVertices[] = {
                //--POSITION----   -----NORMALS-----    -TEX COORD-
                -5.f, -5.f, -5.f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
                5.f, -5.f, -5.f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
                5.f,  5.f, -5.f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
                -5.f,  5.f, -5.f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,

                -5.f, -5.f,  5.f,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f,
                5.f, -5.f,  5.f,  0.0f,  0.0f,  1.0f,  1.0f, 0.0f,
                5.f,  5.f,  5.f,  0.0f,  0.0f,  1.0f,  1.0f, 1.0f,
                -5.f,  5.f,  5.f,  0.0f,  0.0f,  1.0f,  0.0f, 1.0f,

                -5.f,  5.f,  5.f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
                -5.f,  5.f, -5.f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
                -5.f, -5.f, -5.f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
                -5.f, -5.f,  5.f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,

                5.f,  5.f,  5.f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
                5.f,  5.f, -5.f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
                5.f, -5.f, -5.f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
                5.f, -5.f,  5.f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,

                -5.f, -5.f, -5.f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
                5.f, -5.f, -5.f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
                5.f, -5.f,  5.f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
                -5.f, -5.f,  5.f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,

                -5.f,  5.f, -5.f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
                5.f,  5.f, -5.f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
                5.f,  5.f,  5.f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
                -5.f,  5.f,  5.f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f
            };


        unsigned int aIndices[] = {  //2 triangles = 1 face
            0, 2, 1,  // first Triangle
            2, 0, 3,   // second Triangle

            4, 5, 6,  // first Triangle
            6, 7, 4,   // second Triangle

            8, 9, 10,  // first Triangle
            10, 11, 8,   // second Triangle

            12, 14, 13,  // first Triangle
            14, 12, 15,   // second Triangle

            16, 17, 18,  // first Triangle
            18, 19, 16,   // second Triangle

            20, 22, 21,  // first Triangle
            22, 20, 23   // second Triangle
        };

        meshes.push_back(createMesh(aVertices, sizeof(aVertices) / sizeof(*aVertices), aIndices, sizeof(aIndices) / sizeof(*aIndices), texture, resManager, texType));

    }
    if(geometry == "plane2D") {
        float aVertices[] = {
                // ----POSITION----    -----NORMALS-----   -TEX COORD-
                -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
                 1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
                 1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
                -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
            };

        unsigned int aIndices[] = {  //2 triangles = 1 face
            2, 1, 3,  // first Triangle
            1, 0, 3,   // second Triangle
        };
        meshes.push_back(createMesh(aVertices, sizeof(aVertices) / sizeof(*aVertices), aIndices, sizeof(aIndices) / sizeof(*aIndices), texture, resManager, texType));
    }

    if(geometry == "floor") {
        float aVertices[] = {
                // ---POSITION---  ------NORMALS------   -TEX COORD-
                -5.f, -5.f, -5.f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
                 5.f, -5.f, -5.f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
                 5.f, -5.f,  5.f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
                -5.f, -5.f,  5.f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f
            };

        unsigned int aIndices[] = {  //2 triangles = 1 face
            0, 2, 1,  // first Triangle
            2, 0, 3,   // second Triangle
        };
        meshes.push_back(createMesh(aVertices, sizeof(aVertices) / sizeof(*aVertices), aIndices, sizeof(aIndices) / sizeof(*aIndices), texture, resManager, texType));
    }

    if(geometry == "plane") {
        float aVertices[] = {
                // ---POSITION---  ------NORMALS------   -TEX COORD-
                -5.f, -5.f, -5.f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
                 5.f, -5.f, -5.f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
                 5.f, -5.f,  5.f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
                -5.f, -5.f,  5.f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f

            };

        unsigned int aIndices[] = {  //2 triangles = 1 face
            0, 1, 2,  // first Triangle
            2, 3, 0,   // second Triangle
        };
        meshes.push_back(createMesh(aVertices, sizeof(aVertices) / sizeof(*aVertices), aIndices, sizeof(aIndices) / sizeof(*aIndices), texture, resManager, texType));
    }

}

Mesh pap::ResourceModel::createMesh(float aVertices[], int sAVertices, unsigned int aIndices[], int sAIndices, string texture, pap::ResourceManager &resManager, string texType) {

    vector<Vertex> vertices;
    vector<unsigned int> indices;
    vector<Texture> textures;
    unsigned int cont = 0;

    for (unsigned int i = 0; i < sAVertices / 8; i++) {

        Vertex vertex;
        glm::vec3 vector;
        glm::vec2 vectorText;
        vector.x = aVertices[0 + (cont * 8)];
        vector.y = aVertices[1 + (cont * 8)];
        vector.z = aVertices[2 + (cont * 8)];
        vertex.Position = vector;

        vector.x = aVertices[3 + (cont * 8)];
        vector.y = aVertices[4 + (cont * 8)];
        vector.z = aVertices[5 + (cont * 8)];
        vertex.Normal = vector;

        vectorText.x = aVertices[6 + (cont * 8)];
        vectorText.y = aVertices[7 + (cont * 8)];

        vertex.TexCoords = vectorText;
        vertices.push_back(vertex);

        cont += 1;
    }
    for (unsigned int i = 0; i < sAIndices; i++) {
        indices.push_back(aIndices[i]);
    }


    //Process textures

    textures.push_back(resManager.getResourceTexture(texture, texType));


    return Mesh(vertices, indices, textures);
}

void pap::ResourceModel::processNode(aiNode *node, const aiScene *scene, pap::ResourceManager &resManager) {
    // process all the node's meshes (if any)
    for(unsigned int i = 0; i < node->mNumMeshes; i++) {
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(processMesh(mesh, scene, resManager));

    }

    // then do the same for each of its children
    for(unsigned int i = 0; i < node->mNumChildren; i++) {

        processNode(node->mChildren[i], scene, resManager);
    }

}


Mesh pap::ResourceModel::processMesh(aiMesh *mesh, const aiScene *scene, pap::ResourceManager &resManager) {
    vector<Vertex> vertices;
    vector<unsigned int> indices;
    vector<Texture> textures;


    for(unsigned int i = 0; i < mesh->mNumVertices; i++) {
        Vertex vertex;
        // process vertex positions, normals and texture coordinates
        glm::vec3 vector;
        vector.x = mesh->mVertices[i].x;
        vector.y = mesh->mVertices[i].y;
        vector.z = mesh->mVertices[i].z;
        vertex.Position = vector;

        vector.x = mesh->mNormals[i].x;
        vector.y = mesh->mNormals[i].y;
        vector.z = mesh->mNormals[i].z;
        vertex.Normal = vector;

        if(mesh->mTextureCoords[0]) { // does the mesh contain texture coordinates?
            glm::vec2 vec;
            vec.x = mesh->mTextureCoords[0][i].x;
            vec.y = mesh->mTextureCoords[0][i].y;
            vertex.TexCoords = vec;
        } else
            vertex.TexCoords = glm::vec2(0.0f, 0.0f);


        vertices.push_back(vertex);
    }
    // process indices
    for(unsigned int i = 0; i < mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i];
        for(unsigned int j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);

    }
    // process material
    if(mesh->mMaterialIndex >= 0) {
        if(mesh->mMaterialIndex >= 0) {

            aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
            vector<Texture> diffuseMaps = loadMaterialTextures(material,
                                          aiTextureType_DIFFUSE, "texture_diffuse", resManager);

            textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
            vector<Texture> specularMaps = loadMaterialTextures(material,
                                           aiTextureType_SPECULAR, "texture_specular", resManager);

            textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
        }
    }

    return Mesh(vertices, indices, textures);
}



vector<Texture> pap::ResourceModel::loadMaterialTextures(aiMaterial *mat, aiTextureType type, string typeName, pap::ResourceManager &resManager) {
    vector<Texture> textures;
    for(unsigned int i = 0; i < mat->GetTextureCount(type); i++) {
        aiString str;
        mat->GetTexture(type, i, &str);

        string filename = string(str.C_Str());
        filename = directory + '/' + filename;
        textures.push_back(resManager.getResourceTexture(filename, typeName));
    }

    return textures;
}

vector<Mesh> &pap::ResourceModel::getMeshes() {

    return meshes;
}