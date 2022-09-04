The gameplay 3-D framework uses a modern GPU shader based rendering architecture and uses OpenGL 2.0+ (desktop) or OpenGL ES 2.0 (mobile) along with the OpenGL Shading Language (GLSL). Currently, all the code in graphics-related classes uses the OpenGL hardware device directly.

## Using materials

The `gameplay::Material` class is the high level definition of all the rendering information needed to draw a `gameplay::MeshPart`. When you draw a `gameplay::Model`, the mesh's vertex buffer is applied and for each `gameplay::MeshPart` its index buffer(s) and `gameplay::Materials` are applied just before the primitives are drawn.

## RenderState and Effects

Each `gameplay::Material` consists of a `gameplay::RenderState` and a `gameplay::Effect`. The `gameplay::RenderState` stores the GPU render state blocks that are to be applied, as well as any `gameplay::MaterialParameters` to be applied to the `gameplay::Effect`. While a `gameplay::Material` is typically used once per `gameplay::MeshPart`, the `gameplay::Effect` is created internally based on the unique combination of selected vertex and fragment shader programs. The `gameplay::Effect` represents a common reusable shader program.

## Techniques

Since you can bind only one `gameplay::Material` per `gameplay::MeshPart`, an additional feature is supported that's designed to make it quick and easy to change the way you render the parts at runtime. 
You can define multiple techniques by giving them different names. 
Each one can have a completely different rendering technique, and you can even change the technique being applied at runtime by using `Material::setTechnique(const char* name)`. 
When a material is loaded, all the techniques are loaded ahead too. 
This is a practical way of handling different light combinations or having lower-quality rendering techniques, such as disabling bump mapping, when the object being rendered is far away from the camera.

## Creating materials

You can create a `gameplay::Material` from the simple `gameplay::Properties` based .material files. Using this simple file format, you can define your material, specifying all the rendering techniques and pass information.

 

Here is an example of loading a .material file:

 
```c++
Material* planeMaterial = planeNode->getModel()->setMaterial("res/floor.material");
```

## Setting vs. binding material parameters

Once you have created a `gameplay::Material` instance, you'll want to get its parameters and then set or bind various values to them. To set a value, get the `gameplay::MaterialParameter` and then call the appropriate setValue() method on it. Setting material parameter values is most common in parameters that are based on values that are constants.

Here is an example of setting a value on a parameter:


```c++
material->getParameter("u_diffuseColor")->setValue(Vector4(0.53544f, 0.53544f, 0.53544f, 1.0f));
```

For values that are not constants and are determined from other objects, you'll want to bind a value to it. When binding a value, you are giving the parameter a function pointer that will only be resolved just prior to rendering. In this example, we will bind the forward vector for a node (in view space).


Here is an example of binding a value on a parameter:

 
```c++
material->getParameter("u_lightDirection")->bindValue(lightNode, &Node::getForwardVectorView);
```

## .material files

As you can see in the following .material file, we have one Material, one Technique and one Pass. 
The main parts of this material definition are the shaders, uniforms, samplers and renderState. You will see certain upper case values throughout the file. 
These represent constant enumeration values and can usually be found in the `gameplay::RenderState` or `gameplay::Texture` class definitions:

```
material duck
{
    technique
    { 
        pass 0
        {
            // shaders
            vertexShader = res/shaders/textured.vert
            fragmentShader = res/shaders/textured.frag
            defines = SPECULAR
           
            // uniforms
            u_worldViewProjectionMatrix = WORLD_VIEW_PROJECTION_MATRIX
            u_inverseTransposeWorldViewMatrix = INVERSE_TRANSPOSE_WORLD_VIEW_MATRIX
            u_cameraPosition = CAMERA_WORLD_POSITION
           
            // samplers
            sampler u_diffuseTexture
            {
                path = res/duck-diffuse.png
                mipmap = true
                wrapS = CLAMP
                wrapT = CLAMP
                minFilter = NEAREST_MIPMAP_LINEAR
                magFilter = LINEAR
            }
            // render state
            renderState
            {
                cullFace = true
                depthTest = true
            }
        }
    }
}
```

## Built-in shaders

The `<gameplay-root>/gameplay/res/shaders` directory contains a set of the most common shaders used in your games. To reduce shader code duplication the gameplay framework also supports declaring including of shader files within vertex and shader program files.

If there is an error compiling the shaders the expanded shader without the definitions is output with an `.err` file extension in the same directory where the file was loaded from.


**Example:**
 
```c++
#include "lib/lighting.frag"
```

## Shader preprocessor definitions

Using preprocessor definitions, the built-in shaders support various features. Adding certain shader definitions (defines=XXX) will require use specific uniform/samplers 'u_xxxxxxx'. You must set these in your vertex stream in your mesh and/or material parameters.

## Property inheritance

When making materials with multiple techniques or passes, you can put any common things, such as renderState or shaders, above the material or technique definitions. 
The `gameplay::Property` file format for the .material files supports property inheritance. Therefore, if you put the renderState in the material sections, then all techniques and passes will inherit its definition.
