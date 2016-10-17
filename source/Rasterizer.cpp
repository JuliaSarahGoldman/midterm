/** \file Rasterizer.cpp */
#include "Rasterizer.h"
//#include <stack>

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

void Rasterizer::merge(const shared_ptr<Image>& q1, const shared_ptr<Image>& q2, const shared_ptr<Image>& q3, const shared_ptr<Image>& q4, shared_ptr<Image>& image) const {
    int width(image->width());
    int height(image->height());

    Thread::runConcurrently(Point2int32(0, 0), Point2int32(width, height), [&](Point2int32 pixel) {
        int x(pixel.x);
        int y(pixel.y);
        Color4 c(0, 0, 0, 0);

        switch (findQuadrant(x, y, width, height)) {
        case 1:
            if (inBounds(x, y, q1)) q1->get(pixel, c);
            break;
        case 2:
            if (inBounds(x - width / 2, y, q2)) q2->get(Point2int32(x - width / 2, y), c);
            break;
        case 3:
            if (inBounds(x, y - height / 2, q3))q3->get(Point2int32(x, y - height / 2), c);
            break;
        case 4:
            if (inBounds(x - width / 2, y - height / 2, q4))q4->get(Point2int32(x - width / 2, y - height / 2), c);
            break;
        }
        setPixel(x, y, c, image);
    });
};

int Rasterizer::findQuadrant(int x, int y, int width, int height) const {
    int a(x < width / 2 ? 1 : 2);
    int b(a + 2);
    return y < height / 2 ? a : b;
}

void Rasterizer::drawThickLine(const Point2int32& point1, const Point2int32& point2, const Color4& c, int halfGirth, shared_ptr<Image>& image, shared_ptr<Image>& map) const {
    drawThickLine(point1, point2, c, halfGirth, image, map, image->height());
}

void Rasterizer::drawThickLine(const Point2int32& point1, const Point2int32& point2, const Color4& c, int halfGirth, shared_ptr<Image>& image, shared_ptr<Image>& map, int gradientHeight) const {
    Point2 c0(point1.x, point1.y);
    Point2 c1(point2.x, point2.y);
    float r = halfGirth;

    int x0(min<float>(c0.x, c1.x) - r);
    int x1(max<float>(c0.x, c1.x) + r);
    int y0(min<float>(c0.y, c1.y) - r);
    int y1(max<float>(c0.y, c1.y) + r);

    LineSegment2D centerLine(LineSegment2D::fromTwoPoints(c0, c1));

    // What the color value is decremented by at each vertical level
    float div(1.5f*float(gradientHeight));
    Color4 decrement(c.r / div, c.g / div, c.b / div, 0.0);

    // The top pixel within the image bounds
    int top(y0 + r);

    for (int x = x0; x <= x1; ++x) {
        // Factor to compensate the original color by, depending on where the firs pixel is being drawn
        //float f = top < gradientHeight ? top : top - gradientHeight;
       // Color4 orig(c - f*decrement); // Compensate original color by factor

        for (int y(y0); y <= y1; ++y) {
            Point2 P(x, y);

            // Reset color past height to repeat gradient
            //if (y == gradientHeight) orig = c;

            // Only decrement color if y is within image bounds
            // if (y > 0) orig -= decrement;
      
            float alpha(float(y)/(1.5f*float(gradientHeight)));
            Color3 cur(c.rgb().lerp(Color3::black(), alpha));

            if (fabs(centerLine.distance(P)) < r + 0.1f) {
                setPixel(x, y, Color4(cur, 1.0), image);

                float cValue(1.0f - fabs(centerLine.distance(P) / r));
                Color4 shade(cValue, cValue, cValue, 1.0);

                setPixel(x, y, shade, map);

                // If we're at the end of a line segment, but not the end or start of coral
         /*       if (((x < x0 + r || x > x1 - r || y < y0 + r || y > y1 - r)) && !isEnd) {
                    // Smooth out the shading

                    Color4 curCol(0, 0, 0, 1);
                    if (inBounds(x, y, image)) {
                        map->get(Point2int32(x, y), curCol);
                    }
                    setPixel(x, y, shade.max(curCol), map);
                }
                else {
                    // Otherwise just draw the bump gradient normally
                    setPixel(x, y, shade, map);
                }*/
            }
        }
    }
}

void Rasterizer::drawGradiantBackground(const Color4& c0, const Color4& c1, int height, int width, shared_ptr<Image>& image) const {
    Thread::runConcurrently(0, height, [&](int x) {
        for (int y = 0; y < height; ++y) {
            float alpha(float(y)/(1.5f*height));
            setPixel(x, y, c0.lerp(c1,alpha), image);
        }
    });
}