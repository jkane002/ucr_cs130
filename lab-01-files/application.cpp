#include "application.h"

#include <iostream>
#include <vector>
#include <cassert>
#include <string>
#include <cstring>
#include <fstream>
#include <cstdio>

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

application::application()
{
}

application::~application()
{
}

void application::init_triangle() {
	ifstream file("monkey.raw");
	
	
	while (!file.eof()) {
		int i = 0;
		vector<float> v;
		float a;
	
		while(i < 3){
			file >> a;
			v.push_back(a);
			i++;
		}
		
		this->triangles.push_back(v);		// 1 vertex
	/*
	file >> a;
	v.push_back(a);

	file >> a;
	v.push_back(a);

	file >> a;
	v.push_back(a);

	triangles.push_back(v);		// 2 vertices
	
	file >> ;
	v.push_back(c);

	file >> c;
	v.push_back(c);

	file >> c;
	v.push_back(c);

	triangles.push_back(v);		// 3 vertices*/
}

	/*
 for(unsigned i = 0; i < triangles.size(); ++i) {
		std::cout << triangles.at(i).at(0);		std::cout << triangles.at(i).at(1);		std::cout << triangles.at(i).at(2);
	} */

	//file.eof();	
	/*
	vector<double> coordinates; //xyz for 1 point
    vector<vector <float> > triangles;
	string file = "monkey.raw";
	ifstream in;
	double num;
	// outstream out;
	
	in.open(file.c_str());
	if(file.is_open()) {
		for(unsigned i = 9; i < 0; --i) {
			for(unsigned j = 9; j % 3 == 0; --j) {
				//3 points into a vector
				// then all 3 coordinates into another vector
				
				
				}
			}
		//in.get(num);
	
		// return vector?
	} else {
		exit(1);
		}
		in.close();
	}	
	* */
}
// triggered once after the OpenGL context is initialized
void application::init_event()
{

    cout << "CAMERA CONTROLS: \n  LMB: Rotate \n  MMB: Move \n  RMB: Zoom" << endl;
    cout << "KEYBOARD CONTROLS: \n  '=': Toggle wireframe mode" << endl;

    const GLfloat ambient[] = { 0.15, 0.15, 0.15, 1.0 };
    const GLfloat diffuse[] = { 0.6, 0.6, 0.6, 1.0 };
    const GLfloat specular[] = { 1.0, 1.0, 1.0, 1.0 };



	init_triangle();

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

    set_camera_for_box(vec3(-1,-1,-1),vec3(1,1,1));
}

// triggered each time the application needs to redraw
void application::draw_event()
{
	//vector<vector <float> > a = triangles();
	
    // apply our camera transformation
    apply_gl_transform();

    glPolygonMode(GL_FRONT_AND_BACK,GL_LINE); //  this will enable wire-frame mode
    // draw first triangle
    glPushMatrix();
    glTranslatef(0,-0.5,0);
    glBegin(GL_TRIANGLES);
    
    
	// 1 triangle
    //glColor3f(1,0,0); glVertex3f(1,0,0); //point
    //glColor3f(1,0,0); glVertex3f(-1,0,0); // point 
    //glColor3f(1,0,0); glVertex3f(0,1,0); //point
   /*
    std::cout << triangles.size() << std::endl;
	for(unsigned i = 0; i < triangles.size(); ++i){
		std::cout << triangles.at(i).size() << std::endl;
	}
	*/
    for(unsigned i = 0; i < triangles.size(); ++i) {
		glColor3f(1,0,0);
		glVertex3f(triangles.at(i).at(0), triangles.at(i).at(1), triangles.at(i).at(2)); //point
	}
	
    
    glEnd();
    glPopMatrix();
    
    apply_gl_transform();

    glPolygonMode(GL_FRONT_AND_BACK,GL_LINE); //  this will enable wire-frame mode
    // draw first triangle
    glPushMatrix();
    glTranslatef(5,-1,0);
    glBegin(GL_TRIANGLES);
    
    
	// 1 triangle
    //glColor3f(1,0,0); glVertex3f(1,0,0); //point
    //glColor3f(1,0,0); glVertex3f(-1,0,0); // point 
    //glColor3f(1,0,0); glVertex3f(0,1,0); //point
   /*
    std::cout << triangles.size() << std::endl;
	for(unsigned i = 0; i < triangles.size(); ++i){
		std::cout << triangles.at(i).size() << std::endl;
	}
	*/
    for(unsigned i = 0; i < triangles.size(); ++i) {
		glColor3f(1,20,0);
		glVertex3f(triangles.at(i).at(0), triangles.at(i).at(1), triangles.at(i).at(2)); //point
//		glVertex3f(1,0,0);
	}
	
    
    glEnd();
    glPopMatrix();
    /*
    // draw second triangle
    glPushMatrix();
    glTranslatef(0,-0.5,0);
    glBegin(GL_TRIANGLES);
    glColor3f(1,1,1); glVertex3f(1,0,0);
    glColor3f(0,0,1); glVertex3f(-1,0,0);
    glColor3f(1,1,1); glVertex3f(0,1,0);
    glEnd();
    glPopMatrix();
    */
}

// triggered when mouse is clicked
void application::mouse_click_event(int button, int button_state, int x, int y)
{
}

// triggered when mouse button is held down and the mouse is
// moved
void application::mouse_move_event(int x, int y)
{
} 	 

// triggered when a key is pressed on the keyboard
void application::keyboard_event(unsigned char key, int x, int y)
{
}
