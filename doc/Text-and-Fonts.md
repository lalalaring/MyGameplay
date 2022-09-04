The `gameplay::Font` class is used to draw 2D ASCII text. 

**Note:** Unicode is currently not supported.

##Converting a TrueType font to a "GamePlay Bundle" file

You must convert the TrueType font to a "GamePlay Bundle File" and include the `.gpb` file in your project. The `.ttf` file does not need to be included.

- Find or create a TrueType font file (`.ttf`).

- Pass the `.ttf` file to the [gameplay-encoder](https://github.com/gameplay3d/GamePlay/tree/master/tools/encoder) command line tool and specify the size you want.

    `gameplay-encoder -s 28 myfont.ttf`

- The gameplay-encoder will output a `.gpb` file.

- Copy the `.gpb` file to your game's resource directory.

**Note:** Pre-compiled versions of gameplay-encoder is available in GamePlay\bin after [[Installing External Dependencies]].

## Drawing text with a Font

```c++
void MyGame::initialize()
{
    // Create a font from the gpb file
    _font = Font::create("res/myfont.gpb");
}

void MyGame::render(float elapsedTime)
{
    // Clear the frame buffer
    clear(CLEAR_COLOR_DEPTH, Vector4(0, 0, 0, 1), 1.0f, 0);
 
    // Draw the text at position 20,20 using red color
    _font->start();
    char text[1024];
    sprintf(text, "FPS:%d", Game::getFrameRate());
    _font->drawText(text, 20, 20, Vector4(1, 0, 0, 1), _font->getSize());
    _font->finish();
}
 
void MyGame::finalize()
{
    // Use built-in macros to clean up our resources.
    SAFE_RELEASE(_font);
}
```

## Batching Glyphs and Words

It is possible to call `Font::drawText()` multiple times between `Font::start()` and `Font::finish()` in order to draw different text in different places with the same font.

## Examples

All of the samples use `Font::drawText()` to render the framerate.