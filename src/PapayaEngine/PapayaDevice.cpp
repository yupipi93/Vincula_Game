#include "PapayaDevice.hpp"
//console.log
#include <cstdint>
#include <PapayaEngine/Engine/LightPoint.hpp>


const uint16_t MAX_ENTITIES = 9092;
const uint8_t  MAX_POINT_LIGHTS = 16; //IMPORTANT: if changed, change in the shader

pap::PapayaDevice::PapayaDevice(uint32_t width, uint32_t height, bool fullScreen)
    : w {width, height, fullScreen}, resManager{}, sceneNodes{}, textRenderer{},
      modelShader{"./src/PapayaEngine/shaders/materialVertex.vs", "./src/PapayaEngine/shaders/materialFragment.fs"},
      HUDShader{"./src/PapayaEngine/shaders/hudVertex.vs", "./src/PapayaEngine/shaders/hudFragment.fs"},
      textShader{"./src/PapayaEngine/shaders/textVertex.vs", "./src/PapayaEngine/shaders/textFragment.fs"},
      text3DShader{"./src/PapayaEngine/shaders/text3dVertex.vs", "./src/PapayaEngine/shaders/text3dFragment.fs"},
      s2Din3DShader{"./src/PapayaEngine/shaders/bubbleVertex.vs", "./src/PapayaEngine/shaders/bubbleFragment.fs"},
      depthShader{"./src/PapayaEngine/shaders/shadowMappingVertex.vs", "./src/PapayaEngine/shaders/shadowMappingFragment.fs"},
      debugDepthQuad{"./src/PapayaEngine/shaders/lightVertex.vs", "./src/PapayaEngine/shaders/lightFragment.fs"},
      depthMap{} {

    // OpenGl compatibility functions
    glewInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);



    glEnable(GL_DEPTH_TEST);


    /*#ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
    #endif*/

    sceneNodes      .reserve(MAX_ENTITIES);
    modelEntities   .reserve(MAX_ENTITIES);
    cameraEntities  .reserve(10);
    lightEntities   .reserve(20);
    cameraNodes     .reserve(10);
    pointLightNodes .reserve(MAX_POINT_LIGHTS);

    projection2d = glm::ortho(0.0f, static_cast<GLfloat>(Window::windowWidth), static_cast<GLfloat>(Window::windowHeight), 0.0f, -1.0f, 1.0f);

    createNodeScene();
    setUpElement2Din3D();

    double lastTime = glfwGetTime();


    //TEMP
    glfwSetInputMode(w.window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN); //HIDE CURSOR
}

pap::PapayaDevice::~PapayaDevice() {
    close();
}


pap::SceneNode *pap::PapayaDevice::createNodeScene() {

    if(sceneNodes.empty())
        sceneNodes.emplace_back(std::make_unique<pap::SceneNode>());

    return sceneNodes.front().get();
}

pap::SceneNode *pap::PapayaDevice::getNodeScene() {

    return sceneNodes.front().get();
}

pap::SceneNode *pap::PapayaDevice::addAnimationSceneNode(pap::SceneNode *nFather, std::string path, glm::vec3 translation, glm::vec3 rotation, glm::vec3 scale, bool transparency, bool shadow) {

    //std::unique_ptr<SceneNode> nScene    = std::make_unique<SceneNode>();
    if(!nFather)
        nFather = sceneNodes.front().get();

    std::vector <pap::EntityModel *> models;
    ResourceModel *rModel;
    std::string pathResult;
    bool next = true;
    int i = 1;

    while(next) {

        pathResult = path + std::to_string(i) + ".obj";
        rModel = resManager.getResourceModel(pathResult);

        if(rModel == nullptr)
            next = false;
        else {
            modelEntities.emplace_back( std::make_unique<EntityModel>(rModel) );
            models.push_back(modelEntities.back().get());

            // set shadow to the model
            modelEntities.back().get()->hasShadow = shadow;

            i += 1;
        }
    }

    if(!models.empty()) {
        animationEntities.emplace_back( std::make_unique<EntityAnimation>(models) );

        sceneNodes.emplace_back( std::make_unique<SceneNode>(nFather, animationEntities.back().get()) );
        nFather->addSon(sceneNodes.back().get());

        sceneNodes.back().get()->setTranslation(translation);
        sceneNodes.back().get()->setRotation(rotation);
        sceneNodes.back().get()->setScale(scale);

        if(transparency) { // set if it is transparent
            sceneNodes.back().get()->transparency = transparency;
            transparentNodes.emplace_back(sceneNodes.back().get());
        }

        return sceneNodes.back().get();
    }
    return nullptr;
}

