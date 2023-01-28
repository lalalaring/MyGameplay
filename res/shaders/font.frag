#ifdef OPENGL_ES
#extension GL_OES_standard_derivatives : enable
#ifdef GL_FRAGMENT_PRECISION_HIGH
precision highp float;
#else
precision mediump float;
#endif
#endif

///////////////////////////////////////////////////////////
// Uniforms
uniform sampler2D u_texture;

#ifdef DISTANCE_FIELD
uniform vec2 u_cutoff;
#endif

///////////////////////////////////////////////////////////
// Varyings
in vec2 v_texCoord;
in vec4 v_color;

out vec4 FragColor;

void main()
{ 
    #ifdef DISTANCE_FIELD
    
    FragColor = v_color;
    float distance = texture2D(u_texture, v_texCoord).a;
    float smoothing = fwidth(distance);
    float alpha = smoothstep(0.5 - smoothing * u_cutoff.x, 0.5 + smoothing * u_cutoff.y, distance);
    FragColor.a = alpha * v_color.a;
    
    #else
    
    FragColor = v_color;
    FragColor.a = texture2D(u_texture, v_texCoord).a * v_color.a;
    
    #endif
}