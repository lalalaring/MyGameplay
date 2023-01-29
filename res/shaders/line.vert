
uniform vec4 u_viewPort;
uniform float u_lineWidth;

uniform mat4 u_worldViewProjectionMatrix;
//uniform mat4 u_inverseTransposeWorldViewMatrix;

in vec3 a_position;
in vec3 a_refpoint;
in float a_side;
in float a_endoffset;
in float a_distance;

out vec2 v_local_coord;
out float v_segment_len;
out float v_distance;

vec2 ndc_to_screen(vec2 p) {
  p = (p + 1.0)*0.5;
  p.x *= u_viewPort.z;
  p.y *= u_viewPort.w;
  return p;
}

vec2 screen_to_ndc(vec2 p) {
  p.x /= u_viewPort.z;
  p.y /= u_viewPort.w;
  p = p * 2.0 - 1.0;
  return p;
}

void main() {
    vec4 cp0 = u_worldViewProjectionMatrix * vec4(a_position, 1.0);
    vec4 cp1 = u_worldViewProjectionMatrix * vec4(a_refpoint, 1.0);

    //float aspect = u_viewPort.z / u_viewPort.w;
    float p_thickness = u_lineWidth*0.5;

    vec2 p0 = ndc_to_screen(cp0.xy/cp0.w);
    vec2 p1 = ndc_to_screen(cp1.xy/cp1.w);
    vec2 odir = normalize(p1 - p0);
    vec2 dir = odir*a_endoffset;
    vec2 normal_offset = vec2(-dir.y, dir.x) * a_side;
    normal_offset *= p_thickness;
    vec2 cap_offset = -odir * p_thickness;

    vec2 screen_pos = p0 + normal_offset + cap_offset;
    vec2 new_pos = screen_to_ndc(screen_pos);
    vec2 final_offset = new_pos - (cp0.xy/cp0.w);

    v_segment_len = distance(p0, p1)/p_thickness;
    v_local_coord.y = a_side;
    if (a_endoffset < 0.0) {
      v_local_coord.x = -1.0;
    }
    else {
      v_local_coord.x = 1.0 + v_segment_len;
    }
    v_distance = a_distance;

    cp0.xy += final_offset*cp0.w;
    gl_Position = cp0;
}