#include <GL/glew.h>
#include <GL/glut.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#ifndef GRAPH_HPP
#define GRAPH_HPP

void init_plotting(int argc, char *argv[]);
void plot(glm::vec3 vertices[101][101]);

#endif