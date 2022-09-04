#include "GLRenderer.h"
#include "ogl.h"
#include "scene/MeshPart.h"
#include "material/Material.h"
#include "scene/MeshBatch.h"
#include "GLFrameBuffer.h"
#include "../scene/Drawable.h"


/** @script{ignore} */
GLenum __gl_error_code = GL_NO_ERROR;

using namespace gameplay;

GLRenderer::GLRenderer() {
  GLFrameBuffer::initialize();
}

GLRenderer::~GLRenderer() {
  GLFrameBuffer::finalize();
}

void GLRenderer::clear(ClearFlags flags, float red, float green, float blue, float alpha, float clearDepth, int clearStencil) {
    GLbitfield bits = 0;
    if (flags & CLEAR_COLOR)
    {
        glClearColor(red, green, blue, alpha);
        bits |= GL_COLOR_BUFFER_BIT;
    }

    if (flags & CLEAR_DEPTH)
    {
        glClearDepth(clearDepth);
        bits |= GL_DEPTH_BUFFER_BIT;

        // We need to explicitly call the static enableDepthWrite() method on StateBlock
        // to ensure depth writing is enabled before clearing the depth buffer (and to
        // update the global StateBlock render state to reflect this).
        enableDepthWrite();
    }

    if (flags & CLEAR_STENCIL)
    {
        glClearStencil(clearStencil);
        bits |= GL_STENCIL_BUFFER_BIT;
    }
    glClear(bits);

    //reset state
    StateBlock state;
    updateState(&state, 2);
}

void GLRenderer::setViewport(int x, int y, int w, int h) {
    glViewport((GLuint)x, (GLuint)y, (GLuint)w, (GLuint)h);
}

void GLRenderer::updateMesh(Mesh* mesh, unsigned int vertexStart, unsigned int vertexCount)
{
    if (mesh->_vertexBuffer == 0) {
        GLuint vbo;
        GL_ASSERT(glGenBuffers(1, &vbo));
        GL_ASSERT(glBindBuffer(GL_ARRAY_BUFFER, vbo));
        GL_ASSERT(glBufferData(GL_ARRAY_BUFFER, mesh->getVertexSize() * mesh->getVertexCount(), NULL, mesh->isDynamic() ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW));
        mesh->_vertexBuffer = vbo;
    }
    
    GL_ASSERT(glBindBuffer(GL_ARRAY_BUFFER, mesh->_vertexBuffer));

    if (vertexStart == 0 && vertexCount == 0)
    {
        GL_ASSERT(glBufferData(GL_ARRAY_BUFFER, mesh->getVertexSize() * mesh->getVertexCount(), mesh->_vertexData, mesh->isDynamic() ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW));
    }
    else
    {
        if (vertexCount == 0)
        {
            vertexCount = mesh->getVertexCount() - vertexStart;
        }

        GL_ASSERT(glBufferSubData(GL_ARRAY_BUFFER, vertexStart * mesh->getVertexSize(), vertexCount * mesh->getVertexSize(), mesh->_vertexData));
    }
    /*
    unsigned int partCount = mesh->getPartCount();
    for (unsigned int i = 0; i < partCount; ++i)
    {
        MeshPart* part = mesh->getPart(i);
        if (part->_indexDataDirty) {
            updateMeshPart(part, 0, part->getIndexCount());
            part->_indexDataDirty = false;
        }
    }*/
}

static bool drawWireframe(Mesh* mesh)
{
    switch (mesh->getPrimitiveType())
    {
    case Mesh::TRIANGLES:
    {
        unsigned int vertexCount = mesh->getVertexCount();
        for (unsigned int i = 0; i < vertexCount; i += 3)
        {
            GL_ASSERT(glDrawArrays(GL_LINE_LOOP, i, 3));
        }
    }
    return true;

    case Mesh::TRIANGLE_STRIP:
    {
        unsigned int vertexCount = mesh->getVertexCount();
        for (unsigned int i = 2; i < vertexCount; ++i)
        {
            GL_ASSERT(glDrawArrays(GL_LINE_LOOP, i - 2, 3));
        }
    }
    return true;

    default:
        // not supported
        return false;
    }
}

static bool drawWireframe(MeshPart* part)
{
    unsigned int indexCount = part->getIndexCount();
    unsigned int indexSize = 0;
    switch (part->getIndexFormat())
    {
    case Mesh::INDEX8:
        indexSize = 1;
        break;
    case Mesh::INDEX16:
        indexSize = 2;
        break;
    case Mesh::INDEX32:
        indexSize = 4;
        break;
    default:
        GP_ERROR("Unsupported index format (%d).", part->getIndexFormat());
        return false;
    }

    switch (part->getPrimitiveType())
    {
    case Mesh::TRIANGLES:
    {
        for (size_t i = 0; i < indexCount; i += 3)
        {
            GL_ASSERT(glDrawElements(GL_LINE_LOOP, 3, part->getIndexFormat(), ((const GLvoid*)(i * indexSize))));
        }
    }
    return true;

    case Mesh::TRIANGLE_STRIP:
    {
        for (size_t i = 2; i < indexCount; ++i)
        {
            GL_ASSERT(glDrawElements(GL_LINE_LOOP, 3, part->getIndexFormat(), ((const GLvoid*)((i - 2) * indexSize))));
        }
    }
    return true;

    default:
        // not supported
        return false;
    }
}

