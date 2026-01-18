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
struct Obstacle { float x, y; bool isTree; };
struct Bullet { float x, y; bool active; };
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
// PART 1: CONTROL - Player input and movement
//--------------------------------------------
void resetObstacles() {
    for(int i=0;i<MAX_OBS;i++){
        obs[i].y=randRange(1.2f,2.5f);
        obs[i].x=randRange(-0.4f,0.4f);
        obs[i].isTree=rand()%2;
    }
}

void keyboard(unsigned char key, int, int) {
    if(key=='a'||key=='A') playerX -= 0.08f;
    if(key=='d'||key=='D') playerX += 0.08f;
    if(key=='w'||key=='W') {
        if(lives > 0 && !gameOver) {
            lives--;
            // Clear all obstacles on screen (y < 1)
            for(int i=0;i<MAX_OBS;i++){
                if(obs[i].y < 1.0f) {
                    obs[i].y=randRange(1.2f,2.5f);
                    obs[i].x=randRange(-0.4f,0.4f);
                    obs[i].isTree=rand()%2;
                }
            }
        }
    }
    if(key=='r'||key=='R') {
        gameOver = false;
        playerX = 0.0f;
        playerY = -0.75f;
        score = 0;
        lives = 0;
        bullet.active = false;
        resetObstacles();
    }
    if(key==27) exit(0); // ESC to quit

    // keep player within bounds
    if(playerX<-0.45f) playerX=-0.45f;
    if(playerX>0.45f) playerX=0.45f;
}

//--------------------------------------------
// PART 2: LIFE PICKUP - Heart mechanics
//--------------------------------------------
void drawLifePickup(){
    if(!bullet.active) return;
    glPushMatrix();
    glTranslatef(bullet.x, bullet.y, 0);

    // Draw heart shape using two circles and a triangle
    glColor3f(1, 0, 0);

    // Left circle
    glBegin(GL_POLYGON);
    for(int i=0;i<20;i++){
        float a = i*2*3.1416f/20;
        glVertex2f(cos(a)*0.015f - 0.01f, sin(a)*0.015f + 0.01f);
    }
    glEnd();

    // Right circle
    glBegin(GL_POLYGON);
    for(int i=0;i<20;i++){
        float a = i*2*3.1416f/20;
        glVertex2f(cos(a)*0.015f + 0.01f, sin(a)*0.015f + 0.01f);
    }
    glEnd();

    // Bottom triangle
    glBegin(GL_TRIANGLES);
        glVertex2f(-0.025f, 0.01f);
        glVertex2f(0.025f, 0.01f);
        glVertex2f(0.0f, -0.025f);
    glEnd();

    glPopMatrix();
}

void spawnBullet(int){
    if(!gameOver && !bullet.active){
        bullet.active = true;
        bullet.x = randRange(-0.4f, 0.4f);
        bullet.y = 1.2f;
    }
    glutTimerFunc(10000, spawnBullet, 0); // every 10 sec
}

//--------------------------------------------
// PART 3: SCENE DESIGN - Static environment & obstacles
//--------------------------------------------
float getScale(float y) { return 0.6f + (1.0f - y) * 0.2f; }

void drawPlayer() {
    glPushMatrix();
    glTranslatef(playerX, playerY, 0);
    // Body
    glColor3f(0.2f, 0.6f, 0.9f);
    glBegin(GL_QUADS);
        glVertex2f(-0.03f, 0.06f); glVertex2f(0.03f, 0.06f);
        glVertex2f(0.03f, -0.06f); glVertex2f(-0.03f, -0.06f);
    glEnd();
    // Head
    glColor3f(1.0f, 0.8f, 0.6f);
    glBegin(GL_POLYGON);
    for(int i=0;i<20;i++){
        float a = i*2*3.1416f/20;
        glVertex2f(cos(a)*0.025f, sin(a)*0.025f + 0.085f);
    }
    glEnd();
    glPopMatrix();
}

void drawTree(float x,float y,float scale){
    glPushMatrix();
    glTranslatef(x,y,0);
    glScalef(scale,scale,1);
    glColor3f(0,0.6f,0.2f);
    glBegin(GL_TRIANGLES);
        glVertex2f(0,0.12f); glVertex2f(-0.08f,-0.02f); glVertex2f(0.08f,-0.02f);
    glEnd();
    glColor3f(0.45f,0.25f,0.1f);
    glBegin(GL_QUADS);
        glVertex2f(-0.02f,-0.02f); glVertex2f(0.02f,-0.02f);
        glVertex2f(0.02f,-0.10f); glVertex2f(-0.02f,-0.10f);
    glEnd();
    glPopMatrix();
}

void drawRock(float x,float y,float scale){
    glPushMatrix();
    glTranslatef(x,y,0);
    glScalef(scale,scale,1);
    glColor3f(0.5f,0.5f,0.5f);
    glBegin(GL_POLYGON);
        glVertex2f(-0.05f,-0.02f); glVertex2f(-0.02f,0.04f);
        glVertex2f(0.05f,0.03f); glVertex2f(0.06f,-0.02f); glVertex2f(0.02f,-0.05f);
    glEnd();
    glPopMatrix();
}

