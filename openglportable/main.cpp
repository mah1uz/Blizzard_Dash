#include <GL/glut.h>

// Player position
float playerX = 0.0f;
float playerY = -0.75f;
bool gameOver = false;

// Draw player (simple rectangle)
void drawPlayer() {
    glPushMatrix();
    glTranslatef(playerX, playerY, 0);
    glColor3f(0.2f, 0.6f, 0.9f);
    glBegin(GL_QUADS);
        glVertex2f(-0.05f, 0.05f);
        glVertex2f( 0.05f, 0.05f);
        glVertex2f( 0.05f,-0.05f);
        glVertex2f(-0.05f,-0.05f);
    glEnd();
    glPopMatrix();
}

// Keyboard controls
void keyboard(unsigned char key, int, int) {
    if(key == 'a' || key == 'A') playerX -= 0.08f;
    if(key == 'd' || key == 'D') playerX += 0.08f;

    // Restart
    if(key == 'r' || key == 'R') {
        playerX = 0.0f;
        playerY = -0.75f;
        gameOver = false;
    }

    // Quit
    if(key == 27) exit(0);

    // Keep inside road
    if(playerX < -0.45f) playerX = -0.45f;
    if(playerX >  0.45f) playerX =  0.45f;

    glutPostRedisplay();
}

// Display
void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

    // Road
    glColor3f(0.7f,0.7f,0.75f);
    glBegin(GL_QUADS);
        glVertex2f(-0.5f,-1);
        glVertex2f( 0.5f,-1);
        glVertex2f( 0.25f,1);
        glVertex2f(-0.25f,1);
    glEnd();

    drawPlayer();

    glutSwapBuffers();
}

// Init
void init() {
    glClearColor(0.8f,0.8f,0.9f,1);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-1,1,-1,1);
    glMatrixMode(GL_MODELVIEW);
}

// Main
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(800,600);
    glutCreateWindow("Player Movement Only");

    init();
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);

    glutMainLoop();
    return 0;
}
