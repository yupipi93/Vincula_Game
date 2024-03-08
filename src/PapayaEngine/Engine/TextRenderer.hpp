#pragma once
#include <map>
#include <GL/glew.h>
// FreeType
#include <ft2build.h>
#include FT_FREETYPE_H

#include <PapayaEngine/Engine/Shader.hpp>
#include <PapayaEngine/Engine/Character.hpp>


namespace pap {
class PapayaDevice;
} // NAMESPACE

namespace pap {
class ResourceManager;
} // NAMESPACE

namespace pap{

class TextRenderer {
public:
    TextRenderer();
    ~TextRenderer();

	void loadFont(std::string fontPath, uint16_t size);
	void renderText(pap::Shader &s, std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color, glm::mat4 projection, bool centered);
	void renderText3d(pap::PapayaDevice &device, pap::Shader &s, std::string text, glm::vec3 textPos, glm::vec3 textRot, GLfloat scale, glm::vec3 color, std::string textureBackground);
    void setDialogueRotation(glm::vec3 textRotation);
private:
	void setCharacters(FT_Face &face);
    void configureBuffers();
    glm::mat4 calcTranslationModelMat(glm::vec3 textPos) const;
    glm::mat4 calcRotationModelMat(glm::vec3 textRotation) const;
    /*glm::mat4 calcScaleModelMat(glm::vec3 escalade) const;
    void setUpSpeechBubble(pap::ResourceManager &resManager);
    void drawSpeechBubble(pap::Shader &s, glm::vec3 position);*/


    std::map<GLchar, Character> characters;
    GLuint VAO, VBO;
    GLuint bubbleVAO, bubbleVBO;
    glm::vec3 vecTextRotation {0.f, 0.f, 0.f};
    float averageTextWidth = 0.f;
    uint32_t textureID = 0;
};

}