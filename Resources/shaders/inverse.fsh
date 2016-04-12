#ifdef GL_ES
precision mediump float;
#endif

varying vec4 v_fragmentColor;
varying vec2 v_texCoord;

void main() {
	vec4 c = texture2D(CC_Texture0, v_texCoord);
	gl_FragColor.xyz = vec3(1.0) - c.xyz;
	gl_FragColor.w = c.w;
}