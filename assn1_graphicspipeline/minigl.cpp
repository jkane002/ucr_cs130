/**
 * minigl.cpp
 * -------------------------------
 * Implement miniGL here.
 *
 * You may include minigl.h and any of the standard C++ libraries.
 * No other includes are permitted.  Other preprocessing directives
 * are also not permitted.  These requirements are strictly
 * enforced.  Be sure to run a test grading to make sure your file
 * passes the sanity tests.
 *
 * The behavior of the routines your are implenting is documented here:
 * https://www.opengl.org/sdk/docs/man2/
 * Note that you will only be implementing a subset of this.  In particular,
 * you only need to implement enough to pass the tests in the suite.
 */

#include "minigl.h"
#include "vec.h"
#include "mat.h"
#include <algorithm>
#include <cassert>
#include <cmath>
#include <vector>
#include <cstdio>
#include <stack>

using namespace std;

/**
 * Useful data types
 */
typedef mat<MGLfloat,4,4> mat4; //data structure storing a 4x4 matrix, see mat.h
typedef mat<MGLfloat,3,3> mat3; //data structure storing a 3x3 matrix, see mat.h
typedef vec<MGLfloat,4> vec4;   //data structure storing a 4 dimensional vector, see vec.h
typedef vec<MGLfloat,3> vec3;   //data structure storing a 3 dimensional vector, see vec.h
typedef vec<MGLfloat,2> vec2;   //data structure storing a 2 dimensional vector, see vec.h


struct vertex{
	vec4 pos; //x y z w
	vec3 color; //r g b
	};

struct triangle {
	vertex a;
	vertex b;
	vertex c;
	};


vector<triangle> listOfTriangles;
vector<vertex> 	listOfVertices;
vector<stack<mat4> >  vecStack;
stack<mat4> stackOfMod;
stack<mat4> stackOfProj;
stack<mat4> *curr_stack;

//global
vec3 curr_color;
mat4 curr_geometry;
MGLpoly_mode drawMode;
MGLmatrix_mode matmode;
vector<vector<MGLfloat> > minz;



/**
 * Standard macro to report errors
 */
inline void MGL_ERROR(const char* description) {
    printf("%s\n", description);
    exit(1);
}

/**
 * Read pixel data starting with the pixel at coordinates
 * (0, 0), up to (width,  height), into the array
 * pointed to by data.  The boundaries are lower-inclusive,
 * that is, a call with width = height = 1 would just read
 * the pixel at (0, 0).
 *
 * Rasterization and z-buffering should be performed when
 * this function is called, so that the data array is filled
 * with the actual pixel values that should be displayed on
 * the two-dimensional screen.
 */
MGLfloat area(vertex a, vertex b, vertex c) {
	return (a.pos[0]*(b.pos[1] - c.pos[1])) +
			(a.pos[1]*(c.pos[0] - b.pos[0])) +
			((b.pos[0] * c.pos[1]) - (b.pos[1] * c.pos[0]));
}

vertex p2c(vertex a, MGLsize width, MGLsize height) {
	a.pos[0] = (a.pos[0] + 1) * width / 2;
	a.pos[1] = (a.pos[1] + 1) * height / 2;
	return a;
}

void mglReadPixels(MGLsize width,
                   MGLsize height,
                   MGLpixel *data)
{
	minz.resize(width);
	for(unsigned i = 0; i < width; i++) {
		minz.at(i).resize(height);
	}

	for(unsigned i = 0; i < minz.size(); i++) {
		for(unsigned j = 0; j < minz.at(i).size(); j++) {
			minz.at(i).at(j) = 2;
		}
	}

	for(unsigned int i = 0; i < listOfTriangles.size(); i++) {
	for(unsigned int j = 0; j < width; j++) {
	for(unsigned int k = 0; k < height; k++) {
		triangle tri = listOfTriangles.at(i);
		MGLsize w = width;
		MGLsize h = height;

		tri.a.pos /= tri.a.pos[3];
		tri.b.pos /= tri.b.pos[3];
		tri.c.pos /= tri.c.pos[3];

		vertex p;
		p.pos = {j,k,0,0};

		MGLfloat full_area = area(p2c(tri.a, w, h), p2c(tri.b, w, h), p2c(tri.c, w, h));
	 	MGLfloat alpha = area(p, p2c(tri.b, w, h), p2c(tri.c, w, h)) / full_area;
		MGLfloat beta  = area(p2c(tri.a, w, h), p, p2c(tri.c, w, h)) / full_area;
		MGLfloat gamma = area(p2c(tri.a, w, h), p2c(tri.b, w, h), p) / full_area;

		vec3 vecColor = tri.a.color * alpha + tri.b.color * beta + tri.c.color * gamma;

		if(!(alpha < 0.0 || beta < 0.0 || gamma < 0.0)) {
			MGLfloat zdepth = tri.a.pos[2] * alpha + tri.b.pos[2] * beta + tri.c.pos[2] * gamma;
			if( (zdepth < minz.at(j).at(k) ) && (zdepth <= 1 && zdepth >= -1) ) {
				minz.at(j).at(k) = zdepth;
				data[j + k * width] = Make_Pixel(vecColor[0]*255.0, vecColor[1]*255.0, vecColor[2]*255.0);
			}
		}
	}
	}
	}
}



