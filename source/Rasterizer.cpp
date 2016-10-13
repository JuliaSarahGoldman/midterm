/** \file Rasterizer.cpp */
#include "Rasterizer.h"
#include <stack>

Rasterizer::Rasterizer() {};
Rasterizer::~Rasterizer() {};

bool Rasterizer::inBounds(int x, int y, const shared_ptr<Image>& image) const {
    int height(image->height());
    int width(image->width());
    return x >= 0 && y >= 0 && x < width && y < height;
};

void Rasterizer::setPixel(int x, int y, const Color4& c, shared_ptr<Image>& image) const {
    if (inBounds(x, y, image)) {
        image->set(x, y, c);
    }
}


void Rasterizer::drawLine(const Point2int32& point1, const Point2int32& point2, const Color4& c, shared_ptr<Image>& image) const {
    drawLine(point1, point2, 0, c, image, Color4(1, 1, 1), shared_ptr<Image>(Image::create(2, 2, ImageFormat::RGB32F())));
};

void Rasterizer::drawLine(const Point2int32& point1, const Point2int32& point2, int offset, const Color4& c, shared_ptr<Image>& image, const Color4& shade, shared_ptr<Image>& map) const {
    int x0(min<int>(point1.x, point2.x));
    int x1(max<int>(point1.x, point2.x));
    int y0(min<int>(point1.y, point2.y));
    int y1(max<int>(point1.y, point2.y));

    if (x0 == x1) {
        drawVLine(x0 + offset, y0, y1, c, image, shade, map);
    }
    else if (y0 == y1) {
        drawHLine(x0, x1, y0 + offset, c, image, shade, map);
    }
    else {
        float m = (point2.y - point1.y) / (point2.x - point1.x);
        if (fabs(m) <= 1) {

            //drawFlatLine(point1, point2, offset, c, image, shade, map);
        }
        else {
            // drawSteepLine(point1, point2, offset, c, image, shade, map);
        }
    }
}

void Rasterizer::drawVLine(int x, int y0, int y1, const Color4& c, shared_ptr<Image>& image, const Color4& shade, shared_ptr<Image>& map) const {
    for (int y = y0; y <= y1; ++y) {
        setPixel(x, y, c, image);
        setPixel(x, y, shade, map);
    }

}

void Rasterizer::drawHLine(int x0, int x1, int y, const Color4& c, shared_ptr<Image>& image, const Color4& shade, shared_ptr<Image>& map) const {
    for (int x = x0; x <= x1; ++x) {
        setPixel(x, y, c, image);
        setPixel(x, y, shade, map);
    }
}

/*void Rasterizer::drawFlatLine(const Point2int32& point1, const Point2int32& point2, int offset, const Color4& c, shared_ptr<Image>& image, const Color4& shade, shared_ptr<Image>& map) const {
    float y(offset); // Offset y by thick coordinate
    float x0(min<float>(point1.x, point2.x));
    float x1(max<float>(point1.x, point2.x));

    if (point2.x >= point1.x) { // Center y at appropriate coordinate
        y += point1.y;
    }
    else {
        y += point2.y;
    }

    float m = 1.0f * (point2.y - point1.y) / (point2.x - point1.x);


    for (int x = (int)x0; x <= x1; ++x, y += m) {
        setPixel(x, y, c, image);
        setPixel(x, y, shade, map);
    }
}*/

void Rasterizer::drawFlatLine(float y, float x0, float x1, float m, const Color4& c, shared_ptr<Image>& image, const Color4& shade, shared_ptr<Image>& map) const {
    for (int x = (int)x0; x <= x1; ++x, y += m) {
        setPixel(x, y, c, image);
        setPixel(x, y, shade, map);
    }
}

/*void Rasterizer::drawSteepLine(const Point2int32& point1, const Point2int32& point2, int offset, const Color4& c, shared_ptr<Image>& image, const Color4& shade, shared_ptr<Image>& map) const {
    float x(offset); // Offset x by thick coordinate
    float y0(min<float>(point1.y, point2.y));
    float y1(max<float>(point1.y, point2.y));

    if (point2.y >= point1.y) { // Center x at appropriate coordinate
        x += point1.x;
    }
    else {
        x += point2.x;
    }

    // Inverse slope
    float m_i = 1.0f * (point2.x - point1.x) / (point2.y - point1.y);

    for (int y = (int)y0; y <= y1; ++y, x += m_i) {
        setPixel(x, y, c, image);
        setPixel(x, y, shade, map);
    }
}*/

