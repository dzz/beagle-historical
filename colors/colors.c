#include "colors.h"

cp_color rgb_from_dbl(double r,double g, double b) {
	cp_color ret;
	ret.r = (unsigned char)(r*255);
	ret.g = (unsigned char)(g*255);
	ret.b = (unsigned char)(b*255);
	ret.a = 255;
	return ret;
}

cp_color rgb_from_hsv(double h, double s, double v)
{
    double c = 0.0, m = 0.0, x = 0.0;
    {
        c = v * s;
        x = c * (1.0 - fabs(fmod(h / 60.0, 2) - 1.0));
        m = v - c;
        if (h >= 0.0 && h < 60.0)
        {
            return rgb_from_dbl(c + m, x + m, m);
        }
        else if (h >= 60.0 && h < 120.0)
        {
            return rgb_from_dbl(x + m, c + m, m);
        }
        else if (h >= 120.0 && h < 180.0)
        {
            return rgb_from_dbl(m, c + m, x + m);
        }
        else if (h >= 180.0 && h < 240.0)
        {
            return rgb_from_dbl(m, x + m, c + m);
        }
        else if (h >= 240.0 && h < 300.0)
        {
            return rgb_from_dbl(x + m, m, c + m);
        }
        else if (h >= 300.0 && h < 360.0)
        {
            return rgb_from_dbl(c + m, m, x + m);
        }
        else
        {
            return rgb_from_dbl(m, m, m);
        }
    }
    return rgb_from_dbl(0,0,0);
}
