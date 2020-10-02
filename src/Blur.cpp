#include "Pixel.h"

namespace Pixel
{
    Blur::Blur()
    {
        //Construct Blur class
    }
    
    /** 
     * Box blur the passed pixel buffer by a specified radius.
     *
     * @param pixels The framebuffer to operate on.
     * @param length The length of the framebuffer array.
     * @param radius The radius of the blur.
     */
    void Blur::Box(Color pixels[], int length, int radius)
    {
        //duplicate pixels to new array for operation
        Color opPixels[length + (2 * radius)];
        //set the "before pixels" to extend
        for (int i = 0; i < radius; i++)
        {
            //opPixels[i] = pixels[0];
            opPixels[i] = {0,0,0};
        }
        //copy current pixel values
        for (int i = 0; i < length; i++)
        {
            opPixels[i + radius] = pixels[i];
        }
        //set the "after pixels" to extend
        for (int i = 0; i < radius; i++)
        {
            //opPixels[i + length + radius - 1] = pixels[length - 1];
            opPixels[i + length + radius - 1] = {0,0,0};
        }

        //iterate through pixels and add all overlapping kernels
        for (int i = 0; i < length; i++)
        {
            uint32_t red = 0;
            uint32_t green = 0;
            uint32_t blue = 0;

            for (int j = 0; j < (2 * radius) + 1; j++)
            {
                red += opPixels[j + i].r;
                green += opPixels[j + i].g;
                blue += opPixels[j + i].b;
            }

            //now divide by the number of iterations that were just performed
            pixels[i].r = red / ((2 * radius) + 1);
            pixels[i].g = green / ((2 * radius) + 1);
            pixels[i].b = blue / ((2 * radius) + 1);
        }
    }

    /** 
     * Gaussian blur the passed pixel buffer by a specified radius and with a specified spread.
     *
     * @param pixels The framebuffer to operate on.
     * @param length The length of the framebuffer array.
     * @param radius The radius of the blur.
     * @param spread The guassian spread of the blur.
     */
    void Blur::Gaussian(Color pixels[], int length, int radius, float spread)
    {
        //duplicate pixels to new array for operation
        Color opPixels[length + (2 * radius)];
        //set the "before pixels" to extend
        for (int i = 0; i < radius; i++)
        {
            //opPixels[i] = pixels[0];
            opPixels[i] = {0,0,0};
        }
        //copy current pixel values
        for (int i = 0; i < length; i++)
        {
            opPixels[i + radius] = pixels[i];
        }
        //set the "after pixels" to extend
        for (int i = 0; i < radius; i++)
        {
            //opPixels[i + length + radius - 1] = pixels[length - 1];
            opPixels[i + length + radius - 1] = {0,0,0};
        }

        //iterate through pixels and add all overlapping kernels
        for (int i = 0; i < length; i++)
        {
            float red = 0;
            float green = 0;
            float blue = 0;

            float kernelSum = 0;

            for (int j = 0; j < (2 * radius) + 1; j++)
            {
                kernelSum += calcGaussian(j - radius);

                //add to colors
                red += opPixels[j + i].r;
                green += opPixels[j + i].g;
                blue += opPixels[j + i].b;
            }

            //now divide by the kernelSum
            pixels[i].r = red / kernelSum;
            pixels[i].g = green / kernelSum;
            pixels[i].b = blue / kernelSum;
        }
    }

    void Blur::Feather(Color pixels[], int length, int radius, float spread)
    {
        
    }

    float Blur::calcGaussian(int x)
    {
        return 1.0;
    }

}