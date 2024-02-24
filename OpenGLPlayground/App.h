#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>

class App {
public:
    virtual void initCallback(GLFWwindow* window) {}
    virtual void displayCallback(GLFWwindow* window, double elapsed) {}
    virtual void windowCloseCallback(GLFWwindow* window) {}
    virtual void windowSizeCallback(GLFWwindow* window, int width, int height) {}
    virtual void cursorCallback(GLFWwindow* window, double xpos, double ypos) {}
    virtual void keyCallback(GLFWwindow* window, int key, int scancode,
        int action, int mods) {}
    virtual void mouseButtonCallback(GLFWwindow* window, int button, int action,
        int mods) {}
    virtual void scrollCallback(GLFWwindow* window, double xoffset,
        double yoffset) {}
    virtual void joystickCallback(int jid, int event) {}
};
