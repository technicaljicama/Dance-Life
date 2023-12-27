#ifndef __3DS__

#include <GLFW/glfw3.h>
#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>

#include "engine.h"

static GLFWwindow* window = NULL;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    for(int i = 0; i < 4; i++)
        keys[i] = false;
    
    if (key == GLFW_KEY_LEFT && action == GLFW_PRESS)
        keys[0] = true;
    
    if (key == GLFW_KEY_LEFT && action == GLFW_RELEASE)
        keys[0] = false;
    
    if (key == GLFW_KEY_UP && action == GLFW_PRESS)
        keys[1] = true;
    
    if (key == GLFW_KEY_UP && action == GLFW_RELEASE)
        keys[1] = false;
    
    if (key == GLFW_KEY_DOWN && action == GLFW_PRESS)
        keys[2] = true;
    
    if (key == GLFW_KEY_DOWN && action == GLFW_RELEASE)
        keys[2] = false;
    
    if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS)
        keys[3] = true;
    
    if (key == GLFW_KEY_RIGHT && action == GLFW_RELEASE)
        keys[3] = false;
    
}

int setup_window() {
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 1);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    
    window = glfwCreateWindow(600, 480, "Project Dance Dog", NULL, NULL); 
    glfwSetWindowSizeLimits(window, 600, 480, 600, 480);
    glfwMakeContextCurrent(window);
    
    glfwSetKeyCallback(window, key_callback);
    
    return 0;
}

void backend_swap() {
    glfwSwapBuffers(window);
}

int main(int argc, char** argv) {
    glfwInit();
    
    if(setup_window()) {
        perror("Could not setup window.");
    }
    
    if(engine_init_game()) {
        perror("Could not load game.");
    }
    
    // engine_loop();
    // usleep(1000000000);
    // glutMainLoop();
    while(!glfwWindowShouldClose(window)) {
        engine_loop();
        glfwPollEvents();
    }
    
    glfwDestroyWindow(window);
    engine_close();
    
    return 0;
}
#endif
