#ifndef LOAD_OBJ_H
#define LOAD_OBJ_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string>
#include <iostream>

#include <GL/glew.h>
#include <GL/glut.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>


// adapted from http://www.opengl-tutorial.org/beginners-tutorials/tutorial-7-model-loading/
void load_obj_file(const char* path, 
                std::vector<std::vector< glm::vec3 >> &out_vertices,
                std::vector<std::vector< glm::vec3 >> &out_faces,
                std::vector< std::string> &out_names,
                std::vector<glm::vec3> &out_flattened) {

    std::vector< unsigned int > vertexIndices, uvIndices, normalIndices;
    std::vector< glm::vec2 > temp_uvs;
    std::vector< glm::vec3 > temp_normals;
    std::vector< glm::vec3 > temp_vertices;
    std::vector< glm::vec3 > temp_faces;

    FILE * file = fopen(path, "r");
    if( file == NULL ){
        printf("Impossible to open the file !\n");
        exit(1);
    }

    while( 1 ){

        char lineHeader[128];
        // read the first word of the line
        int res = fscanf(file, "%s", lineHeader);
        if (res == EOF)
            break; // EOF = End Of File. Quit the loop.


        // If we found a new object
        if ( strcmp( lineHeader, "o" ) == 0 ){
            
            // Grab object name for some reason
            char name_temp[101];
            fscanf(file, "%100s\n", name_temp );
            std::string object_name = name_temp;
            std::cout << "Found object " << object_name << std::endl;

            out_names.push_back(object_name);


            // If this isn't the first object, store the previous
            if (out_names.size() > 1) {

                out_faces.push_back(temp_faces);
                out_vertices.push_back(temp_vertices);
                // Need to add all the other information here, but this is all I'm using right now
            }


            // Clear all the buffers (since they are per object)
            vertexIndices.clear();
            uvIndices.clear();
            normalIndices.clear();
            temp_uvs.clear();
            temp_normals.clear();
            temp_vertices.clear();
            temp_faces.clear();


        // If we found a new vertex
        } else if ( strcmp( lineHeader, "v" ) == 0 ){
            glm::vec3 vertex;
            fscanf(file, "%f %f %f\n", &vertex.x, &vertex.z, &vertex.y );
            vertex.y = -vertex.y;
            temp_vertices.push_back(vertex);
            out_flattened.push_back(vertex);

        // If we found a new vertex texture
        } else if ( strcmp( lineHeader, "vt" ) == 0 ){
            glm::vec2 uv;
            fscanf(file, "%f %f\n", &uv.x, &uv.y );
            temp_uvs.push_back(uv);

        // if we found a new vertex normal
        } else if ( strcmp( lineHeader, "vn" ) == 0 ){
            glm::vec3 normal;
            fscanf(file, "%f %f %f\n", &normal.x, &normal.z, &normal.y );
            temp_normals.push_back(normal);

        // if we found a new face
        } else if ( strcmp( lineHeader, "f" ) == 0 ){
            std::string vertex1, vertex2, vertex3;
            unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
            int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2] );
            if (matches != 9){
                printf("File can't be read by our simple parser : ( Try exporting with other options\n");
                exit(1);
            }

            glm::vec3 temp_face;

            temp_face.x = vertexIndex[0]-1;
            temp_face.y = vertexIndex[1]-1;
            temp_face.z = vertexIndex[2]-1;

            temp_faces.push_back(temp_face);

            vertexIndices.push_back(vertexIndex[0]);
            vertexIndices.push_back(vertexIndex[1]);
            vertexIndices.push_back(vertexIndex[2]);
            uvIndices    .push_back(uvIndex[0]);
            uvIndices    .push_back(uvIndex[1]);
            uvIndices    .push_back(uvIndex[2]);
            normalIndices.push_back(normalIndex[0]);
            normalIndices.push_back(normalIndex[1]);
            normalIndices.push_back(normalIndex[2]);
        }

    }

    // Since we won't see a now "o" for a new object after the final object, we need to add it here
    out_faces.push_back(temp_faces);
    out_vertices.push_back(temp_vertices);

}

#endif