pap::SceneNode *pap::PapayaDevice::addModelSceneNode(pap::SceneNode *nFather, std::string path, glm::vec3 translation, glm::vec3 rotation, glm::vec3 scale, bool transparency, bool shadow) {

    //std::unique_ptr<SceneNode> nScene    = std::make_unique<SceneNode>();
    if(!nFather)
        nFather = sceneNodes.front().get();

    ResourceModel *rModel = resManager.getResourceModel(path);

    if(rModel != nullptr) {
        modelEntities.emplace_back( std::make_unique<EntityModel>(rModel) );
        // set shadow to the model
        modelEntities.back().get()->hasShadow = shadow;

        sceneNodes.emplace_back( std::make_unique<SceneNode>(nFather, modelEntities.back().get()) );
        nFather->addSon(sceneNodes.back().get());

        sceneNodes.back().get()->setTranslation(translation);
        sceneNodes.back().get()->setRotation(rotation);
        sceneNodes.back().get()->setScale(scale);

        if(transparency) { // set if it is transparent
            sceneNodes.back().get()->transparency = transparency;
            transparentNodes.emplace_back(sceneNodes.back().get());
        }

        return sceneNodes.back().get();
    }
    return nullptr;
}

/*pap::SceneNode *pap::PapayaDevice::addModelSceneNode(pap::SceneNode *nFather, pap::ResourceModel *rModel, glm::vec3 translation, glm::vec3 rotation, glm::vec3 scale) {

    //std::unique_ptr<pap::SceneNode> nScene    = std::make_unique<pap::SceneNode>();
    if(!nFather)
        nFather = sceneNodes.front().get();

    modelEntities.emplace_back( std::make_unique<pap::EntityModel>(rModel) );

    sceneNodes.emplace_back( std::make_unique<pap::SceneNode>(nFather, modelEntities.back().get()) );
    nFather->addSon(sceneNodes.back().get());

    sceneNodes.back().get()->setTranslation(translation);
    sceneNodes.back().get()->setRotation(rotation);
    sceneNodes.back().get()->setScale(scale);

    return sceneNodes.back().get();
}*/

pap::SceneNode *pap::PapayaDevice::addGeometrySceneNode(pap::SceneNode *nFather, std::string type, std::string texture, glm::vec3 translation, glm::vec3 rotation, glm::vec3 scale, bool transparency, bool shadow) {

    //std::unique_ptr<pap::SceneNode> nScene    = std::make_unique<pap::SceneNode>();
    if(!nFather)
        nFather = sceneNodes.front().get();

    pap::ResourceModel *rModel = resManager.getGeometryModel(type, texture);

    modelEntities.emplace_back( std::make_unique<pap::EntityModel>(rModel) );
    // set shadow to the model
    modelEntities.back().get()->hasShadow = shadow;

    sceneNodes.emplace_back( std::make_unique<pap::SceneNode>(nFather, modelEntities.back().get()) );
    nFather->addSon(sceneNodes.back().get());

    sceneNodes.back().get()->setTranslation(translation);
    sceneNodes.back().get()->setRotation(rotation);
    sceneNodes.back().get()->setScale(scale);

    if(transparency) { // set if it is transparent
        sceneNodes.back().get()->transparency = transparency;
        transparentNodes.emplace_back(sceneNodes.back().get());
    }

    return sceneNodes.back().get();

}

pap::SceneNode *pap::PapayaDevice::addCameraSceneNode(pap::SceneNode *nFather, glm::vec3 translation, glm::vec3 rotation) {

    if(!nFather)
        nFather = sceneNodes.front().get();

    cameraEntities.emplace_back(std::make_unique<pap::EntityCamera>());
    sceneNodes.emplace_back( std::make_unique<pap::SceneNode>(nFather, cameraEntities.back().get()) );
    nFather->addSon(sceneNodes.back().get());

    cameraNodes.emplace_back(sceneNodes.back().get());

    cameraEntities.back().get()->setActiveCamera(true);

    sceneNodes.back().get()->setTranslation(translation);
    sceneNodes.back().get()->setRotation(rotation);

    return sceneNodes.back().get();

}


