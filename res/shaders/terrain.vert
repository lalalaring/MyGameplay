
#include "_lighting_def.glsl"

///////////////////////////////////////////////////////////
// Attributes
in vec4 a_position;
#if !defined(NORMAL_MAP) && defined(LIGHTING)
in vec3 a_normal;
#endif
in vec2 a_texCoord0;

#if defined(BUMPED)
in vec3 a_tangent;
in vec3 a_binormal;
#endif

///////////////////////////////////////////////////////////
// Uniforms
uniform mat4 u_worldViewProjectionMatrix;
#if !defined(NORMAL_MAP) && defined(LIGHTING)
uniform mat4 u_normalMatrix;
#endif


///////////////////////////////////////////////////////////
// Varyings
#if defined(LIGHTING)
#include "_lighting.vert"
#endif

out vec2 v_texCoord0;
#if LAYER_COUNT > 0
out vec2 v_texCoordLayer0;
#endif
#if LAYER_COUNT > 1
out vec2 v_texCoordLayer1;
#endif
#if LAYER_COUNT > 2
out vec2 v_texCoordLayer2;
#endif


void main()
{
    // Transform position to clip space.
    gl_Position = u_worldViewProjectionMatrix * a_position;

    #if defined(LIGHTING)

    #if !defined(NORMAL_MAP) 
    v_normalVector = normalize((u_normalMatrix * vec4(a_normal.x, a_normal.y, a_normal.z, 0)).xyz);
    #endif

    applyLight(a_position);

    #endif

    // Pass base texture coord
    v_texCoord0 = a_texCoord0;

    // Pass repeated texture coordinates for each layer
    #if LAYER_COUNT > 0
    v_texCoordLayer0 = a_texCoord0 * TEXTURE_REPEAT_0;
    #endif
    #if LAYER_COUNT > 1
    v_texCoordLayer1 = a_texCoord0 * TEXTURE_REPEAT_1;
    #endif
    #if LAYER_COUNT > 2
    v_texCoordLayer2 = a_texCoord0 * TEXTURE_REPEAT_2;
    #endif
}
