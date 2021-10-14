#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include "image.h"
#define TWOPI 6.2831853

void l1_normalize(image im)
{
    float denom = 0;
    for (int j=0; j<im.h; ++j)
    {
        for (int i =0; i<im.w; ++i)
        {
            denom += get_pixel(im,i,j,0);
        }
    }

    for (int j=0; j<im.h; ++j)
    {
        for (int i =0; i<im.w; ++i)
        {
            set_pixel(im,i,j,0, get_pixel(im,i,j,0)/denom);
        }
    }
}

image make_box_filter(int w)
{
    image im = make_image(w, w, 1);
    for (int j=0; j<im.h; ++j)
    {
        for (int i =0; i<im.w; ++i)
        {
            set_pixel(im,i,j,0,1);
        }
    }
    l1_normalize(im);
    return im;
}

image convolve_image(image im, image filter, int preserve)
{
    assert(filter.c == im.c || filter.c == 1);
    int fw = filter.w;
    int fh = filter.h;
    image result;
    if (preserve==0){
        result = make_image(im.w,im.h,1);
    } else {
        result = make_image(im.w,im.h,im.c);
    }

    for (int j=0; j<im.h; ++j)
    {
        for (int i =0; i<im.w; ++i)
        {
            for (int k=0; k<im.c; ++k)
            {
                float buff = 0;
                int kk = k;
                int ki = k;
                if (filter.c == 1){
                    kk =0;
                }
                if (preserve == 0){
                    ki = 0;
                }
                for (int jk=0; jk<fh; ++jk)
                {
                    for (int ik = 0; ik<fw; ++ik)
                    {
                        // TODO: in case error cobain get pixelnya conditional balikin 0/1 kalau out of bond. (`<0` atau `>h` atau `>w`)
                        buff += get_pixel(im,i-fw/2+ik,j-fh/2+jk,k)*get_pixel(filter,ik,jk,kk);
                    }
                }
                set_pixel(result,i,j,ki,buff + get_pixel(result,i,j,ki));
            }
        }
    }
    return result;
}

image make_highpass_filter()
{
    static const float hpf_kernel[9] = {
        0   ,-1 ,0  ,
        -1  ,4  ,-1 ,
        0   ,-1 ,0  ,
    };
    image hpf = make_image(3,3,1);
    memcpy(hpf.data, hpf_kernel, sizeof(hpf_kernel));
    return hpf;
}

image make_sharpen_filter()
{
    static const float sharpen_kernel[9] = {
        0   ,-1 ,0  ,
        -1  ,5  ,-1 ,
        0   ,-1 ,0  ,
    };
    image sharpen = make_image(3,3,1);
    memcpy(sharpen.data, sharpen_kernel, sizeof(sharpen_kernel));
    return sharpen;
}

image make_emboss_filter()
{
    static const float emboss_kernel[9] = {
        -2  ,-1 ,0  ,
        -1  ,1  ,1  ,
        0   ,1  ,2  ,
    };
    image emboss = make_image(3,3,1);
    memcpy(emboss.data, emboss_kernel, sizeof(emboss_kernel));
    return emboss;
}

// Question 2.2.1: Which of these filters should we use preserve when we run our convolution and which ones should we not? Why?
// Answer: All, because HPF, Sharpen and Emboss can be used on each channel

// Question 2.2.2: Do we have to do any post-processing for the above filters? Which ones and why?
// Answer: On sharpen and emboss, because the kernel is not normalize and risk the result being overflow.

int calc_gaussian_size(float sigma){
    int size = ceil(6 * sigma);
    size = size % 2 == 1 ? size : size+1;
    return size;
}

float calc_gaussian_value(int x, int y, float sigma){
    float exp_val = -(powf(x,2) + powf(y,2))/(2*powf(sigma,2));
    return expf(exp_val)/(TWOPI*powf(sigma,2));
}

image make_gaussian_filter(float sigma)
{
    int size = calc_gaussian_size(sigma);
    image gauss = make_image(size, size, 1);
    for (int j=0; j<gauss.h; ++j)
    {
        for (int i =0; i<gauss.w; ++i)
        {
            set_pixel(gauss,i,j,0, calc_gaussian_value(i-size/2,j-size/2,sigma));
        }
    }
    l1_normalize(gauss);
    return gauss;
}