void Rasterizer::drawSteepLine(float x, float y0, float y1, float m_i, const Color4& c, shared_ptr<Image>& image, const Color4& shade, shared_ptr<Image>& map) const {
    for (int y = (int)y0; y <= y1; ++y, x += m_i) {
        setPixel(x, y, c, image);
        setPixel(x, y, shade, map);
    }
}

void Rasterizer::drawThickLine(const Point2int32& point1, const Point2int32& point2, const Color4& c, int halfGirth, shared_ptr<Image>& image, shared_ptr<Image>& map) const {
    int x0(min<int>(point1.x, point2.x));
    int x1(max<int>(point1.x, point2.x));
    int y0(min<int>(point1.y, point2.y));
    int y1(max<int>(point1.y, point2.y));

    float m(0.0f);

    int t;
    if ((x0 == x1) || (y0 == y1)) {
        t = halfGirth;
    }
    else {
        Vector2 d(point2 - point1);
        float d_x(fabs(d.direction().x));
        float d_y(fabs(d.direction().y));

        m = float(point2.y - point1.y) / float(point2.x - point1.x);
        t = fabs(m) < 1.1f ? abs(halfGirth / d_x) : abs(halfGirth / d_y);
    }

    Color4 shade(0, 0, 0);
    Color4 increment(1.0f / float(t), 1.0f / float(t), 1.0f / float(t), 1.0);

    for (int i(-t); i <= t; ++i) {
        if (x0 == x1) {
            drawVLine(x0 + i, y0, y1, c, image, shade, map);

        }
        else if (y0 == y1) {
            drawHLine(x0, x1, y0 + i, c, image, shade, map);

        }
        else {
            if (fabs(m) < 1.1f) {
                // drawFlatLine(point1, point2, i, c, image, shade, map);

                float y(x1 == point2.x ? point1.y : point2.y); // center x at appropriate coordinate
                drawFlatLine(y + i, x0, x1, m, c, image, shade, map); // offset x by i
            }
            else {
                //drawSteepLine(point1, point2, i, c, image, shade, map);

                float x(y1 == point2.y ? point1.x : point2.x); // center x at appropriate coordinate
                drawSteepLine(x + i, y0, y1, float(1.0f / m), c, image, shade, map); // offset x by i
            }

        }
        shade -= increment*sign(1.0f*i);
    }

    roundCorners(Point2(point1.x, point1.y), Point2(point2.x, point2.y), t + 0.5f, m, c, image, map); // center adds 1 to total girth 
    //roundCorners(point2, t + 0.5f, m, c, image, shade, increment, map);
}

void Rasterizer::roundCorners(const Point2& c0, const Point2& c1, float r, float m, const Color4& c, shared_ptr<Image>& image, shared_ptr<Image>& map) const {
    // Box around p0
    int x0_i(c0.x - r); // Initial x
    int x0_f(c0.x + r); // Final x
    int y0_i(c0.y - r);
    int y0_f(c0.y + r);

    // Box around p1 
    int x1_i(c1.x - r);
    int x1_f(c1.x + r);
    int y1_i(c1.y - r);
    int y1_f(c1.y + r);


    LineSegment2D centerLine(LineSegment2D::fromTwoPoints(c0, c1));

    int offX(0); // potential x offset along steep slope

    float incX(0); // To update x offset according to slope within loop 
    float incY(0); // To update y offset according to slope within loop

    if (m != 0.0f) {
        if (fabs(m) < 1.1f) {
            incY = m;
        }
        else {
            incX = (1.0f / m);
        }
    }
    
    int w0(x0_i);
    int w1(x1_i);
    while (w0 <= x0_f && w1 <= x1_f) {
        int offY(0); // potential y offset along flat slope

        int curX0(w0 + offX);
        int curX1(w1 + offX);

        int h0(y0_i);
        int h1(y1_i);
        while (h0 <= y0_f && h1 <= y1_f) {
            int curY0(h0 + offY);
            int curY1(h1 + offY);

            Point2 P0(curX0, curY0);
            Point2 P1(curX1, curY1);

            if ((P0 - c0).length() <= r) {
                setPixel(curX0, curY0, c, image);

                Color4 curCol(0, 0, 0, 1);
                map->get(Point2int32(curX0, curY0), curCol);


                float cValue(1.0f - fabs(centerLine.distance(P0) / r));
                Color4 shade(cValue, cValue, cValue, 1.0);
                setPixel(curX0, curY0, shade.max(curCol), map);
            }

            if ((P1 - c1).length() <= r) {
                setPixel(curX1, curY1, c, image);
                
                Color4 curCol(0, 0, 0, 1);
                map->get(Point2int32(curX1, curY1), curCol);

                float cValue(1.0f - fabs(centerLine.distance(P1) / r));
                Color4 shade(cValue, cValue, cValue, 1.0);
                setPixel(curX1, curY1, shade.max(curCol), map);

            }
            offY += incY;
            ++h0; ++h1;
        }
        offX += incX;
        ++w0; ++w1;
    }


    /*   for (int x = x0_0; x <= x0_1; ++x) {
           int curX0(x + offX);

           int offY(0); // potential y offset along flat slope
           for (int y = y0; y <= y1; ++y) {
               int curY(y + offY);

               Point2int32 P(curX, curY);
               Vector2 v(P - C);
               if (v.length() <= r) {
                   setPixel(curX, curY, c, image);
                   setPixel(curX, curY, shade, map);
               }
               shade += curY < C.y ? shadeInc : -shadeInc;
               offY += incY;
           }
           offX += incX;
       }*/
};


