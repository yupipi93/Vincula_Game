#include "SceneNode.hpp"
#include <algorithm>
#include <PapayaEngine/PapayaDevice.hpp>


//console.log


const uint16_t MAX_ENTITIES = 9092;

pap::SceneNode::SceneNode(pap::SceneNode *fath, pap::Entity *ent)
    : father{fath}, entity{ent}, updateMatrix{true} {
    //sons.reserve(MAX_ENTITIES);
}

pap::SceneNode::~SceneNode() {
    //delete entity;

    /*for(std::size_t i = 0; i < sons.size(); ++i) {
        delete sons[i];
    }*/
    //sons.clear();
}

void pap::SceneNode::addSon(pap::SceneNode *son) {
    son -> setFather(this);
    sons.push_back(son);
}

void pap::SceneNode::remSon(pap::SceneNode *son) {
    sons.erase(std::remove(sons.begin(), sons.end(), son),
               sons.end());
}

void pap::SceneNode::setEntity(pap::Entity *ent) {
    entity = ent;
}

void pap::SceneNode::setFather(pap::SceneNode *fath) {
    father = fath;
}


pap::Entity const *pap::SceneNode::getEntity() const {
    return entity;
}

pap::Entity *pap::SceneNode::getEntity () {
    return entity;
}


pap::SceneNode const *pap::SceneNode::getFather() const {
    return father;
}

std::vector <pap::SceneNode *> &pap::SceneNode::getSons() {
    return sons;
}


void pap::SceneNode::browse(glm::mat4 acumMatrix, pap::PapayaDevice &device) {

    if(updateMatrix) {
        transfMatrix = acumMatrix * calculateMatrix();
        updateMatrix = false;
    }
    if(entity && !transparency) { // if it is transparent, don't draw now
        entity -> draw(transfMatrix, device, color);
    }

    for(std::size_t i = 0; i < sons.size(); ++i) {

        sons[i] -> browse(transfMatrix, device);
    }
}

void pap::SceneNode::drawTransparent(pap::PapayaDevice &device) const {
    // This doesn't need to draw children or update the transformation matrix
    if(entity)
        entity -> draw(transfMatrix, device, color);
}

void pap::SceneNode::lightBrowse(glm::mat4 const acumMatrix, pap::PapayaDevice &device) {
    // Here we draw the scene from the point of view of the light, to calculate shadows

    if(updateMatrix) {
        transfMatrix = acumMatrix * calculateMatrix();
        updateMatrix = false;
    }
    if(entity) {
        entity -> drawLight(transfMatrix, device);
    }

    for(std::size_t i = 0; i < sons.size(); ++i) {

        sons[i] -> lightBrowse(transfMatrix, device);
    }
}


void pap::SceneNode::setTranslation (glm::vec3 const t) {
    translation = t;
    translation.z = -t.z;
    updateMatrix = true;
}

void pap::SceneNode::setRotation (glm::vec3 const r) {
    rotation = r;
    updateMatrix = true;
}

void pap::SceneNode::setScale (glm::vec3 const e) {
    escalade = e;
    updateMatrix = true;
}


void pap::SceneNode::translate(glm::vec3 const t) {
    translation += t;
    translation.z -= t.z;
    updateMatrix = true;
}

void pap::SceneNode::rotate   (glm::vec3 const r) {
    rotation += r;
    updateMatrix = true;
}

void pap::SceneNode::scale (glm::vec3 const e) {
    escalade *= e;
    updateMatrix = true;
}

glm::vec3 const pap::SceneNode::getTraslation() const {
    return translation;
}



void pap::SceneNode::setTransfMatrix(glm::mat4 trans) { // Later delete
    transfMatrix = trans;
    updateMatrix = true;
}

glm::mat4 const pap::SceneNode::getTransfMatrix() const {
    return transfMatrix;
}


glm::mat4 const pap::SceneNode::getUpdatedTransfMatrix() {
    // TODO PREGUNTAR, no tenemos matrix de acumulacion
    return transfMatrix;
}


glm::mat4 pap::SceneNode::calculateMatrix() const {
    glm::mat4 identityMat = glm::mat4(1.0f);

    glm::mat4 matrixAux = calcTranslationMat() * calcRotationMat() * calcScaleMat();

    return matrixAux;
}



glm::mat4 pap::SceneNode::calcTranslationMat() const {
    glm::mat4 identityMat = glm::mat4(1.0f);

    glm::mat4 matrixAux = glm::translate(identityMat, translation);

    return matrixAux;
}

glm::mat4 pap::SceneNode::calcRotationMat() const {
    glm::mat4 identityMat = glm::mat4(1.0f);

    glm::mat4 matrixAux =
        glm::rotate(identityMat, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f))
        * glm::rotate(identityMat, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f))
        * glm::rotate(identityMat, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));

    return matrixAux;
}

glm::mat4 pap::SceneNode::calcScaleMat() const {
    glm::mat4 identityMat = glm::mat4(1.0f);

    glm::mat4 matrixAux = glm::scale(identityMat, escalade);

    return matrixAux;
}