attribute vec3 coord3d;
varying vec4 graph_coord;

attribute vec4 v_color;
varying vec4 f_color;

uniform mat4 vertex_transform;

uniform lowp float sprite;

void main(void) {
	graph_coord = vec4(coord3d, 1);

	gl_Position = vertex_transform * vec4(coord3d, 1);

	gl_PointSize = max(1.0, sprite);

	f_color = v_color;
}
