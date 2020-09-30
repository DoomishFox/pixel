#ifndef Pixel_h
#define Pixel_h

#include <Arduino.h>
#include <FastLED.h>

#define NUM_LEDS 128
#define LED_PIN 13

namespace Pixel
{
    struct HSV
    {
        uint8_t h;
        uint8_t s;
        uint8_t v;
    };

    struct Color
    {
        uint8_t r;
        uint8_t g;
        uint8_t b;
        float a;
        void SetHSV(HSV in)
        {
            double      hh, p, q, t, ff;
            long        i;

            if(in.s <= 0.0) {       // < is bogus, just shuts up warnings
                r = in.v;
                g = in.v;
                b = in.v;
                return;
            }
            hh = in.h;
            if(hh >= 360.0) hh = 0.0;
            hh /= 60.0;
            i = (long)hh;
            ff = hh - i;
            p = in.v * (1.0 - in.s);
            q = in.v * (1.0 - (in.s * ff));
            t = in.v * (1.0 - (in.s * (1.0 - ff)));

            switch(i) {
            case 0:
                r = in.v;
                g = t;
                b = p;
                break;
            case 1:
                r = q;
                g = in.v;
                b = p;
                break;
            case 2:
                r = p;
                g = in.v;
                b = t;
                break;

            case 3:
                r = p;
                g = q;
                b = in.v;
                break;
            case 4:
                r = t;
                g = p;
                b = in.v;
                break;
            case 5:
            default:
                r = in.v;
                g = p;
                b = q;
                break;
            }
            return;
        }
        HSV GetHSV()
        {
            HSV         out;
            double      min, max, delta;

            min = r < g ? r : g;
            min = min  < b ? min  : b;

            max = r > g ? r : g;
            max = max  > b ? max  : b;

            out.v = max;                                // v
            delta = max - min;
            if (delta < 0.00001)
            {
                out.s = 0;
                out.h = 0; // undefined, maybe nan?
                return out;
            }
            if( max > 0.0 ) { // NOTE: if Max is == 0, this divide would cause a crash
                out.s = (delta / max);                  // s
            } else {
                // if max is 0, then r = g = b = 0              
                // s = 0, h is undefined
                out.s = 0;
                out.h = 0;                            // its now undefined
                return out;
            }
            if( r >= max )                           // > is bogus, just keeps compilor happy
                out.h = ( g - b ) / delta;        // between yellow & magenta
            else
            if( g >= max )
                out.h = 2 + ( b - r ) / delta;  // between cyan & yellow
            else
                out.h = 4 + ( r - g ) / delta;  // between magenta & cyan

            out.h *= 60;                              // degrees

            if( out.h < 0 )
                out.h += 360;

            return out;
        }
        void SetFromString(char * input)
        {

        }
    };

    enum BlendMode
    {
        Normal,
        Add,
        Multiply,
        Screen
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
            void Fill(Color color, float opacity, BlendMode blendMode);
            void DrawPixel(Color color, int index, float opacity, BlendMode blendMode);
            void DrawLine(Color color, int start, int end, float opacity, BlendMode blendMode);
            void DrawLinearGradient(Color color1, Color color2, int offset, float opacity, BlendMode blendMode, int feather);
            void SetAlpha(Color mask[]);
            void BoxBlur(int radius);
            void GaussianBlur(int radius, float spread);
            void Write();
        private:
            void MergePixel(Color &pixel1, Color &pixel2, float opacity, BlendMode blendMode);
            CRGB leds[NUM_LEDS];
            Color pixelBuffer[NUM_LEDS];
            Blur blurHelper;
            const static int length = NUM_LEDS;
    };

    extern PixelStrip Pixels;
}

#endif