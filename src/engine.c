
#include "engine.h"

#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <stdio.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include "music.h"
#include "bomb.h"
#include "machine.h"
#include "net.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define THREE_FLOAT 3 * sizeof(float)
#define TWO_FLOAT 2 * sizeof(float)
#define STATE_GAME 0x2

static int bomb_time = 0;

static bool draw_bomb = false;
static Model *music_arrows;
static Model *music_arrows2;
// static bool quit = false;
static Texture arrow_tex;
static Texture music_arrow_tex[4];
static Model arrow[4];

static Model bomb;
static Texture bomb_tex;

static Texture machine_tex;
static Model machine;

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
static float depth = -.1;
static float light_ambient[] = {1.0f, 1.0f, 1.0f, 0.0f};
static float light_position[] = {-4.0f, 2.0f, -10.0f, 0.0f};
static float light_diffuse[] = {1.0f, 1.0f, 1.0f, 0.0f};
static float light_specular[] = {1.0f, 1.0f, 1.0f, 0.0f};

static bool machine_back = false;
static bool machine_anim = false;

bool keys[4] = {false, false, false, false};
bool other_keys[4] = {false, false, false, false};


void backend_swap();

Texture load_texture(const char* path) {
    Texture t;
    int channels;
    GLenum format = GL_RGBA;
    
    unsigned char* data = stbi_load(path, &t.w, &t.h, &channels, 0);
    
    if(!data)
        perror("Could not load texture.");
    
    if (channels == 3) {
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        format = GL_RGB;
    }

    
    glGenTextures(1, &t.tex_id);
    
    glBindTexture(GL_TEXTURE_2D, t.tex_id);
    glTexImage2D(GL_TEXTURE_2D, 0, format, t.w, t.h, 0, format, GL_UNSIGNED_BYTE, data);

    // glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP);
    // glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP);
    // glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REFLECTION_MAP);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    
    stbi_image_free(data);
    
    return t;
}

int engine_init_game() {
    srand(time(NULL));
    init_music();
    
    arrow_tex = load_texture("assets/arrowInv.png");
    
    machine_tex = load_texture("assets/machine.png");
    engine_create_model(&machine, machine_tex.tex_id, 0.0f, 0.0f, -20.0f, machineVerts);
    machine.sx = 5.0f;
    machine.sy = 5.0f;
    machine.sz = 5.0f;
    
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
    
    music_arrows = (Model*)malloc(sizeof(Model) * mpack.num_arrows);
    music_arrows2 = (Model*)malloc(sizeof(Model) * mpack.num_arrows);
    
    bomb_time = rand() % 100;
    
    for(int i = 0; i < mpack.num_arrows; i++) {
        if(music[i] != 0) {
            engine_create_model(&music_arrows[i], music_arrow_tex[music[i]-1].tex_id, coord_lookup[music[i]], -2.0f*i, -10.0f, arrowVerts);
            music_arrows[i].angle = angle_lookup[music[i]];
            
            engine_create_model(&music_arrows2[i], music_arrow_tex[music[i]-1].tex_id, coord_lookup[music[i]], -2.0f*i, -10.0f, arrowVerts);
            music_arrows2[i].angle = angle_lookup[music[i]];
        }
        
        if(i == bomb_time) {
            engine_create_model(&bomb, bomb_tex.tex_id, 0.0f, -2.0f*i, -10.0f, bombVerts);
            printf("bomb at %d\n", bomb_time);
        }
    }
    
    
    
    float dif[] = {0.784,0.071,0.667, 1.0};
    float em[] = {1,1,1, 1.0};
    
    // glMaterialfv(GL_FRONT, GL_EMISSION, em);
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, em);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    // glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_DEPTH_TEST);
    // glEnable(GL_TEXTURE_CUBE_MAP);
    // glEnable(GL_TEXTURE_GEN_S);
    // glEnable(GL_TEXTURE_GEN_T);
    // glEnable(GL_TEXTURE_GEN_R);
    glClearDepth(1.0);
    
    glClearColor(0.0, 0.0, 0.0, 0.0);
    // glClearColor(1.0, 1.0, 1.0, 0.0);
    glShadeModel(GL_SMOOTH);
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    
    // gluLookAt(0.0, 0.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
    gluPerspective(45.0f, 1200.0f/480.0f, 0.1f, 100.0f);

    glMatrixMode(GL_MODELVIEW);

    return 0;
}

void engine_start_frame() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    // glColor3f(1.0, 0.0, 0.0);
}

