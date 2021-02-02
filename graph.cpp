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
#include "graph.hpp"

GLuint program;
GLint attribute_coord3d;
GLint uniform_vertex_transform;
GLint uniform_v_color;

GLuint program_faces;
GLint attribute_coord3d_faces;
GLint uniform_vertex_transform_faces;
GLint uniform_v_color_faces;

glm::vec3 uniform_vertices[MAX_NUM_POINTS];
glm::vec4 uniform_color[MAX_NUM_POINTS];

glm::vec3 uniform_face_vertices[MAX_NUM_POINTS];
glm::vec4 uniform_face_color[MAX_NUM_POINTS];
glm::vec3 uniform_face[MAX_NUM_POINTS];

int sprite_mode{0};

float offset_x = 0.0;
float offset_y = 0.0;
float scale = 1.0;

bool interpolate = false;
bool clamp = false;
bool rotate = false;

float point_length{0.01};
void point_size(float pl) {
	point_length = pl;
}

float h_distance(glm::vec3 camera_position, glm::vec3 camera_look_at) {

    return sqrt( pow(camera_position.x - camera_look_at.x, 2) + 
                 pow(camera_position.y - camera_look_at.y, 2));
}

GLuint vbo[4];

bool draw_faces{false};

int num_faces{0};

int init_resources(std::vector<glm::vec3> vertices, std::vector<glm::vec4> v_color) {
	program = create_program("graph.v.glsl", "graph.f.glsl");
	if (program == 0)
		return 0;

	attribute_coord3d = get_attrib(program, "coord3d");
	uniform_vertex_transform = get_uniform(program, "vertex_transform");
	uniform_v_color = get_attrib(program, "v_color");

	if (attribute_coord3d == -1 || uniform_vertex_transform == -1 || uniform_v_color == -1)
		return 0;


	// glm::vec3 cross_points[MAX_NUM_POINTS*4];

	update_display(vertices, v_color);


	/* Enable point sprites (not necessary for true OpenGL ES 2.0) */
	#ifndef GL_ES_VERSION_2_0
		glEnable(GL_POINT_SPRITE);
		glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);
	#endif

	// Create four vertex buffer objects
	glGenBuffers(4, vbo);

	return 1;
}


uint num_loaded_points{0};

void update_display(std::vector<glm::vec3> vertices) {
	std::vector<glm::vec4> v_color;

	for (auto i=0; i < vertices.size(); ++i) {
		glm::vec4 boring_color(1,1,1,1);

		v_color.push_back(boring_color);
	}

	update_display(vertices, v_color);

}

void update_display(std::vector<glm::vec3> vertices, std::vector<glm::vec4> v_color) {
	for (auto i=0; i < vertices.size(); i+=1) {
		uniform_vertices[i] = vertices[i];
		uniform_color[i] = v_color[i];
	}
	num_loaded_points = vertices.size();
}


glm::vec3 reset_camera_pos(0.0f, 2.0f, 2.0f);
glm::vec3 reset_camera_look(0.0f, 0.0f, 0.0f);

glm::vec3 camera_pos(0.0f, 2.0f, 2.0f);
glm::vec3 camera_look(0.0f, 0.0f, 0.0f);

void set_camera_home(glm::vec3 cl, glm::vec3 cp) {
	camera_pos = cp;
	camera_look = cl;

	reset_camera_pos = cp;
	reset_camera_look = cl;

}


