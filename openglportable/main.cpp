#include <GL/glut.h>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <cstdio>

#define MAX_OBS 8
#define MAX_SNOW 150

struct Obstacle { float x, y; bool isTree; };
struct Bullet { float x, y; bool active; }; // Life pickup (heart)
struct Snow { float x, y; };

Obstacle obs[MAX_OBS];
Bullet bullet = {0, 1.2f, false};
Snow snow[MAX_SNOW];

float playerX = 0.0f;
float playerY = -0.75f;
bool gameOver = false;
int lives = 0;
int score = 0;
static int highScore = 0;

float randRange(float a, float b) {
    return a + (b - a) * (rand() / (float)RAND_MAX);
}

//--------------------------------------------
// CONTROL - Player input and movement
//--------------------------------------------
void resetObstacles() {
    for(int i = 0; i < MAX_OBS; i++) {
        obs[i].y = randRange(1.2f, 2.5f);
        obs[i].x = randRange(-0.4f, 0.4f);
        obs[i].isTree = rand() % 2;
    }
}