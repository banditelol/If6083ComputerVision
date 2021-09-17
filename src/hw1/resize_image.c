#include <math.h>
#include "image.h"

float nn_interpolate(image im, float x, float y, int c)
{
    return get_pixel(im, round(x), round(y), c);
}

image nn_resize(image im, int w, int h)
{
    // Create image of new size
    image new_im = make_image(w, h, im.c);
    // matching the new coordinate space (with aX + b = Y)
    float ax = (float) im.w / (float) w;
    float bx = 0.5*ax - 0.5;
    float ay = (float) im.h / (float) h;
    float by = 0.5*ay - 0.5;
    // Map to old im coordinates
    for (int j=0; j<h; ++j)
    {
        for (int i =0; i<w; ++i)
        {
            for (int k =0; k<im.c; ++k){
                float p = nn_interpolate(im,i*ax+bx,j*ay+by,k);
                set_pixel(new_im,i,j,k,p);
            }
        }
    }

    return new_im;
}

float bilinear_interpolate(image im, float x, float y, int c)
{
    float p1 = get_pixel(im, floor(x), floor(y), c);
    float p2 = get_pixel(im, ceil(x), floor(y), c);
    float p3 = get_pixel(im, floor(x), ceil(y), c);
    float p4 = get_pixel(im, ceil(x), ceil(y), c);
    float q1  = (y-floor(y))*p3 + (ceil(y)-y)*p1;
    float q2  = (y-floor(y))*p4 + (ceil(y)-y)*p2;
    float q = (x-floor(x))*q2 + (ceil(x)-x)*q1;
    return q;
}

image bilinear_resize(image im, int w, int h)
{
    image new_im = make_image(w, h, im.c);
    // matching the new coordinate space (with aX + b = Y)
    float ax = (float) im.w / (float) w;
    float bx = 0.5*ax - 0.5;
    float ay = (float) im.h / (float) h;
    float by = 0.5*ay - 0.5;
    // Map to old im coordinates
    for (int j=0; j<h; ++j)
    {
        for (int i =0; i<w; ++i)
        {
            for (int k =0; k<im.c; ++k){
                float p = bilinear_interpolate(im,i*ax+bx,j*ay+by,k);
                set_pixel(new_im,i,j,k,p);
            }
        }
    }

    return new_im;
}

