#include "_lighting_def.glsl"

///////////////////////////////////////////////////////////
// Attributes
in vec4 a_position;

#if defined(SKINNING)
in vec4 a_blendWeights;
in vec4 a_blendIndices;
#endif

#if defined(LIGHTMAP)
in vec2 a_texCoord1;
#endif

#if defined(LIGHTING)
in vec3 a_normal;
#endif

#if defined(VERTEX_COLOR)
in vec3 a_color;
#endif

#if defined(BUMPED)
in vec3 a_tangent;
in vec3 a_binormal;
#endif

///////////////////////////////////////////////////////////
// Uniforms
uniform mat4 u_worldViewProjectionMatrix;

#if defined(SKINNING)
uniform vec4 u_matrixPalette[SKINNING_JOINT_COUNT * 3];
#endif

#if defined(LIGHTING)
#include "_lighting.vert"
#endif

#if defined(CLIP_PLANE)
uniform mat4 u_worldMatrix;
uniform vec4 u_clipPlane;
#endif

///////////////////////////////////////////////////////////
// Varyings
#if defined(LIGHTMAP)
out vec2 v_texCoord1;
#endif

#if defined(VERTEX_COLOR)
out vec3 v_color;
#endif

#if defined(SKINNING)
#include "_skinning.vert"
#else
#include "_skinning-none.vert" 
#endif

#if defined(CLIP_PLANE)
out float v_clipDistance;
#endif

void main()
{
    vec4 position = getPosition();
    gl_Position = u_worldViewProjectionMatrix * position;

    #if defined (LIGHTING)

    vec3 normal = getNormal();

    // Transform normal to view space.
    mat3 inverseTransposeWorldViewMatrix = mat3(u_inverseTransposeWorldViewMatrix[0].xyz, u_inverseTransposeWorldViewMatrix[1].xyz, u_inverseTransposeWorldViewMatrix[2].xyz);
    v_normalVector = inverseTransposeWorldViewMatrix * normal;

    // Apply light.
    applyLight(position);

    #endif

    // Pass the lightmap texture coordinate
    #if defined(LIGHTMAP)
    v_texCoord1 = a_texCoord1;
    #endif
    
    // Pass the vertex color
    #if defined(VERTEX_COLOR)
	v_color = a_color;
    #endif
    
    #if defined(CLIP_PLANE)
    v_clipDistance = dot(u_worldMatrix * position, u_clipPlane);
    #endif    
}
