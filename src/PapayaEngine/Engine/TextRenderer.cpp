#include "TextRenderer.hpp"
//console.log
#include <PapayaEngine/PapayaDevice.hpp>
#include <PapayaEngine/Engine/ResourceManager.hpp>
#include <PapayaEngine/Engine/Mesh.hpp>
#include <PapayaEngine/Engine/Texture.hpp>

#include <codecvt>
#include <locale>


const uint16_t MAX_CHARACTERS = 10000;

pap::TextRenderer::TextRenderer() {

}

pap::TextRenderer::~TextRenderer() {

}

void pap::TextRenderer::loadFont(std::string fontPath, uint16_t size) {
    FT_Library ft;
    if (FT_Init_FreeType(&ft)) {
    }

    FT_Face face;
    if (FT_New_Face(ft, fontPath.data(), 0, &face)) {
    }

    FT_Set_Pixel_Sizes(face, 0, size); // Set the font size of the face only needs the height

    //Now create the texture from the character
    setCharacters(face);
    configureBuffers();
}


void pap::TextRenderer::setCharacters(FT_Face &face) {
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // Disable byte-alignment restriction
    int nCharacters = 255;

    for (GLubyte c = 0; c < nCharacters; c++) {  // This works on the first 128 characters of ASCII
        // Load character glyph
        if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
            continue;
        }
        // Generate texture
        GLuint texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RED,
            face->glyph->bitmap.width,
            face->glyph->bitmap.rows,
            0,
            GL_RED,
            GL_UNSIGNED_BYTE,
            face->glyph->bitmap.buffer
        );

        // Set texture options
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // Now store character for later use
        Character character = {
            texture,
            glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
            glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
            face->glyph->advance.x
        };
        characters.insert(std::pair<GLchar, Character>(c, character));
        averageTextWidth += face->glyph->bitmap.width+2;
    }
    averageTextWidth = averageTextWidth / (float)nCharacters;
}

void pap::TextRenderer::configureBuffers() {
    // Configure VAO/VBO for texture quads
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}


void pap::TextRenderer::renderText(pap::Shader &s, std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color, glm::mat4 projection, bool centered) {
    // Activate corresponding render state
    s.use();
    //Enable alpha chanel
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    //Disable depth test
    glDisable(GL_DEPTH_TEST);

    glUniform3f(glGetUniformLocation(s.programID, "textColor"), color.x, color.y, color.z);
    s.setMat4("projection", projection);
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(VAO);

    //Centering text
    if(centered) {
        //Precision
        /*float avPhraseWidth = 0.f;
        for (auto c = text.begin(); c != text.end(); c++) {
            Character ch = characters[*c];
            avPhraseWidth += ch.Size.x;
        }
        x -= scale / 2 * avPhraseWidth;*/

        //Imprecision (Much Better)
        x -= text.size()*scale / 2 * averageTextWidth;
    }


    std::wstring wText = std::wstring_convert<std::codecvt_utf8<wchar_t>>().from_bytes(text);
    // Iterate through all characters
    std::wstring::const_iterator c;
    for (c = wText.begin(); c != wText.end(); c++) {
        Character ch = characters[*c];

        GLfloat xpos = x + ch.Bearing.x * scale;
        GLfloat ypos = y + (this->characters['H'].Bearing.y - ch.Bearing.y) * scale;

        GLfloat w = ch.Size.x * scale;
        GLfloat h = ch.Size.y * scale;
        // Update VBO for each character
        GLfloat vertices[6][4] = {
            { xpos,     ypos + h,   0.0, 1.0 },
            { xpos + w, ypos,       1.0, 0.0 },
            { xpos,     ypos,       0.0, 0.0 },

            { xpos,     ypos + h,   0.0, 1.0 },
            { xpos + w, ypos + h,   1.0, 1.0 },
            { xpos + w, ypos,       1.0, 0.0 }
        };
        // Render glyph texture over quad
        glBindTexture(GL_TEXTURE_2D, ch.TextureID);
        // Update content of VBO memory
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        // Render quad
        glDrawArrays(GL_TRIANGLES, 0, 6);
        // Now advance cursors for next glyph (note that advance is number of 1/64 pixels)
        x += (ch.Advance >> 6) * scale; // Bitshift by 6 to get value in pixels (2^6 = 64)
    }
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);

    // Disable the alpha
    glDisable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
}