void GLRenderer::renderMesh(Mesh* mesh, Material* _material,
                                        unsigned int _partCount,
                                        Material** _partMaterials, RenderView* view, Node* node)
{
    unsigned int partCount = mesh->getPartCount();
    if (partCount == 0)
    {
        // No mesh parts (index buffers).
        for (Material* material = _material; material != NULL; material = material->getNextPass())
        {
            material->bind(view, node);
            if (!mesh->_vertexAttributeArray) {
                mesh->_vertexAttributeArray = VertexAttributeBinding::create(mesh, material->getEffect());
            }
            bindVertexAttributeObj(mesh->_vertexAttributeArray);

            GL_ASSERT(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
            if (!view->wireframe || !drawWireframe(mesh))
            {
                GL_ASSERT(glDrawArrays(mesh->getPrimitiveType(), 0, mesh->getVertexCount()));
            }

            unbindVertexAttributeObj(mesh->_vertexAttributeArray);
            material->unbind();
        }
        return;
    }

    for (unsigned int i = 0; i < partCount; ++i)
    {
        MeshPart* part = mesh->getPart(i);
        GP_ASSERT(part);

        // Get the material for this mesh part.
        Material* material = NULL;
        if (i < _partCount && _partMaterials) material = _partMaterials[i];
        else if (_material) material = _material;

        for (; material != NULL; material = material->getNextPass())
        {
            material->bind(view, node);
            if (!mesh->_vertexAttributeArray) {
                mesh->_vertexAttributeArray = VertexAttributeBinding::create(mesh, material->getEffect());
            }
            bindVertexAttributeObj(mesh->_vertexAttributeArray);

            GL_ASSERT(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, part->_indexBuffer));
            if (!view->wireframe || !drawWireframe(part))
            {
                GL_ASSERT(glDrawElements(part->getPrimitiveType(), part->getIndexCount(), part->getIndexFormat(), 0));
            }

            unbindVertexAttributeObj(mesh->_vertexAttributeArray);
            material->unbind();
        }
    }
}

void GLRenderer::deleteMesh(Mesh* mesh) {
    if (mesh->_vertexBuffer)
    {
        glDeleteBuffers(1, &mesh->_vertexBuffer);
        mesh->_vertexBuffer = 0;

        unsigned int partCount = mesh->getPartCount();
        for (unsigned int i = 0; i < partCount; ++i)
        {
            MeshPart* part = mesh->getPart(i);
            deleteMeshPart(part);
        }
    }
    if (mesh->_vertexAttributeArray) {
        deleteVertexAttributeObj(mesh->_vertexAttributeArray);
        SAFE_DELETE(mesh->_vertexAttributeArray);
        mesh->_vertexAttributeArray = NULL;
    }
}

void GLRenderer::updateMeshPart(MeshPart* part, unsigned int indexStart, unsigned int indexCount) {
    if (part->_indexBuffer == 0) {
        // Create a VBO for our index buffer.
        GLuint vbo;
        GL_ASSERT(glGenBuffers(1, &vbo));
        GL_ASSERT(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo));

        unsigned int indexSize = 0;
        switch (part->getIndexFormat())
        {
        case Mesh::INDEX8:
            indexSize = 1;
            break;
        case Mesh::INDEX16:
            indexSize = 2;
            break;
        case Mesh::INDEX32:
            indexSize = 4;
            break;
        default:
            GP_ERROR("Unsupported index format (%d).", part->getIndexFormat());
            GLuint vbo = part->_indexBuffer;
            glDeleteBuffers(1, &vbo);
            part->_indexBuffer = 0;
            return;
        }

        GL_ASSERT(glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexSize * indexCount, NULL, part->isDynamic() ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW));
        part->_indexBuffer = vbo;
    }

    unsigned int indexSize = 0;
    switch (part->getIndexFormat())
    {
    case Mesh::INDEX8:
        indexSize = 1;
        break;
    case Mesh::INDEX16:
        indexSize = 2;
        break;
    case Mesh::INDEX32:
        indexSize = 4;
        break;
    default:
        GP_ERROR("Unsupported index format (%d).", part->getIndexFormat());
        GLuint vbo = part->_indexBuffer;
        glDeleteBuffers(1, &vbo);
        part->_indexBuffer = 0;
        return;
    }

    GL_ASSERT(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, part->_indexBuffer));

    if (indexStart == 0 && indexCount == 0)
    {
        GL_ASSERT(glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexSize * part->getIndexCount(), part->_indexData, part->isDynamic() ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW));
    }
    else
    {
        if (indexCount == 0)
        {
            indexCount = part->getIndexCount() - indexStart;
        }

        GL_ASSERT(glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, indexStart * indexSize, indexCount * indexSize, part->_indexData));
    }
}

void GLRenderer::deleteMeshPart(MeshPart* part) {
    if (part->_indexBuffer)
    {
        glDeleteBuffers(1, &part->_indexBuffer);
        part->_indexBuffer = 0;
    }
}

