#include <GL/glut.h>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <cstdio>

#define MAX_OBS 8
#define MAX_SNOW 150

//--------------------------------------------
// FORWARD DECLARATIONS & GLOBAL VARIABLES
//--------------------------------------------
struct Obstacle { float x, y; bool isTree; bool isSnowman; };
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
        int r = rand() % 10;
        if (r < 5) { // 50%: Trees
            obs[i].isTree = true;
            obs[i].isSnowman = false;
            obs[i].x = randRange(-0.4f, 0.4f);
        } else if (r < 8) { // 30%: Rocks
            obs[i].isTree = false;
            obs[i].isSnowman = false;
            obs[i].x = randRange(-0.4f, 0.4f);
        } else { // 20%: Snowmen (only on sides)
            obs[i].isTree = false;
            obs[i].isSnowman = true;
            if (rand() % 2 == 0)
                obs[i].x = randRange(-0.4f, -0.3f); // Left
            else
                obs[i].x = randRange(0.3f, 0.4f);   // Right
        }
    }
}

void keyboard(unsigned char key, int, int) {
    if (key == 'a' || key == 'A') playerX -= 0.08f;
    if (key == 'd' || key == 'D') playerX += 0.08f;
    if (key == 'r' || key == 'R') {
        gameOver = false;
        playerX = 0.0f;
        playerY = -0.75f;
        score = 0;
        lives = 0;
        bullet.active = false;
        resetObstacles();
    }
    if (key == 27) exit(0); // ESC to quit

    if (playerX < -0.45f) playerX = -0.45f;
    if (playerX > 0.45f) playerX = 0.45f;
}

//--------------------------------------------
// LIFE PICKUP SYSTEM
//--------------------------------------------
void drawLifePickup() {
    if (!bullet.active) return;
    glPushMatrix();
    glTranslatef(bullet.x, bullet.y, 0);
    glColor3f(1, 0, 0);

    glBegin(GL_POLYGON);
    for (int i = 0; i < 20; i++) {
        float a = i * 2 * 3.1416f / 20;
        glVertex2f(cos(a) * 0.015f - 0.01f, sin(a) * 0.015f + 0.01f);
    }
    glEnd();

    glBegin(GL_POLYGON);
    for (int i = 0; i < 20; i++) {
        float a = i * 2 * 3.1416f / 20;
        glVertex2f(cos(a) * 0.015f + 0.01f, sin(a) * 0.015f + 0.01f);
    }
    glEnd();

    glBegin(GL_TRIANGLES);
        glVertex2f(-0.025f, 0.01f);
        glVertex2f(0.025f, 0.01f);
        glVertex2f(0.0f, -0.025f);
    glEnd();
    glPopMatrix();
}

void spawnLifePickup(int) {
    if (!gameOver && !bullet.active) {
        bullet.active = true;
        bullet.x = randRange(-0.4f, 0.4f);
        bullet.y = 1.2f;
    }
    glutTimerFunc(10000, spawnLifePickup, 0);
}

void drawLifeIcon(float x, float y, float scale) {
    glPushMatrix();
    glTranslatef(x, y, 0);
    glScalef(scale, scale, 1);
    glColor3f(1, 0, 0);

    glBegin(GL_POLYGON);
    for (int i = 0; i < 20; i++) {
        float a = i * 2 * 3.1416f / 20;
        glVertex2f(cos(a) * 0.015f - 0.01f, sin(a) * 0.015f + 0.01f);
    }
    glEnd();

    glBegin(GL_POLYGON);
    for (int i = 0; i < 20; i++) {
        float a = i * 2 * 3.1416f / 20;
        glVertex2f(cos(a) * 0.015f + 0.01f, sin(a) * 0.015f + 0.01f);
    }
    glEnd();

    glBegin(GL_TRIANGLES);
        glVertex2f(-0.025f, 0.01f);
        glVertex2f(0.025f, 0.01f);
        glVertex2f(0.0f, -0.025f);
    glEnd();
    glPopMatrix();
}

