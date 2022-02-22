#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <GL/glew.h>
#include <GL/glut.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

constexpr uint MAX_NUM_POINTS{250000};

void set_camera_home(glm::vec3 cl, glm::vec3 cp);

void init_plotting(int argc, char *argv[]);

void preallocate_vertices(uint num);

void plot(std::vector<glm::vec3> vertices);
void plot(std::vector<glm::vec3> vertices, std::vector<glm::vec4> v_color);

void plot_object(std::vector<std::vector<glm::vec3>> vertices, std::vector<std::vector<glm::vec3>> faces);
void plot_object(std::vector<std::vector<glm::vec3>> vertices, std::vector<std::vector<glm::vec3>> faces, std::vector<std::vector<glm::vec4>> v_color);


void plot(std::vector<glm::vec3> vertices);
void plot(std::vector<glm::vec3> vertices, std::vector<glm::vec4> v_color);

void update_display(std::vector<glm::vec3> vertices);
void update_display(std::vector<glm::vec3> vertices, std::vector<glm::vec4> v_color);

void point_size(float point_length);

void display();


#endif