#include <GL/glut.h>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <cstdio>

#define MAX_OBS 8
#define MAX_SNOW 150
void drawPlayer() {
    glPushMatrix();
    glTranslatef(playerX, playerY, 0);

    // Slim Sweater Body
    glColor3f(0.2f, 0.6f, 0.9f);
    glBegin(GL_QUADS);
        glVertex2f(-0.03f,  0.06f); glVertex2f( 0.03f,  0.06f);
        glVertex2f( 0.03f, -0.06f); glVertex2f(-0.03f, -0.06f);
    glEnd();

    // Slim Head
    glColor3f(1.0f, 0.8f, 0.6f);
    glBegin(GL_POLYGON);
    for(int i=0;i<20;i++){
        float a = i*2*3.1416f/20;
        glVertex2f(cos(a)*0.025f, sin(a)*0.025f + 0.085f);
    }
    glEnd();
    
    glPopMatrix();
}

// ---------------- DRAW OBSTACLES ----------------
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