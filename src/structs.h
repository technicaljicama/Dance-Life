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