// THIS IS TO RENDER 3D TEXT, BUT IT HAS TO BE MODIFIED
void pap::TextRenderer::renderText3d(pap::PapayaDevice &device, pap::Shader &s, std::string text, glm::vec3 textPos, glm::vec3 textRot, GLfloat scale, glm::vec3 color, std::string textureBackground) {

    glm::mat4 projection = glm::mat4(1.0f);
    glm::mat4 view       = glm::mat4(1.0f);
    glm::mat4 model = calcTranslationModelMat(textPos) * calcRotationModelMat(textRot);

    //float x = -((averageTextWidth * text.size() * scale) / 6.f);
    float x = 0.f;
    float y = 0.f;

    // Get the active camera
    pap::SceneNode *camera = device.getActiveCameraNode();
    if(camera) {
        //obtain the View and the Projection from the Scene Tree
        //view        = camera -> getTransfMatrix();

        pap::EntityCamera *cameraEnt = static_cast<pap::EntityCamera *>(camera -> getEntity());
        view        = cameraEnt -> view;
        projection  = cameraEnt -> projection;
    }



    //Draw Speech Bubble

    if(textureBackground != "") {
        x = -43.f;
        y = 35.f;
        glm::vec3 escalade = glm::vec3(10.f, 10.f, 10.f);
        textPos.y += 25.f;
        device.drawElement2Din3D(textPos, textRot, escalade, textureBackground);
    }


    /*
    sBubble.use();

    sBubble.setMat4("projection", projection);
    sBubble.setMat4("view", view);*/

    //Enable alpha chanel
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    //Disable depth test
    glDisable(GL_DEPTH_TEST);
    //Draw text
    // Activate corresponding render state
    s.use();

    s.setMat4("projection", projection);
    s.setMat4("view", view);
    s.setMat4("model", model);
    glUniform3f(glGetUniformLocation(s.programID, "textColor"), color.x, color.y, color.z);
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(VAO);


    std::wstring wText = std::wstring_convert<std::codecvt_utf8<wchar_t>>().from_bytes(text);

    // Iterate through all characters
    std::wstring::const_iterator c;
    for (c = wText.begin(); c != wText.end(); c++) {
        if(*c == '@') {

            x = -43.f;
            y -= 15.f;
            continue;
        }

        Character ch = characters[*c];

        GLfloat xpos = x + ch.Bearing.x * scale;
        GLfloat ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

        GLfloat w = ch.Size.x * scale;
        GLfloat h = ch.Size.y * scale;
        // Update VBO for each character
        GLfloat vertexPos[6][4] = {
            { xpos,     ypos + h, 0.0, 0.0 },
            { xpos,     ypos, 0.0, 1.0 },
            { xpos + w, ypos, 1.0, 1.0 },

            { xpos,     ypos + h, 0.0, 0.0 },
            { xpos + w, ypos, 1.0, 1.0 },
            { xpos + w, ypos + h, 1.0, 0.0 }
        };

        // Render glyph texture over quad
        glBindTexture(GL_TEXTURE_2D, ch.TextureID);
        // Update content of VBO memory
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertexPos), vertexPos);
        //glBufferSubData(GL_ARRAY_BUFFER, 1, sizeof(textPos), textPos);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        // Render quad
        glDrawArrays(GL_TRIANGLES, 0, 6);
        // Now advance cursors for next glyph (note that advance is number of 1/64 pixels)
        x += (ch.Advance >> 6) * scale; // Bitshift by 6 to get value in pixels (2^6 = 64)
    }

    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);

    // Disable the alpha
    glDisable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
}


glm::mat4 pap::TextRenderer::calcTranslationModelMat(glm::vec3 textPos) const {
    glm::mat4 identityMat = glm::mat4(1.0f);
    glm::mat4 matrixAux = glm::translate(identityMat, textPos);

    return matrixAux;
}

glm::mat4 pap::TextRenderer::calcRotationModelMat(glm::vec3 textRotation) const {
    glm::mat4 identityMat = glm::mat4(1.0f);
    glm::mat4 matrixAux =
        glm::rotate(identityMat, glm::radians(textRotation.x), glm::vec3(1.0f, 0.0f, 0.0f))
        * glm::rotate(identityMat, glm::radians(textRotation.y), glm::vec3(0.0f, 1.0f, 0.0f))
        * glm::rotate(identityMat, glm::radians(textRotation.z), glm::vec3(0.0f, 0.0f, 1.0f));

    return matrixAux;
}

void pap::TextRenderer::setDialogueRotation(glm::vec3 textRotation) {

    vecTextRotation = textRotation;

}

/*glm::mat4 pap::TextRenderer::calcScaleModelMat(glm::vec3 escalade) const {
    glm::mat4 identityMat = glm::mat4(1.0f);

    glm::mat4 matrixAux = glm::scale(identityMat, escalade);

    return matrixAux;
}

void pap::TextRenderer::setUpSpeechBubble(pap::ResourceManager &resManager){

     if (bubbleVAO == 0)
    {
        float quadVertices[] = {
            // positions        // texture Coords
            -5.0f,  5.0f, 0.0f, 1.0f,
            -5.0f, -5.0f, 0.0f, 0.0f,
             5.0f,  5.0f, 1.0f, 1.0f,
             5.0f, -5.0f, 1.0f, 0.0f,
        };
        // setup bubble VAO
        glGenVertexArrays(1, &bubbleVAO);
        glGenBuffers(1, &bubbleVBO);
        glBindVertexArray(bubbleVAO);
        glBindBuffer(GL_ARRAY_BUFFER, bubbleVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);

        Texture texture = resManager.getResourceTexture("./media/boxes/speechBubble/speechBubble.png", "texture2D");
        textureID = texture.id;

    }

}


void pap::TextRenderer::drawSpeechBubble(pap::Shader &s, glm::vec3 position){

    glm::vec3 escalade = glm::vec3(10.f, 10.f, 10.f);
    position.y += 25.f;
    glm::mat4 model = calcTranslationModelMat(position) * calcRotationModelMat(vecTextRotation) * calcScaleModelMat(escalade);

    s.setMat4("model", model);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glBindVertexArray(bubbleVAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);

}
*/