void GLRenderer::renderMeshBatch(MeshBatch* mbatch, RenderView* view, Node* node) {
    if (mbatch->_vertexCount == 0 || (mbatch->_indexed && mbatch->_indexCount == 0))
        return; // nothing to draw

    // Not using VBOs, so unbind the element array buffer.
    // ARRAY_BUFFER will be unbound automatically during pass->bind().
    GL_ASSERT(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));

    GP_ASSERT(mbatch->_material);
    if (mbatch->_indexed)
        GP_ASSERT(mbatch->_indices);

    // Bind the material.
    for (Material* material = mbatch->_material; material != NULL; material = material->getNextPass())
    {
        material->bind(view, node);

        if (!mbatch->_vertexAttributeArray || mbatch->_vertexAttributeArray->_isDirty) {
            if (mbatch->_vertexAttributeArray) {
                deleteVertexAttributeObj(mbatch->_vertexAttributeArray);
                SAFE_DELETE(mbatch->_vertexAttributeArray);
            }
            mbatch->_vertexAttributeArray = VertexAttributeBinding::create(mbatch->_vertexFormat, mbatch->_vertices, material->getEffect());
        }
        bindVertexAttributeObj(mbatch->_vertexAttributeArray);

        if (mbatch->_indexed)
        {
            GL_ASSERT(glDrawElements(mbatch->_primitiveType, mbatch->_indexCount, GL_UNSIGNED_SHORT, (GLvoid*)mbatch->_indices));
        }
        else
        {
            GL_ASSERT(glDrawArrays(mbatch->_primitiveType, 0, mbatch->_vertexCount));
        }

        unbindVertexAttributeObj(mbatch->_vertexAttributeArray);
        material->unbind();
    }
}

void GLRenderer::deleteMeshBatch(MeshBatch* mesh) {
    if (mesh->_vertexAttributeArray) {
        deleteVertexAttributeObj(mesh->_vertexAttributeArray);
        SAFE_DELETE(mesh->_vertexAttributeArray);
        mesh->_vertexAttributeArray = NULL;
    }
}

void GLRenderer::enableDepthWrite()
{
    StateBlock* _defaultState = &stateBlock;

    // Internal method used by Game::clear() to restore depth writing before a
    // clear operation. This is necessary if the last code to draw before the
    // next frame leaves depth writing disabled.
    if (!_defaultState->_depthWriteEnabled)
    {
        GL_ASSERT(glDepthMask(GL_TRUE));
        _defaultState->_depthWriteEnabled = true;
    }
}

void GLRenderer::updateState(StateBlock* state, int force) {
    StateBlock* _defaultState = &stateBlock;

    // Update any state that differs from _defaultState and flip _defaultState bits
    if (force == 2 || ((force || (state->_bits & RS_BLEND)) && (state->_blendEnabled != _defaultState->_blendEnabled)))
    {
        if (state->_blendEnabled)
            GL_ASSERT(glEnable(GL_BLEND));
        else
            GL_ASSERT(glDisable(GL_BLEND));
        _defaultState->_blendEnabled = state->_blendEnabled;
    }
    if (force == 2 || ((force || (state->_bits & RS_BLEND_FUNC)) && (state->_blendSrc != _defaultState->_blendSrc || state->_blendDst != _defaultState->_blendDst)))
    {
        GL_ASSERT(glBlendFunc((GLenum)state->_blendSrc, (GLenum)state->_blendDst));
        _defaultState->_blendSrc = state->_blendSrc;
        _defaultState->_blendDst = state->_blendDst;
    }
    if (force == 2 || ((force || (state->_bits & RS_CULL_FACE)) && (state->_cullFaceEnabled != _defaultState->_cullFaceEnabled)))
    {
        if (state->_cullFaceEnabled)
            GL_ASSERT(glEnable(GL_CULL_FACE));
        else
            GL_ASSERT(glDisable(GL_CULL_FACE));
        _defaultState->_cullFaceEnabled = state->_cullFaceEnabled;
    }
    if (force == 2 || ((force || (state->_bits & RS_CULL_FACE_SIDE)) && (state->_cullFaceSide != _defaultState->_cullFaceSide)))
    {
        GL_ASSERT(glCullFace((GLenum)state->_cullFaceSide));
        _defaultState->_cullFaceSide = state->_cullFaceSide;
    }
    if (force == 2 || ((force || (state->_bits & RS_FRONT_FACE)) && (state->_frontFace != _defaultState->_frontFace)))
    {
        GL_ASSERT(glFrontFace((GLenum)state->_frontFace));
        _defaultState->_frontFace = state->_frontFace;
    }
    if (force == 2 || ((force || (state->_bits & RS_DEPTH_TEST)) && (state->_depthTestEnabled != _defaultState->_depthTestEnabled)))
    {
        if (state->_depthTestEnabled)
            GL_ASSERT(glEnable(GL_DEPTH_TEST));
        else
            GL_ASSERT(glDisable(GL_DEPTH_TEST));
        _defaultState->_depthTestEnabled = state->_depthTestEnabled;
    }
    if (force == 2 || ((force || (state->_bits & RS_DEPTH_WRITE)) && (state->_depthWriteEnabled != _defaultState->_depthWriteEnabled)))
    {
        GL_ASSERT(glDepthMask(state->_depthWriteEnabled ? GL_TRUE : GL_FALSE));
        _defaultState->_depthWriteEnabled = state->_depthWriteEnabled;
    }
    if (force == 2 || ((force || (state->_bits & RS_DEPTH_FUNC)) && (state->_depthFunction != _defaultState->_depthFunction)))
    {
        GL_ASSERT(glDepthFunc((GLenum)state->_depthFunction));
        _defaultState->_depthFunction = state->_depthFunction;
    }
    if (force == 2 || ((force || (state->_bits & RS_STENCIL_TEST)) && (state->_stencilTestEnabled != _defaultState->_stencilTestEnabled)))
    {
        if (state->_stencilTestEnabled)
            GL_ASSERT(glEnable(GL_STENCIL_TEST));
        else
            GL_ASSERT(glDisable(GL_STENCIL_TEST));
        _defaultState->_stencilTestEnabled = state->_stencilTestEnabled;
    }
    if (force == 2 || ((force || (state->_bits & RS_STENCIL_WRITE)) && (state->_stencilWrite != _defaultState->_stencilWrite)))
    {
        GL_ASSERT(glStencilMask(state->_stencilWrite));
        _defaultState->_stencilWrite = state->_stencilWrite;
    }
    if (force == 2 || ((force || (state->_bits & RS_STENCIL_FUNC)) && (state->_stencilFunction != _defaultState->_stencilFunction ||
        state->_stencilFunctionRef != _defaultState->_stencilFunctionRef ||
        state->_stencilFunctionMask != _defaultState->_stencilFunctionMask)))
    {
        GL_ASSERT(glStencilFunc((GLenum)state->_stencilFunction, state->_stencilFunctionRef, state->_stencilFunctionMask));
        _defaultState->_stencilFunction = state->_stencilFunction;
        _defaultState->_stencilFunctionRef = state->_stencilFunctionRef;
        _defaultState->_stencilFunctionMask = state->_stencilFunctionMask;
    }
    if (force == 2 || ((force || (state->_bits & RS_STENCIL_OP)) && (state->_stencilOpSfail != _defaultState->_stencilOpSfail ||
        state->_stencilOpDpfail != _defaultState->_stencilOpDpfail ||
        state->_stencilOpDppass != _defaultState->_stencilOpDppass)))
    {
        GL_ASSERT(glStencilOp((GLenum)state->_stencilOpSfail, (GLenum)state->_stencilOpDpfail, (GLenum)state->_stencilOpDppass));
        _defaultState->_stencilOpSfail = state->_stencilOpSfail;
        _defaultState->_stencilOpDpfail = state->_stencilOpDpfail;
        _defaultState->_stencilOpDppass = state->_stencilOpDppass;
    }
}


