
#include "engine.h"

#include <time.h>
#include <stdbool.h>
#include <stdio.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include "music.h"
#include "bomb.h"
#include "willyoubemine.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define THREE_FLOAT 3 * sizeof(float)
#define TWO_FLOAT 2 * sizeof(float)
#define STATE_GAME 0x2

static int bomb_time = 0;

static bool draw_bomb = false;
static Model music_arrows[BPM];
// static bool quit = false;
static Texture arrow_tex;
static Texture music_arrow_tex[4];
static Model arrow[4];
static Model bomb;
static Texture bomb_tex;
static int state = STATE_GAME;
static float coord_lookup[5] = {
    -10.0f,
    -4.0f,
    -2.0f,
    0.0f,
    2.0f
};

static float explosionSize = 0.0f;
static bool explosion;

static float angle_lookup[5] = {
    -10.0f,
    90.0f,
    0.0f,
    180.0f,
    -90.0f
};

bool keys[4] = {false, false, false, false};;

void backend_swap();

Texture load_texture(const char* path) {
    Texture t;
    int channels;
    
    unsigned char* data = stbi_load(path, &t.w, &t.h, &channels, 0);
    
    if(!data)
        perror("Could not load texture.");
    
    glGenTextures(1, &t.tex_id);
    
    glBindTexture(GL_TEXTURE_2D, t.tex_id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, t.w, t.h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    
    stbi_image_free(data);
    
    return t;
}

int engine_init_game() {
    srand(time(NULL));
    
    arrow_tex = load_texture("assets/arrowInv.png");
    
    music_arrow_tex[0] = load_texture("assets/arrowA.png");
    music_arrow_tex[1] = load_texture("assets/arrowB.png");
    music_arrow_tex[2] = load_texture("assets/arrowC.png");
    music_arrow_tex[3] = load_texture("assets/arrowD.png");
    
    bomb_tex = load_texture("assets/bomb.png");
    
    
    engine_create_model(&arrow[0], arrow_tex.tex_id, -4.0f, 2.0f, -10.0f, arrowVerts);
    engine_create_model(&arrow[1], arrow_tex.tex_id, -2.0f, 2.0f, -10.0f, arrowVerts);
    engine_create_model(&arrow[2], arrow_tex.tex_id, 0.0f, 2.0f, -10.0f, arrowVerts);
    engine_create_model(&arrow[3], arrow_tex.tex_id, 2.0f, 2.0f, -10.0f, arrowVerts);
    arrow[0].angle = 90.0f;
    arrow[2].angle = 180.0f;
    arrow[3].angle = -90.0f;
    
    bomb_time = rand() % 100;
    
    for(int i = 0; i < BPM; i++) {
        if(music[i] != 0) {
            engine_create_model(&music_arrows[i], music_arrow_tex[music[i]-1].tex_id, coord_lookup[music[i]], -2.0f*i, -10.0f, arrowVerts);
            music_arrows[i].angle = angle_lookup[music[i]];
        }
        
        if(i == bomb_time) {
            engine_create_model(&bomb, bomb_tex.tex_id, 0.0f, -2.0f*i, -10.0f, bombVerts);
            printf("bomb at %d\n", bomb_time);
        }
    }
    
    init_music();
    
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_DEPTH_TEST);
    glClearDepth(1.0);
    
    // glClearColor(0.0, 0.0, 0.0, 0.0);
    glClearColor(1.0, 1.0, 1.0, 0.0);
    glShadeModel(GL_SMOOTH);
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    
    // gluLookAt(0.0, 0.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
    gluPerspective(45.0f, 600.0f/480.0f, 0.1f, 100.0f);

    glMatrixMode(GL_MODELVIEW);

    
    return 0;
}

void engine_start_frame() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    // glColor3f(1.0, 0.0, 0.0);
}

void engine_reset_arrows() {
    for(int i = 0; i < BPM; i++) {
        if(music[i] != 0) {
            music_arrows[i].x = coord_lookup[music[i]];
            music_arrows[i].y = -2.0f*i;
            music_arrows[i].z = -10.0f;
        }
        if(i == bomb_time) {
            bomb.y = -2.0f*i;
            bomb.z = -10.0f;
        }
    }
    
    draw_bomb = false;
}

