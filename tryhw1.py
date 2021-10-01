from uwimg import *

def test_nn_resize():
    im = load_image("data/dogsmall.jpg")
    a = nn_resize(im, im.w*4, im.h*4)
    save_image(a, "dog4x-nn")

def test_bl_resize():
    im = load_image("data/dogsmall.jpg")
    a = bilinear_resize(im, im.w*4, im.h*4)
    save_image(a, "dog4x-bl")

def test_nn_thumb():
    im = load_image("data/dog.jpg")
    a = nn_resize(im, im.w//7, im.h//7)
    save_image(a, "dog7th-nn")

def test_bl_thumb():
    im = load_image("data/dog.jpg")
    a = bilinear_resize(im, im.w//7, im.h//7)
    save_image(a, "dog7th-bl")

if __name__ == "__main__":
    print("Test NN Resize 4x")
    test_nn_resize()
    
    print("Test BL Resize 4x")
    test_bl_resize()

    print("Test NN Thumbnail")
    test_nn_thumb()

    print("Test BL Thumbnail")
    test_bl_thumb()