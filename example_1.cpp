#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "graph.hpp"
#include "update.hpp"

int main(int argc, char *argv[]) {

	init_plotting(argc, argv);

	// Create an array for 101 * 101 vertices
	glm::vec3 vertices[101*101];
	// Create our datapoints, store it as bytes

	for (int i = 0; i < 101; i++) {
		for (int j = 0; j < 101; j++) {
			// printf("%i\n", i*101+j);
			vertices[i*101 + j].x = (j - 50) / 50.0;
			vertices[i*101 + j].y = (i - 50) / 50.0;

			float x = (i - 100 / 2) / (100 / 2.0);
			float y = (j - 100 / 2) / (100 / 2.0);
			float d = hypotf(x, y) * 4.0;
			float z = (1 - d * d) * expf(d * d / -2.0);

			vertices[i*101 + j].z = z;
		}

	}

	plot(vertices);

	return 0;
}


void update_data() {


	float scaling = sin(glutGet(GLUT_ELAPSED_TIME) / 1000.0f);

	glm::vec3 vertices[101*101];
	// Create our datapoints, store it as bytes

	for (int i = 0; i < 101; i++) {
		for (int j = 0; j < 101; j++) {
			// printf("%i\n", i*101+j);
			vertices[i*101 + j].x = (j - 50) / 50.0;
			vertices[i*101 + j].y = (i - 50) / 50.0;

			float x = (i - 100 / 2) / (100 / 2.0);
			float y = (j - 100 / 2) / (100 / 2.0);
			float d = hypotf(x, y) * 4.0;
			float z = (1 - d * d) * expf(d * d / -2.0);

			vertices[i*101 + j].z = z*scaling;
		}

	}



	update_display(vertices);
	display();
}

// Next steps
// Camera controls
// Change data format to allow per-point coloring
// Change point sizes
// Change where lines are calculated to ease computational overhead
// On screen drawing for points instead of "3d"