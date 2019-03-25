#include "application.h"

#include <iostream>
#include <cassert>

#ifndef __APPLE__
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#else
#include <GLUT/glut.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#endif

using namespace std;
void draw_grid();

application::application()
    : solid(true)
{
}

application::~application()
{
}

// triggered once after the OpenGL context is initialized
void application::init_event()
{

    cout << "CAMERA CONTROLS: \n  LMB: Rotate \n  MMB: Move \n  RMB: Zoom" << endl;
    cout << "KEYBOARD CONTROLS: \n  '=': Toggle wireframe mode" << endl;

    const GLfloat ambient[] = { 0.15, 0.15, 0.15, 1.0 };
    const GLfloat diffuse[] = { 0.6, 0.6, 0.6, 1.0 };
    const GLfloat specular[] = { 1.0, 1.0, 1.0, 1.0 };

    // enable a light
    glLightfv(GL_LIGHT1, GL_AMBIENT, ambient);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuse);
    glLightfv(GL_LIGHT1, GL_SPECULAR, specular);
    glEnable(GL_LIGHT1);

    // enable depth-testing, colored materials, and lighting
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHTING);

    // normalize normals so lighting calculations are correct
    // when using GLUT primitives
    glEnable(GL_NORMALIZE);

    // enable smooth shading
    glShadeModel(GL_SMOOTH);

    set_camera_for_box(vec3(-10,-10,-10),vec3(10,10,10));

    t.reset();
}

// triggered each time the application needs to redraw
void application::draw_event()
{
    // apply our camera transformation
    apply_gl_transform();

    // set the position of the light
    const GLfloat light_pos1[] = { 0.0, 10.0, 0.0, 1 };
    glLightfv(GL_LIGHT1, GL_POSITION, light_pos1);

    // draws the grid and frame at the origin
    draw_grid();

    //
    // create some various objects in the world
    //

    //red ball
    // glPushMatrix();
    // glColor3f(1, 0, 0);
    // glTranslatef(5, 1, -5);
    // solid ? glutSolidSphere(1, 20, 20) : glutWireSphere(1, 20, 20);
    // glPopMatrix();


    //blue cone
    // glPushMatrix();
    // glColor3f(0, 0, 1);
    // glTranslatef(-5, 1, -5);
    // solid ? glutSolidCone(1, 2, 10, 10) : glutWireCone(1, 2, 10, 10);
    // glPopMatrix();

    //sun
    glPushMatrix();
    glColor3f(1, 1, 0);
    glTranslatef(t.elapsed()*2, 0, t.elapsed()*-0.5);
    glRotatef(t.elapsed()*45, 0, 1, 0);
    solid ? glutSolidTorus(0.5, 1, 20, 20) : glutWireTorus(0.5, 1, 20, 20);
    //glPopMatrix();

    //earth
    //glPushMatrix();
    glColor3f(0, 0, 1);
    glTranslatef(10, 3, 0);
    glRotatef(t.elapsed()*180, 0, 1, 0);
    glScalef(0.75, 0.75, 0.75);
    solid ? glutSolidDodecahedron() : glutWireDodecahedron();
    //glPopMatrix();

    //moon
    //glPushMatrix();
    glColor3f(1, 0.5, 0.5);
    glTranslatef(5, 0, 0);
    // rotate 180 degrees/second about the y-axis
    glRotatef(t.elapsed()*360, 1, 0, 0);
    glScalef(0.5, 0.5, 0.5);
    solid ? glutSolidCube(1) : glutWireCube(1);
    glPopMatrix();
}

// triggered when mouse is clicked
void application::mouse_click_event(int button, int button_state, int x, int y)
{
}

// triggered when mouse button is held down and the mouse is
// moved
void application::mouse_move_event(
    int x, int y
    )
{
}

// triggered when a key is pressed on the keyboard
void application::keyboard_event(unsigned char key, int x, int y)
{
}

void draw_grid()
{
    glDisable(GL_LIGHTING);
    glLineWidth(2.0);

    //
    // Draws the coordinate frame at origin
    //
    glPushMatrix();
    glScalef(0.5, 0.5, 0.5);
    glBegin(GL_LINES);

    // x-axis
    glColor3f(1.0, 0.0, 0.0);
    glVertex3f(0.0, 0.0, 0.0);
    glVertex3f(1.0, 0.0, 0.0);

    // y-axis
    glColor3f(0.0, 1.0, 0.0);
    glVertex3f(0.0, 0.0, 0.0);
    glVertex3f(0.0, 1.0, 0.0);

    // z-axis
    glColor3f(0.0, 0.0, 1.0);
    glVertex3f(0.0, 0.0, 0.0);
    glVertex3f(0.0, 0.0, 1.0);
    glEnd();
    glPopMatrix();

    //
    // Draws a grid along the x-z plane
    //
    glLineWidth(1.0);
    glColor3f(.20, .20, .20);
    glBegin(GL_LINES);

    int ncells = 20;
    int ncells2 = ncells/2;

    for (int i= 0; i <= ncells; i++)
    {
        int k = -ncells2;
        k +=i;
        glVertex3f(ncells2,0,k);
        glVertex3f(-ncells2,0,k);
        glVertex3f(k,0,ncells2);
        glVertex3f(k,0,-ncells2);
    }
    glEnd();
    glEnable(GL_LIGHTING);
}
