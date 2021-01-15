#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <GL/glew.h>
#include <GL/glut.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

constexpr uint MAX_NUM_POINTS{101*101};

void init_plotting(int argc, char *argv[]);

void plot(std::vector<glm::vec3> vertices);
void plot(std::vector<glm::vec3> vertices, std::vector<glm::vec4> v_color);

void update_display(std::vector<glm::vec3> vertices);
void update_display(std::vector<glm::vec3> vertices, std::vector<glm::vec4> v_color);

void point_size(float point_length);

void display();


#endif