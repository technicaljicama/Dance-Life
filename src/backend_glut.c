//UNFINISHED
#ifndef __3DS__
#include <GL/glut.h>
#include <stdio.h>
#include <unistd.h>

#include "engine.h"

static int window_id = -1;

void keyboardCallback(unsigned char key, int x, int y) {
    switch (key) {
        case 27:
            glutDestroyWindow(window_id);
            exit(0);
            break;
    }
    glutPostRedisplay();
}

int setup_window() {
    glutInitWindowSize(600, 480);
    
    window_id = glutCreateWindow("Project Dance Dog");
    
    glutKeyboardFunc(keyboardCallback);
    glutDisplayFunc(engine_loop);
    
    return 0;
}

void backend_swap() {
    glutSwapBuffers();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    
    if(setup_window()) {
        perror("Could not setup window.");
    }
    
    if(engine_init_game()) {
        perror("Could not load game.");
    }
    
    // engine_loop();
    // usleep(1000000000);
    glutMainLoop();
    
    return 0;
}
#endif
