#ifndef Pixel_h
#define Pixel_h

#include <Arduino.h>
#include <FastLED.h>
#include "config.h"

#ifndef Config_h

#ifndef PIXEL_NUM_LEDS
#define PIXEL_NUM_LEDS 8
#endif

#ifndef PIXEL_LED_PIN
#define PIXEL_LED_PIN 13
#endif

#endif

namespace Pixel
{
    struct HSV
    {
        uint8_t h;
        uint8_t s;
        uint8_t v;
    };

    struct ColorOp
    {
        float r;
        float g;
        float b;
    };

    struct Color
    {
        uint8_t r;
        uint8_t g;
        uint8_t b;
    };

    enum BlendMode
    {
        Normal,
        Add,
        Multiply,
        Screen
    };

    class ColorHelper
    {
        public:
            ColorHelper();
            ColorOp GetColorOp(Color color);
            Color SetColorOp(ColorOp colorOp);
            HSV GetHSV(Color color);
            Color SetHSV(HSV in);
            Color MergePixel(Color pixel1, Color pixel2, float opacity, BlendMode blendMode);
    };

    class Blur
    {
        public:
            Blur();
            void Box(Color pixels[], int length, int radius);
            void Gaussian(Color pixels[], int length, int radius, float spread);
            void Feather(Color pixels[], int length, int radius, float spread);
        private:
            float calcGaussian(int x);
    };

    class PixelStrip
    {
        public:
            PixelStrip();
            bool begin();
            Color ReadPixel(int index);
            void Fill(Color color, float opacity, BlendMode blendMode);
            void DrawPixel(Color color, int index, float opacity, BlendMode blendMode);
            void DrawLine(Color color, int start, int end, float opacity, BlendMode blendMode);
            void DrawLinearGradient(Color color1, Color color2, int offset, float opacity, BlendMode blendMode, int feather);
            void SetAlpha(Color mask[]);
            void BoxBlur(int radius);
            void GaussianBlur(int radius, float spread);
            void Write();
        private:
            CRGB leds[PIXEL_NUM_LEDS];
            Color pixelBuffer[PIXEL_NUM_LEDS];
            ColorHelper colorHelper;
            Blur blurHelper;
            const static int length = PIXEL_NUM_LEDS;
    };

    extern PixelStrip Pixels;
}

#endif