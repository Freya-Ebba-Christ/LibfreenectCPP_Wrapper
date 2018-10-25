/* 
 *    Copyright [2012] Olaf - blinky0815 - christ ]
 *    Licensed under the Apache License, Version 2.0 (the "License");
 *    you may not use this file except in compliance with the License.
 *    You may obtain a copy of the License at
 * 
 *      http://www.apache.org/licenses/LICENSE-2.0
 * 
 *    Unless required by applicable law or agreed to in writing, software
 *    distributed under the License is distributed on an "AS IS" BASIS,
 *    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *    See the License for the specific language governing permissions and
 *    limitations under the License.
 */

#ifndef COLORSPACECONVERTER_H
#define	COLORSPACECONVERTER_H

#include <math.h>

class ColorspaceConverter {
public:

    ColorspaceConverter() {
    }

    ColorspaceConverter(const ColorspaceConverter& orig) {
    }

    ~ColorspaceConverter() {
    }

    void initialize() {
        for (int i = 0; i < sizeof (xyzPow) / sizeof (xyzPow[0]); i++) {
            float tmp = i / 255.0;

            if (tmp <= 0.04045) {
                xyzPow[i] = tmp / 12;
            } else {
                xyzPow[i] = (float) pow((tmp + 0.055) / 1.055, 2.4);
            }
        }
    }

    inline int* getColor() {
        return color;
    }

    inline void rgb2luv(int red, int green, int blue) {
        //http://www.brucelindbloom.com

        float rf, gf, bf;
        float r, g, b, X_, Y_, Z_, X, Y, Z, fx, fy, fz, xr, yr, zr;
        float L;
        float eps = 216.f / 24389.f;
        float k = 24389.f / 27.f;

        float Xr = 0.964221f; // reference white D50
        float Yr = 1.0f;
        float Zr = 0.825211f;

        // RGB to XYZ

        r = red / 255.f; //R 0..1
        g = green / 255.f; //G 0..1
        b = blue / 255.f; //B 0..1

        // assuming sRGB (D65)
        if (r <= 0.04045) {
            r = r / 12;
        } else {
            r = (float) pow((r + 0.055) / 1.055, 2.4);
        }
        if (g <= 0.04045) {
            g = g / 12;
        } else {
            g = (float) pow((g + 0.055) / 1.055, 2.4);
        }
        if (b <= 0.04045) {
            b = b / 12;
        } else {
            b = (float) pow((b + 0.055) / 1.055, 2.4);
        }
        X = 0.436052025f * r + 0.385081593f * g + 0.143087414f * b;
        Y = 0.222491598f * r + 0.71688606f * g + 0.060621486f * b;
        Z = 0.013929122f * r + 0.097097002f * g + 0.71418547f * b;

        // XYZ to Luv

        float u, v, u_, v_, ur_, vr_;

        u_ = 4 * X / (X + 15 * Y + 3 * Z);
        v_ = 9 * Y / (X + 15 * Y + 3 * Z);

        ur_ = 4 * Xr / (Xr + 15 * Yr + 3 * Zr);
        vr_ = 9 * Yr / (Xr + 15 * Yr + 3 * Zr);

        yr = Y / Yr;

        if (yr > eps) {
            L = (float) (116 * pow(yr, 1 / 3.) - 16);
        } else {
            L = k * yr;
        }
        u = 13 * L * (u_ - ur_);
        v = 13 * L * (v_ - vr_);

        color[0] = (int) (2.55 * L + .5);
        color[1] = (int) (u + .5);
        color[2] = (int) (v + .5);
    }