void engine_create_model(Model* model, unsigned int texture, float x, float y, float z, Obj2glVerts model_data) {
    model->x = x;
    model->y = y;
    model->z = z;
    model->angle = 0.0f;
    model->tex_id = texture;
    model->verts = model_data.vertCoords;
    model->normals = model_data.normalCoords;
    model->uvs = model_data.texCoords;
    model->vertSize = model_data.numVerts;
}

void engine_draw_model(Model* model) {
    glLoadIdentity();
    glTranslatef(model->x, model->y, model->z);

    glBindTexture(GL_TEXTURE_2D, model->tex_id); 
    
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    
    glVertexPointer(3, GL_FLOAT, THREE_FLOAT, model->verts);
    glNormalPointer(GL_FLOAT, THREE_FLOAT, model->normals);
    glTexCoordPointer(2, GL_FLOAT, TWO_FLOAT, model->uvs);
    
    glDrawArrays(GL_TRIANGLES, 0, model->vertSize);
    
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}

void engine_draw_model_rotated(Model* model) {
    glLoadIdentity();
    glTranslatef(model->x, model->y, model->z);
    glRotatef(model->angle, 0.0f, 0.0f, 1.0f);

    glBindTexture(GL_TEXTURE_2D, model->tex_id); 
    
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    
    glVertexPointer(3, GL_FLOAT, THREE_FLOAT, model->verts);
    glNormalPointer(GL_FLOAT, THREE_FLOAT, model->normals);
    glTexCoordPointer(2, GL_FLOAT, TWO_FLOAT, model->uvs);
    
    glDrawArrays(GL_TRIANGLES, 0, model->vertSize);
    
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}

bool engine_check_collision(Model* model) {
    bool result = false;
    if(model->y > 1.5f && model->y < 2.8f) {
        
        if(model->x == -4.0f && keys[0] == true) //pressed at the right moment
            result = true;
        
        else if(model->x == -2.0f && keys[1] == true) //pressed at the right moment
            result = true;
        
        else if(model->x == 0.0f && keys[2] == true) //pressed at the right moment
            result = true;
        
        else if(model->x == 2.0f && keys[3] == true) //pressed at the right moment
            result = true;
    }
    return result;
}

void engine_draw_explosion() {
    // glColor3f(1.0f, 0.0f, 0.0f);
    
    glBegin(GL_LINES);
    for (int i = 0; i < 2000; ++i) {
        float theta = (float)i / 2000 * 2 * M_PI;
        float length = ((float)rand() / RAND_MAX) * explosionSize;
        float x = length * cos(theta);
        float y = length * sin(theta);

        glColor3f(1.0f, 0.0f, 0.1f);
        glVertex3f(0.0, 0.0, 0.0);
        glVertex3f(x, y, -10.0f);
    }
    glEnd();
    glColor3f(1.0, 1.0, 1.0);
}

void engine_end_frame() {
    backend_swap();
}

void engine_state_manager(int state) {
    switch(state) {
        case STATE_GAME:
            for(int i = 0; i < 4; i++) {
                // arrow[i].y += 0.01f;
                engine_draw_model_rotated(&arrow[i]);
            }
            
            for(int i = 0; i < BPM; i++) {
                music_arrows[i].y += .1f;

                engine_draw_model_rotated(&music_arrows[i]);
                if(engine_check_collision(&music_arrows[i])) {
                    for(int b = 0; b < 60; b++) {
                        music_arrows[i].z -= 1.0f;
                        engine_draw_model_rotated(&music_arrows[i]);
                    }
                    music_arrows[i].y = 16.0f;
                }
                
                if(i == bomb_time)
                    draw_bomb = true;
                
            }
            if(draw_bomb) {
                bomb.y += .1f;
                bomb.angle += 5.0f;
                engine_draw_model_rotated(&bomb);
                if(engine_check_collision(&bomb))
                    explosion = true;

                if(explosion) {
                    explosionSize += 4.0f;
                    engine_draw_explosion();
                }
                if(explosionSize > 30.0f) {
                    exit(0); //TODO: Gameover screen
                }
            }
            
            if(music_arrows[BPM-1].y > 6.0f)
                engine_reset_arrows();
        
            break;
        
        default:
            perror("Wrong state");
            break;
    }
}

void engine_loop() {
    
    engine_start_frame();
        
    engine_state_manager(state);

    engine_end_frame();
}

void engine_close() {
    exit_music();
}