image add_image(image a, image b)
{
    assert(a.w==b.w  && a.h==b.h && a.c==b.c);
    image result = make_image(a.w,a.h,a.c);
    for (int j=0; j<result.h; ++j)
    {
        for (int i =0; i<result.w; ++i)
        {
            for (int k =0; k<result.c; ++k)
            {
                set_pixel(result,i,j,k, get_pixel(a,i,j,k)+get_pixel(b,i,j,k));
            }
        }
    }
    return result;
}

image sub_image(image a, image b)
{
    assert(a.w==b.w  && a.h==b.h && a.c==b.c);
    image result = make_image(a.w,a.h,a.c);
    for (int j=0; j<result.h; ++j)
    {
        for (int i =0; i<result.w; ++i)
        {
            for (int k =0; k<result.c; ++k)
            {
                set_pixel(result,i,j,k, get_pixel(a,i,j,k)-get_pixel(b,i,j,k));
            }
        }
    }
    return result;
}

image make_gx_filter()
{
    image filter = make_image(3,3,1);
    set_pixel(filter, 0, 0, 0, -1);
    set_pixel(filter, 0, 1, 0, -2);
    set_pixel(filter, 0, 2, 0, -1);
    set_pixel(filter, 1, 0, 0, 0);
    set_pixel(filter, 1, 1, 0, 0);
    set_pixel(filter, 1, 2, 0, 0);
    set_pixel(filter, 2, 0, 0, 1);
    set_pixel(filter, 2, 1, 0, 2);
    set_pixel(filter, 2, 2, 0, 1);
    return filter;
}

image make_gy_filter()
{
    image filter = make_image(3,3,1);
    set_pixel(filter, 0, 0, 0, -1);
    set_pixel(filter, 0, 1, 0, 0);
    set_pixel(filter, 0, 2, 0, 1);
    set_pixel(filter, 1, 0, 0, -2);
    set_pixel(filter, 1, 1, 0, 0);
    set_pixel(filter, 1, 2, 0, 2);
    set_pixel(filter, 2, 0, 0, -1);
    set_pixel(filter, 2, 1, 0, 0);
    set_pixel(filter, 2, 2, 0, 1);
    return filter;
}

void feature_normalize(image im)
{
    for (int k =0; k<im.c; ++k)
    {
        float min_val=1;
        float max_val=0;
        // First iteration for Min Value Searching
        for (int j=0; j<im.h; ++j)
        {
            for (int i =0; i<im.w; ++i)
            {
                float current = get_pixel(im,i,j,k);
                min_val = current<min_val?current:min_val;
            }
        }
        // Second Iteration for Rescaling
        for (int j=0; j<im.h; ++j)
        {
            for (int i =0; i<im.w; ++i)
            {
                float rescale_val = 0;
                if (max_val-min_val > 0){
                    rescale_val = (get_pixel(im,i,j,k) - min_val) / (max_val - min_val);
                }
                set_pixel(im,i,j,k,rescale_val);
            }
        }
    }
}

image *sobel_image(image im)
{
    image *result = calloc(2, sizeof(image));
    image magnitude = make_image(im.w,im.h,1);
    image direction = make_image(im.w,im.h,1);

    image gx = make_gx_filter();
    image gy = make_gy_filter();
    image gx_im = convolve_image(im, gx, 0);
    image gy_im = convolve_image(im, gy, 0);

    for (int j=0; j<im.h; ++j)
    {
        for (int i =0; i<im.w; ++i)
        {
            float gx_px = get_pixel(gx_im,i,j,0);
            float gy_px = get_pixel(gy_im,i,j,0);
            float mag = sqrtf(powf(gx_px,2)+powf(gy_px,2));
            float dir = atan2f(gy_px, gx_px);

            set_pixel(magnitude, i, j, 0, mag);
            set_pixel(direction, i, j, 0, dir);
        }
    }

    result[0] = magnitude;
    result[1] = direction;
    return result;
}

image colorize_sobel(image im)
{
    // TODO
    return make_image(1,1,1);
}
