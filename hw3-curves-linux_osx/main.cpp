/***********************************************************************/
/***** The Skeleton Code for the Curves Assignment in CSE 167      *****/
/***** Originally written by Aner Ben-Artzi several years ago      *****/
/***** Modified in Dec 2009 by Ravi Ramamoorthi for more comments, *****/
/***** and consistency with modern C++ rules                        ****/
/***** Modified in Dec 2011 by Ravi Ramamoorthi to be consistent    ****/
/***** Modified in Aug 2016 by Hoang Tran to only use modern OpenGL ****/
/***********************************************************************/

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#include <OpenGL/glext.h>
#include <GLUT/glut.h>
#else
#include <GL/glew.h>
#include <GL/glut.h>
#endif
#include "shaders.h"
#include "WorkingScene.h"

#define ASSIGNMENT_NAME "Homework: Curves"

WorkingScene scene; // the scene information 
GLuint vertexshader, fragmentshader, shaderprogram; // shaders
GLuint VAO, VBO; // Needed for vertex data
GLuint projectionPos, modelviewPos, colorPos; // Uniform variables

// Basic display and main setup routines.  
// Display just does basic OpenGL stuff to display 2D curve
// The main routine just sets up a basic display function


void display() {	
	//First, the modelview and projection matrices get set up
	//this determines how the scene will be viewed
	float m[] = {	2.0, 0, 0, 0,
                    0, 2.0, 0, 0,
                    0, 0, 2.0, 0,
                    -1.0, -1.0, 0, 1.0};
	// Use identity matrix as projection
	float projection[] = { 1.0, 0, 0, 0,
						   0, 1.0, 0, 0,
						   0, 0, 1.0, 0,
						   0, 0, 0, 1.0 }; // Identity matrix
	// Pass these matrices to the shader
	glUniformMatrix4fv(modelviewPos, 1, GL_FALSE, m);
	glUniformMatrix4fv(projectionPos, 1, GL_FALSE, projection);

	//Second, the framebuffer is filled with the objects in the scene
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glBindVertexArray(VAO);
	if (scene.theOnlyCurve) {
		scene.theOnlyCurve->draw(scene.levelOfDetail);
	}
	glBindVertexArray(0);
	glutSwapBuffers();
}

// The initialization function for curves does almost nothing. 
// We only need very simple shaders since we're only drawing in 2D
void init() {
	vertexshader = initshaders(GL_VERTEX_SHADER, "shaders/nop.vert.glsl") ;
	fragmentshader = initshaders(GL_FRAGMENT_SHADER, "shaders/nop.frag.glsl") ;
	shaderprogram = initprogram(vertexshader, fragmentshader) ; 
	// Get the locations of the uniform variables
	projectionPos = glGetUniformLocation(shaderprogram, "projection");
	modelviewPos = glGetUniformLocation(shaderprogram, "modelview");
	colorPos = glGetUniformLocation(shaderprogram, "color");
	// Have OpenGL generate a VAO and VBO for us
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	// Set up the VAO and VBO
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, 0, 0, GL_STATIC_DRAW);
	// Use layout location 0 for the positions.
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), 0);
	glBindVertexArray(0);
	// Pass information to the scene (so it can be forwarded to the curves)
	scene.colorPos = colorPos;
}

int main(int argc, char* argv[]) {
	glutInit( &argc, argv );
	glutInitWindowSize( scene.width, scene.height );
    // OSX systems require an extra init flag
#ifdef __APPLE__
    glutInitDisplayMode(GLUT_3_2_CORE_PROFILE | GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
#else
	glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH );
#endif
	glutCreateWindow( ASSIGNMENT_NAME );
#ifndef __APPLE__ // GLEW not needed on OSX
	GLenum err = glewInit() ; 
	if (GLEW_OK != err) { 
		std::cerr << "Error: " << glewGetString(err) << std::endl; 
	} 
#endif
	init(); 
	glutDisplayFunc( display );
	glutKeyboardFunc( scene.keyboard );
	glutMouseFunc( scene.mouse );
	glutReshapeFunc( scene.reshape );
	glutPassiveMotionFunc( scene.passiveMotion );
	glutMotionFunc( scene.drag );	
	glutMainLoop();
	// Program termination, destroy the VAO and VBO
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	return 0;
}
