#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <GL/glew.h>
#include <GL/glut.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "./shader_utils/shader_utils.h"

#include "./sprites/plus.c"
#include "./sprites/cross.c"
#include "./sprites/circle.c"
#include "./sprites/heart.c"

#include "update.hpp"

GLuint program;
GLint attribute_coord2d;
GLint uniform_vertex_transform;
GLuint texture_id;
GLuint texture_id2;
GLuint texture_id3;
GLuint texture_id4;

GLint uniform_mytexture;

GLint uniform_sprite;

glm::vec3 uniform_vertices[101*101];

int sprite_mode{0};

float offset_x = 0.0;
float offset_y = 0.0;
float scale = 1.0;

bool interpolate = false;
bool clamp = false;
bool rotate = false;

GLuint vbo[2];

int init_resources(glm::vec3 vertices[101*101]) {
	program = create_program("graph.v.glsl", "graph.f.glsl");
	if (program == 0)
		return 0;



	attribute_coord2d = get_attrib(program, "coord2d");
	uniform_vertex_transform = get_uniform(program, "vertex_transform");
	uniform_sprite = get_uniform(program, "sprite");
	// uniform_mytexture = get_uniform(program, "mytexture");

	if (attribute_coord2d == -1 || uniform_vertex_transform == -1 )
		return 0;


	glm::vec3 cross_points[101*101*4];

	for (auto i=0; i < 101*101; i+=1) {
		uniform_vertices[i] = vertices[i];
		// cross_points[i*4].x = vertices[i].x+.01;
		// cross_points[i*4].y = vertices[i].y;
		// cross_points[i*4].z = vertices[i].z;

		// cross_points[i*4+1].x = vertices[i].x-.01;
		// cross_points[i*4+1].y = vertices[i].y;
		// cross_points[i*4+1].z = vertices[i].z;

		// cross_points[i*4+2].x = vertices[i].x;
		// cross_points[i*4+2].y = vertices[i].y;
		// cross_points[i*4+2].z = vertices[i].z+.01;

		// cross_points[i*4+3].x = vertices[i].x;
		// cross_points[i*4+3].y = vertices[i].y;
		// cross_points[i*4+3].z = vertices[i].z-.01;		
	}


	/* Enable point sprites (not necessary for true OpenGL ES 2.0) */
	#ifndef GL_ES_VERSION_2_0
		glEnable(GL_POINT_SPRITE);
		glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);
	#endif

glm::vec3 c_test[101*101];
	// Create two vertex buffer objects
	glGenBuffers(1, vbo);

	// Tell OpenGL to copy our array to the buffer objects
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof cross_points, cross_points, GL_STATIC_DRAW);


	return 1;
}

void update_display(glm::vec3 vertices[101*101]) {
	for (auto i=0; i < 101*101; i+=1) {
		uniform_vertices[i] = vertices[i];
	}
}

void display() {
	glUseProgram(program);
	glUniform1i(uniform_mytexture, sprite_mode);

	glm::mat4 model;
	glm::mat4 view;

	float angle = 0.0f;

	if (rotate) {
		// model = glm::rotate(glm::mat4(1.0f), glm::radians(glutGet(GLUT_ELAPSED_TIME) / 100.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		angle = glutGet(GLUT_ELAPSED_TIME) / 1000.0f;
		view = glm::lookAt(glm::vec3(sin(angle)*2.0, cos(angle)*2.0f, 2.0), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 0.0, 1.0));
	} else {
		//  model = glm::mat4(1.0f);
		view = glm::lookAt(glm::vec3(0.0, 2.0, 2.0), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 0.0, 1.0));
	}

 model = glm::mat4(1.0f);

glm::vec3 cross_points[101*101*4];
	for (auto i=0; i < 101*101; i+=1) {
		cross_points[i*4].x = uniform_vertices[i].x+cos(-angle)*.01;
		cross_points[i*4].y = uniform_vertices[i].y+sin(-angle)*.01;
		cross_points[i*4].z = uniform_vertices[i].z;

		cross_points[i*4+1].x = uniform_vertices[i].x-cos(-angle)*.01;
		cross_points[i*4+1].y = uniform_vertices[i].y-sin(-angle)*.01;
		cross_points[i*4+1].z = uniform_vertices[i].z;

		cross_points[i*4+2].x = uniform_vertices[i].x;
		cross_points[i*4+2].y = uniform_vertices[i].y;
		cross_points[i*4+2].z = uniform_vertices[i].z+.01/cos(.7071);

		cross_points[i*4+3].x = uniform_vertices[i].x;
		cross_points[i*4+3].y = uniform_vertices[i].y;
		cross_points[i*4+3].z = uniform_vertices[i].z-.01/cos(.7071);		
	}

