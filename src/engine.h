#include "structs.h"
#include <stdbool.h>
#include "arrow.h"

extern bool keys[4]; // respresents the arrows from left to right

int engine_init_game();
void engine_loop();
void engine_start_frame();
void engine_draw_model();
void engine_end_frame();
void engine_create_model(Model* model, unsigned int texture, float x, float y, float z, Obj2glVerts model_data);
void engine_state_manager(int state);
void engine_draw_model(Model* model);
void engine_draw_model_rotated(Model* model);
bool engine_check_collision(Model* model);
void engine_close();
void engine_draw_explosion();