pap::SceneNode *pap::PapayaDevice::addPointLightSceneNode(pap::SceneNode *nFather, glm::vec3 position, glm::vec3 color, float constant, float linear, float quadratic) {
    if(pointLightNodes.size() < MAX_POINT_LIGHTS ) {

        if(!nFather)
            nFather = sceneNodes.front().get();

        lightEntities.emplace_back(std::make_unique<pap::LightPoint>());
        sceneNodes.emplace_back( std::make_unique<pap::SceneNode>(nFather, lightEntities.back().get()) );
        nFather->addSon(sceneNodes.back().get());

        pointLightNodes.emplace_back(sceneNodes.back().get());

        pap::LightPoint *lightEntity = static_cast<pap::LightPoint *>( lightEntities.back().get() );
        lightEntity->color     = color;
        lightEntity->constant  = constant;
        lightEntity->linear    = linear;
        lightEntity->quadratic = quadratic;

        sceneNodes.back().get()->setTranslation(position);

        return sceneNodes.back().get();
    }
    return nullptr;
}


pap::SceneNode *pap::PapayaDevice::addDirectionalLightSceneNode(pap::SceneNode *nFather, glm::vec3 position, glm::vec3 color, glm::vec3 direction) {
    if(!dirLightNode ) { // only can be a directional light

        if(!nFather)
            nFather = sceneNodes.front().get();

        lightEntities.emplace_back(std::make_unique<pap::LightDirectional>(direction));
        sceneNodes.emplace_back( std::make_unique<pap::SceneNode>(nFather, lightEntities.back().get()) );
        nFather->addSon(sceneNodes.back().get());

        dirLightNode = sceneNodes.back().get();

        pap::LightDirectional *dirLightEntity = static_cast<pap::LightDirectional *>( lightEntities.back().get() );
        dirLightEntity->color     = color;

        sceneNodes.back().get()->setTranslation(position);

        return sceneNodes.back().get();
    }
    return nullptr;
}


std::vector <pap::SceneNode *> &pap::PapayaDevice::getPointLightNodes() {
    return pointLightNodes;
}

pap::SceneNode *pap::PapayaDevice::getDirLightNode() const {
    return dirLightNode;
}


Window *pap::PapayaDevice::getWindow() const {
    return const_cast<Window *>(&w);
}

pap::Shader &pap::PapayaDevice::getModelShader() {
    return modelShader;
}

pap::Shader &pap::PapayaDevice::getHUDShader() {
    return HUDShader;
}

pap::Shader &pap::PapayaDevice::getTextShader() {
    return textShader;
}


pap::Shader &pap::PapayaDevice::getText3DShader() {
    return text3DShader;
}

pap::Shader &pap::PapayaDevice::getDepthShader() {
    return depthShader;
}

pap::ResourceManager &pap::PapayaDevice::getResourceManager() {
    return resManager;
}


pap::EntityCamera *pap::PapayaDevice::getActiveCameraEntity() {
    pap::SceneNode *camera = getActiveCameraNode();
    if(camera)
        return static_cast<pap::EntityCamera *>(camera -> getEntity());
    return nullptr;
}

pap::SceneNode *pap::PapayaDevice::getActiveCameraNode() {

    for(std::size_t i = 0; i < cameraNodes.size(); ++i) {
        pap::EntityCamera *cameraEnt = static_cast<pap::EntityCamera *>(cameraNodes[i]->getEntity());
        if(cameraEnt && cameraEnt->getActiveCamera()) {
            return cameraNodes[i];
        }
    }

    return nullptr;
}



bool pap::PapayaDevice::shouldClose() const {
    return glfwWindowShouldClose(w.window);
}

void pap::PapayaDevice::close() {
    w.close();
}



/*unsigned int quadVAO = 0;
unsigned int quadVBO;
void renderQuad() {
    if (quadVAO == 0) {
        float quadVertices[] = {
            // positions        // texture Coords
            -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
                -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
                1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
                1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
            };
        // setup plane VAO
        glGenVertexArrays(1, &quadVAO);
        glGenBuffers(1, &quadVBO);
        glBindVertexArray(quadVAO);
        glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));
    }
    glBindVertexArray(quadVAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
}*/



