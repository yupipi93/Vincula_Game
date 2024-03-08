#pragma once
#include <glew.h>
#include <glfw3.h>
#include <string>
#include <vector>
#include <memory>
#include <PapayaEngine/Engine/Window.hpp>
#include <PapayaEngine/Engine/SceneNode.hpp>
#include <PapayaEngine/Engine/Shader.hpp>
#include <PapayaEngine/Engine/ResourceManager.hpp>
#include <PapayaEngine/Engine/EntityAnimation.hpp>
#include <PapayaEngine/Engine/EntityModel.hpp>
#include <PapayaEngine/Engine/EntityCamera.hpp>
#include <PapayaEngine/Engine/EntityLight.hpp>
#include <PapayaEngine/Engine/LightDirectional.hpp>
#include <PapayaEngine/Engine/TextRenderer.hpp>
#include <PapayaEngine/Engine/DepthMap.hpp>


namespace pap {

class PapayaDevice {
public:
    PapayaDevice(uint32_t width, uint32_t height, bool fullScreen);
    ~PapayaDevice();

    void createShader(const char *vertexPath, const char *fragmentPath);
    pap::SceneNode                  *getNodeScene();
    pap::SceneNode                  *addCameraSceneNode(pap::SceneNode *nFather = nullptr, glm::vec3 translation = glm::vec3(0.f, 0.f, 0.f), glm::vec3 rotation = glm::vec3(0.f, 0.f, 0.f));
    pap::SceneNode                  *addAnimationSceneNode(pap::SceneNode *nFather = nullptr, std::string path = nullptr, glm::vec3 translation = glm::vec3(0.f, 0.f, 0.f), glm::vec3 rotation = glm::vec3(0.f, 0.f, 0.f), glm::vec3 scale = glm::vec3(1.f, 1.f, 1.f), bool transparecy = false, bool shadow = true);
    pap::SceneNode                  *addModelSceneNode(pap::SceneNode *nFather = nullptr, std::string path = nullptr, glm::vec3 translation = glm::vec3(0.f, 0.f, 0.f), glm::vec3 rotation = glm::vec3(0.f, 0.f, 0.f), glm::vec3 scale = glm::vec3(1.f, 1.f, 1.f), bool transparecy = false, bool shadow = true);
    //pap::SceneNode                  *addModelSceneNode(pap::SceneNode *nFather, pap::ResourceModel *rModel, glm::vec3 translation, glm::vec3 rotation, glm::vec3 scale, bool transparecy);
    pap::SceneNode                  *addGeometrySceneNode(pap::SceneNode *nFather = nullptr, std::string type = nullptr, std::string texture = nullptr, glm::vec3 translation = glm::vec3(0.f, 0.f, 0.f), glm::vec3 rotation = glm::vec3(0.f, 0.f, 0.f), glm::vec3 scale = glm::vec3(1.f, 1.f, 1.f), bool transparecy = false, bool shadow = true);
    pap::SceneNode                  *addPointLightSceneNode(pap::SceneNode *nFather = nullptr, glm::vec3 position = glm::vec3(0.f, 0.f, 0.f), glm::vec3 color = glm::vec3(0.f, 0.f, 0.f), float constant = 1.f, float linear = 0.014f, float quadratic = 0.0007f);
    pap::SceneNode                  *addDirectionalLightSceneNode(pap::SceneNode *nFather, glm::vec3 position, glm::vec3 color, glm::vec3 direction);
    pap::SceneNode                  *getDirLightNode() const;
    Window                          *getWindow() const;
    pap::EntityCamera               *getActiveCameraEntity();
    pap::SceneNode                  *getActiveCameraNode();
    std::vector <pap::SceneNode *>  &getPointLightNodes();
    pap::Shader                     &getModelShader();
    pap::Shader                     &getHUDShader();
    pap::Shader                     &getTextShader();
    pap::Shader                     &getText3DShader();
    pap::Shader                     &getDepthShader();
    pap::ResourceManager            &getResourceManager();
    pap::TextRenderer               &getTextRenderer();

