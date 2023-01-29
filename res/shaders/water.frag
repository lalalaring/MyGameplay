#ifdef GL_ES
precision mediump float;
#endif

in vec3 v_position;
in vec3 v_normalVector;
out vec4 FragColor;

uniform samplerCube u_skybox;
uniform sampler2D u_waterNormal;
uniform vec3 u_cameraPosition;

uniform vec3 u_albedo;
//uniform float u_roughness;
//uniform float u_metallic;
uniform float u_time;

const float PI = 3.14159265359;

vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}

void main(void) {
  vec3 noise = texture(u_waterNormal, v_position.xz/1000.0+u_time/30.0).xzy;
  vec3 V = normalize(u_cameraPosition - v_position);
  vec3 I = -V;
  vec3 N = normalize(v_normalVector+noise*0.00001);
  vec3 R = reflect(I, N);
  vec3 specular = texture(u_skybox, R).rgb;
  vec3 T = refract(I, N, 0.66);
  vec3 refractC = texture(u_skybox, T).rgb;
  
  vec3 F0 = vec3(0.04); 
  vec3 fresnel = fresnelSchlick(dot(V,N), F0);

  vec3 diffuse = u_albedo;
  float fog = 1.0/(length(u_cameraPosition - v_position)/100.0);

  vec3 c = diffuse + specular * fresnel + refractC * fog;

  FragColor = vec4(c, 1.0);
  //FragColor = vec4(vec3(fresnel), 1.0);
}