//--------------------------------------------
// SCENE DESIGN
//--------------------------------------------
float getScale(float y) {
    return 0.6f + (1.0f - y) * 0.1f;
}

void drawPlayer() {
    glPushMatrix();
    glTranslatef(playerX, playerY, 0);
    glColor3f(0.2f, 0.6f, 0.9f);
    glBegin(GL_QUADS);
        glVertex2f(-0.03f, 0.06f); glVertex2f(0.03f, 0.06f);
        glVertex2f(0.03f, -0.06f); glVertex2f(-0.03f, -0.06f);
    glEnd();
    glColor3f(1.0f, 0.8f, 0.6f);
    glBegin(GL_POLYGON);
    for (int i = 0; i < 20; i++) {
        float a = i * 2 * 3.1416f / 20;
        glVertex2f(cos(a) * 0.025f, sin(a) * 0.025f + 0.085f);
    }
    glEnd();
    glPopMatrix();
}

// >>>>>>>>>>>  TREE DESIGN <<<<<<<<<<<<
void drawTree(float x, float y, float scale) {
    glPushMatrix();
    glTranslatef(x, y, 0);
    glScalef(scale, scale, 1);

    // Top layer
    glColor3f(0.0f, 0.7f, 0.2f);
    glBegin(GL_TRIANGLES);
        glVertex2f(0.0f, 0.15f);
        glVertex2f(-0.06f, 0.05f);
        glVertex2f(0.06f, 0.05f);
    glEnd();

    // Middle layer
    glColor3f(0.0f, 0.6f, 0.2f);
    glBegin(GL_TRIANGLES);
        glVertex2f(0.0f, 0.08f);
        glVertex2f(-0.08f, -0.02f);
        glVertex2f(0.08f, -0.02f);
    glEnd();

    // Bottom layer
    glColor3f(0.0f, 0.5f, 0.15f);
    glBegin(GL_TRIANGLES);
        glVertex2f(0.0f, 0.0f);
        glVertex2f(-0.10f, -0.10f);
        glVertex2f(0.10f, -0.10f);
    glEnd();

    // Trunk
    glColor3f(0.45f, 0.25f, 0.1f);
    glBegin(GL_QUADS);
        glVertex2f(-0.02f, -0.10f);
        glVertex2f(0.02f, -0.10f);
        glVertex2f(0.02f, -0.18f);
        glVertex2f(-0.02f, -0.18f);
    glEnd();

    glPopMatrix();
}

void drawRock(float x, float y, float scale) {
    glPushMatrix();
    glTranslatef(x, y, 0);
    glScalef(scale, scale, 1);
    glColor3f(0.5f, 0.5f, 0.5f);
    glBegin(GL_POLYGON);
        glVertex2f(-0.05f, -0.02f);
        glVertex2f(-0.02f, 0.04f);
        glVertex2f(0.05f, 0.03f);
        glVertex2f(0.06f, -0.02f);
        glVertex2f(0.02f, -0.05f);
    glEnd();
    glPopMatrix();
}