GLint getFormatInternal(Texture::Format format)
{
    switch (format)
    {
    case Texture::RGB888:
    case Texture::RGB565:
        return GL_RGB;
    case Texture::RGBA8888:
    case Texture::RGBA4444:
    case Texture::RGBA5551:
        return GL_RGBA;
    case Texture::ALPHA:
        return GL_ALPHA;
    case Texture::DEPTH:
#if !defined(OPENGL_ES) || defined(GL_ES_VERSION_3_0)
        return GL_DEPTH_COMPONENT32F;
#else
        return GL_DEPTH_COMPONENT;
#endif
    default:
        return 0;
    }
}

GLenum getFormatTexel(Texture::Format format)
{
    switch (format)
    {
    case Texture::RGB888:
    case Texture::RGBA8888:
    case Texture::ALPHA:
        return GL_UNSIGNED_BYTE;
    case Texture::RGB565:
        return GL_UNSIGNED_SHORT_5_6_5;
    case Texture::RGBA4444:
        return GL_UNSIGNED_SHORT_4_4_4_4;
    case Texture::RGBA5551:
        return GL_UNSIGNED_SHORT_5_5_5_1;
    case Texture::DEPTH:
#if !defined(OPENGL_ES) || defined(GL_ES_VERSION_3_0)
        return GL_FLOAT;
#else
        return GL_UNSIGNED_INT;
#endif
    default:
        return 0;
    }
}

size_t getFormatBPP(Texture::Format format)
{
    switch (format)
    {
    case Texture::RGB565:
    case Texture::RGBA4444:
    case Texture::RGBA5551:
        return 2;
    case Texture::RGB888:
        return 3;
    case Texture::RGBA8888:
        return 4;
    case Texture::ALPHA:
        return 1;
    default:
        return 0;
    }
}

