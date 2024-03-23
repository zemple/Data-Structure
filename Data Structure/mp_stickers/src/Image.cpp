#include "Image.h"
using namespace cs225;

Image::Image() : PNG() {}
Image::Image(unsigned int width, unsigned int height) : PNG(width, height) {}

void Image::darken() {
    unsigned int a = this->width();
    unsigned int b = this->height();
    for (unsigned int i = 0; i < a; i++) {
        for (unsigned int j = 0; j < b; j++) {
            HSLAPixel &p = this->getPixel(i,j);
            if (p.l - 0.1 < 0) {
                p.l = 0;
            } else {
                p.l -= 0.1;
            }
        }
    }
}

void Image::darken(double amount) {
    unsigned int a = this->width();
    unsigned int b = this->height();
    for (unsigned int i = 0; i < a; i++) {
        for (unsigned int j = 0; j < b; j++) {
            HSLAPixel &p = this->getPixel(i,j);
            if (p.l - amount < 0) {
                p.l = 0;
            } else {
                p.l -= amount;
            }
        }
    }
}

void Image::lighten() {
    unsigned int a = this->width();
    unsigned int b = this->height();
    for (unsigned int i = 0; i < a; i++) {
        for (unsigned int j = 0; j < b; j++) {
            HSLAPixel &p = this->getPixel(i,j);
            if (p.l + 0.1 > 1) {
                p.l = 1;
            } else {
                p.l += 0.1;
            }
        }
    }
}

void Image::lighten(double amount) {
    unsigned int a = this->width();
    unsigned int b = this->height();
    for (unsigned int i = 0; i < a; i++) {
        for (unsigned int j = 0; j < b; j++) {
            HSLAPixel &p = this->getPixel(i,j);
            if (p.l + amount > 1) {
                p.l = 1;
            } else {
                p.l += amount;
            }
        }
    }
}

void Image::desaturate() {
    unsigned int a = this->width();
    unsigned int b = this->height();
    for (unsigned int i = 0; i < a; i++) {
        for (unsigned int j = 0; j < b; j++) {
            HSLAPixel &p = this->getPixel(i,j);
            if (p.s - 0.1 < 0) {
                p.s = 0;
            } else {
                p.s -= 0.1;
            }
        }
    }
}
void Image::desaturate(double amount) {
    unsigned int a = this->width();
    unsigned int b = this->height();
    for (unsigned int i = 0; i < a; i++) {
        for (unsigned int j = 0; j < b; j++) {
            HSLAPixel &p = this->getPixel(i,j);
            if (p.s - amount < 0) {
                p.s = 0;
            } else {
                p.s -= amount;
            }
        }
    }
}

void Image::saturate() {
    unsigned int a = this->width();
    unsigned int b = this->height();
    for (unsigned int i = 0; i < a; i++) {
        for (unsigned int j = 0; j < b; j++) {
            HSLAPixel &p = this->getPixel(i,j);
            if (p.s + 0.1 > 1) {
                p.s = 1;
            } else {
                p.s += 0.1;
            }
        }
    }
}
void Image::saturate(double amount) {
    unsigned int a = this->width();
    unsigned int b = this->height();
    for (unsigned int i = 0; i < a; i++) {
        for (unsigned int j = 0; j < b; j++) {
            HSLAPixel &p = this->getPixel(i,j);
            if (p.s + amount > 1) {
                p.s = 1;
            } else {
                p.s += amount;
            }
        }
    }
}

void Image::grayscale() {
    unsigned int a = this->width();
    unsigned int b = this->height();
    for (unsigned int i = 0; i < a; i++) {
        for (unsigned int j = 0; j < b; j++) {
            HSLAPixel &p = this->getPixel(i,j);
            p.s = 0;
        }
    }
}

void Image::illinify() {
    unsigned int a = this->width();
    unsigned int b = this->height();
    for (unsigned int i = 0; i < a; i++) {
        for (unsigned int j = 0; j < b; j++) {
            HSLAPixel &p = this->getPixel(i,j);
            if (p.h >= 113.5 && p.h < 293.5) {
                p.h = 216;
            } else {
                p.h = 11;
            }
        }
    }
}

void Image::rotateColor(double degrees) {
    unsigned int a = this->width();
    unsigned int b = this->height();
    for (unsigned int i  = 0; i < a; i++) {
        for (unsigned int j = 0; j < b; j++) {
            HSLAPixel &p = this->getPixel(i,j);
            if (p.h + degrees > 360) {
                p.h = p.h + degrees - 360;
            } else if (p.h + degrees < 0) {
                p.h = p.h + degrees + 360;
            } else {
                p.h += degrees;
            }
        }
    }
}
        
void Image::scale(double factor) {
    unsigned int w = width() * factor;
    unsigned int h = height() * factor;

    Image * im = new Image();
	*im = *this;
	resize(w, h);

    for (unsigned int i = 0; i < w; i++) {
		for (unsigned int j = 0; j < h; j++) {
			HSLAPixel &p = getPixel(i, j);
			HSLAPixel &p2 = im->getPixel(i / factor, j / factor);
			p = p2;
        }
	}
	delete im;
}

void Image::scale(unsigned w, unsigned h) {
	double a = (double)w / (double)width();
	double b = (double)h / (double)height();

	if (a > b) {
        scale(b);
    } else {
        scale(a);
    }
}