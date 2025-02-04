
#include "_lighting_def.glsl"
///////////////////////////////////////////////////////////
// Atributes
in vec4 a_position;

#if !defined(NORMAL_MAP) && defined(LIGHTING)
in vec3 a_normal;
#endif

#if defined(SKINNING)
in vec4 a_blendWeights;
in vec4 a_blendIndices;
#endif

in vec2 a_texCoord;

#if defined(LIGHTMAP)
in vec2 a_texCoord1; 
#endif

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
#if defined(SKINNING)
uniform vec4 u_matrixPalette[SKINNING_JOINT_COUNT * 3];
#endif


#if defined(TEXTURE_REPEAT)
uniform vec2 u_textureRepeat;
#endif

#if defined(TEXTURE_OFFSET)
uniform vec2 u_textureOffset;
#endif

#if defined(CLIP_PLANE)
uniform mat4 u_worldMatrix;
uniform vec4 u_clipPlane;
#endif

///////////////////////////////////////////////////////////
// Varyings
out vec2 v_texCoord;

#if defined(LIGHTMAP)
out vec2 v_texCoord1;
#endif

#if defined(LIGHTING)
#include "_lighting.vert"
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

    #if defined(LIGHTING)
    vec3 normal = getNormal();
    // Transform the normal, tangent and binormals to view space.
    mat3 inverseTransposeWorldViewMatrix = mat3(u_inverseTransposeWorldViewMatrix[0].xyz, u_inverseTransposeWorldViewMatrix[1].xyz, u_inverseTransposeWorldViewMatrix[2].xyz);
    vec3 normalVector = normalize(inverseTransposeWorldViewMatrix * normal);
    
    #if defined(BUMPED)
    
    vec3 tangent = getTangent();
    vec3 binormal = getBinormal();
    vec3 tangentVector  = normalize(inverseTransposeWorldViewMatrix * tangent);
    vec3 binormalVector = normalize(inverseTransposeWorldViewMatrix * binormal);
    mat3 tangentSpaceTransformMatrix = mat3(tangentVector.x, binormalVector.x, normalVector.x, tangentVector.y, binormalVector.y, normalVector.y, tangentVector.z, binormalVector.z, normalVector.z);
    applyLight(position, tangentSpaceTransformMatrix);
    
    #else
    
    v_normalVector = normalVector;
    applyLight(position);
    
    #endif
    
    #endif 
    
    v_texCoord = a_texCoord;
    
    #if defined(TEXTURE_REPEAT)
    v_texCoord *= u_textureRepeat;
    #endif
    
    #if defined(TEXTURE_OFFSET)
    v_texCoord += u_textureOffset;
    #endif
    
    #if defined(LIGHTMAP)
    v_texCoord1 = a_texCoord1;
    #endif
    
    #if defined(CLIP_PLANE)
    v_clipDistance = dot(u_worldMatrix * position, u_clipPlane);
    #endif
}
