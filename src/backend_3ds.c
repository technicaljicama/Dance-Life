//UNFINISHED
#ifdef __3DS__
#include <GL/picaGL.h>
#include <3ds.h>
#include <stdio.h>
#include <unistd.h>

#include "engine.h"

// int __stacksize__ = 4 * 1024 * 1024;

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
    gfxInit(GSP_RGBA8_OES, GSP_RGB565_OES, false);
    pglInit();
    
    if(setup_window()) {
        perror("Could not setup window.");
    }
    
    glViewport(0, 0, 400, 240);
    
    if(engine_init_game()) {
        perror("Could not load game.");
    }
    
    while(aptMainLoop()) {
        gspWaitForVBlank();
        hidScanInput();
        
        engine_loop();
        
        if(hidKeysDown() & KEY_START)
            break;
    }
    
    pglExit();
    gfxExit();
    
    return 0;
}
#endif
