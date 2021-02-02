#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "graph.hpp"
#include "update.hpp"

int main(int argc, char *argv[]) {

	init_plotting(argc, argv);

	// Create an array for 101 * 101 vertices
	std::vector<glm::vec3> data;
	std::vector<glm::vec4> d_color;
	glm::vec3 vertex;
	// Create our datapoints, store it as bytes

	for (int i = 0; i < 101; i++) {
		for (int j = 0; j < 101; j++) {
			// printf("%i\n", i*101+j);
			vertex.x = (j - 50) / 50.0;
			vertex.y = (i - 50) / 50.0;

			float x = (i - 100 / 2) / (100 / 2.0);
			float y = (j - 100 / 2) / (100 / 2.0);
			float d = hypotf(x, y) * 4.0;
			float z = (1 - d * d) * expf(d * d / -2.0);

			vertex.z = z;

			glm::vec4 boring_color(1,1,1,1);
			d_color.push_back(boring_color);

			data.push_back(vertex);
		}

	}

	// Plots data once, use update_data to vary over time
	// plot(data);  // Plots all points as white
	plot(data, d_color); // Plots all points as specififed color per point

	return 0;
}


void update_data() {


	// We can use glutGet(GLUT_ELAPSED_TIME) to change update rate
	float scaling = sin(glutGet(GLUT_ELAPSED_TIME) / 1000.0f);

	// Create an array for 101 * 101 vertices
	std::vector<glm::vec3> data;
	std::vector<glm::vec4> d_color;

	// Create our datapoints, store it as bytes
	for (int i = 0; i < 101; i++) {
		for (int j = 0; j < 101; j++) {
			glm::vec3 vertex;

			vertex.x = (j - 50) / 50.0;
			vertex.y = (i - 50) / 50.0;

			float x = (i - 100 / 2) / (100 / 2.0);
			float y = (j - 100 / 2) / (100 / 2.0);
			float d = hypotf(x, y) * 4.0;
			float z = (1 - d * d) * expf(d * d / -2.0);

			vertex.z = z*scaling;
			
			// R, G, B, alpha
			glm::vec4 v_color(abs(sin(i)), abs(cos(i+j)), abs(sin(j)), 1.0);
			d_color.push_back(v_color);
			data.push_back(vertex);
		}

	}

	// point_size( 0.03*(scaling + 1) );



	update_display(data, d_color); // Renders per point with specified color
	// update_display(data); // Renders as white
	display();
}

// Next steps
// Change where lines are calculated to ease computational overhead
// On screen drawing for points instead of "3d"