void pap::PapayaDevice::drawAll() {
    //w.processInput(w.window);
    if(!sceneNodes.empty()) {
        // For the FPS counter

        // Window clear
        w.clearColor(clearColor);

        // Calculate depth from light
        calculateShadowMap();

        //activate face culling
        glEnable(GL_CULL_FACE);
        glCullFace(GL_FRONT);
        glFrontFace(GL_CW);

        // Render the scene normaly
        renderScene();
        // Sort all the transparent objects
        // TODO: Not done for optimizations, if transparency bugs, do it!
        // Draw all the transparent nodes
        drawTransparentNodes();

        glDisable(GL_CULL_FACE);


        /*// For debugging the depth map
        float nearPlane = 0.1f, farPlane = 700.f;
        debugDepthQuad.use();
        debugDepthQuad.setFloat("near_plane", nearPlane);
        debugDepthQuad.setFloat("far_plane", farPlane);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, depthMap.depthMapTexture);
        renderQuad();*/

        calculateProjectedCursor(); //Calculate this once per iteration

        calculateFPS();

    }
}

void pap::PapayaDevice::drawTransparentNodes() {

    for(std::size_t i = 0; i < transparentNodes.size(); ++i) {
        transparentNodes[i]->drawTransparent(*this);
    }

    glDisable(GL_BLEND);
}


void pap::PapayaDevice::calculateShadowMap() {
    // To calculate the shadows, firs we have to render in the depth map
    updateLightSpaceMatrix();

    depthShader.use();
    depthShader.setMat4("lightSpaceMatrix", lightSpaceMatrix);

    glViewport(0, 0, depthMap.SHADOW_WIDTH, depthMap.SHADOW_HEIGHT);
    glBindFramebuffer(GL_FRAMEBUFFER, depthMap.depthMapFBO);
    glClear(GL_DEPTH_BUFFER_BIT);

    //glCullFace(GL_FRONT); // Peter panning

    // Draw the scene from the light point of view, using the light shader
    glm::mat4 identityMatrix = glm::mat4(1.0f);
    sceneNodes[0].get()->lightBrowse(identityMatrix, *this);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    //glCullFace(GL_BACK);
}

void pap::PapayaDevice::renderScene() {
    // Clear buffers
    glViewport(0, 0, Window::windowWidth, Window::windowHeight);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    modelShader.use();

    setModelShaderData();

    // activate blending for transparecy
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Bind shadowmap texture
    activateDepthTexture();

    glm::mat4 identityMatrix = glm::mat4(1.0f);
    // Draw all the opaque objects
    sceneNodes[0].get()->browse(identityMatrix, *this);

}

void pap::PapayaDevice::activateDepthTexture() {
    // Set the depthMap texture
    int numTexture = 79; // The number of texture is 79 because is the last
    glActiveTexture(GL_TEXTURE0 + numTexture);
    modelShader.setInt("shadowMap", numTexture);
    glBindTexture(GL_TEXTURE_2D, depthMap.depthMapTexture);
}

void pap::PapayaDevice::calculateFPS() {
    double currentTime = glfwGetTime();
    nbFrames++;
    if ( currentTime - lastTime >= 1.0 ) { // If last prinf() was more than 1 sec ago
        // Update fps
        fpsText = std::to_string( nbFrames );

        // Reset counters
        nbFrames = 0;
        lastTime += 1.0;
    }

    // Draw the fps text
    renderText(fpsText, 15 * Window::windowWidth / 1024, 5 * Window::windowHeight / 768, 1.0f, glm::vec3(0.03f, 0.8f, 0.05f), false);
}

void pap::PapayaDevice::endScene() const {
    glfwSwapBuffers(w.window);
    glfwPollEvents();
}



void pap::PapayaDevice::setModelShaderData() {
    // Shadow map data
    modelShader.setMat4("lightSpaceMatrix", lightSpaceMatrix);

    // Camera data
    updateActiveCamera();

    // Light data
    setLightsInShader();
}


void pap::PapayaDevice::updateLightSpaceMatrix() {
    if(dirLightNode) {
        //glm::vec3 lightPos {60.f, 67.f, 45.f};
        //glm::vec3 lightObjective {0.0f, 0.0f, 0.0f};

        float nearPlane = -500.f, farPlane = 500.f;
        glm::mat4 depthProjectionMatrix = glm::ortho(-550.f, 550.f, -550.f, 550.f, nearPlane, farPlane);


        pap::LightDirectional *directionalLight = static_cast<pap::LightDirectional *>( dirLightNode->getEntity() );
        glm::mat4 depthViewMatrix = glm::lookAt(dirLightNode->getTraslation(), directionalLight->objective, glm::vec3(0.0, 1.0, 0.0));

        lightSpaceMatrix = depthProjectionMatrix * depthViewMatrix;
    }
}


