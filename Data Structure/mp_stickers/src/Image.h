/**
 * @file Image.h
 * Contains your declaration of the interface for the Image class.
 */

#pragma once
#include "cs225/PNG.h"
#include "cs225/HSLAPixel.h"
#include <string>
using namespace std;

class Image : public cs225::PNG {
    private:

    public:

        Image();
        Image(unsigned int width, unsigned int height);

        void darken();
        void darken(double amount);

        void lighten();
        void lighten(double amount);

        void desaturate();
        void desaturate(double amount);

        void saturate();
        void saturate(double amount);

        void grayscale();

        void illinify();

        void rotateColor(double degrees);

        void scale(double factor);
        void scale(unsigned w, unsigned h);
};