void display() {
	glUseProgram(program);

	glm::mat4 model;
	glm::mat4 view;

	float angle = 0.0f;

	if (rotate) {
		// model = glm::rotate(glm::mat4(1.0f), glm::radians(glutGet(GLUT_ELAPSED_TIME) / 100.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		angle = glutGet(GLUT_ELAPSED_TIME) / 1000.0f;
		view = glm::lookAt(glm::vec3(sin(angle)*2.0, cos(angle)*2.0f, 2.0), glm::vec3(camera_look.x, camera_look.y, camera_look.z), glm::vec3(0.0, 0.0, 1.0));
	} else {
		//  model = glm::mat4(1.0f);
		view = glm::lookAt(glm::vec3(camera_pos.x, camera_pos.y, camera_pos.z), glm::vec3(camera_look.x, camera_look.y, camera_look.z), glm::vec3(0.0, 0.0, 1.0));
	}

 	model = glm::mat4(1.0f);


	glm::vec3 six_points[MAX_NUM_POINTS*6 + MAX_NUM_POINTS*3];
	glm::vec4 six_points_color[MAX_NUM_POINTS*6 + MAX_NUM_POINTS*3];

	// Load cross points into buffer

	for (auto i=0; i < num_loaded_points; i+=1) {
		six_points[i*6].x = uniform_vertices[i].x+point_length;
		six_points[i*6].y = uniform_vertices[i].y;
		six_points[i*6].z = uniform_vertices[i].z;
		six_points_color[i*6] = uniform_color[i];

		six_points[i*6+1].x = uniform_vertices[i].x-point_length;
		six_points[i*6+1].y = uniform_vertices[i].y;
		six_points[i*6+1].z = uniform_vertices[i].z;
		six_points_color[i*6+1] = uniform_color[i];

		six_points[i*6+2].x = uniform_vertices[i].x;
		six_points[i*6+2].y = uniform_vertices[i].y+point_length;
		six_points[i*6+2].z = uniform_vertices[i].z;
		six_points_color[i*6+2] = uniform_color[i];

		six_points[i*6+3].x = uniform_vertices[i].x;
		six_points[i*6+3].y = uniform_vertices[i].y-point_length;
		six_points[i*6+3].z = uniform_vertices[i].z;
		six_points_color[i*6+3] = uniform_color[i];		

		six_points[i*6+4].x = uniform_vertices[i].x;
		six_points[i*6+4].y = uniform_vertices[i].y;
		six_points[i*6+4].z = uniform_vertices[i].z+point_length;
		six_points_color[i*6+4] = uniform_color[i];

		six_points[i*6+5].x = uniform_vertices[i].x;
		six_points[i*6+5].y = uniform_vertices[i].y;
		six_points[i*6+5].z = uniform_vertices[i].z-point_length;
		six_points_color[i*6+5] = uniform_color[i];

	}



	glm::mat4 projection = glm::perspective(45.0f, 1.0f * 640 / 480, 0.1f, 10.0f);
	glm::mat4 vertex_transform = projection * view * model;
	glUniformMatrix4fv(uniform_vertex_transform, 1, GL_FALSE, glm::value_ptr(vertex_transform));

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClearDepth(10.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (draw_faces) {

		for (auto i=0; i < num_faces; i+=1) {

			six_points[i*3 + num_loaded_points*6] = uniform_face_vertices[int(uniform_face[i].x)];
			six_points_color[i*3 + num_loaded_points*6] = uniform_face_color[i];

			six_points[i*3+1 + num_loaded_points*6] = uniform_face_vertices[int(uniform_face[i].y)];
			six_points_color[i*3+1 + num_loaded_points*6] = uniform_face_color[i];

			six_points[i*3+2 + num_loaded_points*6] =  uniform_face_vertices[int(uniform_face[i].z)];
			six_points_color[i*3+2 + num_loaded_points*6] = uniform_face_color[i];

			// printf(" %i %i %i\n", int(uniform_face[i].x), int(uniform_face[i].y), int(uniform_face[i].z));
			// printf("%f %f %f %f\n", six_points_color[i*3+2 + num_loaded_points*6].r, six_points_color[i*3+2 + num_loaded_points*6].g, six_points_color[i*3+2 + num_loaded_points*6].b, six_points_color[i*3+2 + num_loaded_points*6].a);

		}

		// printf(" num loaded points: %i final index %i num faces %i start index %i final index %i\n", num_loaded_points, (num_loaded_points-1)*6+5, num_faces, num_loaded_points*6, num_loaded_points*6 + num_faces*3);

	}



	// Tell OpenGL to copy our array to the buffer objects
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof six_points, six_points, GL_STATIC_DRAW);


	/* Draw the 6-point symbol */
	glEnableVertexAttribArray(attribute_coord3d);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glVertexAttribPointer(attribute_coord3d, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glDrawArrays(GL_LINES, 0, num_loaded_points*6);

	// Draw the triangles
	// glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glDrawArrays(GL_TRIANGLES, num_loaded_points*6, num_faces*3); 

	// COLOR
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof six_points_color, six_points_color, GL_STATIC_DRAW);

	glEnableVertexAttribArray(uniform_v_color);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glVertexAttribPointer(uniform_v_color, 4, GL_FLOAT, GL_FALSE, 0, 0);


	/* Stop using the vertex buffer object */
	glDisableVertexAttribArray(attribute_coord3d);
	glDisableVertexAttribArray(attribute_coord3d_faces);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glutSwapBuffers();
}