void pap::PapayaDevice::updateActiveCamera() {
    pap::SceneNode *activeCamera = getActiveCameraNode();

    if(activeCamera) {
        calculateLookAt(activeCamera);
        calculateProjection(activeCamera);
    }
}

void pap::PapayaDevice::calculateLookAt(pap::SceneNode *activeCamera) {
    // TODO preguntar
    //glm::vec3 cameraPos     = activeCamera->getUpdatedTransfMatrix() * glm::vec4(0.f, 0.f, 0.f, 1.f);


    glm::vec3 cameraPos     = activeCamera->getTraslation();
    pap::EntityCamera *cameraEnt = static_cast<pap::EntityCamera *>(activeCamera -> getEntity());
    glm::vec3 target        = cameraEnt -> cameraTarget;


    // Calculate view
    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::mat4 view = glm::lookAt(cameraPos, target, up);
    cameraEnt->view = view;

    modelShader.setMat4("view", view);
    modelShader.setVec3("viewPos", cameraPos);

    /*float difX = cameraPos.x - target.x;
      float difY = cameraPos.y - target.y;
      float difZ = cameraPos.z - target.z;

      float rotX = atan2(difY, difZ) * 180 / 3.14159;
      float rotY = atan2(difX, difZ) * 180 / 3.14159;

      //glm::vec3 cameraDirection = glm::normalize(translation - target);

      //cameraPos     = activeCamera->getTransfMatrix() * glm::vec4(0.f,0.f,0.f,1.f);


      */

    //activeCamera->setRotation(glm::vec3(rotX, rotY, 0.f));
}


void pap::PapayaDevice::calculateProjection(pap::SceneNode *activeCamera) {
    pap::EntityCamera *cameraEnt = static_cast<pap::EntityCamera *>(activeCamera -> getEntity());
    modelShader.setMat4("projection", cameraEnt->updateProjection());
}



void pap::PapayaDevice::setLightsInShader() const {
    setDirectionalLights();
    setPointLights();
}

void pap::PapayaDevice::setDirectionalLights() const {
    glm::vec3 lightColor     {1.f, 1.0f, 1.0f};
    glm::vec3 lightDirection {0.f, 0.f, 0.f};

    if(dirLightNode) {
        pap::LightDirectional *directionalLight = static_cast<pap::LightDirectional *>( dirLightNode->getEntity() );

        lightColor = directionalLight->color;
        lightDirection = directionalLight->direction;

    }

    // Setting the light properties
    glm::vec3 diffuseColor = lightColor   * glm::vec3(0.80f);
    glm::vec3 ambientColor = diffuseColor * glm::vec3(0.55f);

    // For the directional light
    modelShader.setVec3("dirLight.direction", lightDirection);
    modelShader.setVec3("dirLight.ambient",   ambientColor);
    modelShader.setVec3("dirLight.diffuse",   diffuseColor); // darken diffuse dirLight a bit
    modelShader.setVec3("dirLight.specular",  lightColor); // Normaly set on all 1.f
}


void pap::PapayaDevice::setPointLights() const {
    int nbPointLights = 0; // number of point lights in the scene

    for(std::size_t i = 0; i < pointLightNodes.size(); ++i) {
        // Get the values from the lights
        glm::vec3 lightPos = pointLightNodes[i]->getTraslation();

        pap::LightPoint *entityPL = static_cast<pap::LightPoint *>( pointLightNodes[i]->getEntity() );
        if(entityPL) {
            ++nbPointLights;

            glm::vec3 lightColor = entityPL->color;
            glm::vec3 diffuseColor = lightColor   * glm::vec3(0.95f);
            glm::vec3 ambientColor = diffuseColor * glm::vec3(0.8f);

            float constant = entityPL->constant;
            float linear = entityPL->linear;
            float quadratic = entityPL->quadratic;


            // Set the values in the shader
            modelShader.setVec3(  "pointLights[" + std::to_string( i ) + "].position",  lightPos);
            modelShader.setFloat( "pointLights[" + std::to_string( i ) + "].constant",  constant);
            modelShader.setFloat( "pointLights[" + std::to_string( i ) + "].linear",  linear);
            modelShader.setFloat( "pointLights[" + std::to_string( i ) + "].quadratic",  quadratic);
            modelShader.setVec3(  "pointLights[" + std::to_string( i ) + "].ambient",  ambientColor);
            modelShader.setVec3(  "pointLights[" + std::to_string( i ) + "].diffuse",  diffuseColor);
            modelShader.setVec3(  "pointLights[" + std::to_string( i ) + "].specular",  lightColor); // Normaly set on all 1.f
        }
    }

    modelShader.setInt("sizePLights", nbPointLights);
}



