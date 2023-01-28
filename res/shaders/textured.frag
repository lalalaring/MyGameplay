#ifdef OPENGL_ES
#ifdef GL_FRAGMENT_PRECISION_HIGH
precision highp float;
#else
precision mediump float;
#endif
#endif

#include "_lighting_def.glsl"

///////////////////////////////////////////////////////////
// Uniforms
uniform vec3 u_ambientColor;

uniform sampler2D u_diffuseTexture;

#if defined(LIGHTMAP)
uniform sampler2D u_lightmapTexture;
#endif

#if defined(MODULATE_COLOR)
uniform vec4 u_modulateColor;
#endif

#if defined(MODULATE_ALPHA)
uniform float u_modulateAlpha;
#endif

///////////////////////////////////////////////////////////
// Variables
vec4 _baseColor;

///////////////////////////////////////////////////////////
// Varyings
in vec2 v_texCoord;

#if defined(LIGHTMAP)
in vec2 v_texCoord1;
#endif

#if defined(LIGHTING)
#include "_lighting.frag"
#endif

#if defined(CLIP_PLANE)
in float v_clipDistance;
#endif

out vec4 FragColor;

void main()
{
    #if defined(CLIP_PLANE)
    if(v_clipDistance < 0.0) discard;
    #endif
 
    _baseColor = texture2D(u_diffuseTexture, v_texCoord);
 
    FragColor.a = _baseColor.a;

    #if defined(TEXTURE_DISCARD_ALPHA)
    if (FragColor.a < 0.5)
        discard;
    #endif

    #if defined(LIGHTING)
    FragColor.rgb = getLitPixel();
    #else
    FragColor.rgb = _baseColor.rgb;
    #endif

	#if defined(LIGHTMAP)
	vec4 lightColor = texture2D(u_lightmapTexture, v_texCoord1);
	FragColor.rgb *= lightColor.rgb;
	#endif

    #if defined(MODULATE_COLOR)
    FragColor *= u_modulateColor;
    #endif

    #if defined(MODULATE_ALPHA)
    FragColor.a *= u_modulateAlpha;
    #endif
}
