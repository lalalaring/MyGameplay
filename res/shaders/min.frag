#ifdef OPENGL_ES
#ifdef GL_FRAGMENT_PRECISION_HIGH
precision highp float;
#else
precision mediump float;
#endif
#endif

uniform vec4 u_diffuseColor;

void main()
{
    gl_FragColor = u_diffuseColor;
}
