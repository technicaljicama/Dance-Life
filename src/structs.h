#include <stdint.h>

typedef struct {
    unsigned int tex_id;
    int w;
    int h;
} Texture;

typedef struct {
    unsigned int tex_id;
    float* verts;
    float* normals;
    float* uvs;
    int vertSize;
    float x;
    float y;
    float z;
    float angle;
} Model;


typedef struct {
    uint32_t mp3_filename_size;
    char* mp3_file;
    uint32_t num_arrows;
} MusicPack;

extern uint32_t *arrows;
extern MusicPack mpack;
