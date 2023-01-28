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
uniform vec4 u_diffuseColor;

#if defined(LIGHTMAP)
uniform sampler2D u_lightmapTexture;
#endif


#if defined(LIGHTING)
#include "_lighting.frag"
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
#if defined(VERTEX_COLOR)
in vec3 v_color;
#endif

#if defined(LIGHTMAP)
in vec2 v_texCoord1;
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
 
    #if defined(LIGHTING)

    #if defined(VERTEX_COLOR)
	_baseColor.rgb = v_color;
    #else
    _baseColor = u_diffuseColor;
	#endif
    
    FragColor.a = _baseColor.a;
    FragColor.rgb = getLitPixel();
    
    #else
    
    #if defined(VERTEX_COLOR)
    FragColor.rgb = v_color;
    FragColor.a = 1.0;
    #else
    FragColor = u_diffuseColor;
    #endif
    
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