/**
 * Start specifying the vertices for a group of primitives,
 * whose type is specified by the given mode.
 */
void mglBegin(MGLpoly_mode mode)
{
	drawMode = mode;
}

/**
 * Stop specifying the vertices for a group of primitives.
 */

void mglEnd()
{
	//store the tri info, reset vertex info
	//if TRIANGLES: store each three vertexes as a triangle in your global triangles variable
	if(drawMode == MGL_TRIANGLES) {
		//WHAT IF NUM OF VERTICES NOT DIVISIBLE BY 3
		for(unsigned int i = 0; i < listOfVertices.size(); i+=3){
			triangle temp;
			temp.a = listOfVertices.at(i);
			temp.b = listOfVertices.at(i + 1);
			temp.c = listOfVertices.at(i + 2);
			listOfTriangles.push_back(temp);
		}
	} else if(drawMode ==  MGL_QUADS) {
		//	if QUAD: split the four vertexes into two triangles (A,B,C,D) -> (A,B,C) + (A, C, D)
			// and store the triangles in your global triangles
		for(unsigned int i = 0; i < listOfVertices.size(); i+=4) {
			triangle tempOne;
			triangle tempTwo;
			tempOne.a = listOfVertices.at(i);
			tempOne.b = listOfVertices.at(i + 1);
			tempOne.c = listOfVertices.at(i + 2);
			listOfTriangles.push_back(tempOne);
			tempTwo.a = listOfVertices.at(i);
			tempTwo.b = listOfVertices.at(i + 2);
			tempTwo.c = listOfVertices.at(i + 3);
			listOfTriangles.push_back(tempTwo);
		}
	} else {
		cout << "Error in mglEnd()!!\nNeither a tri nor a quad\n";
	}
	listOfVertices.clear();

}

/**
 * Specify a two-dimensional vertex; the x- and y-coordinates
 * are explicitly specified, while the z-coordinate is assumed
 * to be zero.  Must appear between calls to mglBegin() and
 * mglEnd().
 */
void mglVertex2(MGLfloat x,
                MGLfloat y)
{
	vertex v;
	v.pos = {x, y, 0, 1};
	v.color = curr_color;
	//stackOfProj

	v.pos = stackOfMod.top() * v.pos;
	v.pos = stackOfProj.top() * v.pos;

	//stackOfMod
	#if 0
	v.pos[0] = v.pos[0]*stackOfMod.top()(0,0) + v.pos[1]*stackOfMod.top()(0,1)
	+ v.pos[2]*stackOfMod.top()(0,2) + v.pos[3]*stackOfMod.top()(0,3);
	v.pos[1] = v.pos[0]*stackOfMod.top()(1,0) + v.pos[1]*stackOfMod.top()(1,1)
	+ v.pos[2]*stackOfMod.top()(1,2) + v.pos[3]*stackOfMod.top()(1,3);
	v.pos[2] = v.pos[0]*stackOfMod.top()(2,0) + v.pos[1]*stackOfMod.top()(2,1)
	+ v.pos[2]*stackOfMod.top()(2,2) + v.pos[3]*stackOfMod.top()(2,3);
	v.pos[3] = v.pos[0]*stackOfMod.top()(3,0) + v.pos[1]*stackOfMod.top()(3,1)
	+ v.pos[2]*stackOfMod.top()(3,2) + v.pos[3]*stackOfMod.top()(3,3);

	v.pos[0] = v.pos[0]*stackOfProj.top()(0,0) + v.pos[1]*stackOfProj.top()(0,1)
	+ v.pos[2]*stackOfProj.top()(0,2) + v.pos[3]*stackOfProj.top()(0,3);
	v.pos[1] = v.pos[0]*stackOfProj.top()(1,0) + v.pos[1]*stackOfProj.top()(1,1)
	+ v.pos[2]*stackOfProj.top()(1,2) + v.pos[3]*stackOfProj.top()(1,3);
	v.pos[2] = v.pos[0]*stackOfProj.top()(2,0) + v.pos[1]*stackOfProj.top()(2,1)
	+ v.pos[2]*stackOfProj.top()(2,2) + v.pos[3]*stackOfProj.top()(2,3);
	v.pos[3] = v.pos[0]*stackOfProj.top()(3,0) + v.pos[1]*stackOfProj.top()(3,1)
	+ v.pos[2]*stackOfProj.top()(3,2) + v.pos[3]*stackOfProj.top()(3,3);
#endif

	listOfVertices.push_back(v);
}

