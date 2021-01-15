uniform sampler2D mytexture;
varying vec4 f_color;
uniform float sprite;

void main(void) {

		//gl_FragColor = vec4(1,1,1,1);

		gl_FragColor = f_color;
}
