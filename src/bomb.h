/*
 * auto created by obj2gl
 *
 * repo  : https://github.com/xiaozhuai/obj2gl
 * obj   : assets/mine.obj
 * faces : 768
 * verts : 2304
 *
 */

#ifndef OBJ2GL_OBJ2GLVERTS
#define OBJ2GL_OBJ2GLVERTS

typedef struct {
    int numFaces;
    int numVerts;
    float *vertCoords;
    float *normalCoords;
    float *texCoords;
} Obj2glVerts;

#endif



#ifndef OBJ2GL_BOMB
#define OBJ2GL_BOMB

extern Obj2glVerts bombVerts;

#endif
