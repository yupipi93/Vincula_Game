#pragma once
#include <glew.h>
#include <glfw3.h>
#include <string>
#include <glm.hpp> // OpenGL mathematics
#include <PapayaEngine/Engine/Shader.hpp>
#include <PapayaEngine/Engine/ResourceModel.hpp>
#include <PapayaEngine/Engine/KeyboardReceiver.hpp>
#include <PapayaEngine/Engine/MouseReceiver.hpp>

class Window {
public:
    Window(uint32_t width, uint32_t height, bool fullScreen);
    ~Window();

    static void framebuffer_resize_callback(GLFWwindow *window, int width, int height);
    static void mouse_pos_callback(GLFWwindow *window, double xpos, double ypos);
    static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods);
    static void mouse_callback(GLFWwindow *window, int button, int action, int mods);
    static void mouse_scroll_callback(GLFWwindow *window, double xoffset, double yoffset);


    //void processInput(GLFWwindow *window);
    //void update(unsigned int cubeVAO, unsigned int lightVAO, pap::Shader cubepap::Shader, pap::Shader lightpap::Shader, pap::ResourceModel& model);
    void close();
    void clearColor(glm::vec4 color) const;



    GLFWwindow *window;
    inline static KeyboardReceiver receiver{};
    inline static MouseReceiver mouseReceiver{};

    static int  windowWidth;
    static int  windowHeight;
private:
    static void mouse_set_callback(int action, bool &aux);

};