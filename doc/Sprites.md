The `gameplay::SpriteBatch` class is used to draw 2D sprites for both 2D and 3D games.

## Drawing Sprites

```c++
void MyGame::initialize()
{
    // Create your sprite batch from a PNG image
    _batch = SpriteBatch::create("res/image.png");
}

void MyGame::render(float elapsedTime)
{
    // Clear the frame buffer
    clear(CLEAR_COLOR_DEPTH, Vector4(0, 0, 0, 1), 1.0f, 0);
 
    // Draw your sprites
    _batch->start();
    _batch->draw(Rectangle(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT), 
                 Rectangle(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT), Vector4::one());
    // SpriteBatch::draw() can be called multiple times between start() and finish()
    _batch->finish();
}
 
void MyGame::finalize()
{
    // Use built-in macros to clean up our resources.
    SAFE_DELETE(_batch);
}
```

## Batching

It is possible to use `SpriteBatch` with a _Sprite Sheet_ or _Texture Atlas_ and call `SpriteBatch::draw()` multiple times between `SpriteBatch::start()` and `SpriteBatch::finish()` with different source and destination regions.

## Enabling depth testing

Some of the `SpriteBatch::draw()` methods take a Z coordinate. If you want `SpriteBatch` draw behind other objects then make sure depth test is enable.

```c++
spriteBatch->getStateBlock()->setDepthTest(true);
```

## Custom Projection (Billboards)

`SpriteBatch` uses an orthographic projection by default but you can specify a different projection matrix using `SpriteBatch::setProjectionMatrix()`. This can be used to draw _billboards_.

## Example

- [samples/browser/src/SpriteBatchSample.cpp](https://github.com/gameplay3d/GamePlay/blob/master/samples/browser/src/SpriteBatchSample.cpp)