void special(int key, int x, int y) {
	switch (key) {
	case GLUT_KEY_F1:
		sprite_mode = 0;
		printf("Quick Exit\n");
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

		printf("camera_pos %f, %f, %f\n", camera_pos.x, camera_pos.y, camera_pos.z);
		printf("camera_look %f, %f, %f\n", camera_look.x, camera_look.y, camera_look.z);


		printf("Resetting View\n");
		camera_pos = reset_camera_pos;
		camera_look = reset_camera_look;
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


// Capture the start x,y positions so we can calculate camera motion
bool rotate_enabled{false};
bool pan_enabled{false};
bool zoom_enabled{false};
int x_start;
int y_start;
float radius_change{0.0f};
void mouse(int button, int state, int x, int y) {

	// Enable or disable rotation based on left button
	if (button == GLUT_LEFT_BUTTON) {

		if (state == GLUT_DOWN)   {

			x_start = x;
			y_start = y;
			rotate_enabled = true;

		} else if (state == GLUT_UP) {

			rotate_enabled = false;

		}

	}

	// Enable or disable pan based on right button
	else if (button == GLUT_RIGHT_BUTTON) {
		printf("Panning\n");

		if (state == GLUT_DOWN)   {

			x_start = x;
			y_start = y;
			pan_enabled = true;

		} else if (state == GLUT_UP) {

			pan_enabled = false;

		}

	}

	// scroll up (zoom out)
	else if (button == 3) {
		radius_change = 0.03;
		zoom_enabled = true;
	} 
	// scroll down (zoom in)
	else if (button == 4) {
		radius_change = -0.32;
		zoom_enabled = true;
	}

	// Calculate Zooming
	if (zoom_enabled) {

		float radius = distance(camera_pos, camera_look);
		float horizontal_radius = h_distance(camera_pos, camera_look);

		radius += radius_change;
		radius_change = 0;

		float azimuth = atan2(camera_pos.y - camera_look.y, camera_pos.x - camera_look.x);
		float elevation = atan2( (camera_pos.z - camera_look.z) , horizontal_radius);


		camera_pos.x = cos(azimuth)*(radius*cos(elevation)) + camera_look.x;
		camera_pos.y = sin(azimuth)*(radius*cos(elevation)) + camera_look.y;
		camera_pos.z = sin(elevation)*radius + camera_look.z;

		zoom_enabled = false;
	}



}

float camera_vertical_delta{0.0f};
float camera_horizontal_delta{0.0f};
void motion(int x, int y) {

	if (rotate_enabled) {
		float radius = distance(camera_pos, camera_look);
		float horizontal_radius = h_distance(camera_pos, camera_look);


		float azimuth = atan2(camera_pos.y - camera_look.y, camera_pos.x - camera_look.x);
		float elevation = atan2( (camera_pos.z - camera_look.z) , horizontal_radius);
		
		camera_vertical_delta = (y_start - y) / 50.0f;
		camera_horizontal_delta = (x_start - x) / 50.0f;

		azimuth += camera_horizontal_delta;
		elevation += camera_vertical_delta;

		x_start = x;
		y_start = y;

		camera_pos.x = cos(azimuth)*(radius*cos(elevation)) + camera_look.x;
		camera_pos.y = sin(azimuth)*(radius*cos(elevation)) + camera_look.y;
		camera_pos.z = sin(elevation)*radius + camera_look.z;


	} 
	

	else if (pan_enabled) {

		float radius = distance(camera_pos, camera_look);
		float horizontal_radius = h_distance(camera_pos, camera_look);
		float azimuth = atan2(camera_pos.y - camera_look.y, camera_pos.x - camera_look.x);
		float elevation = atan2( (camera_pos.z - camera_look.z) , horizontal_radius);

		/** Vertical component **/
		glm::vec3 B;

		// XZ rotation
		B.x = cos(elevation + M_PI/2);
		B.y = 0.0f;
		B.z = sin(elevation + M_PI/2);

		// XY Rotation
		B.x = cos(azimuth)*B.x;
		B.y = sin(azimuth)*B.x;

		B = glm::normalize(B);

		/** Horizontal Component **/

		glm::vec3 C;

		// XY Rotation
		C.x = cos(azimuth + M_PI/2);
		C.y = sin(azimuth + M_PI/2);
		C.z = 0.0f;

		// XZ Rotation
		C.x = cos(elevation)*C.x;
		C.z = sin(elevation)*C.x;

		C = glm::normalize(C);

		camera_vertical_delta = (y_start - y);
		camera_horizontal_delta = (x_start - x);

		float total_delta = abs(camera_vertical_delta) + abs(camera_horizontal_delta);

		if (total_delta > 0) {
			glm::vec3 pan_delta;

			pan_delta = B * ( -camera_vertical_delta / total_delta ) + C * ( camera_horizontal_delta / total_delta);

			pan_delta *= radius / 200.0f;

			// printf("%f %f %f   ---   %f %f\n", pan_delta.x, pan_delta.y, pan_delta.z, azimuth, elevation);

			x_start = x;
			y_start = y;

			camera_pos += pan_delta;
			camera_look += pan_delta;

		}

		

	} 
	
}

// If no color is supplied make it white and call the real function
void plot(std::vector<glm::vec3> vertices) {

	std::vector<glm::vec4> v_color;

	for (auto i=0; i < vertices.size(); ++i) {
		glm::vec4 boring_color(1,1,1,1);

		v_color.push_back(boring_color);
	}

	plot(vertices, v_color);
}

void plot(std::vector<glm::vec3> verticies, std::vector<glm::vec4> v_color) {


	GLint max_units;

	glGetIntegerv(GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS, &max_units);
	if (max_units < 1) {
		fprintf(stderr, "Your GPU does not have any vertex texture image units\n");
		exit(-1);
	}

	num_loaded_points = verticies.size();


	if (init_resources(verticies, v_color)) {

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


		glutMouseFunc(mouse);
		glutMotionFunc(motion);

		glutMainLoop();
	}

	free_resources();

}


void plot_object(std::vector<std::vector<glm::vec3>> vertices, std::vector<std::vector<glm::vec3>> faces) {

	std::vector<std::vector<glm::vec4>> v_color;


	float intensity_scale = 1.0f / float(vertices.size());

	for (auto object=0; object < vertices.size(); ++object) {

		std::vector<glm::vec4> v_color_temp;

		printf(" Object %i Color Scale %f\n", object, (object+1)*intensity_scale);

		for (auto i=0; i < faces[object].size(); ++i) {


			// glm::vec3 face;

			// face = vertices[int(faces[i].x)];

			// float distance = sqrt( pow(camera_pos.x - face.x,2) + 
			// 					pow(camera_pos.y - face.y,2) + 
			// 					pow(camera_pos.z - face.z,2));

			// float intensity = (1 - distance/10.0f);

			float intensity = intensity_scale*(object+1);

			glm::vec4 boring_color(intensity,intensity,intensity,1);

			v_color_temp.push_back(boring_color);
		}

		v_color.push_back(v_color_temp);
	}

	plot_object(vertices, faces, v_color);


}



void update_face_display(std::vector<std::vector<glm::vec3>> vertices, std::vector<std::vector<glm::vec3>> faces, std::vector<std::vector<glm::vec4>> v_color) {

	num_faces = 0;

	int cumulative_size{0};
	for (auto object=0; object < vertices.size(); ++object) {

		// printf("Object: %i\n", object);
		for (auto i=0; i < vertices[object].size(); i+=1) {

			// printf("unform index: %i\n", cumulative_size);

			uniform_face_vertices[cumulative_size] = vertices[object][i];

			cumulative_size++;
			// printf(" %f %f %f\n", uniform_face_vertices[i*(object+1)].x, uniform_face_vertices[i*(object+1)].y, uniform_face_vertices[i*(object+1)].z);
		}

	}
	
	cumulative_size = 0;
	for (auto object=0; object < vertices.size(); ++object) {

		num_faces += faces[object].size();

		for (auto i=0; i < faces[object].size(); i+=1) {
			uniform_face[cumulative_size] = faces[object][i];
			uniform_face_color[cumulative_size] = v_color[object][i];


			// printf(" %i %i %i\n", int(uniform_face[i*(object+1)].x), int(uniform_face[i*(object+1)].y), int(uniform_face[i*(object+1)].z));
			// printf(" %f %f %f %f\n", uniform_face_color[i*(object+1)].r, uniform_face_color[i*(object+1)].g, uniform_face_color[i*(object+1)].b, uniform_face_color[i*(object+1)].a);

			cumulative_size++;
		}
	}

	// for (auto i=0; i < num_faces; ++i) {
	// 	printf(" %f %f %f %f\n", uniform_face_color[i].r, uniform_face_color[i].g, uniform_face_color[i].b, uniform_face_color[i].a);
	// }

	
	printf("num_faces=%i\n",num_faces);
}


int init_face_resources(std::vector<std::vector<glm::vec3>> vertices, std::vector<std::vector<glm::vec3>> faces, std::vector<std::vector<glm::vec4>> v_color) {
	program_faces = create_program("graph.v.glsl", "graph.f.glsl");
	if (program_faces == 0)
		return 0;



	attribute_coord3d_faces = get_attrib(program_faces, "coord3d");
	uniform_vertex_transform_faces = get_uniform(program_faces, "vertex_transform");
	uniform_v_color_faces = get_attrib(program_faces, "v_color");

	if (attribute_coord3d_faces == -1 || uniform_vertex_transform_faces == -1 || uniform_v_color_faces == -1)
		return 0;



	update_face_display(vertices, faces, v_color);


	/* Enable point sprites (not necessary for true OpenGL ES 2.0) */
	#ifndef GL_ES_VERSION_2_0
		glEnable(GL_POINT_SPRITE);
		glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);
	#endif


	glDisable(GL_CULL_FACE); 


	return 1;
}

void plot_object(std::vector<std::vector<glm::vec3>> vertices, std::vector<std::vector<glm::vec3>> faces, std::vector<std::vector<glm::vec4>> v_color) {

	init_face_resources(vertices, faces, v_color);

	draw_faces = true;

}