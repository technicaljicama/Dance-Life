/*
 * auto created by obj2gl
 *
 * repo  : https://github.com/xiaozhuai/obj2gl
 * obj   : assets/machine.obj
 * faces : 1648
 * verts : 4944
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



#ifndef OBJ2GL_MACHINE
#define OBJ2GL_MACHINE

extern Obj2glVerts machineVerts;

#endif