    inline void rgb2lab(int red, int green, int blue) {
        //http://www.brucelindbloom.com

        float r, g, b, X, Y, Z, fx, fy, fz, xr, yr, zr;
        float Ls, as, bs;
        float eps = 216.f / 24389.f;
        float k = 24389.f / 27.f;

        float Xr = 0.964221f; // reference white D50
        float Yr = 1.0f;
        float Zr = 0.825211f;

        // RGB to XYZ
        r = red / 255.f; //R 0..1
        g = green / 255.f; //G 0..1
        b = blue / 255.f; //B 0..1

        // assuming sRGB (D65)
        if (r <= 0.04045) {
            r = r / 12;
        } else {
            r = (float) pow((r + 0.055) / 1.055, 2.4);
        }
        if (g <= 0.04045) {
            g = g / 12;
        } else {
            g = (float) pow((g + 0.055) / 1.055, 2.4);
        }
        if (b <= 0.04045) {
            b = b / 12;
        } else {
            b = (float) pow((b + 0.055) / 1.055, 2.4);
        }
        X = 0.436052025f * r + 0.385081593f * g + 0.143087414f * b;
        Y = 0.222491598f * r + 0.71688606f * g + 0.060621486f * b;
        Z = 0.013929122f * r + 0.097097002f * g + 0.71418547f * b;

        // XYZ to Lab
        xr = X / Xr;
        yr = Y / Yr;
        zr = Z / Zr;

        if (xr > eps) {
            fx = (float) pow(xr, 1 / 3.);
        } else {
            fx = (float) ((k * xr + 16.) / 116.);
        }
        if (yr > eps) {
            fy = (float) pow(yr, 1 / 3.);
        } else {
            fy = (float) ((k * yr + 16.) / 116.);
        }
        if (zr > eps) {
            fz = (float) pow(zr, 1 / 3.);
        } else {
            fz = (float) ((k * zr + 16.) / 116);
        }
        Ls = (116 * fy) - 16;
        as = 500 * (fx - fy);
        bs = 200 * (fy - fz);

        color[0] = (int) (2.55 * Ls + .5);
        color[1] = (int) (as + .5);
        color[2] = (int) (bs + .5);
    }

    inline void rgb2xyz(int red, int green, int blue) {
        float x, y, z;
        /*
        float rf, gf, bf;
        float r, g, b, x, y, z;
        
        r = red / 255.f; //R 0..1
        g = green / 255.f; //G 0..1
        b = blue / 255.f; //B 0..1
         */

        /*
        if (r <= 0.04045) {
        r = r / 12;
        } else {
        r = (float) Math.pow((r + 0.055) / 1.055, 2.4);
        }
        if (g <= 0.04045) {
        g = g / 12;
        } else {
        g = (float) Math.pow((g + 0.055) / 1.055, 2.4);
        }
        if (b <= 0.04045) {
        b = b / 12;
        } else {
        b = (float) Math.pow((b + 0.055) / 1.055, 2.4);
        }
         */
        x = 0.436052025f * xyzPow[red] + 0.385081593f * xyzPow[green] + 0.143087414f * xyzPow[blue];
        y = 0.222491598f * xyzPow[red] + 0.71688606f * xyzPow[green] + 0.060621486f * xyzPow[blue];
        z = 0.013929122f * xyzPow[red] + 0.097097002f * xyzPow[green] + 0.71418547f * xyzPow[blue];

        color[0] = (int) (255 * x + .5);
        color[1] = (int) (255 * y + .5);
        color[2] = (int) (255 * z + .5);
    }

    inline void rgb2xyY(int red, int green, int blue) {
        //http://www.brucelindbloom.com

        float rf, gf, bf;
        float r, g, b, X, Y, Z;

        // RGB to XYZ
        r = red / 255.f; //R 0..1
        g = green / 255.f; //G 0..1
        b = blue / 255.f; //B 0..1

        if (r <= 0.04045) {
            r = r / 12;
        } else {
            r = (float) pow((r + 0.055) / 1.055, 2.4);
        }
        if (g <= 0.04045) {
            g = g / 12;
        } else {
            g = (float) pow((g + 0.055) / 1.055, 2.4);
        }
        if (b <= 0.04045) {
            b = b / 12;
        } else {
            b = (float) pow((b + 0.055) / 1.055, 2.4);
        }
        X = 0.436052025f * r + 0.385081593f * g + 0.143087414f * b;
        Y = 0.222491598f * r + 0.71688606f * g + 0.060621486f * b;
        Z = 0.013929122f * r + 0.097097002f * g + 0.71418547f * b;

        float x;
        float y;

        float sum = X + Y + Z;
        if (sum != 0) {
            x = X / sum;
            y = Y / sum;
        } else {
            float Xr = 0.964221f; // reference white
            float Yr = 1.0f;
            float Zr = 0.825211f;

            x = Xr / (Xr + Yr + Zr);
            y = Yr / (Xr + Yr + Zr);
        }

        color[0] = (int) (255 * x + .5);
        color[1] = (int) (255 * y + .5);
        color[2] = (int) (255 * Y + .5);
    }