// >>>>>>>>>>> SNOWMAN DRAWING FUNCTION <<<<<<<<<<<<
void drawSnowman(float x, float y, float scale) {
    glPushMatrix();
    glTranslatef(x, y, 0);
    glScalef(scale, scale, 1);

    // Bottom ball
    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_POLYGON);
    for (int i = 0; i < 20; i++) {
        float a = i * 2 * 3.1416f / 20;
        glVertex2f(cos(a) * 0.06f, sin(a) * 0.06f - 0.08f);
    }
    glEnd();

    // Middle ball
    glBegin(GL_POLYGON);
    for (int i = 0; i < 20; i++) {
        float a = i * 2 * 3.1416f / 20;
        glVertex2f(cos(a) * 0.045f, sin(a) * 0.045f);
    }
    glEnd();

    // Head
    glBegin(GL_POLYGON);
    for (int i = 0; i < 20; i++) {
        float a = i * 2 * 3.1416f / 20;
        glVertex2f(cos(a) * 0.03f, sin(a) * 0.03f + 0.06f);
    }
    glEnd();

    // Eyes
    glColor3f(0.0f, 0.0f, 0.0f);
    glBegin(GL_POLYGON);
    for (int i = 0; i < 10; i++) {
        float a = i * 2 * 3.1416f / 10;
        glVertex2f(cos(a) * 0.005f - 0.012f, sin(a) * 0.005f + 0.075f);
    }
    glEnd();
    glBegin(GL_POLYGON);
    for (int i = 0; i < 10; i++) {
        float a = i * 2 * 3.1416f / 10;
        glVertex2f(cos(a) * 0.005f + 0.012f, sin(a) * 0.005f + 0.075f);
    }
    glEnd();

    // Nose (carrot)
    glColor3f(1.0f, 0.5f, 0.0f);
    glBegin(GL_TRIANGLES);
        glVertex2f(0.0f, 0.07f);
        glVertex2f(0.0f, 0.06f);
        glVertex2f(0.02f, 0.065f);
    glEnd();

    glPopMatrix();
}

void drawSnow() {
    glColor3f(1, 1, 1);
    glPointSize(2);
    glBegin(GL_POINTS);
    for (int i = 0; i < MAX_SNOW; i++)
        glVertex2f(snow[i].x, snow[i].y);
    glEnd();
}

//--------------------------------------------
// PIXEL FONT DEFINITIONS (5x7 grid)
//--------------------------------------------
const unsigned char font_G[7][5] = {{1,1,1,1,1},{1,0,0,0,0},{1,0,0,0,0},{1,0,1,1,1},{1,0,0,0,1},{1,0,0,0,1},{1,1,1,1,1}};
const unsigned char font_A[7][5] = {{0,1,1,1,0},{1,0,0,0,1},{1,0,0,0,1},{1,1,1,1,1},{1,0,0,0,1},{1,0,0,0,1},{1,0,0,0,1}};
const unsigned char font_M[7][5] = {{1,0,0,0,1},{1,1,0,1,1},{1,0,1,0,1},{1,0,1,0,1},{1,0,0,0,1},{1,0,0,0,1},{1,0,0,0,1}};
const unsigned char font_E[7][5] = {{1,1,1,1,1},{1,0,0,0,0},{1,0,0,0,0},{1,1,1,1,1},{1,0,0,0,0},{1,0,0,0,0},{1,1,1,1,1}};
const unsigned char font_O[7][5] = {{0,1,1,1,0},{1,0,0,0,1},{1,0,0,0,1},{1,0,0,0,1},{1,0,0,0,1},{1,0,0,0,1},{0,1,1,1,0}};
const unsigned char font_V[7][5] = {{1,0,0,0,1},{1,0,0,0,1},{1,0,0,0,1},{1,0,0,0,1},{1,0,0,0,1},{0,1,0,1,0},{0,0,1,0,0}};
const unsigned char font_R[7][5] = {{1,1,1,1,1},{1,0,0,0,1},{1,0,0,0,1},{1,1,1,1,1},{1,0,1,0,1},{1,0,0,1,0},{1,0,0,0,1}};

