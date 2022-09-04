The `gameplay::Light` class can be attached to any `gameplay::Node` in order to add lighting information into a `gameplay::Scene`. This lighting information must be bound to the `gameplay::Material` that is being applied onto the `gameplay::MeshParts`. There are three types of lights in the gameplay 3-D framework - directional, point, and spot lights.

All `gameplay::Light` components can be loaded into a `gameplay::Scene` using the `gameplay::Bundle` class. However, it is your responsibility to bind the relevant lighting information stored in the light into the `gameplay::Material` class.

You can also programmatically create a light using the factory methods on the `gameplay::Light` class. Here is an example of how to create and add a directional light to your scene and bind the lighting information onto a model's material(s):

```c++
void MyGame::initialize()
{
    // ...
    // Create a node and light attaching the light to the node
    Node* lightNode = Node::create("directionalLight1");
    Light* light = Light::createDirectional(Vector3(1, 0, 0));
    lightNode->setLight(light);
 
    // Bind the relevant lighting information into the materials
    Material* material = _modelNode->getModel()->getMaterial();
    MaterialParameter* parameter = material->getParameter("u_lightDirection");
    parameter->bindValue(lightNode, &Node::getForwardVectorView);
}
```

## Pre-computed lighting maps

Adding lighting information into `gameplay::Material` adds computationally expensive graphics computations. In many games, there are usually multiple static lights and objects in the scene. In this relationship, the additive light colors contributing to the objects can be pre-computed during the design phase. 3-D modeling tools typically support the ability to compute the light's additive color contributions using a process called baking. This process allows the artist to direct the contributing light and color information into a separate or combined texture so that this is not required during the rendering.

You can optionally declare and pass in pre-generated light maps using the colored-unlit.frag/textured-unlit.frag shaders and specifying in your materials `defines = TEXTURE_LIGHTMAP`.
Then you just assign them using the sampler `m_lightmapTexture` in your material definition to the image that was pre-generated that contains the light+color for your object. It is recommended to use 8-bit alpha textures to reduce the size.

## Directional lights

In most games, you’ll want to add a `gameplay::Light` class whose type is `Light::DIRECTIONAL`. This type of light is used as the primary light source, such as a sun or moon. The directional light represents a light source whose color is affected only by the constant direction vector. It is typical to bind this onto the `gameplay::Materials` of objects that are dynamic or moving.

## Point and spot lights

Due to the expensive processing overhead in using point and spot lights, many games are designed to restrict point and spot light use to be static, baked into light and color maps. However, the point and spot light types add exceptional realism to games. Using them in separate or combined rendering passes, you can bind point and spot lights into material to add dynamic point and spot light rendering. All the built-in gameplay .materials files support directional, point and spot lights. Also, with minor modification to the shaders, you can add additional passes to combine two or more lights. It should be noted that there is a significant performance impact in doing this. For these cases, you’ll usually want to restrict the influence of lights on a material to no more than the one or two closest lights at a time. This can be achieved by using a simple test in the `Game::update()` method to find the closest light to a `gameplay::Model` and then bind them to the `gameplay::Material` once they are found.