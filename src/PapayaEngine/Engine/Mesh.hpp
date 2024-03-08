#pragma once
#include <vector>
#include <string>
#include <cstdint>
#include <PapayaEngine/Engine/Shader.hpp>
#include <PapayaEngine/Engine/Texture.hpp>
#include <assimp/scene.h>           // Output data structure
#include <glm.hpp> // OpenGL mathematics

namespace pap {
class PapayaDevice;
} // NAMESPACE

using namespace std;

struct Vertex {
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
};

class Mesh {
public:
    /*  Functions  */
    Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures);
    void setTexture(Texture texture);
    void Draw(pap::Shader &shader) const ;
    void Draw2D(pap::Shader &shader, glm::vec2 translation, glm::vec2 scale, glm::mat4 projection2d, glm::vec4 color) const ;

    ~Mesh();
    /*  Mesh Data  */
    vector<Vertex> vertices;
    vector<unsigned int> indices;
    vector<Texture> textures;
private:
    /*  Functions    */
    void setupMesh();
    glm::mat4 calculateMatrix(glm::vec2 translation, glm::vec2 scale) const;

    /*  Render data  */
    unsigned int VAO, VBO, EBO;
};