void drawPixelLetter(const unsigned char letter[7][5], float startX, float startY, float pixelSize) {
    glColor3f(1.0f, 0.0f, 0.0f);
    for (int row = 0; row < 7; row++) {
        for (int col = 0; col < 5; col++) {
            if (letter[row][col]) {
                float x = startX + col * pixelSize + 0.001f;
                float y = startY - row * pixelSize - 0.001f;
                glBegin(GL_QUADS);
                    glVertex2f(x, y);
                    glVertex2f(x + pixelSize, y);
                    glVertex2f(x + pixelSize, y - pixelSize);
                    glVertex2f(x, y - pixelSize);
                glEnd();
            }
        }
    }
    glColor3f(1.0f, 1.0f, 0.0f);
    for (int row = 0; row < 7; row++) {
        for (int col = 0; col < 5; col++) {
            if (letter[row][col]) {
                float x = startX + col * pixelSize;
                float y = startY - row * pixelSize;
                glBegin(GL_QUADS);
                    glVertex2f(x, y);
                    glVertex2f(x + pixelSize, y);
                    glVertex2f(x + pixelSize, y - pixelSize);
                    glVertex2f(x, y - pixelSize);
                glEnd();
            }
        }
    }
}

void drawGameOverPixelFont() {
    float startX = -0.3f;
    float startY = 0.1f;
    float pixelSize = 0.025f;
    drawPixelLetter(font_G, startX, startY, pixelSize);
    drawPixelLetter(font_A, startX + 6 * pixelSize, startY, pixelSize);
    drawPixelLetter(font_M, startX + 12 * pixelSize, startY, pixelSize);
    drawPixelLetter(font_E, startX + 18 * pixelSize, startY, pixelSize);
    float startY2 = startY - 8 * pixelSize;
    drawPixelLetter(font_O, startX, startY2, pixelSize);
    drawPixelLetter(font_V, startX + 6 * pixelSize, startY2, pixelSize);
    drawPixelLetter(font_E, startX + 12 * pixelSize, startY2, pixelSize);
    drawPixelLetter(font_R, startX + 18 * pixelSize, startY2, pixelSize);
}

//--------------------------------------------
// DISPLAY FUNCTION
//--------------------------------------------
void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

    // Background
    glColor3f(0.45f, 0.45f, 0.45f);
    glBegin(GL_QUADS);
        glVertex2f(-1, -1); glVertex2f(1, -1);
        glVertex2f(1, 1); glVertex2f(-1, 1);
    glEnd();

    // Road
    glColor3f(0.7f, 0.7f, 0.75f);
    glBegin(GL_QUADS);
        glVertex2f(-0.5f, -1); glVertex2f(0.5f, -1);
        glVertex2f(0.5f, 1); glVertex2f(-0.5f, 1);
    glEnd();

    // Obstacles: Trees, Rocks, Snowmen
   // Draw rocks first
for (int i = 0; i < MAX_OBS; i++)
    if (!obs[i].isTree && !obs[i].isSnowman)
        drawRock(obs[i].x, obs[i].y, getScale(obs[i].y));

// Draw snowmen next
for (int i = 0; i < MAX_OBS; i++)
    if (obs[i].isSnowman)
        drawSnowman(obs[i].x, obs[i].y, getScale(obs[i].y));

// Draw trees last (on top)
for (int i = 0; i < MAX_OBS; i++)
    if (obs[i].isTree)
        drawTree(obs[i].x, obs[i].y, getScale(obs[i].y));


    drawPlayer();
    drawSnow();
    drawLifePickup();

    // Score & High Score
    char buf[64];
    snprintf(buf, sizeof(buf), "Score: %d  High Score: %d", score, highScore);
    glColor3f(0, 0, 0);
    glRasterPos2f(-0.95f, 0.9f);
    for (char* c = buf; *c; c++)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);

    // Lives
    glColor3f(0, 0, 0);
    glRasterPos2f(0.65f, 0.9f);
    char liveBuf[32];
    snprintf(liveBuf, sizeof(liveBuf), "Lives: %d", lives);
    for (char* c = liveBuf; *c; c++)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
    drawLifeIcon(0.92f, 0.905f, 1.2f);

    if (gameOver) {
        drawGameOverPixelFont();

        glColor3f(1.0f, 1.0f, 1.0f);
        glRasterPos2f(-0.15f, -0.40f);
        char subMsg[128];
        snprintf(subMsg, sizeof(subMsg), "Score: %d   High Score: %d", score, highScore);
        for (const char* c = subMsg; *c != '\0'; c++) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *c);
        }

        glRasterPos2f(-0.12f, -0.48f);
        const char* restart = "Press R to Restart";
        for (const char* c = restart; *c != '\0'; c++) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *c);
        }
    }

    glutSwapBuffers();
}

