uniform sampler2D mytexture;
varying vec4 f_color;
uniform float sprite;

void main(void) {
	if (sprite > 1.0)
		gl_FragColor = texture2D(mytexture, gl_PointCoord) * vec4(1,1,1,1);
	else
		gl_FragColor = vec4(1,1,1,0);
}
