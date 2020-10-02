#include "Pixel.h"

namespace Pixel
{
    PixelStrip::PixelStrip()
    {
        //construct the class
    }

    /**
     * Initialize Pixel strip based on defined values.
     * 
     * Define PIXEL_NUM_LEDS and PIXEL_LED_PIN in a config.h header file to set parameters.
     */
    bool PixelStrip::begin()
    {
        FastLED.addLeds<NEOPIXEL, PIXEL_LED_PIN>(leds, PIXEL_NUM_LEDS);
        Color baseColor = {0,0,0};
        Fill(baseColor, 1, Normal);
        return true;
    }

    Color PixelStrip::ReadPixel(int index)
    {
        return pixelBuffer[index];
    }

    /**
     * Fill the entire strip with a color with a specified opacity and blend mode.
     * 
     * @param color     The color to fill the strip with.
     * @param opacity   Optional, a float value for opacity between 0 and 1.
     * @param blendMode Optional, the blend mode to use when blending. Enum of type BlendMode. 
     */
    void PixelStrip::Fill(Color color, float opacity = 1, BlendMode blendMode = Normal)
    {
        for (int i = 0; i < length; i++)
        {
            pixelBuffer[i] = colorHelper.MergePixel(pixelBuffer[i], color, opacity, blendMode);
        }
    }

    /**
     * Draw a single pixel of a color at an index with a specified opacity and blend mode.
     * 
     * @param color     The color of the pixel to draw.
     * @param index     The index at which to draw the pixel. Starts at 0.
     * @param opacity   Optional, a float value for opacity between 0 and 1.
     * @param blendMode Optional, the blend mode to use when blending. Enum of type BlendMode. 
     */
    void PixelStrip::DrawPixel(Color color, int index, float opacity = 1, BlendMode blendMode = Normal)
    {
        pixelBuffer[index] = colorHelper.MergePixel(pixelBuffer[index], color, opacity, blendMode);
    }

    /**
     * Draw a line between a start and an end point with a specified opacity and blend mode.
     * 
     * @param color     The color of the new line to draw.
     * @param start     The start position.
     * @param end       The end position.
     * @param opactiy   Optional, A float value for opacity between 0 and 1.
     * @param blendMode Optional, the blend mode to use when blending. Enum of type BlendMode.
     */
    void PixelStrip::DrawLine(Color color, int start, int end, float opacity = 1, BlendMode blendMode = Normal)
    {
        for (int i = start; i < end; i++)
        {
            pixelBuffer[i] = colorHelper.MergePixel(pixelBuffer[i], color, opacity, blendMode);
        }
    }

    /**
     * Draw a linear gradient on the framebuffer.
     * 
     * @param color1    The color of the new line to draw.
     * @param color2    The start position.
     * @param offset    The end position.
     * @param opactiy   Optional, A float value for opacity between 0 and 1.
     * @param blendMode Optional, The blend mode to use when blending. Enum of type BlendMode.
     * @param feather   Optional, The amount to feather the edge of the gradient.
     */
    void PixelStrip::DrawLinearGradient(Color color1, Color color2, int offset, float opacity = 1, BlendMode blendMode = Normal, int feather = 0)
    {
        for (int i = 0; i < offset; i++)
        {
            pixelBuffer[i] = colorHelper.MergePixel(pixelBuffer[i], color1, opacity, blendMode);
        }
        for (int i = 0; i < length - offset; i++)
        {
            pixelBuffer[i + offset] = colorHelper.MergePixel(pixelBuffer[i + offset], color2, opacity, blendMode);
        }

        if (feather > 0)
        {
            //feather using a gaussian blur technique at the specified offset
        }
    }

    /**
     * Apply a box blur to the current framebuffer.
     * 
     * @param radius        The radius in LEDs to blur by.
     */
    void PixelStrip::BoxBlur(int radius)
    {
        blurHelper.Box(pixelBuffer, length, radius);
    }

    /**
     * Apply a gaussian blur to the current framebuffer.
     * 
     * @param radius        The radius in LEDs to blur by.
     * @param spread        The guassian spread to use in the calculation.
     */
    void PixelStrip::GaussianBlur(int radius, float spread)
    {
        blurHelper.Gaussian(pixelBuffer, length, radius, spread);
    }

    /**
     * Write the internal framebuffer to the LED strip.
     */
    void PixelStrip::Write()
    {
        for (int i = 0; i < length; i++)
        {
            leds[i] = CRGB(pixelBuffer[i].r,
                        pixelBuffer[i].g,
                        pixelBuffer[i].b);
        }
        FastLED.show();
    }

    PixelStrip Pixels;
}