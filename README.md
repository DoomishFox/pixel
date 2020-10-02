# Pixel 

Pixel is a 1-Dimensional graphics library written primarily for an ESP32. It uses FastLED under the hood to actually drive a Neopixel LED strip and exposes a variety of simple draw commands to draw specific pixels, lines, and gradients with varying opacities. Four blend modes, Normal, Add, Multiply, and Screen, are also available and are accurate to within +- 1 of Photoshop's equivalent blend modes*. Also in this library is Box and Gaussian style blur effects to apply to your LED strip for smooth transitions.

[^*]: Photoshop seems to have some weird occasional rounding errors compared to the algorithms in this library. They don't crop up very often but can be seen in the `test_merge_rpixel_screen` unit test if assertion delta is set to 0.

## Usage

Pixel was build with PlatformIO for an ESP32 and as such all examples and installation instructions are for that platform.

Simply clone the repository and drop all the `.h` and `.cpp` files into a single folder inside of PlatformIO's lib folder structure of your project and include the `Pixel.h` header. Everything involving the Pixel library is inside of the Pixel namespace so be sure to also include a `using namespace Pixel` in your code as well.

Pixel also includes a `test_pixel.cpp` file for use with PlatformIO's built-in unit testing should you want to verify the color handling works as intended.

Initialization and draw commands can be accessed with the extern class `Pixels`. You must call `Pixels.begin()` before using any of the functions in this library as this will set up the FastLED strip and initialize the framebuffer to a black state. The Pixel library has an internal framebuffer you write to with the provided functions that will only be written out to the LED strip when you call `Pixels.Write();`. For example:

```c++
Pixels.begin();

Color firstColor = {10, 43, 156};
Pixels.DrawPixel(firstColor);

Color secondColor = {35, 195, 236};
Pixels.DrawLine(secondColor, 0, 15, 0.6, Multiply);

Pixels.Write();
```

Configure the library by creating a file called `config.h` in your PlatformIO include directory and define these two constants:

```c++
#define PIXEL_NUM_LEDS 128
#define PIXEL_LED_PIN 13
```

Once this file is created you can initialize the library by calling `Pixels.begin();` in your setup function.

Current draw commands include:

- `Fill(Color color, float opacity, BlendMode blendMode)`: Fills the entire strip with a color. Opacity is an optional float between 0 and 1 (default 1) and blend mode is an optional Enum (default Normal).
- `DrawPixel(Color color, int index, float opacity, BlendMode blendMode)`: Draws a single pixel on the strip of a color at a zero-based index. Opacity is an optional float between 0 and 1 (default 1) and blend mode is an optional Enum (default Normal).
- `DrawLine(Color color, int start, int end, float opacity, BlendMode blendMode)`: Draws a line of pixels on the strip of a color from a zero-based start index to an end index. Opacity is an optional float between 0 and 1 (default 1) and blend mode is an optional Enum (default Normal).
- `DrawLinearGradient(Color color1, Color color2, int start, int end, int offset, int feather, float opacity, BlendMode blendMode)`: Draws a linear gradient between color1 and color2 from a start to an end value. Offset determines where the colors will change while the optional feather integer controls the radius in LEDS of the localized blur to use when fading between colors. Opacity is an optional float between 0 and 1 (default 1) and blend mode is an optional Enum (default Normal).

Current FX commands include:

- `BoxBlur(int radius)`: Applies a fast box blur of a radius in LEDs to the entire framebuffer.
- `GaussianBlur(int radius, float spread)`: Applies a slower gaussian blur of a radius in LEDS to the entire framebuffer. Spread would control the gaussian spread of the equation used, if it actually currently used one.



## Current State

There's no easy way to configure pretty much anything at this point. I'm not even positive the config.h works properly, I need to test it. As it stands, FastLED is setup for NEOPIXEL type lights only

What works:

- Basic draw functions such as Fill, DrawPixel, and DrawLine have full functionality.
- Advanced draw functions such as DrawGradient have partial functionality; feathering a gradient is not available yet. If you absolutely need it, you can draw a gradient and apply a blur over the whole strip to mimic feathering.
- Box blur is fully functional while Gaussian blur currently doesn't make use of a proper gaussian equation and as such acts the same as a standard Box blur.
- Getting and setting HSV values is still untested and may or may not get accurate results. I stole the algorithms from the internet so who knows.