void GLRenderer::updateTexture(Texture* texture) {
    Texture::Format format = texture->getFormat();
    Texture::Type type = texture->getType();
    GP_ASSERT(type == Texture::TEXTURE_2D || type == Texture::TEXTURE_CUBE);

    GLenum target = (GLenum)type;

    GLint internalFormat = getFormatInternal(format);
    GP_ASSERT(internalFormat != 0);

    GLenum texelType = getFormatTexel(format);
    GP_ASSERT(texelType != 0);

    // Create the texture.
    if (texture->_handle == 0) {
        GLuint textureId;
        GL_ASSERT(glGenTextures(1, &textureId));
        texture->_handle = textureId;
        GL_ASSERT(glBindTexture(target, textureId));
        GL_ASSERT(glPixelStorei(GL_UNPACK_ALIGNMENT, 1));
#ifndef OPENGL_ES
        // glGenerateMipmap is new in OpenGL 3.0. For OpenGL 2.0 we must fallback to use glTexParameteri
        // with GL_GENERATE_MIPMAP prior to actual texture creation (glTexImage2D)
        if (texture->isMipmapped() && !std::addressof(glGenerateMipmap))
            GL_ASSERT(glTexParameteri(target, GL_GENERATE_MIPMAP, GL_TRUE));
#endif
    }

    unsigned int width = texture->getWidth();
    unsigned int height = texture->getHeight();

    GLuint textureId = texture->_handle;
    GL_ASSERT(glBindTexture(target, textureId));

    // Load the texture
    size_t bpp = getFormatBPP(format);
    if (type == Texture::TEXTURE_2D)
    {
        GLenum f = (format == Texture::DEPTH) ? GL_DEPTH_COMPONENT : internalFormat;
        GL_ASSERT(glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, f, texelType, texture->_data));
    }
    else
    {
        // Get texture size
        unsigned int textureSize = width * height;
        if (bpp == 0)
        {
            glDeleteTextures(1, &textureId);
            GP_ERROR("Failed to determine texture size because format is UNKNOWN.");
            texture->_handle = 0;
            return;
        }
        textureSize *= bpp;
        // Texture Cube
        for (unsigned int i = 0; i < 6; i++)
        {
            const unsigned char* texturePtr = (texture->_data == NULL) ? NULL : &texture->_data[i * textureSize];
            GL_ASSERT(glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, internalFormat, width, height, 0, internalFormat, texelType, texturePtr));
        }
    }

    // Set initial minification filter based on whether or not mipmaping was enabled.
    if (texture->_minFilter == Texture::NEAREST)
    {
        GL_ASSERT(glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
        GL_ASSERT(glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
        GL_ASSERT(glTexParameteri(target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
        GL_ASSERT(glTexParameteri(target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
#if !defined(OPENGL_ES) || defined(GL_ES_VERSION_3_0) && GL_ES_VERSION_3_0
        GL_ASSERT(glTexParameteri(target, GL_TEXTURE_COMPARE_MODE, GL_NONE));
#endif    	
    }
    else
    {
        GL_ASSERT(glTexParameteri(target, GL_TEXTURE_MIN_FILTER, texture->_minFilter));
    }

    if (texture->isMipmapped()) {
        GL_ASSERT(glHint(GL_GENERATE_MIPMAP_HINT, GL_NICEST));
        if (std::addressof(glGenerateMipmap))
            GL_ASSERT(glGenerateMipmap(target));
    }
}

void GLRenderer::deleteTexture(Texture* texture) {
    if (texture->_handle)
    {
        GL_ASSERT(glDeleteTextures(1, &texture->_handle));
        texture->_handle = 0;
    }
}

void GLRenderer::bindTextureSampler(Texture::Sampler* sampler) {
    Texture* _texture = sampler->getTexture();
    GP_ASSERT(_texture);

    Texture::Type type = _texture->getType();
    GLenum target = (GLenum)type;

    GLuint textureId = _texture->_handle;
    GL_ASSERT(glBindTexture(target, textureId));


    GL_ASSERT(glTexParameteri(target, GL_TEXTURE_MIN_FILTER, (GLenum)sampler->_minFilter));
    GL_ASSERT(glTexParameteri(target, GL_TEXTURE_MAG_FILTER, (GLenum)sampler->_magFilter));
    GL_ASSERT(glTexParameteri(target, GL_TEXTURE_WRAP_S, (GLenum)sampler->_wrapS));
    GL_ASSERT(glTexParameteri(target, GL_TEXTURE_WRAP_T, (GLenum)sampler->_wrapT));
#if defined(GL_TEXTURE_WRAP_R) // OpenGL ES 3.x and up, OpenGL 1.2 and up
    if (target == GL_TEXTURE_CUBE_MAP) // We don't want to run this on something that we know will fail
        GL_ASSERT(glTexParameteri(target, GL_TEXTURE_WRAP_R, (GLenum)sampler->_wrapR));
#endif
}

ShaderProgram* GLRenderer::createProgram(ProgramSrc* src) {
    const char* defines = src->defines;
    const char* vshSource = src->vshSource;
    const char* fshSource = src->fshSource;

    GP_ASSERT(vshSource);
    GP_ASSERT(fshSource);

    const unsigned int SHADER_SOURCE_LENGTH = 3;
    const GLchar* shaderSource[SHADER_SOURCE_LENGTH];
    char* infoLog = NULL;
    GLuint vertexShader;
    GLuint fragmentShader;
    GLuint program;
    GLint length;
    GLint success;


    shaderSource[0] = defines;
    shaderSource[1] = "\n";
    //std::string vshSourceStr = "";
    shaderSource[2] = vshSource;
    GL_ASSERT(vertexShader = glCreateShader(GL_VERTEX_SHADER));
    GL_ASSERT(glShaderSource(vertexShader, SHADER_SOURCE_LENGTH, shaderSource, NULL));
    GL_ASSERT(glCompileShader(vertexShader));
    GL_ASSERT(glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success));
    if (success != GL_TRUE)
    {
        GL_ASSERT(glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &length));
        if (length == 0)
        {
            length = 4096;
        }
        if (length > 0)
        {
            infoLog = new char[length];
            GL_ASSERT(glGetShaderInfoLog(vertexShader, length, NULL, infoLog));
            infoLog[length - 1] = '\0';
        }

        // Write out the expanded shader file.
        //if (vshPath)
        //    writeShaderToErrorFile(vshPath, shaderSource[2]);

        GP_ERROR("Compile failed for vertex shader '%s' with error '%s'.", vshSource, infoLog == NULL ? "" : infoLog);
        SAFE_DELETE_ARRAY(infoLog);

        // Clean up.
        GL_ASSERT(glDeleteShader(vertexShader));

        return NULL;
    }

    shaderSource[2] = fshSource;
    GL_ASSERT(fragmentShader = glCreateShader(GL_FRAGMENT_SHADER));
    GL_ASSERT(glShaderSource(fragmentShader, SHADER_SOURCE_LENGTH, shaderSource, NULL));
    GL_ASSERT(glCompileShader(fragmentShader));
    GL_ASSERT(glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success));
    if (success != GL_TRUE)
    {
        GL_ASSERT(glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &length));
        if (length == 0)
        {
            length = 4096;
        }
        if (length > 0)
        {
            infoLog = new char[length];
            GL_ASSERT(glGetShaderInfoLog(fragmentShader, length, NULL, infoLog));
            infoLog[length - 1] = '\0';
        }

        // Write out the expanded shader file.
        //if (fshPath)
        //    writeShaderToErrorFile(fshPath, shaderSource[2]);

        GP_ERROR("Compile failed for fragment shader (%s): %s", fshSource, infoLog == NULL ? "" : infoLog);
        SAFE_DELETE_ARRAY(infoLog);

        // Clean up.
        GL_ASSERT(glDeleteShader(vertexShader));
        GL_ASSERT(glDeleteShader(fragmentShader));

        return NULL;
    }

    // Link program.
    GL_ASSERT(program = glCreateProgram());
    GL_ASSERT(glAttachShader(program, vertexShader));
    GL_ASSERT(glAttachShader(program, fragmentShader));
    GL_ASSERT(glLinkProgram(program));
    GL_ASSERT(glGetProgramiv(program, GL_LINK_STATUS, &success));

    // Delete shaders after linking.
    GL_ASSERT(glDeleteShader(vertexShader));
    GL_ASSERT(glDeleteShader(fragmentShader));

    // Check link status.
    if (success != GL_TRUE)
    {
        GL_ASSERT(glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length));
        if (length == 0)
        {
            length = 4096;
        }
        if (length > 0)
        {
            infoLog = new char[length];
            GL_ASSERT(glGetProgramInfoLog(program, length, NULL, infoLog));
            infoLog[length - 1] = '\0';
        }
        GP_ERROR("Linking program failed (%s,%s): %s", "NULL", "NULL", infoLog == NULL ? "" : infoLog);
        SAFE_DELETE_ARRAY(infoLog);

        // Clean up.
        GL_ASSERT(glDeleteProgram(program));

        return NULL;
    }

    // Create and return the new ShaderProgram.
    ShaderProgram* effect = new ShaderProgram();
    effect->_program = program;

    // Query and store vertex attribute meta-data from the program.
    // NOTE: Rather than using glBindAttribLocation to explicitly specify our own
    // preferred attribute locations, we're going to query the locations that were
    // automatically bound by the GPU. While it can sometimes be convenient to use
    // glBindAttribLocation, some vendors actually reserve certain attribute indices
    // and therefore using this function can create compatibility issues between
    // different hardware vendors.
    GLint activeAttributes;
    GL_ASSERT(glGetProgramiv(program, GL_ACTIVE_ATTRIBUTES, &activeAttributes));
    if (activeAttributes > 0)
    {
        GL_ASSERT(glGetProgramiv(program, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &length));
        if (length > 0)
        {
            GLchar* attribName = new GLchar[length + 1];
            GLint attribSize;
            GLenum attribType;
            GLint attribLocation;
            for (int i = 0; i < activeAttributes; ++i)
            {
                // Query attribute info.
                GL_ASSERT(glGetActiveAttrib(program, i, length, NULL, &attribSize, &attribType, attribName));
                attribName[length] = '\0';

                // Query the pre-assigned attribute location.
                GL_ASSERT(attribLocation = glGetAttribLocation(program, attribName));

                // Assign the vertex attribute mapping for the effect.
                effect->_vertexAttributes[attribName] = attribLocation;
            }
            SAFE_DELETE_ARRAY(attribName);
        }
    }

    // Query and store uniforms from the program.
    GLint activeUniforms;
    GL_ASSERT(glGetProgramiv(program, GL_ACTIVE_UNIFORMS, &activeUniforms));
    if (activeUniforms > 0)
    {
        GL_ASSERT(glGetProgramiv(program, GL_ACTIVE_UNIFORM_MAX_LENGTH, &length));
        if (length > 0)
        {
            GLchar* uniformName = new GLchar[length + 1];
            GLint uniformSize;
            GLenum uniformType;
            GLint uniformLocation;
            unsigned int samplerIndex = 0;
            for (int i = 0; i < activeUniforms; ++i)
            {
                // Query uniform info.
                GL_ASSERT(glGetActiveUniform(program, i, length, NULL, &uniformSize, &uniformType, uniformName));
                uniformName[length] = '\0';  // null terminate
                if (length > 3)
                {
                    // If this is an array uniform, strip array indexers off it since GL does not
                    // seem to be consistent across different drivers/implementations in how it returns
                    // array uniforms. On some systems it will return "u_matrixArray", while on others
                    // it will return "u_matrixArray[0]".
                    char* c = strrchr(uniformName, '[');
                    if (c)
                    {
                        *c = '\0';
                    }
                }

                // Query the pre-assigned uniform location.
                GL_ASSERT(uniformLocation = glGetUniformLocation(program, uniformName));

                Uniform* uniform = new Uniform();
                uniform->_effect = effect;
                uniform->_name = uniformName;
                uniform->_location = uniformLocation;
                uniform->_type = uniformType;
                if (uniformType == GL_SAMPLER_2D || uniformType == GL_SAMPLER_CUBE)
                {
                    uniform->_index = samplerIndex;
                    samplerIndex += uniformSize;
                }
                else
                {
                    uniform->_index = 0;
                }

                effect->_uniforms[uniformName] = uniform;
            }
            SAFE_DELETE_ARRAY(uniformName);
        }
    }

    return effect;
}