glm::vec3 six_points[101*101*6];
	for (auto i=0; i < 101*101; i+=1) {
		six_points[i*6].x = uniform_vertices[i].x+.01;
		six_points[i*6].y = uniform_vertices[i].y;
		six_points[i*6].z = uniform_vertices[i].z;

		six_points[i*6+1].x = uniform_vertices[i].x-.01;
		six_points[i*6+1].y = uniform_vertices[i].y;
		six_points[i*6+1].z = uniform_vertices[i].z;

		six_points[i*6+2].x = uniform_vertices[i].x;
		six_points[i*6+2].y = uniform_vertices[i].y+.01;
		six_points[i*6+2].z = uniform_vertices[i].z;

		six_points[i*6+3].x = uniform_vertices[i].x;
		six_points[i*6+3].y = uniform_vertices[i].y-.01;
		six_points[i*6+3].z = uniform_vertices[i].z;		

		six_points[i*6+4].x = uniform_vertices[i].x;
		six_points[i*6+4].y = uniform_vertices[i].y;
		six_points[i*6+4].z = uniform_vertices[i].z+.01;

		six_points[i*6+5].x = uniform_vertices[i].x;
		six_points[i*6+5].y = uniform_vertices[i].y;
		six_points[i*6+5].z = uniform_vertices[i].z-.01;

	}

	// Tell OpenGL to copy our array to the buffer objects
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof six_points, six_points, GL_STATIC_DRAW);


	// glm::mat4 view = glm::lookAt(glm::vec3(0.0, 2.0, 2.0), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 0.0, 1.0));
	glm::mat4 projection = glm::perspective(45.0f, 1.0f * 640 / 480, 0.1f, 10.0f);

	glm::mat4 vertex_transform = projection * view * model;
	// glm::mat4 texture_transform = glm::translate(glm::scale(glm::mat4(1.0f), glm::vec3(scale, scale, 1)), glm::vec3(offset_x, offset_y, 0));

	glUniformMatrix4fv(uniform_vertex_transform, 1, GL_FALSE, glm::value_ptr(vertex_transform));
	// glUniformMatrix4fv(uniform_texture_transform, 1, GL_FALSE, glm::value_ptr(texture_transform));

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClearDepth(10.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/* Draw the grid using the indices to our vertices using our vertex buffer objects */
	glEnableVertexAttribArray(attribute_coord2d);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glVertexAttribPointer(attribute_coord2d, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glDrawArrays(GL_LINES, 0, 101 * 101 * 6);


	// glUniform1f(uniform_sprite, plus_sprite.width);
	// glDrawElements(GL_POINTS, 100 * 101 * 4, GL_UNSIGNED_SHORT, 0);

	/* Stop using the vertex buffer object */
	glDisableVertexAttribArray(attribute_coord2d);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glutSwapBuffers();
}

void special(int key, int x, int y) {
	switch (key) {
	case GLUT_KEY_F1:
		sprite_mode = 0;
		printf("Using sprint 0\n");
		exit(0);
		break;
	case GLUT_KEY_F2:
		sprite_mode = 1;
		printf("Using sprint 1\n");
		break;
	case GLUT_KEY_F3:
		sprite_mode = 2;
		printf("Using sprint 2\n");
		break;
	case GLUT_KEY_F4:
		sprite_mode = 3;
		printf("Using sprint 3\n");
		break;		
	case GLUT_KEY_F12:
		rotate = !rotate;
		printf("Rotation is now %s\n", rotate ? "on" : "off");
		break;
	case GLUT_KEY_LEFT:
		offset_x -= 0.03;
		break;
	case GLUT_KEY_RIGHT:
		offset_x += 0.03;
		break;
	case GLUT_KEY_UP:
		offset_y += 0.03;
		break;
	case GLUT_KEY_DOWN:
		offset_y -= 0.03;
		break;
	case GLUT_KEY_PAGE_UP:
		scale *= 1.5;
		break;
	case GLUT_KEY_PAGE_DOWN:
		scale /= 1.5;
		break;
	case GLUT_KEY_HOME:
		offset_x = 0.0;
		offset_y = 0.0;
		scale = 1.0;
		break;
	// case GLUT_KEY_
	}

	glutPostRedisplay();
}

void free_resources() {
	glDeleteProgram(program);
}


void init_plotting(int argc, char *argv[]) {

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(640, 480);
	glutCreateWindow("My Graph");

	GLenum glew_status = glewInit();

	if (GLEW_OK != glew_status) {
		fprintf(stderr, "Error: %s\n", glewGetErrorString(glew_status));
		exit(1);
	}

	if (!GLEW_VERSION_2_0) {
		fprintf(stderr, "No support for OpenGL 2.0 found\n");
		exit(1);
	}
}


void plot(glm::vec3 verticies[101*101]) {


	GLint max_units;

	glGetIntegerv(GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS, &max_units);
	if (max_units < 1) {
		fprintf(stderr, "Your GPU does not have any vertex texture image units\n");
		exit(-1);
	}


	if (init_resources(verticies)) {

		// Transparency for sprites
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_POINT_SPRITE);
		glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);

		// Depth stuff
		glEnable(GL_DEPTH_TEST);
		glDepthMask(GL_TRUE);
		glDepthFunc(GL_LEQUAL);
		glDepthRange(0.0f, 10.0f);


		glutDisplayFunc(display);
		glutIdleFunc(update_data);
		glutSpecialFunc(special);
		glutMainLoop();
	}

	free_resources();

}