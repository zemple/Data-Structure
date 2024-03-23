#include "StickerSheet.h"
using namespace cs225;


StickerSheet::StickerSheet (const Image &picture, unsigned max) {
    max_ = max;
    base_ = picture;
    num_ = 0;
    sheets_ = new Image*[max];
    x_ = new int[max];
    y_ = new int[max];
    for (unsigned i = 0; i < max; i++) {
        sheets_[i] = NULL;
        x_[i] = 0;
        y_[i] = 0;
    }
}

StickerSheet::StickerSheet (const StickerSheet &other) {
    copy(other);
}

const StickerSheet & StickerSheet::operator= (const StickerSheet &other) {
    if (this != &other) {
        clear();
        copy(other);
    }
    return *this;
}

void StickerSheet::changeMaxStickers (unsigned max) {
    Image** new_sheets_ = new Image*[max];
    for (unsigned i = 0; i < max; i++) {
        new_sheets_[i] = sheets_[i];
    }
    if (max < num_) {
        num_ = max;
    }
    delete[] sheets_;
    sheets_ = new_sheets_;
    max_ = max;
    new_sheets_ = NULL;
}

int StickerSheet::addSticker (Image &sticker, int x, int y) {
    for (unsigned i = 0; i < max_; i++) {
        if (i == num_) {
            sheets_[i] = &sticker;
            x_[i] = x;
            y_[i] = y;
            num_++;
            return i;
        }
    }
    changeMaxStickers (max_ + 1);
    sheets_[num_] = &sticker;
    x_[num_] = x;
    y_[num_] = y;
    num_++;
    return num_ - 1;
}

int StickerSheet::setStickerAtLayer (Image &sticker, unsigned layer, int x, int y) {
    if (layer >= num_) {
        return -1;
    }
    sheets_[layer] = &sticker;
    x_[layer] = x;
    y_[layer] = y;
    return layer;
}

bool StickerSheet::translate (unsigned index, int x, int y) {
    if (index >= num_) {
        return false;
    } else {
        x_[index] = x;
        y_[index] = y;
        return true;
    }
}

void StickerSheet::removeSticker (unsigned index) {
    if (index < num_) {
        for (unsigned i = index; i < num_ - 1; i++) {
            sheets_[i] = sheets_[i+1];
            x_[i] = x_[i+1];
            y_[i] = y_[i+1];
        }
        sheets_[num_ - 1] = NULL;
        x_[num_ - 1] = 0;
        y_[num_ - 1] = 0;
        num_--;
    }
}

Image * StickerSheet::getSticker (unsigned index) {
    if (index < num_) {
        return sheets_[index];
    }
    return NULL;
}

int StickerSheet::layers () const {
    return max_;
}

Image StickerSheet::render () const {
    int x_negative = 0;
    int y_negative = 0;
    for(unsigned i = 0; i < num_; i++) {
        if(x_[i] < x_negative) {
            x_negative = x_[i];
        }
        if(y_[i] < y_negative) {
            y_negative = y_[i];
        }
    }

    int pre_width = base_.width();
    int pre_height = base_.height();
    int final_width = pre_width;
    int final_height = pre_height;
    Image new_base(pre_width, pre_height);
    for (unsigned i = 0; i < num_; i++) {
        if ((x_[i] + (int)sheets_[i]->width() - x_negative) > final_width) {
            final_width = x_[i] + (int)sheets_[i]->width() - x_negative;
        }
        if ((y_[i] + (int)sheets_[i]->height() - y_negative) > final_height) {
            final_height = y_[i] + (int)sheets_[i]->height() - y_negative;
        }
    }

    new_base.resize(final_width - x_negative, final_height - y_negative);
    for(int x = 0; x < pre_width; x++) {
        for(int y = 0; y < pre_height; y++) {
            cs225::HSLAPixel &pixel = new_base.getPixel(x - x_negative, y - y_negative);
            const cs225::HSLAPixel base_pixel = base_.getPixel(x, y);
            pixel = base_pixel;
        }
    }

    for (unsigned i = 0; i < num_; i++) {
        for (unsigned x = 0; x < sheets_[i]->width(); x++) {
            for (unsigned y = 0; y < sheets_[i]->height(); y++) {
                const cs225::HSLAPixel &pixel = sheets_[i]->getPixel(x, y);
                cs225::HSLAPixel &base_pixel = new_base.getPixel(x + x_[i] - x_negative, y + y_[i] -y_negative);
                if (pixel.a != 0) {
                    base_pixel = pixel;
                }
            }
        }
    }

    return new_base;
}

StickerSheet::~StickerSheet() {
    clear();
}

void StickerSheet::copy(const StickerSheet & other) {
    max_ = other.max_;
    num_ = other.num_;
    base_ = other.base_;
    sheets_ = new Image *[max_];
    x_ = new int[max_];
    y_ = new int[max_];
    for (unsigned i = 0; i < num_; i++) {
        (sheets_[i]) = (other.sheets_[i]);
        x_[i] = other.x_[i];
        y_[i] = other.y_[i];
    }
}

void StickerSheet::clear() {
    delete[] sheets_;
    sheets_ = NULL;
    delete[] x_;
    x_ = NULL;
    delete[] y_;
    y_ = NULL;
}
