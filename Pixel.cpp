#include "Pixel.h"

namespace Pixel
{
    PixelStrip::PixelStrip()
    {
        //construct the class
    }

    /**
     * Initialize Pixel strip.
     */
    bool PixelStrip::begin()
    {
        FastLED.addLeds<NEOPIXEL, LED_PIN>(leds, NUM_LEDS);
        return true;
    }

    void PixelStrip::Fill(Color color, float opacity = 1, BlendMode blendMode = Normal)
    {
        for (int i = 0; i < length; i++)
        {
            pixelBuffer[i] = color;
        }
    }

    void PixelStrip::DrawPixel(Color color, int index, float opacity = 1, BlendMode blendMode = Normal)
    {
        pixelBuffer[index] = color;
    }

    /**
     * Draw a line between a start and an end point with a specified opacity and blend mode.
     * 
     * @param color     The color of the new line to draw.
     * @param start     The start position.
     * @param end       The end position.
     * @param opactiy   Optional, A float value for opacity.
     * @param blendMode Optional, The blend mode to use with an opacity less than 1.
     *                      Enum of type BlendMode.
     */
    void PixelStrip::DrawLine(Color color, int start, int end, float opacity = 1, BlendMode blendMode = Normal)
    {
        for (int i = start; i < end; i++)
        {
            pixelBuffer[i] = color;
        }
    }

    /**
     * Draw a linear gradient on the framebuffer.
     * 
     * @param color1    The color of the new line to draw.
     * @param color2    The start position.
     * @param offset    The end position.
     * @param opactiy   Optional, A float value for opacity.
     * @param blendMode Optional, The blend mode to use with an opacity less than 1.
     *                      Enum of type BlendMode.
     * @param feather   Optional, The amount to feather the edge of the gradient.
     */
    void PixelStrip::DrawLinearGradient(Color color1, Color color2, int offset, float opacity = 1, BlendMode blendMode = Normal, int feather = 0)
    {
        for (int i = 0; i < offset; i++)
        {
            pixelBuffer[i] = color1;
        }
        for (int i = 0; i < length - offset; i++)
        {
            pixelBuffer[i + offset] = color2;
        }

        if (feather > 0)
        {
            //feather using a gaussian blur technique at the specified offset
        }
    }

    void PixelStrip::SetAlpha(Color mask[])
    {
        //something like this i think
        for (int i = 0; i < length; i++)
        {
            pixelBuffer[i].a = mask[i].a;
        }
    }

    void PixelStrip::BoxBlur(int radius)
    {
        blurHelper.Box(pixelBuffer, length, radius);
    }

    void PixelStrip::GaussianBlur(int radius, float spread)
    {
        blurHelper.Gaussian(pixelBuffer, length, radius, spread);
    }

    void PixelStrip::Write()
    {
        for (int i = 0; i < length; i++)
        {
            leds[i] = CRGB(pixelBuffer[i].r * (pixelBuffer[i].a / 255),
                        pixelBuffer[i].g * (pixelBuffer[i].a / 255),
                        pixelBuffer[i].b * (pixelBuffer[i].a / 255));
        }
        FastLED.show();
    }

    void PixelStrip::MergePixel(Color &pixel1, Color &pixel2, float opacity, BlendMode blendMode)
    {
        switch (blendMode)
        {
            case Normal:
            {
                //merge pixels with respect to Normal blend mode
                        
                //first lets get a new alpha we can work with
                float alpha2 = pixel2.a * opacity;

                //basic A alpha over B algorithm where A is pixel2 and B is pixel1
                pixel1.r = ((pixel2.r * alpha2) + ((pixel1.r * pixel1.a) * (1 - alpha2)))/(alpha2 + (pixel1.a * (1 - alpha2)));
                pixel1.g = ((pixel2.g * alpha2) + ((pixel1.g * pixel1.a) * (1 - alpha2)))/(alpha2 + (pixel1.a * (1 - alpha2)));
                pixel1.r = ((pixel2.b * alpha2) + ((pixel1.b * pixel1.a) * (1 - alpha2)))/(alpha2 + (pixel1.a * (1 - alpha2)));
                //also calculate the resultant alpha channel
                pixel1.a = alpha2 + (pixel1.a * (1 - alpha2));
                break;
            }
            case Add:
            case Multiply:
            case Screen:
                break;
        }
    }

    
    PixelStrip Pixels;
}

//comment reference

    /**
     * Initialize Pixels
     *
     * @param partition_label  Optional, label of the partition to check.
     *                         If not specified, first partition with subtype=spiffs is used.
     *
     * @return  
     *          - true    if mounted
     *          - false   if not mounted
     */