    inline void rgb2hsv(int red, int green, int blue) {
        int min; //Min. value of RGB
        int max; //Max. value of RGB
        int delMax; //Delta RGB value

        if (red > green) {
            min = green;
            max = red;
        } else {
            min = red;
            max = green;
        }
        if (blue > max) {
            max = blue;
        }
        if (blue < min) {
            min = blue;
        }
        delMax = max - min;

        float H = 0, S;
        float V = max;

        if (delMax == 0) {
            H = 0;
            S = 0;
        } else {
            S = delMax / 255.0;
            if (red == max) {
                H = ((green - blue) / (float) delMax) * 60;
            } else if (green == max) {
                H = (2 + (blue - red) / (float) delMax) * 60;
            } else if (blue == max) {
                H = (4 + (red - green) / (float) delMax) * 60;
            }
        }

        color[0] = (int) (H);
        color[1] = (int) (S * 100);
        color[2] = (int) (V * 100);
    }

    inline void rgb2hsl(int red, int green, int blue) {
        float var_R = (red / 255.0);
        float var_G = (green / 255.0);
        float var_B = (blue / 255.0);

        float var_Min; //Min. value of RGB
        float var_Max; //Max. value of RGB
        float del_Max; //Delta RGB value

        if (var_R > var_G) {
            var_Min = var_G;
            var_Max = var_R;
        } else {
            var_Min = var_R;
            var_Max = var_G;
        }

        if (var_B > var_Max) {
            var_Max = var_B;
        }
        if (var_B < var_Min) {
            var_Min = var_B;
        }
        del_Max = var_Max - var_Min;

        float H = 0, S, L;
        L = (var_Max + var_Min) / 2.0;

        if (del_Max == 0) {
            H = 0;
            S = 0;
        }// gray
        else { //Chroma
            if (L < 0.5) {
                S = del_Max / (var_Max + var_Min);
            } else {
                S = del_Max / (2 - var_Max - var_Min);
            }
            float del_R = (((var_Max - var_R) / 6.0) + (del_Max / 2.0)) / del_Max;
            float del_G = (((var_Max - var_G) / 6.0) + (del_Max / 2.0)) / del_Max;
            float del_B = (((var_Max - var_B) / 6.0) + (del_Max / 2.0)) / del_Max;

            if (var_R == var_Max) {
                H = del_B - del_G;
            } else if (var_G == var_Max) {
                H = (1 / 3.0) + del_R - del_B;
            } else if (var_B == var_Max) {
                H = (2 / 3.0) + del_G - del_R;
            }
            if (H < 0) {
                H += 1;
            }
            if (H > 1) {
                H -= 1;
            }
        }
        color[0] = (int) (360 * H);
        color[1] = (int) (S * 100);
        color[2] = (int) (L * 100);
    }

    inline void rgb2yuv(int red, int green, int blue) {
        int y = (int) (0.299 * red + 0.587 * green + 0.114 * blue);
        int u = (int) ((blue - y) * 0.492f);
        int v = (int) ((red - y) * 0.877f);

        color[0] = y;
        color[1] = u;
        color[2] = v;
    }
    
    inline void rgb2ycbcr(int red, int green, int blue) {
        int y = (int) (0.299 * red + 0.587 * green + 0.114 * blue);
        int cb = (int) (-0.16874 * red - 0.33126 * green + 0.50000 * blue);
        int cr = (int) (0.50000 * red - 0.41869 * green - 0.08131 * blue);

        color[0] = y;
        color[1] = cb;
        color[2] = cr;
    }

