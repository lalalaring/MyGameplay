#ifdef OPENGL_ES
#ifdef GL_FRAGMENT_PRECISION_HIGH
precision highp float;
#else
precision mediump float;
#endif
#endif

out vec4 FragColor;
in vec3 v_texCoord;
uniform samplerCube u_diffuseTexture;

void main()
{    
    FragColor = texture(u_diffuseTexture, v_texCoord);
    //FragColor = vec4(v_texCoord, 1.0);
}