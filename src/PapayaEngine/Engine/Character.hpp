#pragma once

struct Character {
    GLuint     TextureID;  // ID handle of the glyph texture
    glm::vec2  Size;       // Size of glyph
    glm::vec2  Bearing;    // Offset from baseline to left/top of glyph
    GLuint     Advance;    // Offset to advance to next glyph
};