void GLRenderer::deleteProgram(ShaderProgram* effect) {
    if (effect->_program)
    {
        // If our program object is currently bound, unbind it before we're destroyed.
        if (__currentEffect == effect)
        {
            GL_ASSERT(glUseProgram(0));
            __currentEffect = NULL;
        }

        GL_ASSERT(glDeleteProgram(effect->_program));
        effect->_program = 0;
    }
}
void GLRenderer::bindProgram(ShaderProgram* effect) {
    GL_ASSERT(glUseProgram(effect->_program));

    __currentEffect = effect;
}
void GLRenderer::bindUniform(MaterialParameter* value, Uniform* uniform, ShaderProgram* effect) {
    GP_ASSERT(uniform);
    GP_ASSERT(value);
    GP_ASSERT(effect);

    if (value->_methodBinding) {
        value->_methodBinding->setValue(effect);
    }

    switch (value->_type)
    {
    case MaterialParameter::FLOAT:
        GL_ASSERT(glUniform1f(uniform->_location, value->_value.floatValue));
        break;
    case MaterialParameter::FLOAT_ARRAY:
        GP_ASSERT(value->_value.floatPtrValue);
        GL_ASSERT(glUniform1fv(uniform->_location, value->_count, value->_value.floatPtrValue));
        break;
    case MaterialParameter::INT:
        GL_ASSERT(glUniform1i(uniform->_location, value->_value.intValue));
        break;
    case MaterialParameter::INT_ARRAY:
        GL_ASSERT(glUniform1iv(uniform->_location, value->_count, value->_value.intPtrValue));
        break;
    case MaterialParameter::VECTOR2: {
        Vector2* values2 = reinterpret_cast<Vector2*>(value->_value.floatPtrValue);
        GP_ASSERT(values2);
        GL_ASSERT(glUniform2fv(uniform->_location, value->_count, (GLfloat*)values2));
        break;
    }
    case MaterialParameter::VECTOR3: {
        Vector3* values3 = reinterpret_cast<Vector3*>(value->_value.floatPtrValue);
        GP_ASSERT(values3);
        GL_ASSERT(glUniform3fv(uniform->_location, value->_count, (GLfloat*)values3));
        break;
    }
    case MaterialParameter::VECTOR4: {
        Vector4* values4 = reinterpret_cast<Vector4*>(value->_value.floatPtrValue);
        GP_ASSERT(values4);
        GL_ASSERT(glUniform4fv(uniform->_location, value->_count, (GLfloat*)values4));
        break;
    }
    case MaterialParameter::MATRIX: {
        //GL_ASSERT(glUniformMatrix4fv(uniform->_location, 1, GL_FALSE, value.m));
        Matrix* valuesm = reinterpret_cast<Matrix*>(value->_value.floatPtrValue);
        GP_ASSERT(valuesm);
        GL_ASSERT(glUniformMatrix4fv(uniform->_location, value->_count, GL_FALSE, (GLfloat*)valuesm));
        break;
    }
    case MaterialParameter::SAMPLER: {
        const Texture::Sampler* sampler = value->_value.samplerValue;
        GP_ASSERT(uniform->_type == GL_SAMPLER_2D || uniform->_type == GL_SAMPLER_CUBE);
        GP_ASSERT(sampler);
        GP_ASSERT((sampler->getTexture()->getType() == Texture::TEXTURE_2D && uniform->_type == GL_SAMPLER_2D) ||
            (sampler->getTexture()->getType() == Texture::TEXTURE_CUBE && uniform->_type == GL_SAMPLER_CUBE));

        GL_ASSERT(glActiveTexture(GL_TEXTURE0 + uniform->_index));

        // Bind the sampler - this binds the texture and applies sampler state
        const_cast<Texture::Sampler*>(sampler)->bind();

        GL_ASSERT(glUniform1i(uniform->_location, uniform->_index));
        break;
    }
    case MaterialParameter::SAMPLER_ARRAY: {
        const Texture::Sampler** values = value->_value.samplerArrayValue;
        GP_ASSERT(uniform->_type == GL_SAMPLER_2D || uniform->_type == GL_SAMPLER_CUBE);
        GP_ASSERT(values);

        // Set samplers as active and load texture unit array
        GLint units[32];
        for (unsigned int i = 0; i < value->_count; ++i)
        {
            GP_ASSERT((const_cast<Texture::Sampler*>(values[i])->getTexture()->getType() == Texture::TEXTURE_2D && uniform->_type == GL_SAMPLER_2D) ||
                (const_cast<Texture::Sampler*>(values[i])->getTexture()->getType() == Texture::TEXTURE_CUBE && uniform->_type == GL_SAMPLER_CUBE));
            GL_ASSERT(glActiveTexture(GL_TEXTURE0 + uniform->_index + i));

            // Bind the sampler - this binds the texture and applies sampler state
            const_cast<Texture::Sampler*>(values[i])->bind();

            units[i] = uniform->_index + i;
        }

        // Pass texture unit array to GL
        GL_ASSERT(glUniform1iv(uniform->_location, value->_count, units));
        break;
        //case MaterialParameter::METHOD:
            //if (_value.method)
            //    _value.method->setValue(effect);
            //break;
    }
    default:
    {
        if ((value->_loggerDirtyBits & MaterialParameter::PARAMETER_VALUE_NOT_SET) == 0)
        {
            GP_WARN("Material parameter value not set for: '%s' in effect: '%s'.", value->_name.c_str(), effect->getId());
            value->_loggerDirtyBits |= MaterialParameter::PARAMETER_VALUE_NOT_SET;
        }
        break;
    }
    }
}