void drawSnow(){
    glColor3f(1,1,1);
    glPointSize(2);
    glBegin(GL_POINTS);
    for(int i=0;i<MAX_SNOW;i++) glVertex2f(snow[i].x,snow[i].y);
    glEnd();
}

void drawLifeIcon(float x, float y, float scale) {
    glPushMatrix();
    glTranslatef(x, y, 0);
    glScalef(scale, scale, 1);

    glColor3f(1, 0, 0);

    // Left circle
    glBegin(GL_POLYGON);
    for(int i=0;i<20;i++){
        float a = i*2*3.1416f/20;
        glVertex2f(cos(a)*0.015f - 0.01f, sin(a)*0.015f + 0.01f);
    }
    glEnd();

    // Right circle
    glBegin(GL_POLYGON);
    for(int i=0;i<20;i++){
        float a = i*2*3.1416f/20;
        glVertex2f(cos(a)*0.015f + 0.01f, sin(a)*0.015f + 0.01f);
    }
    glEnd();

    // Bottom triangle
    glBegin(GL_TRIANGLES);
        glVertex2f(-0.025f, 0.01f);
        glVertex2f(0.025f, 0.01f);
        glVertex2f(0.0f, -0.025f);
    glEnd();

    glPopMatrix();
}

//--------------------------------------------
// PART 4: DISPLAY & RENDER LOOP
//--------------------------------------------
void display(){
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

    // Background
    glColor3f(0.45f,0.45f,0.45f);
    glBegin(GL_QUADS);
        glVertex2f(-1,-1); glVertex2f(1,-1);
        glVertex2f(1,1); glVertex2f(-1,1);
    glEnd();

    // Road
    glColor3f(0.7f,0.7f,0.75f);
    glBegin(GL_QUADS);
        glVertex2f(-0.5f,-1); glVertex2f(0.5f,-1);
        glVertex2f(0.25f,1); glVertex2f(-0.25f,1);
    glEnd();

    // Obstacles
    for(int i=0;i<MAX_OBS;i++){
        float scale=getScale(obs[i].y);
        if(obs[i].isTree) drawTree(obs[i].x,obs[i].y,scale);
        else drawRock(obs[i].x,obs[i].y,scale);
    }

    drawPlayer();
    drawSnow();
    drawLifePickup();

    // Scores
    char buf[64];
    snprintf(buf,sizeof(buf),"Score: %d  High Score: %d",score,highScore);
    glColor3f(0,0,0);
    glRasterPos2f(-0.95f,0.9f);
    for(char* c=buf;*c;c++) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,*c);

    // Lives display
    glColor3f(0,0,0);
    glRasterPos2f(0.65f,0.9f);
    char liveBuf[32];
    snprintf(liveBuf,sizeof(liveBuf),"Lives: %d",lives);
    for(char* c=liveBuf;*c;c++) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,*c);

    // Draw life icon next to text
    drawLifeIcon(0.92f, 0.905f, 1.2f);

    // Game Over popup
    if(gameOver){
        glColor3f(1,0,0);
        glRasterPos2f(-0.45f,0);
        char msg[128];
        snprintf(msg,sizeof(msg),"GAME OVER! Score: %d High Score: %d. Press R to Restart",score,highScore);
        for(const char* c=msg;*c;c++) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,*c);
    }

    glutSwapBuffers();
}

void update(int){
    if(!gameOver){
        // Obstacles movement & collision
        for(int i=0;i<MAX_OBS;i++){
            obs[i].y -= 0.018f;
            float scale = getScale(obs[i].y);
            if(fabs(playerX-obs[i].x)<0.07f*scale && fabs(playerY-obs[i].y)<0.08f*scale){
                gameOver = true;
                if(score>highScore) highScore = score;
            }
            if(obs[i].y<-1){
                obs[i].y = randRange(1.2f,2.0f);
                obs[i].x = randRange(-0.4f,0.4f);
                obs[i].isTree = rand()%2;
                score++;
            }
        }

        // Life pickup collision
        if(bullet.active){
            bullet.y -= 0.02f;

            // Check collision with player
            if(fabs(bullet.x - playerX) < 0.05f && fabs(bullet.y - playerY) < 0.08f){
                lives++;
                bullet.active = false;
            }

            if(bullet.y < -1) bullet.active = false;
        }
    }

    // Snow movement
    for(int i=0;i<MAX_SNOW;i++){
        snow[i].y-=0.01f;
        if(snow[i].y<-1){ snow[i].y=1; snow[i].x=randRange(-1,1); }
    }

    glutPostRedisplay();
    glutTimerFunc(16,update,0);
}

// Initialize obstacles & snow
void init(){
    srand(time(0));
    glClearColor(0.45f,0.45f,0.45f,1);

    resetObstacles();

    for(int i=0;i<MAX_SNOW;i++){
        snow[i].x=randRange(-1,1);
        snow[i].y=randRange(-1,1);
    }

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-1,1,-1,1);
    glMatrixMode(GL_MODELVIEW);
}

//--------------------------------------------
// MAIN FUNCTION
//--------------------------------------------
int main(int argc,char** argv){
    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB);
    glutInitWindowSize(900,600);
    glutCreateWindow("Blizzard Dash - With Life System");

    init();
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutTimerFunc(0,update,0);
    glutTimerFunc(10000,spawnBullet,0);
    glutMainLoop();
    return 0;
}
