#pragma once
#include <glew.h>
#include <glfw3.h>
#include <stdint.h>

class DepthMap {
public:
    DepthMap();
    ~DepthMap();

    const uint32_t SHADOW_WIDTH = 2048, SHADOW_HEIGHT = 2048;
    unsigned int depthMapFBO;
    unsigned int depthMapTexture;
private:
};