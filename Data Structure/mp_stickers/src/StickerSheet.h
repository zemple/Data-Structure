/**
 * @file StickerSheet.h
 * Contains your declaration of the interface for the StickerSheet class.
 */
#pragma once
#include "Image.h"
#include "cs225/PNG.h"
#include "cs225/HSLAPixel.h"
 
using namespace std;

class StickerSheet {
    public:
        StickerSheet (const Image &picture, unsigned max);
        StickerSheet (const StickerSheet &other);
        const StickerSheet & operator= (const StickerSheet &other);
        void changeMaxStickers (unsigned max);
        int addSticker (Image &sticker, int x, int y);
        int setStickerAtLayer (Image &sticker, unsigned layer, int x, int y);
        bool translate (unsigned index, int x, int y);
        void removeSticker (unsigned index);
        Image * getSticker (unsigned index);
        int layers () const;
        Image render () const;

        ~StickerSheet();
        void copy(const StickerSheet & other);
        void clear();

    private:
        Image base_;
        unsigned max_;
        Image** sheets_;
        unsigned num_;
        int* x_;
        int* y_;
};