/**
 * Specify a three-dimensional vertex.  Must appear between
 * calls to mglBegin() and mglEnd().
 */
void mglVertex3(MGLfloat x,
                MGLfloat y,
                MGLfloat z)
{
	vertex v;
	v.pos = {x, y, z, 1};
	v.color = curr_color;

	v.pos = stackOfMod.top() * v.pos;
	v.pos = stackOfProj.top() * v.pos;
	// v.pos /= v.pos[3];

	listOfVertices.push_back(v);
}

/**
 * Set the current matrix mode (modelview or projection).
 */
void mglMatrixMode(MGLmatrix_mode mode)
{
	matmode = mode;
	if(matmode == MGL_PROJECTION) {
		curr_stack = &stackOfProj;
	} else if(matmode == MGL_MODELVIEW){
		curr_stack = &stackOfMod;
	}
}

/**
 * Push a copy of the current matrix onto the stack for the
 * current matrix mode.
 */
void mglPushMatrix()
{
	curr_stack->push(curr_stack->top());
}

/**
 * Pop the top matrix from the stack for the current matrix
 * mode.
 */
void mglPopMatrix()
{
		curr_stack->pop();
}

/**
 * Replace the current matrix with the identity.
 */
void mglLoadIdentity()
{
	mat4 identity;
	identity.make_zero();
	identity.values[0] = 1;
	identity.values[5] = 1;
	identity.values[10] = 1;
	identity.values[15] = 1;

	if(matmode == MGL_PROJECTION) {
		if(stackOfProj.empty()) {
			stackOfProj.push(identity);
		}else {
			stackOfProj.pop();
			stackOfProj.push(identity);
		}
	} else if (matmode == MGL_MODELVIEW) {
		if(stackOfMod.empty()) {
			stackOfMod.push(identity);
		}else {
			stackOfMod.pop();
			stackOfMod.push(identity);
		}
	} else {
		cout << "ERROR IN mglLoadIdentity!\n";
	}
}

/**
 * Replace the current matrix with an arbitrary 4x4 matrix,
 * specified in column-major order.  That is, the matrix
 * is stored as:
 *
 *   ( a0  a4  a8  a12 )
 *   ( a1  a5  a9  a13 )
 *   ( a2  a6  a10 a14 )
 *   ( a3  a7  a11 a15 )
 *
 * where ai is the i'th entry of the array.
 */
void mglLoadMatrix(const MGLfloat *matrix)
{
	mat4 temp = curr_stack->top();

	for(int i = 0; i < 4; i++) {
		for(int j = 0; j < 4; j++){
				temp(i,j) = matrix[i + (j * 4)];
		}
	}
	curr_stack->top() = temp;
}

/**
 * Multiply the current matrix by an arbitrary 4x4 matrix,
 * specified in column-major order.  That is, the matrix
 * is stored as:
 *
 *   ( a0  a4  a8  a12 )
 *   ( a1  a5  a9  a13 )
 *   ( a2  a6  a10 a14 )
 *   ( a3  a7  a11 a15 )
 *
 * where ai is the i'th entry of the array.
 */
void mglMultMatrix(const MGLfloat *matrix)
{
	mat4 temp;
	temp.make_zero();

	for(int i = 0; i < 4; i++) {
		for(int j = 0; j < 4; j++){
				temp(i,j) = matrix[i + (j * 4)];
		}
	}

	curr_stack->top() = curr_stack->top() * temp;
}

/**
 * Multiply the current matrix by the translation matrix
 * for the translation vector given by (x, y, z).
 */
