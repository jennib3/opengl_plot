#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "graph.hpp"

int main(int argc, char *argv[]) {

	init_plotting(argc, argv);

	// Create an array for 101 * 101 vertices
	glm::vec3 vertices[101][101];
	// Create our datapoints, store it as bytes

	for (int i = 0; i < 101; i++) {
		for (int j = 0; j < 101; j++) {
			vertices[i][j].x = (j - 50) / 50.0;
			vertices[i][j].y = (i - 50) / 50.0;

			float x = (i - 100 / 2) / (100 / 2.0);
			float y = (j - 100 / 2) / (100 / 2.0);
			float d = hypotf(x, y) * 4.0;
			float z = (1 - d * d) * expf(d * d / -2.0);

			vertices[i][j].z = z;
		}
	}


	// Create an array for 101 * 101 vertices
	glm::vec3 invertices[101][101];
	// Create our datapoints, store it as bytes

	for (int i = 0; i < 101; i++) {
		for (int j = 0; j < 101; j++) {
			invertices[i][j].x = (j - 50) / 50.0;
			invertices[i][j].y = (i - 50) / 50.0;

			float x = (i - 100 / 2) / (100 / 2.0);
			float y = (j - 100 / 2) / (100 / 2.0);
			float d = hypotf(x, y) * 4.0;
			float z = (1 - d * d) * expf(d * d / -2.0);

			invertices[i][j].z = -z;
		}
	}

	plot(vertices);

	return 0;
}
