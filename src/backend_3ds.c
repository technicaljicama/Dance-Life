//UNFINISHED
#ifdef __3DS__
#include <GL/picaGL.h>
#include <3ds.h>
#include <stdio.h>
#include <unistd.h>

#include "engine.h"

// static int window_id = -1;
/*
void keyboardCallback(unsigned char key, int x, int y) {
    switch (key) {
        case 27:
            glutDestroyWindow(window_id);
            exit(0);
            break;
    }
    glutPostRedisplay();
}*/

int setup_window() {
    pglSelectScreen(GFX_TOP, GFX_LEFT);
    
    return 0;
}

void backend_swap() {
    pglSwapBuffers();
}

int main(int argc, char** argv) {
    // glutInit(&argc, argv);
    gfxInitDefault();
    pglInit();
    
    if(setup_window()) {
        perror("Could not setup window.");
    }
    
    if(engine_init_game()) {
        perror("Could not load game.");
    }
    
    while(1) {
        engine_loop();
    }
    // usleep(1000000000);
    // glutMainLoop();
    
    return 0;
}
#endif
