#include <GL/glut.h>

void display()  //this functions name can be anything
{
   glClear(GL_COLOR_BUFFER_BIT);



   glFlush(); // flush whatever has been done
}

//--------------------------------------------
// PIXEL FONT DEFINITIONS (5x7 grid)
//--------------------------------------------
// Each letter is a 5x7 grid (width x height). 1 = pixel on, 0 = off.
const unsigned char font_G[7][5] = {
    {1,1,1,1,1},
    {1,0,0,0,0},
    {1,0,0,0,0},
    {1,0,1,1,1},
    {1,0,0,0,1},
    {1,0,0,0,1},
    {1,1,1,1,1}
};

const unsigned char font_A[7][5] = {
    {0,1,1,1,0},
    {1,0,0,0,1},
    {1,0,0,0,1},
    {1,1,1,1,1},
    {1,0,0,0,1},
    {1,0,0,0,1},
    {1,0,0,0,1}
};

const unsigned char font_M[7][5] = {
    {1,0,0,0,1},
    {1,1,0,1,1},
    {1,0,1,0,1},
    {1,0,1,0,1},
    {1,0,0,0,1},
    {1,0,0,0,1},
    {1,0,0,0,1}
};

const unsigned char font_E[7][5] = {
    {1,1,1,1,1},
    {1,0,0,0,0},
    {1,0,0,0,0},
    {1,1,1,1,1},
    {1,0,0,0,0},
    {1,0,0,0,0},
    {1,1,1,1,1}
};

const unsigned char font_O[7][5] = {
    {0,1,1,1,0},
    {1,0,0,0,1},
    {1,0,0,0,1},
    {1,0,0,0,1},
    {1,0,0,0,1},
    {1,0,0,0,1},
    {0,1,1,1,0}
};

const unsigned char font_V[7][5] = {
    {1,0,0,0,1},
    {1,0,0,0,1},
    {1,0,0,0,1},
    {1,0,0,0,1},
    {1,0,0,0,1},
    {0,1,0,1,0},
    {0,0,1,0,0}
};

const unsigned char font_R[7][5] = {
    {1,1,1,1,1},
    {1,0,0,0,1},
    {1,0,0,0,1},
    {1,1,1,1,1},
    {1,0,1,0,1},
    {1,0,0,1,0},
    {1,0,0,0,1}
};

//--------------------------------------------
// DRAW PIXEL LETTER (with shadow effect)
//--------------------------------------------
void drawPixelLetter(const unsigned char letter[7][5], float startX, float startY, float pixelSize) {
    // Draw red shadow first (offset by 0.1 pixel size)
    glColor3f(1.0f, 0.0f, 0.0f); // Red
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

    // Draw yellow foreground
    glColor3f(1.0f, 1.0f, 0.0f); // Yellow
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

//--------------------------------------------
// DRAW "GAME OVER" IN PIXEL FONT
//--------------------------------------------
void drawGameOverPixelFont() {
    float startX = -0.3f; // Horizontal start position
    float startY = 0.1f;  // Vertical start position
    float pixelSize = 0.025f; // Size of each pixel

    // Draw "GAME"
    drawPixelLetter(font_G, startX, startY, pixelSize);
    drawPixelLetter(font_A, startX + 6 * pixelSize, startY, pixelSize);
    drawPixelLetter(font_M, startX + 12 * pixelSize, startY, pixelSize);
    drawPixelLetter(font_E, startX + 18 * pixelSize, startY, pixelSize);

    // Draw "OVER" below
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
        glVertex2f(0.25f, 1); glVertex2f(-0.25f, 1);
    glEnd();

    // Obstacles
    for (int i = 0; i < MAX_OBS; i++) {
        float scale = getScale(obs[i].y);
        if (obs[i].isTree)
            drawTree(obs[i].x, obs[i].y, scale);
        else
            drawRock(obs[i].x, obs[i].y, scale);
    }

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

// >>> PIXELATED GAME OVER MESSAGE <<<
if (gameOver) {
    drawGameOverPixelFont();

    // Subtitle: score info — centered below "GAME OVER"
    glColor3f(1.0f, 1.0f, 1.0f);
    glRasterPos2f(-0.15f, -0.40f); // Centered x, below pixel text
    char subMsg[128];
    snprintf(subMsg, sizeof(subMsg), "Score: %d   High Score: %d", score, highScore);
    for (const char* c = subMsg; *c != '\0'; c++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *c);
    }

    // Restart prompt — centered below score
    glRasterPos2f(-0.12f, -0.48f);
    const char* restart = "Press R to Restart";
    for (const char* c = restart; *c != '\0'; c++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *c);
    }
}

    glutSwapBuffers();
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitWindowSize(640, 500);
    glutCreateWindow("OpenGL 64bit");
    glutDisplayFunc(display);  //call the display function
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glutMainLoop();
    return 0;
} 
 