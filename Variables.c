#include <time.h>

int tx1, ty1;
int vidas1 = 3;
int dir1X = 0, dir1Y = -1;
int spawnX1, spawnY1;

int tx2, ty2;
int vidas2 = 3;
int dir2X = 0, dir2Y = -1;
int spawnX2, spawnY2;

time_t inicio;

int corazonX = -1, corazonY = -1;
time_t spawnCorazon = 0;

time_t ultimoCorazon = 0;
time_t ultimaBomba = 0;

int bombaX = -1;
int bombaY = -1;
time_t tiempoBomba = 0;

int corazon_get = 0;
int disparando = 0;