glm::vec3 intersectPlane(const glm::vec3 &planeNorm, const glm::vec3 &planePoint, const glm::vec3 &rayVec, const glm::vec3 &rayPoint) {
    glm::vec3 diff = rayPoint - planePoint;
    double prod1 = glm::dot(planeNorm, diff);
    double prod2 = glm::dot(rayVec, planeNorm);
    double prod3 = prod1 / prod2;

    glm::vec3 result;
    result.x = rayPoint.x - rayVec.x * prod3;
    result.y = rayPoint.y - rayVec.y * prod3;
    result.z = rayPoint.z - rayVec.z * prod3;
    return result;
}
void pap::PapayaDevice::calculateProjectedCursor() {
    pap::SceneNode    *cameraNode = getActiveCameraNode();
    pap::EntityCamera *cameraEnt  = static_cast<pap::EntityCamera *>(cameraNode->getEntity());
    float mouseX = w.mouseReceiver.mouseX;
    float mouseY = w.mouseReceiver.mouseY;

    // Get the normalized window coords
    float posX = (2.f * mouseX) / Window::windowWidth - 1.f;
    float posY = (2.f * mouseY) / Window::windowHeight - 1.f;
    posY *= -1.f;

    // Get the clip coordinates
    glm::vec4 clipCoords {posX, posY, -1.f, 1.f};

    // Inverse projection to get view coordinates
    glm::mat4 invProjection = glm::inverse(cameraEnt->projection);
    glm::vec4 eyeCoords     = invProjection * clipCoords;
    eyeCoords = glm::vec4(eyeCoords.x, eyeCoords.y, -1.f, 0.f);

    // Translate to world coordinates and get mouse ray
    glm::mat4 invView  = glm::inverse(cameraEnt->view);
    glm::vec4 rayWorld = invView * eyeCoords;
    glm::vec3 mouseRay {rayWorld.x, rayWorld.y, rayWorld.z};
    mouseRay = glm::normalize(mouseRay);


    // Get the intersection point with the floor plane
    glm::vec3 planeNorm  {0.f, 1.f, 0.f};
    glm::vec3 planePoint {0.f, 0.f, 0.f};
    glm::vec3 rayPoint = cameraNode->getTraslation();
    mouseProjectedPos = intersectPlane(planeNorm, planePoint, mouseRay, rayPoint);

    //DEBUG
    //mousePos.z *= -1.f;
    //pointLightNodes[0]->setTranslation(mousePos);
    //sceneNodes[5].get()->setTranslation(mousePos);
    //return mousePos;
}

glm::vec3 pap::PapayaDevice::getProjectedCursor() const {
    return mouseProjectedPos;
}




void pap::PapayaDevice::deleteSceneNode(pap::SceneNode *objetiveNode) {
    if(objetiveNode) {
        //First delete the sons
        deleteSceneNodeSons( objetiveNode );

        // Delete the node from the father
        pap::SceneNode *father = const_cast<pap::SceneNode *>(objetiveNode->getFather());
        if(father)
            father->remSon( objetiveNode );

        //Delete the entity from the vector
        deleteEntity( objetiveNode->getEntity() );

        //Delete the node from the auxiliar vectors
        deleteNodeFromAuxiliarVectors( objetiveNode );

        //Delete the node from the vector
        deleteNodeFromVector( objetiveNode );
    }
}

