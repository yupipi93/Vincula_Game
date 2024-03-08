#include <glew.h>
#include "Mesh.hpp"
#include <gtc/matrix_transform.hpp> // OpenGL mathematics
//console.log
#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/postprocess.h>     // Post processing flags

#include <PapayaEngine/PapayaDevice.hpp>


Mesh::Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures) {
    this->vertices = vertices;
    this->indices = indices;
    this->textures = textures;

    setupMesh();
}

Mesh::~Mesh() {
    /*glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);*/
}

/*bool Mesh::loadFile(std::string p) {

    // Create an instance of the Importer class
    Assimp::Importer importer;
    // And have it read the given file with some example postprocessing
    // Usually - if speed is not the most important aspect for you - you'll
    // propably to request more postprocessing than we do in this example.
    const aiScene *scene = importer.ReadFile( p.c_str(),
                           aiProcess_CalcTangentSpace       |
                           aiProcess_Triangulate            |
                           aiProcess_JoinIdenticalVertices  |
                           aiProcess_SortByPType);

    // If the import failed, report it
    if(!scene) {
        return false;
    }

    //void fillMeshData(scene);

    return true;
}*/

void Mesh::setupMesh() {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int),
                 &indices[0], GL_STATIC_DRAW);

    // vertex positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)0);
    // vertex normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, Normal));
    // vertex texture coords
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, TexCoords));

    glBindVertexArray(0);
}


void Mesh::setTexture(Texture texture) {

    textures.pop_back();
    textures.push_back(texture);

}

void Mesh::Draw(pap::Shader &shader) const {
    if(textures.size() > 0) {

        glActiveTexture(GL_TEXTURE0); // activate proper texture unit before binding

        // set the shinniness
        string name = textures[0].type;
        if(name == "texture_specular") {
            shader.setFloat("shininess", 16.f);

        } else {
            shader.setFloat("shininess", 2.f);
        }

        shader.setInt("ourTexture", 0);
        glBindTexture(GL_TEXTURE_2D, textures[0].id);
    }

    // draw mesh
    glBindVertexArray(VAO);
    
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}


void Mesh::Draw2D(pap::Shader &shader, glm::vec2 translation, glm::vec2 scale, glm::mat4 projection2d, glm::vec4 color) const {
    // render container
    shader.use();
    // Set texture
    glBindTexture(GL_TEXTURE_2D, textures[0].id);
    //Enable alpha chanel
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    //Disable depth test
    glDisable(GL_DEPTH_TEST);

    // Transformations
    shader.setMat4("model", calculateMatrix(translation, scale));
    shader.setMat4("projection", projection2d);
    shader.setVec2("aPos", glm::vec2(0.f, 0.f));
    shader.setVec4("objectColor", color);
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    // Disable the alpha
    glDisable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
}

glm::mat4 Mesh::calculateMatrix(glm::vec2 translation, glm::vec2 scale) const {
    GLfloat rotate = 0.f;

    glm::mat4 model = glm::mat4(1.0f);

    model = glm::translate(model, glm::vec3(translation, 0.0f));
    model = glm::rotate(model, glm::radians(rotate), glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::scale(model, glm::vec3(scale, 1.0f));



    return model;
}