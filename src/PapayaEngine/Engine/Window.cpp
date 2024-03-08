#include "Window.hpp"
//console.log
#include <math.h>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

int Window::windowWidth  = 0;
int Window::windowHeight = 0;

Window::Window(uint32_t width, uint32_t height, bool fullScreen) {
    Window::windowWidth  = width;
    Window::windowHeight = height;


    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    if(fullScreen) {
        window = glfwCreateWindow(width, height, "Vincula", glfwGetPrimaryMonitor(), NULL);
    } else {
        window = glfwCreateWindow(width, height, "Vincula", NULL, NULL);
    }
    if (window == NULL) {
        close();
    }
    glfwMakeContextCurrent(window);
    glViewport(0, 0, width, height);

    glfwSetFramebufferSizeCallback(window, framebuffer_resize_callback);
    glfwSetCursorPosCallback(window, mouse_pos_callback);
    glfwSetKeyCallback(window, key_callback);
    glfwSetMouseButtonCallback(window, mouse_callback);
    glfwSetScrollCallback(window, mouse_scroll_callback);
}

Window::~Window() {

}

void Window::framebuffer_resize_callback(GLFWwindow *window, GLsizei width, GLsizei height) {
    glViewport(0, 0, width, height);
    Window::windowWidth  = width ;
    Window::windowHeight = height;
}

void Window::key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    switch(action) {
    case GLFW_PRESS:
        receiver.setKeyValue(key, true);
        break;

    case GLFW_REPEAT:
        receiver.setKeyValue(key, true);
        break;

    case GLFW_RELEASE:
        receiver.setKeyValue(key, false);
        break;

    default:
        break;
    }

}

void Window::mouse_callback(GLFWwindow *window, int button, int action, int mods) {

    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        mouse_set_callback(action, mouseReceiver.LeftButtonDown);
    }

    if (button == GLFW_MOUSE_BUTTON_RIGHT) {
        mouse_set_callback(action, mouseReceiver.RightButtonDown);
    }

    if (button == GLFW_MOUSE_BUTTON_MIDDLE ) {
        mouse_set_callback(action, mouseReceiver.MiddleButtonDown);
    }
}

void Window::mouse_set_callback(int action, bool &aux) {
    switch(action) {
    case GLFW_PRESS:
        aux = true;
        break;

    case GLFW_RELEASE:
        aux = false;
        break;

    default:
        break;
    }
}

void Window::mouse_scroll_callback(GLFWwindow *window, double xoffset, double yoffset) {
    if (yoffset == 1) {
        mouseReceiver.ScrollUp      = true;
        mouseReceiver.ScrollDown    = false;
    } else if (yoffset == -1) {
        mouseReceiver.ScrollUp      = false;
        mouseReceiver.ScrollDown    = true;
    } else {
        mouseReceiver.ScrollUp      = false;
        mouseReceiver.ScrollDown    = false;
    }
}


void Window::mouse_pos_callback(GLFWwindow *window, double xpos, double ypos) {
    mouseReceiver.mouseX = (float)xpos;
    mouseReceiver.mouseY = (float)ypos;
}

void Window::close() {
    glfwSetWindowShouldClose(window, true);
    glfwTerminate();
}

void Window::clearColor(glm::vec4 color) const {
    glClearColor(color[0], color[1], color[2], color[3]);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}