void pap::PapayaDevice::deleteEntity(pap::Entity *objetiveEntity) {
    if(objetiveEntity) {
        // Find in the model
        for(std::size_t i = 0; i < modelEntities.size(); ++i) {
            if(modelEntities[i].get() == objetiveEntity) {
                //delete the EntityObject
                //deleteUniquePtrObject(modelEntities[i]);

                modelEntities.erase(modelEntities.begin() + i);
                return;
            }
        }

        // Find in the cameras
        for(std::size_t i = 0; i < cameraEntities.size(); ++i) {
            if(cameraEntities[i].get() == objetiveEntity) {
                //delete the EntityObject
                //deleteUniquePtrObject(cameraEntities[i]);

                cameraEntities.erase(cameraEntities.begin() + i);
                return;
            }
        }

        // Find in the lights
        for(std::size_t i = 0; i < lightEntities.size(); ++i) {
            if(lightEntities[i].get() == objetiveEntity) {
                //delete the EntityObject
                //deleteUniquePtrObject(lightEntities[i]);

                lightEntities.erase(lightEntities.begin() + i);
                return;
            }
        }

    }
}

void pap::PapayaDevice::deleteNodeFromAuxiliarVectors(pap::SceneNode *objetiveNode) {
    // Search in camera nodes
    for(std::size_t i = 0; i < cameraNodes.size(); ++i) {
        if(cameraNodes[i] == objetiveNode) {

            cameraNodes.erase(cameraNodes.begin() + i);
            return;
        }
    }

    // Search in point Light nodes
    for(std::size_t i = 0; i < pointLightNodes.size(); ++i) {
        if(pointLightNodes[i] == objetiveNode) {

            pointLightNodes.erase(pointLightNodes.begin() + i);
            return;
        }
    }

    // Search in transparent nodes
    for(std::size_t i = 0; i < transparentNodes.size(); ++i) {
        if(transparentNodes[i] == objetiveNode) {

            transparentNodes.erase(transparentNodes.begin() + i);
            return;
        }
    }
}


void pap::PapayaDevice::deleteNodeFromVector(pap::SceneNode *objetiveNode) {
    for(std::size_t i = 0; i < sceneNodes.size(); ++i) {
        if(sceneNodes[i].get() == objetiveNode) {
            //delete the pap::SceneNodeObject
            //deleteUniquePtrObject(sceneNodes[i]);

            sceneNodes.erase(sceneNodes.begin() + i);
            return;
        }
    }
}

void pap::PapayaDevice::deleteSceneNodeSons(pap::SceneNode *objetiveNode) {
    std::vector <pap::SceneNode *> &sons = objetiveNode->getSons();

    for(std::size_t i = 0; i < sons.size(); ++i) {
        deleteSceneNode(sons[i]);
    }
}


void pap::PapayaDevice::draw2DImage(std::string texturePath, glm::vec2 translation, glm::vec2 scale) {
    draw2DImage(texturePath, translation, scale, glm::vec4(1.f, 1.f, 1.f, 1.f));
}

void pap::PapayaDevice::draw2DImage(std::string texturePath, glm::vec2 translation, glm::vec2 scale, glm::vec4 color) {
    // Create the plane with texture
    pap::ResourceModel *hudElement = resManager.getGeometryModel("plane2D", texturePath, "texture2D");

    // Draw 2D
    if(hudElement) {
        // Coordenate adjustment
        scale.x = scale.x / 2.f;
        scale.y = scale.y / 2.f;

        glm::vec2 position;
        position.x = translation.x + scale.x;
        position.y = translation.y + scale.y;


        hudElement->draw2D(HUDShader, position, scale, projection2d, color);
    }
}

void pap::PapayaDevice::renderText(std::string text, GLfloat posX, GLfloat posY, GLfloat scale, glm::vec3 color, bool centered) {
    textRenderer.renderText(textShader, text, posX, posY, scale, color, projection2d, centered);
}

void pap::PapayaDevice::renderText3d(std::string text, glm::vec3 textPos, glm::vec3 textRot, GLfloat scale, glm::vec3 color, std::string texture) {

    textRenderer.renderText3d(*this, text3DShader, text, textPos, textRot, scale, color, texture);
}

void pap::PapayaDevice::setTextureSceneNode(pap::SceneNode *objetiveNode, std::string texturePath, bool transparency) {

    if(objetiveNode) {
        if(transparency && !objetiveNode->transparency) { // if it is changing to a transparent texture
            objetiveNode->transparency = true;
            transparentNodes.emplace_back(objetiveNode);
        }
        // TODO: Change from transparent to not transparent


        pap::EntityModel *entity = static_cast<pap::EntityModel *>(objetiveNode->getEntity());
        pap::ResourceModel *model = entity->getResourceModel();
        if(model) {
            std::string geometry = splitIterator(model->path, '?')[0];
            pap::ResourceModel *newModel = resManager.getGeometryModel(geometry, texturePath);
            static_cast<pap::EntityModel *>(objetiveNode->getEntity())->setResourceModel(newModel);
        }
    }
}

