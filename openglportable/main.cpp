#include <GL/glut.h>

void display()  //this functions name can be anything
{
   glClear(GL_COLOR_BUFFER_BIT);



   glFlush(); // flush whatever has been done
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
 