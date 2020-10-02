#include "Pixel.h"

namespace Pixel
{
    ColorHelper::ColorHelper()
    {
        //Construct ColorHelper class
    }

    ColorOp ColorHelper::GetColorOp(Color color)
    {
        ColorOp colorOut;
        colorOut.r = (float)color.r / 255;
        colorOut.g = (float)color.r / 255;
        colorOut.b = (float)color.r / 255;

        return colorOut;
    }

    Color ColorHelper::SetColorOp(ColorOp colorOp)
    {
        Color color;
        color.r = (uint8_t)round(colorOp.r * 255);
        color.g = (uint8_t)round(colorOp.g * 255);
        color.b = (uint8_t)round(colorOp.b * 255);

        return color;
    }

    HSV ColorHelper::GetHSV(Color color)
    {
        HSV         out;
        double      min, max, delta;

        min = color.r < color.g ? color.r : color.g;
        min = min  < color.b ? min  : color.b;

        max = color.r > color.g ? color.r : color.g;
        max = max  > color.b ? max  : color.b;

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
        if( color.r >= max )                           // > is bogus, just keeps compilor happy
            out.h = ( color.g - color.b ) / delta;        // between yellow & magenta
        else
        if( color.g >= max )
            out.h = 2 + ( color.b - color.r ) / delta;  // between cyan & yellow
        else
            out.h = 4 + ( color.r - color.g ) / delta;  // between magenta & cyan

        out.h *= 60;                              // degrees

        if( out.h < 0 )
            out.h += 360;

        return out;
    }

    Color ColorHelper::SetHSV(HSV in)
    {
        Color out;
        double      hh, p, q, t, ff;
        long        i;

        if(in.s <= 0.0) {       // < is bogus, just shuts up warnings
            out.r = in.v;
            out.g = in.v;
            out.b = in.v;
            return out;
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
            out.r = in.v;
            out.g = t;
            out.b = p;
            break;
        case 1:
            out.r = q;
            out.g = in.v;
            out.b = p;
            break;
        case 2:
            out.r = p;
            out.g = in.v;
            out.b = t;
            break;

        case 3:
            out.r = p;
            out.g = q;
            out.b = in.v;
            break;
        case 4:
            out.r = t;
            out.g = p;
            out.b = in.v;
            break;
        case 5:
        default:
            out.r = in.v;
            out.g = p;
            out.b = q;
            break;
        }
        return out;
    }

    /**
     * Merge referenced pixels in an A over B method based on proper opacity and blend modes.
     * 
     * @param pixel1    Reference to the pixel B.
     * @param pixel2    Reference to the pixel A.
     * @param opactiy   A float value for opacity between 0 and 1.
     * @param blendMode The blend mode to use when blending. Enum of type BlendMode.
     * 
     * @returns         Assigns final color to pixel1 parameter.
     */
    Color ColorHelper::MergePixel(Color pixelb, Color pixela, float opacity, BlendMode blendMode)
    {
        //merge pixels with respect to blend mode
        switch (blendMode)
        {
            default:
            case Normal:
            {
                ColorOp colora = GetColorOp(pixela);
                ColorOp colorb = GetColorOp(pixelb);
                
                //basic A alpha over B algorithm where A is pixel2 and B is pixel1
                colorb.r = ((colora.r * opacity) + ((colorb.r) * (1 - opacity)))/(opacity + (1 * (1 - opacity)));
                colorb.g = ((colora.g * opacity) + ((colorb.g) * (1 - opacity)))/(opacity + (1 * (1 - opacity)));
                colorb.r = ((colora.b * opacity) + ((colorb.b) * (1 - opacity)))/(opacity + (1 * (1 - opacity)));

                return SetColorOp(colorb);
                break;
            }
            case Add:
            {
                ColorOp colora = GetColorOp(pixela);
                ColorOp colorb = GetColorOp(pixelb);

                colorb.r = (colorb.r) + (colora.r * opacity) <= 1 ? (colorb.r) + (colora.r * opacity) : 1;
                colorb.g = (colorb.g) + (colora.g * opacity) <= 1 ? (colorb.g) + (colora.g * opacity) : 1;
                colorb.b = (colorb.b) + (colora.b * opacity) <= 1 ? (colorb.b) + (colora.b * opacity) : 1;

                return SetColorOp(colorb);
                break;
            }
            case Multiply:
            {
                ColorOp colora = GetColorOp(pixela);
                ColorOp colorb = GetColorOp(pixelb);

                //just multiply
                colorb.r *= (colora.r + ((1 - colora.r) * (1 - opacity)));
                colorb.g *= (colora.g + ((1 - colora.g) * (1 - opacity)));
                colorb.b *= (colora.b + ((1 - colora.b) * (1 - opacity)));
                
                return SetColorOp(colorb);
                break;
            }
            case Screen:
            {
                ColorOp colora = GetColorOp(pixela);
                ColorOp colorb = GetColorOp(pixelb);

                colorb.r = 1 - ((1 - colora.r * opacity) * ( 1- colorb.r));
                colorb.g = 1 - ((1 - colora.g * opacity) * ( 1- colorb.g));
                colorb.b = 1 - ((1 - colora.g * opacity) * ( 1- colorb.g));
                
                return SetColorOp(colorb);
                break;
            }
        }
    }
        
}