    void deleteSceneNode(pap::SceneNode *objetiveNode);
    void deleteEntity(pap::Entity *objetiveEntity);
    void setTextureSceneNode(pap::SceneNode *objetiveNode, std::string texturePath, bool transparecy = false);

    void drawAll();
    void draw2DImage(std::string texturePath, glm::vec2 translation, glm::vec2 scale);
    void draw2DImage(std::string texturePath, glm::vec2 translation, glm::vec2 scale, glm::vec4 color);
    void drawElement2Din3D(glm::vec3 position, glm::vec3 vecTextRotation, glm::vec3 escalade, std::string texture);
    void renderText(std::string text, GLfloat posX, GLfloat posY, GLfloat scale, glm::vec3 color, bool centered);
    void renderText3d(std::string text, glm::vec3 textPos, glm::vec3 textRot, GLfloat scale, glm::vec3 color, std::string texture = "");

    glm::vec3 getProjectedCursor() const;


    void endScene() const;
    bool shouldClose() const;
    void close();

    glm::vec4 clearColor {0.2f, 0.3f, 0.3f, 1.0f};
    
private:
    Window                                                  w;
    DepthMap                                                depthMap;
    pap::ResourceManager                                    resManager;
    std::vector <std::unique_ptr<pap::SceneNode>>           sceneNodes;
    std::vector <std::unique_ptr<pap::EntityAnimation>>     animationEntities;
    std::vector <std::unique_ptr<pap::EntityModel>>         modelEntities;
    std::vector <std::unique_ptr<pap::EntityCamera>>        cameraEntities;
    std::vector <std::unique_ptr<pap::EntityLight>>         lightEntities;
    std::vector <pap::SceneNode *>                          cameraNodes;
    std::vector <pap::SceneNode *>                          pointLightNodes;
    std::vector <pap::SceneNode *>                          transparentNodes;
    pap::SceneNode                                          *dirLightNode;
    pap::Shader                                             modelShader;
    pap::Shader                                             HUDShader;
    pap::Shader                                             textShader;
    pap::Shader                                             text3DShader;
    pap::Shader                                             s2Din3DShader;
    pap::TextRenderer                                       textRenderer;
    pap::Shader                                             depthShader;
    pap::Shader                                             debugDepthQuad;

    glm::mat4 projection2d;
    glm::mat4 lightSpaceMatrix;

    // For the FPS counter
    double lastTime = 0;
    int nbFrames = 0;
    std::string fpsText = "0";

    // Mouse projected position
    glm::vec3 mouseProjectedPos {0.f, 0.f, 0.f};

    //For render elements 2D in 3D world
    GLuint b2Din3DVAO, b2Din3DVBO;
    //uint32_t textureID = 0;


    pap::SceneNode *createNodeScene();
    void setModelShaderData();
    void updateActiveCamera();
    void calculateLookAt(pap::SceneNode *activeCamera);
    void calculateProjection(pap::SceneNode *activeCamera);
    void setLightsInShader() const;
    void setDirectionalLights() const;
    void setPointLights() const;
    void deleteNodeFromAuxiliarVectors(pap::SceneNode *objetiveNode);
    void deleteNodeFromVector(pap::SceneNode *objetiveNode);
    void deleteSceneNodeSons(pap::SceneNode *objetiveNode);
    void calculateFPS();
    void drawTransparentNodes();
    void setUpElement2Din3D();
    glm::mat4 calcTranslationModelMat(glm::vec3 textPos) const;
    glm::mat4 calcRotationModelMat(glm::vec3 textRotation) const;
    glm::mat4 calcScaleModelMat(glm::vec3 escalade) const;

    std::vector<std::string> splitIterator(const std::string &str, const char separator);
    void calculateShadowMap();
    void renderScene();
    void updateLightSpaceMatrix();
    void activateDepthTexture();

    void calculateProjectedCursor();

    //Templete function to delete the object of a unique_ptr
    /*template<typename T>
    void deleteUniquePtrObject(unique_ptr<T> &uPtr) {
        T *obj = uPtr.release();        // pointer to no-longer-managed object
        uPtr.get_deleter() ( obj );
    }*/
};

} // NAMESPACE