void mglTranslate(MGLfloat x,
                  MGLfloat y,
                  MGLfloat z)
{
	mat4 translateMat;
	translateMat.make_zero();

	translateMat(0,0) = 1;
	translateMat(1,1) = 1;
	translateMat(2,2) = 1;
	translateMat(3,3) = 1;

	translateMat(0,3) = x;
	translateMat(1,3) = y;
	translateMat(2,3) = z;

	curr_stack->top() = curr_stack->top() * translateMat;
}

/**
 * Multiply the current matrix by the rotation matrix
 * for a rotation of (angle) degrees about the vector
 * from the origin to the point (x, y, z).
 */
void mglRotate(MGLfloat angle,
               MGLfloat x,
               MGLfloat y,
               MGLfloat z)
{
	MGLfloat sdeg = sin(angle * M_PI / 180.0);
	MGLfloat cdeg = cos(angle * M_PI / 180.0);

	vec3 tp = {x,y,z};
	tp = tp.normalized();

	mat4 rotateMat;
	rotateMat.make_zero();

	rotateMat(0,0) = cdeg + (pow(tp[0],2)*(1-cdeg));
	rotateMat(0,1) = (tp[0]*tp[1]*(1-cdeg))- (tp[2]*sdeg);
	rotateMat(0,2) = (tp[0]*tp[2]*(1-cdeg))+ (tp[1]*sdeg);

	rotateMat(1,0) = (tp[1]*tp[0]*(1-cdeg))+ (tp[2]*sdeg);
	rotateMat(1,1) = cdeg + (pow(tp[1],2)*(1-cdeg));
	rotateMat(1,2) = (tp[1]*tp[2]*(1-cdeg))- (tp[0]*sdeg);

	rotateMat(2,0) = (tp[2]*tp[0]*(1-cdeg))- (tp[1]*sdeg);
	rotateMat(2,1) = (tp[2]*tp[1]*(1-cdeg))+ (tp[0]*sdeg);
	rotateMat(2,2) = cdeg + (pow(tp[2],2)*(1-cdeg));

	rotateMat(3,3) = 1;

	curr_stack->top() = curr_stack->top() * rotateMat;
}

/**
 * Multiply the current matrix by the scale matrix
 * for the given scale factors.
 */
void mglScale(MGLfloat x,
              MGLfloat y,
              MGLfloat z)
{
	mat4 scaleMat;
	scaleMat.make_zero();

	scaleMat(0,0) = x;
	scaleMat(1,1) = y;
	scaleMat(2,2) = z;
	scaleMat(3,3) = 1;

	curr_stack->top() = curr_stack->top() * scaleMat;
}

/**
 * Multiply the current matrix by the perspective matrix
 * with the given clipping plane coordinates.
 */
void mglFrustum(MGLfloat left,
                MGLfloat right,
                MGLfloat bottom,
                MGLfloat top,
                MGLfloat near,
                MGLfloat far)
{
	mat4 frustumMat;
	frustumMat.make_zero();

	frustumMat(0,0) = (2*near)/(right-left);
	frustumMat(0,2) = (right+left)/(right-left);
	frustumMat(1,1) = (2*near)/(top-bottom);
	frustumMat(1,2) = (top+bottom)/(top-bottom);
	frustumMat(2,2) = -1*(far+near)/(far-near);
	frustumMat(2,3) = (-2*far*near)/(far-near);
	frustumMat(3,2) = -1;
	curr_stack->top() = curr_stack->top() * frustumMat;
}

/**
 * Multiply the current matrix by the orthographic matrix
 * with the given clipping plane coordinates.
 */
void mglOrtho(MGLfloat left,
              MGLfloat right,
              MGLfloat bottom,
              MGLfloat top,
              MGLfloat near,
              MGLfloat far)
{
	mat4 orthoMatrix;
	orthoMatrix.make_zero();
	orthoMatrix(0,0) = 2/(right-left);
	orthoMatrix(0,3) = -1*((right+left)/(right-left));
	orthoMatrix(1,1) = 2/(top-bottom);
	orthoMatrix(1,3) = -1*((top+bottom)/(top-bottom));
	orthoMatrix(2,2) = -2/(far-near);
	orthoMatrix(2,3) = -1*((far+near)/(far-near));
	orthoMatrix(3,3) = 1;

	curr_stack->top() = curr_stack->top() * orthoMatrix;

}

/**
 * Set the current color for drawn shapes.
 */
void mglColor(MGLfloat red,
              MGLfloat green,
              MGLfloat blue)
{
	curr_color = {red, green, blue};
}
