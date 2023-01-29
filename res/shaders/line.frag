#ifdef OPENGL_ES
#ifdef GL_FRAGMENT_PRECISION_HIGH
precision highp float;
#else
precision mediump float;
#endif
#endif

uniform vec4 u_diffuseColor;
uniform vec4 u_viewPort;
uniform float u_lineWidth;
uniform float u_dashSize;

in vec2 v_local_coord;
in float v_segment_len;
in float v_distance;
out vec4 FragColor;

void main() {

  #ifdef DASH
    if ( mod( v_distance, u_dashSize*2.0 ) > u_dashSize ) discard;
  #endif

    FragColor = vec4(u_diffuseColor);
    
    if (v_local_coord.x < 0.0 || v_local_coord.x > v_segment_len) {
      float y = v_local_coord.y;
      float x = v_local_coord.x < 0.0 ? -v_local_coord.x : v_local_coord.x-v_segment_len;
      float len = x*x + y*y;
      if (len > 1.0) discard;
      
      FragColor.a *= 1.0-smoothstep(0.0, 1.0, len);
    }
    else {
      FragColor.a *= 1.0-smoothstep(0.0, 1.0, v_local_coord.y*v_local_coord.y);
    }

}