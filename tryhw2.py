from uwimg import *

def test_conv():
    im = load_image("data/dog.jpg")
    f = make_box_filter(7)
    blur = convolve_image(im, f, 1)
    save_image(blur, "dog-box7")
    free_image(im)

def test_hpf():
    im = load_image("data/dog.jpg")
    f = make_highpass_filter()
    hpf = convolve_image(im, f, 1)
    clamp_image(hpf)
    save_image(hpf, "dog-hpf")
    free_image(im)

def test_sharpen():
    im = load_image("data/dog.jpg")
    f = make_sharpen_filter()
    sharpen = convolve_image(im, f, 1)
    clamp_image(sharpen)
    save_image(sharpen, "dog-sharpen")
    free_image(im)
    
def test_emboss():
    im = load_image("data/dog.jpg")
    f = make_emboss_filter()
    emboss = convolve_image(im, f, 1)
    clamp_image(emboss)
    save_image(emboss, "dog-emboss")
    free_image(im)

def test_gx():
    im = load_image("data/dog.jpg")
    f = make_gx_filter()
    gx = convolve_image(im, f, 0)
    clamp_image(gx)
    save_image(gx, "dog-ggx")
    free_image(im)

def test_gy():
    im = load_image("data/dog.jpg")
    f = make_gy_filter()
    gy = convolve_image(im, f, 0)
    clamp_image(gy)
    save_image(gy, "dog-ggy")
    free_image(im)

def test_thumb():
    im = load_image("data/dog.jpg")
    f = make_box_filter(7)
    blur = convolve_image(im, f, 1)
    thumb = nn_resize(blur, blur.w//7, blur.h//7)
    save_image(thumb, "dogthumb")
    free_image(im)
    free_image(thumb)
    free_image(blur)

def test_gauss():
    im = load_image("data/dog.jpg")
    f = make_gaussian_filter(2)
    blur = convolve_image(im, f, 1)
    save_image(blur, "dog-gauss2")
    free_image(im)

if __name__ == "__main__":
    # print("Testing HPF")
    # test_hpf()
    # print("Testing Sharpen")
    # test_sharpen()
    # print("Testing Emboss")
    # test_emboss()
    # print("Testing Gauss")
    # test_gauss()
    print("Testing gx")
    test_gx()
    print("Testing gy")
    test_gy()