//--------------------------------------------
// UPDATE LOOP
//--------------------------------------------
void update(int) {
    if (!gameOver) {
        for (int i = 0; i < MAX_OBS; i++) {
            obs[i].y -= 0.020f;
            float scale = getScale(obs[i].y);

            bool collided = (fabs(playerX - obs[i].x) < 0.07f * scale) &&
                            (fabs(playerY - obs[i].y) < 0.08f * scale);

            if (collided) {
                if (lives > 0) {
                    lives--;
                    obs[i].y = randRange(1.2f, 2.5f);
                    int r = rand() % 10;
                    if (r < 5) {
                        obs[i].isTree = true;
                        obs[i].isSnowman = false;
                        obs[i].x = randRange(-0.4f, 0.4f);
                    } else if (r < 8) {
                        obs[i].isTree = false;
                        obs[i].isSnowman = false;
                        obs[i].x = randRange(-0.4f, 0.4f);
                    } else {
                        obs[i].isTree = false;
                        obs[i].isSnowman = true;
                        if (rand() % 2 == 0)
                            obs[i].x = randRange(-0.4f, -0.3f);
                        else
                            obs[i].x = randRange(0.3f, 0.4f);
                    }
                } else {
                    gameOver = true;
                    if (score > highScore) highScore = score;
                }
            }

            if (obs[i].y < -1) {
                obs[i].y = randRange(1.2f, 2.0f);
                int r = rand() % 10;
                if (r < 5) {
                    obs[i].isTree = true;
                    obs[i].isSnowman = false;
                    obs[i].x = randRange(-0.4f, 0.4f);
                } else if (r < 8) {
                    obs[i].isTree = false;
                    obs[i].isSnowman = false;
                    obs[i].x = randRange(-0.4f, 0.4f);
                } else {
                    obs[i].isTree = false;
                    obs[i].isSnowman = true;
                    if (rand() % 2 == 0)
                        obs[i].x = randRange(-0.4f, -0.3f);
                    else
                        obs[i].x = randRange(0.3f, 0.4f);
                }
                score++;
            }
        }

        if (bullet.active) {
            bullet.y -= 0.02f;
            if (fabs(bullet.x - playerX) < 0.05f && fabs(bullet.y - playerY) < 0.08f) {
                lives++;
                bullet.active = false;
            }
            if (bullet.y < -1) {
                bullet.active = false;
            }
        }
    }

    for (int i = 0; i < MAX_SNOW; i++) {
        snow[i].y -= 0.01f;
        if (snow[i].y < -1) {
            snow[i].y = 1.0f;
            snow[i].x = randRange(-1.0f, 1.0f);
        }
    }

    glutPostRedisplay();
    glutTimerFunc(16, update, 0);
}

//--------------------------------------------
// INITIALIZATION
//--------------------------------------------
void init() {
    srand((unsigned int)time(0));
    glClearColor(0.45f, 0.45f, 0.45f, 1.0f);

    resetObstacles();

    for (int i = 0; i < MAX_SNOW; i++) {
        snow[i].x = randRange(-1, 1);
        snow[i].y = randRange(-1, 1);
    }

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-1, 1, -1, 1);
    glMatrixMode(GL_MODELVIEW);
}

//--------------------------------------------
// MAIN
//--------------------------------------------
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(900, 600);
    glutCreateWindow("Blizzard Dash - Improved Trees");

    init();
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutTimerFunc(0, update, 0);
    glutTimerFunc(10000, spawnLifePickup, 0);

    glutMainLoop();
    return 0;
}