std::vector<std::string> pap::PapayaDevice::splitIterator(const std::string &str, const char separator) {
    std::vector<std::string> result;

    std::string::const_iterator itBegin = str.begin();
    std::string::const_iterator itEnd   = str.end();

    int numItems = 1;
    for( std::string::const_iterator it = itBegin; it != itEnd; ++it )
        numItems += *it == separator;

    result.reserve(numItems);

    for( std::string::const_iterator it = itBegin; it != itEnd; ++it ) {
        if( *it == separator ) {
            result.push_back(std::string(itBegin, it));
            itBegin = it + 1;
        }
    }

    if( itBegin != itEnd )
        result.push_back(std::string(itBegin, itEnd));

    return result;
}


pap::TextRenderer &pap::PapayaDevice::getTextRenderer() {
    return textRenderer;
}



void pap::PapayaDevice::setUpElement2Din3D() {

    if (b2Din3DVAO == 0) {
        float quadVertices[] = {
            // positions        // texture Coords
            -5.0f,  5.0f, 0.0f, 1.0f,
                -5.0f, -5.0f, 0.0f, 0.0f,
                5.0f,  5.0f, 1.0f, 1.0f,
                5.0f, -5.0f, 1.0f, 0.0f,
            };
        // setup bubble VAO
        glGenVertexArrays(1, &b2Din3DVAO);
        glGenBuffers(1, &b2Din3DVBO);
        glBindVertexArray(b2Din3DVAO);
        glBindBuffer(GL_ARRAY_BUFFER, b2Din3DVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)0);

    }

    /*Texture texture = resManager.getResourceTexture("./media/boxes/speechBubble/speechBubble.png", "texture2D");
    textureID = texture.id;*/
}


void pap::PapayaDevice::drawElement2Din3D(glm::vec3 position, glm::vec3 vecTextRotation, glm::vec3 escalade, std::string texture) {

    uint32_t textureID = resManager.getResourceTexture(texture, "texture2D").id;

    glm::mat4 projection = glm::mat4(1.0f);
    glm::mat4 view       = glm::mat4(1.0f);

    pap::EntityCamera *cameraEnt = static_cast<pap::EntityCamera *>(getActiveCameraEntity());
    view        = cameraEnt -> view;
    projection  = cameraEnt -> projection;

    glm::mat4 model = calcTranslationModelMat(position) * calcRotationModelMat(vecTextRotation) * calcScaleModelMat(escalade);


    //Enable alpha chanel
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    //Disable depth test
    glDisable(GL_DEPTH_TEST);


    s2Din3DShader.use();

    s2Din3DShader.setMat4("projection", projection);
    s2Din3DShader.setMat4("view", view);

    s2Din3DShader.setMat4("model", model);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glBindVertexArray(b2Din3DVAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);


    // Disable the alpha
    glDisable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);

}

glm::mat4 pap::PapayaDevice::calcRotationModelMat(glm::vec3 textRotation) const {
    glm::mat4 identityMat = glm::mat4(1.0f);
    glm::mat4 matrixAux =
        glm::rotate(identityMat, glm::radians(textRotation.x), glm::vec3(1.0f, 0.0f, 0.0f))
        * glm::rotate(identityMat, glm::radians(textRotation.y), glm::vec3(0.0f, 1.0f, 0.0f))
        * glm::rotate(identityMat, glm::radians(textRotation.z), glm::vec3(0.0f, 0.0f, 1.0f));

    return matrixAux;
}

glm::mat4 pap::PapayaDevice::calcScaleModelMat(glm::vec3 escalade) const {
    glm::mat4 identityMat = glm::mat4(1.0f);

    glm::mat4 matrixAux = glm::scale(identityMat, escalade);

    return matrixAux;
}

glm::mat4 pap::PapayaDevice::calcTranslationModelMat(glm::vec3 textPos) const {
    glm::mat4 identityMat = glm::mat4(1.0f);
    glm::mat4 matrixAux = glm::translate(identityMat, textPos);

    return matrixAux;
}