    inline void rgb2lms(int red, int green, int blue) {
        /*
    rgb2xyz(red, green, blue);
        
    int x = color[0];
    int y = color[1];
    int z = color[2];
        
    int l = (int) (0.8951 * x + 0.2664 * y + -0.1614 * z);
    int m = (int) (-0.7502 * x + 1.7135 * y + 0.0367 * z);
    int s = (int) (0.0389 * x - 0.0685 * y + 1.0296 * z);
        
    color[0] = l;
    color[1] = m;
    color[2] = s;
         */
        float x, y, z;
        /*    
        float rf, gf, bf;
        float r, g, b, X, Y, Z;
        
        r = red / 255.f; //R 0..1
        g = green / 255.f; //G 0..1
        b = blue / 255.f; //B 0..1
        
        if (r <= 0.04045) {
        r = r / 12;
        } else {
        r = (float) Math.pow((r + 0.055) / 1.055, 2.4);
        }
        if (g <= 0.04045) {
        g = g / 12;
        } else {
        g = (float) Math.pow((g + 0.055) / 1.055, 2.4);
        }
        if (b <= 0.04045) {
        b = b / 12;
        } else {
        b = (float) Math.pow((b + 0.055) / 1.055, 2.4);
        }
         */
        x = 0.4473f * xyzPow[red] + 0.52f * xyzPow[green] + 0.029f * xyzPow[blue];
        y = 0.0546f * xyzPow[red] + 0.9431f * xyzPow[green] + 0.0227f * xyzPow[blue];
        z = 0.0161f * xyzPow[red] + 0.0658f * xyzPow[green] + 0.7367f * xyzPow[blue];

        color[0] = (int) (255 * x + .5);
        color[1] = (int) (255 * y + .5);
        color[2] = (int) (255 * z + .5);
    }

    inline void rgb2rg(float red, float green, float blue) {
        color[0] = 0;
        color[1] = 0;
        color[2] = -1;

        if (red + blue + green != 0) {
            color[0] = (int) ((red / (red + green + blue)) * 255.0);
            color[1] = (int) ((green / (red + green + blue)) * 255.0);
            color[2] = -1;
        }
    }

    inline unsigned char clamp(int i) {
        if (i < 0) {
            i = 0;
        } else if (i > 255) {
            i = 255;
        }
        return (char) i;
    }

    inline void yvu420p_to_yuv444(unsigned char* src, unsigned char* dst, int width, int height) {
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                int pos = (y * width + x) * 3;

                dst[pos] = src[y * width + x];
                dst[pos + 1] = src[(y / 2) * (width / 2) + (x / 2) + width * height + width * height / 4];
                dst[pos + 2] = src[(y / 2) * (width / 2) + (x / 2) + width * height];
            }
        }
    }

    inline void yuy422_to_yuv444(unsigned char* src, unsigned char* dst, int width, int height) {
        for (int i = 0; i < width * height / 2; i++) {
            dst[i * 6] = src[i * 4];
            dst[i * 6 + 1] = src[i * 4 + 3];
            dst[i * 6 + 2] = src[i * 4 + 1];
            dst[i * 6 + 3] = src[i * 4 + 2];
            dst[i * 6 + 4] = src[i * 4 + 3];
            dst[i * 6 + 5] = src[i * 4 + 1];
        }
    }

    inline void yuy422_to_rgb(unsigned char* src, unsigned char* dst, int width, int height) {
        int Y, U, V, spos, dpos;

        for (int i = 0; i < width * height / 2; i++) {
            spos = i * 4;
            dpos = i * 6;

            Y = src[spos] - 16;
            U = src[spos + 3] - 128;
            V = src[spos + 1] - 128;

            dst[dpos] = clamp(1.164 * Y + 1.596 * V);
            dst[dpos + 1] = clamp(1.164 * Y - 0.813 * V - 0.391 * U);
            dst[dpos + 2] = clamp(1.164 * Y + 2.018 * U);

            Y = src[spos + 2] - 16;
            U = src[spos + 3] - 128;
            V = src[spos + 1] - 128;

            dst[dpos + 3] = clamp(1.164 * Y + 1.596 * V);
            dst[dpos + 4] = clamp(1.164 * Y - 0.813 * V - 0.391 * U);
            dst[dpos + 5] = clamp(1.164 * Y + 2.018 * U);
        }
    }

    inline void insitu_yuv444_to_rgb(unsigned char* buff, int width, int height) {
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                int pos = (y * width + x) * 3;

                int Y = buff[pos] - 16;
                int U = buff[pos + 1] - 128;
                int V = buff[pos + 2] - 128;

                buff[pos] = clamp(1.164 * Y + 1.596 * V);
                buff[pos + 1] = clamp(1.164 * Y - 0.813 * V - 0.391 * U);
                buff[pos + 2] = clamp(1.164 * Y + 2.018 * U);
            }
        }
    }

    inline void rgb2average(int red, int green, int blue) {
        int average = (red + green + blue) / 3.0;
        color[0] = average;
        color[1] = average;
        color[2] = average;
    }

private:
    int color[3];
    float xyzPow[256];
};

#endif	/* COLORSPACECONVERTER_H */

