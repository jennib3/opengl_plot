attribute vec3 coord2d;
varying vec4 graph_coord;
uniform mat4 texture_transform;
uniform mat4 vertex_transform;
//uniform sampler2D mytexture;

uniform lowp float sprite;

void main(void) {
	graph_coord = vec4(coord2d, 1);
	//graph_coord.z = (texture2D(mytexture, graph_coord.xy / 2.0 + 0.5).r);

	gl_Position = vertex_transform * vec4(coord2d, 1);

	gl_PointSize = max(1.0, sprite);
}
