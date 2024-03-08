#pragma once

#include <vector>
#include <PapayaEngine/Engine/Entity.hpp>
#include <glm.hpp> // OpenGL mathematics
#include <gtc/matrix_transform.hpp> // OpenGL mathematics

namespace pap {
    class PapayaDevice;
} // NAMESPACE


namespace pap {


class SceneNode {
public:
    // Entity and scene tree methods
    SceneNode(pap::SceneNode *fath = nullptr, pap::Entity *ent = nullptr);
    ~SceneNode();
    void             addSon     (pap::SceneNode *son);
    void             remSon     (pap::SceneNode *son);
    void             setEntity  (pap::Entity    *ent);
    void             setFather  (pap::SceneNode *fath);
    pap::Entity const    *getEntity  () const;
    pap::Entity          *getEntity  ();
    pap::SceneNode const *getFather  () const;
    std::vector <pap::SceneNode *> &getSons  ();
    void             browse     (glm::mat4 const acumMatrix, pap::PapayaDevice &device); //recorrer
    void             lightBrowse(glm::mat4 const acumMatrix, pap::PapayaDevice &device);
    void drawTransparent(pap::PapayaDevice &device) const;


    // Transformation methods
    void setTranslation (glm::vec3 const t);
    void setRotation    (glm::vec3 const r);
    void setScale       (glm::vec3 const e);
    void translate      (glm::vec3 const t);
    void rotate         (glm::vec3 const r);
    void scale          (glm::vec3 const e);

    void setTransfMatrix(glm::mat4 trans); // Later delete

    glm::vec3 const getTraslation() const;
    glm::vec3 const getRotation()   const;
    glm::vec3 const getScale()      const;
    glm::mat4 const getTransfMatrix() const;
    glm::mat4 const getUpdatedTransfMatrix();
    glm::mat4 calculateMatrix() const;

    bool transparency {false}; // if the texture has transparency
    glm::vec3 color {1.f, 1.f, 1.f};


private:
    glm::mat4 calcTranslationMat()  const;
    glm::mat4 calcRotationMat()     const;
    glm::mat4 calcScaleMat()        const;



    pap::Entity *entity;
    std::vector <pap::SceneNode *> sons;
    pap::SceneNode                *father;
    bool updateMatrix;

    // Transformations
    glm::vec3 translation   = glm::vec3(0, 0, 0);
    glm::vec3 rotation      = glm::vec3(0, 0, 0);
    glm::vec3 escalade      = glm::vec3(1, 1, 1);
    glm::mat4 transfMatrix  = glm::mat4(1.0f);
};

} // NAMESPACE