void engine_reset_arrows() {
    for(int i = 0; i < mpack.num_arrows; i++) {
        if(music[i] != 0) {
            music_arrows[i].x = coord_lookup[music[i]];
            music_arrows[i].y = -2.0f*i;
            music_arrows[i].z = -10.0f;
        }
        if(music[i] != 0) {
            music_arrows2[i].x = coord_lookup[music[i]];
            music_arrows2[i].y = -2.0f*i;
            music_arrows2[i].z = -10.0f;
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
    model->sx = 1.0f;
    model->sy = 1.0f;
    model->sz = 1.0f;
}

void engine_draw_model(Model* model) {
    glLoadIdentity();
    glTranslatef(model->x, model->y, model->z);
    glScalef(model->sx, model->sy, model->sz);
    glRotatef(model->angle, 0.0f, 1.0f, 1.0f);

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

void engine_draw_model_rotated(Model* model, int draw_other) {
    if(draw_other == 1 || draw_other == 0) {
        glLoadIdentity();
        glTranslatef(model->x - 5.0f, model->y, model->z);
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
    
    if(net_func != NULL && draw_other == 2 || draw_other == 0) {
        glLoadIdentity();
        glTranslatef(model->x + 6.0f, model->y, model->z);
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

bool engine_check_other_collision(Model* model) {
    bool result = false;
    if(model->y > 1.5f && model->y < 2.8f) {
        
        if(model->x == -4.0f && other_keys[0] == true) //pressed at the right moment
            result = true;
        
        else if(model->x == -2.0f && other_keys[1] == true) //pressed at the right moment
            result = true;
        
        else if(model->x == 0.0f && other_keys[2] == true) //pressed at the right moment
            result = true;
        
        else if(model->x == 2.0f && other_keys[3] == true) //pressed at the right moment
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
                engine_draw_model_rotated(&arrow[i], 0);
            }
            if(machine_anim == false) {
                machine.z++;
                
                if(machine.z > -11.0f)
                    machine_anim = true;
            }
            if(!machine_back)
                machine.angle += 0.1f;
            else
                machine.angle -= 0.1f;
            if(machine.angle > 18.0f)
                machine_back = true;
            if(machine.angle < -18.0f)
                machine_back = false;
        
            engine_draw_model(&machine);
            for(int i = 0; i < mpack.num_arrows; i++) {
                music_arrows[i].y += .1f;
                
                // music_arrows[i].z += depth;

                engine_draw_model_rotated(&music_arrows[i], 1);
                if(engine_check_collision(&music_arrows[i])) {
                    for(int b = 0; b < 60; b++) {
                        music_arrows[i].z -= 1.0f;
                        engine_draw_model_rotated(&music_arrows[i], 1);
                    }
                    music_arrows[i].y = 16.0f;
                    light_ambient[0] = ((float)rand() / RAND_MAX) * 1.0f;
                    light_ambient[1] = ((float)rand() / RAND_MAX) * 1.0f;
                    light_ambient[2] = ((float)rand() / RAND_MAX) * 1.0f;
                    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
                }
                if(net_func != NULL) {
                    music_arrows2[i].y += .1f;
                    engine_draw_model_rotated(&music_arrows2[i], 2);
                    
                    if(engine_check_other_collision(&music_arrows2[i])) {
                        for(int b = 0; b < 60; b++) {
                            music_arrows2[i].z -= 1.0f;
                            engine_draw_model_rotated(&music_arrows2[i], 2);
                        }
                        music_arrows2[i].y = 16.0f;
                        light_ambient[0] = ((float)rand() / RAND_MAX) * 1.0f;
                        light_ambient[1] = ((float)rand() / RAND_MAX) * 1.0f;
                        light_ambient[2] = ((float)rand() / RAND_MAX) * 1.0f;
                        glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
                    }
                }
                if(i == bomb_time && net_func == NULL)
                    draw_bomb = true;
                
            }
            if(draw_bomb) {
                bomb.y += .1f;
                bomb.angle += 5.0f;
                engine_draw_model_rotated(&bomb, 1);
                if(engine_check_collision(&bomb))
                    explosion = true;

                if(explosion) {
                    explosionSize += 4.0f;
                    engine_draw_explosion();
                }
                if(explosionSize > 30.0f) {
                    printf("Hit bomb\n");
                    exit(0); //TODO: Gameover screen
                }
            }
            
            if(music_arrows[mpack.num_arrows-1].y > 6.0f)
                engine_reset_arrows();
        
            break;
        
        default:
            perror("Wrong state");
            break;
    }
}

void engine_loop() {
    
    engine_start_frame();
    
    if(net_func != NULL) {
        net_func();
        if(net_func != net_send_keys) {
            net_server_send_keys();
            // player = 1;
        }
        if(net_func != net_read_keys) {
            net_client_read_keys();
            // player = 2;
        }
    }
        
    engine_state_manager(state);

    engine_end_frame();
}

void engine_close() {
    free(music_arrows);
    free(music_arrows2);
    exit_music();
}