void GLRenderer::bindVertexAttributeObj(VertexAttributeBinding* vertextAttribute) {

    // Create a new VertexAttributeBinding.
    VertexAttributeBinding* b = vertextAttribute;

    bool needInitVAO = false;

#ifdef GP_USE_VAO
    if (b->_handle == 0 && vertextAttribute->_mesh && glGenVertexArrays)
    {
        GL_ASSERT(glBindBuffer(GL_ARRAY_BUFFER, 0));
        GL_ASSERT(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));

        // Use hardware VAOs.
        GL_ASSERT(glGenVertexArrays(1, &b->_handle));

        if (b->_handle == 0)
        {
            GP_ERROR("Failed to create VAO handle.");
            //SAFE_DELETE(b);
            return;
        }

        // Bind the new VAO.
        GL_ASSERT(glBindVertexArray(b->_handle));

        // Bind the Mesh VBO so our glVertexAttribPointer calls use it.
        GL_ASSERT(glBindBuffer(GL_ARRAY_BUFFER, vertextAttribute->_mesh->getVertexBuffer()));

        needInitVAO = true;
    }
#endif


    if (b->_handle)
    {
        // Hardware mode
        GL_ASSERT(glBindVertexArray(b->_handle));
    }
    else
    {
        // Software mode
        if (b->_mesh)
        {
            GL_ASSERT(glBindBuffer(GL_ARRAY_BUFFER, b->_mesh->getVertexBuffer()));
        }
        else
        {
            GL_ASSERT(glBindBuffer(GL_ARRAY_BUFFER, 0));
        }
    }

    if (b->_handle == 0 || needInitVAO) {
        // Call setVertexAttribPointer for each vertex element.
        for (size_t i = 0, count = vertextAttribute->_attributes.size(); i < count; ++i)
        {
            VertexAttributeBinding::VertexAttribute& attribute = vertextAttribute->_attributes[i];

            if (attribute.location == -1)
            {
                //GP_WARN("Warning: Vertex element with usage '%s' in mesh '%s' does not correspond to an attribute in effect '%s'.", VertexFormat::toString(e.usage), mesh->getUrl(), effect->getId());
            }
            else
            {
                void* pointer = attribute.pointer;
                GL_ASSERT(glVertexAttribPointer(attribute.location, (GLint)attribute.size, GL_FLOAT, GL_FALSE, (GLsizei)attribute.stride, pointer));
                GL_ASSERT(glEnableVertexAttribArray(attribute.location));
            }
        }
    }

}