void Rasterizer::drawGradiantBackground(const Color4& c1, const Color4& c2, int height, int width, shared_ptr<Image>& image) const {
    Color4 current = c1;
    for (int y = 0; y < height; ++y) {
        current = current + (c2 - c1) / (height - 1);
        drawLine(Point2int32(0, y), Point2int32(width - 1, y), current, image);
    }
}

void Rasterizer::drawAxes(int rad, int rng, int xOff, int yOff, shared_ptr<Image>& image, shared_ptr<Image>& map) const {
    drawThickLine(Point2int32(0 + xOff, 10 * rng + yOff), Point2int32(20 * rad + xOff, 10 * rng + yOff), Color4(1, 0, 0), 5, image, map);
    drawThickLine(Point2int32(10 * rad + xOff, 0 + yOff), Point2int32(10 * rad + xOff, 20 * rng + yOff), Color4(1, 0, 0), 5, image, map);
    for (int x = 0; x <= 2 * rad; ++x) {
        drawThickLine(Point2int32(xOff + x * 10, yOff + rng * 10 - 2), Point2int32(xOff + x * 10, yOff + rng * 10 + 2), Color4(1, 0, 0), 0, image, map);
    }
    for (int y = 0; y <= 2 * rng; ++y) {
        drawThickLine(Point2int32(xOff + rad * 10 - 2, yOff + y * 10), Point2int32(xOff + rad * 10 + 2, yOff + y * 10), Color4(1, 0, 0), 0, image, map);
    }
}

void Rasterizer::drawVarGraph(bool isClock, int width, int height, int xOff, int yOff, int sign, shared_ptr<Image>& image) const {
    float realX = -1 * width;
    float realY = 0;
    Point2int32 p1 = Point2int32(-1, -1);
    Point2int32 p2 = Point2int32(-1, -1);
    for (int x = xOff; x <= 20 * width + xOff; ++x) {
        if (isClock) {
            realY = sign * sqrt(36 - realX*realX);
        }
        else {
            realY = .13 * realX * realX * realX;
        }
        float pixY = 10 * height;
        if (realY > 0) {
            pixY = 10 * height - 10 * realY;
        }
        if (realY < 0) {
            pixY = 10 * height - 10 * realY;
        }
        pixY += yOff;
        p2 = Point2int32(1.0f * x, pixY);
        if (p1.x != -1) {
            drawLine(p1, p2, Color4(0, 0, 0), image);
        }
        p1 = p2;
        realX += .1;
    }
}

void Rasterizer::drawMyGraph(shared_ptr<Image>& image) const {
    //drawAxes(4, 10, 5, 5, image);
    drawVarGraph(false, 4, 10, 5, 5, 1, image);
}

void Rasterizer::drawClock(shared_ptr<Image>& image) const {
    Point2int32 center = Point2int32(210, 65);
    drawVarGraph(true, 6, 6, 150, 5, 1, image);
    drawVarGraph(true, 6, 6, 150, 5, -1, image);
    float rotation = 0;
    int time = 0;
    Point2int32 outer = Point2int32((float)(1.0f*center.x + (50.0f * cos(rotation))), (float)(1.0f*center.y + (50.0f * sin(rotation))));
    drawLine(center, outer, Color4(1, 0, 0), image);
    if (time % 5 == 0) {
        rotation += (3.14159 / 12);
    }
}

void Rasterizer::drawCantorDust(float xSt, float xEn, float y, int level, shared_ptr<Image>& image) const {
    drawLine(Point2int32(xSt, y), Point2int32(xEn, y), Color4(0, 0, 1), image);
    if (level > 1) {
        drawCantorDust(xSt, (xEn - xSt) / 3 + xSt, y + 10, level - 1, image);
        drawCantorDust(2.0 * (xEn - xSt) / 3 + xSt, xEn, y + 10, level - 1, image);
    }
}