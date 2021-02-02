#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string>
#include <iostream>

#include "graph.hpp"
#include "update.hpp"
#include "load_object.h"

int main(int argc, char *argv[]) {

	init_plotting(argc, argv);

    // vector of vectors for each object we find
    std::vector<std::vector< glm::vec3 >> out_vertices;
    std::vector<std::vector< glm::vec3 >> out_faces;

    // Vector of object names
    std::vector< std::string> out_names;

    load_obj_file("cylinder.obj", out_vertices, out_faces, out_names);

	// Plots data once, use update_data to vary over time
	plot_object(out_vertices, out_faces);

    // flatten these vertices because the regular plot function doesn't care about "objects"
    std::vector<glm::vec3> flattened_vertices;
    for (auto object = 0; object < out_vertices.size(); ++object) {
        for (auto i=0; i < out_vertices[object].size(); ++i) {
            flattened_vertices.push_back(out_vertices[object][i]);
        }
    }


    plot(flattened_vertices);  // Plots all points as white
    

	return 0;
}


void update_data() {

    // You can call update_display and update_display_faces here if you want to change the data;
	display();
}