void GLRenderer::unbindVertexAttributeObj(VertexAttributeBinding* vertextAttribute) {
    if (vertextAttribute->_handle)
    {
        // Hardware mode
        GL_ASSERT(glBindVertexArray(0));
    }
    else
    {
        // Software mode
        if (vertextAttribute->_mesh)
        {
            GL_ASSERT(glBindBuffer(GL_ARRAY_BUFFER, 0));
        }

        for (size_t i = 0, count = vertextAttribute->_attributes.size(); i < count; ++i)
        {
            VertexAttributeBinding::VertexAttribute& attribute = vertextAttribute->_attributes[i];
            GL_ASSERT(glDisableVertexAttribArray(attribute.location));
        }
    }
}

void GLRenderer::deleteVertexAttributeObj(VertexAttributeBinding* vertextAttribute) {
    if (vertextAttribute->_handle)
    {
        GL_ASSERT(glDeleteVertexArrays(1, &vertextAttribute->_handle));
        vertextAttribute->_handle = 0;
    }
}

FrameBuffer* GLRenderer::createFrameBuffer(const char* id, unsigned int width, unsigned int height, Texture::Format format) {
    return GLFrameBuffer::create(id, width, height, format);
}
FrameBuffer* GLRenderer::bindDefaultFrameBuffer(unsigned int type) {
    return GLFrameBuffer::bindDefault(type);
}
FrameBuffer* GLRenderer::getCurrentFrameBuffer() {
    return GLFrameBuffer::getCurrent();
}
FrameBuffer* GLRenderer::getFrameBuffer(const char* id) {
    return GLFrameBuffer::getFrameBuffer(id);
}
