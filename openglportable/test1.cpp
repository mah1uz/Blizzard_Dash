
#include <GL/glut.h>

void display()  //this functions name can be anything
{
   glClear(GL_COLOR_BUFFER_BIT);
   glLineWidth(5);



              //HOUSE
    //glBegin(GL_LINES);
//       glColor3f(0.9f,0.9f,0.9f);
//       glVertex2f(-.75,0);  //x axis line
//       glVertex2f(.75,0);
//
//       glVertex2f(0,0.65);  //right line
//       glVertex2f(.75,0);
//
//
//       glVertex2f(0,0.65);  //left line
//       glVertex2f(-.75,0);
//
//       glVertex2f(-.75,0);     //left column
//       glVertex2f(-.75,-.85);
//
//       glVertex2f(.75,0);     //right column
//       glVertex2f(.75,-.85);
//
//       glVertex2f(-.75,-.85);  //left bottom to r bottom
//       glVertex2f(.75,-.85);
//
//       glVertex2f(-.1,-.85);
//       glVertex2f(-.1,-.65);
//
//       glVertex2f(-.1,-.65);
//       glVertex2f(.1,-.65);
//
//       glVertex2f(.1,-.65);
//       glVertex2f(.1,-.85);
//
//       glVertex2f(.75,0);
//       glVertex2f(.75,-.85);
//
//       glVertex2f(-.45,-.75);
//       glVertex2f(-.55,-.75);
//
//       glVertex2f(-.55,-.60);
//       glVertex2f(-.55,-.75);






      //Solid Triangle using triangle function

  glBegin(GL_TRIANGLES);
       glColor3f(0.0f,0.0f,0.9f);
       glVertex2f(0,0.8);

       glColor3f(0.0f,0.9f,0.5f);
       glVertex2f(0.8,0);

       glColor3f(0.9f,0.0f,0.5f);
       glVertex2f(-0.8,0);


   glEnd();
   glFlush();
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
