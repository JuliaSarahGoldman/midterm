/** \file Rasterizer.cpp */
 /*  John Freeman
     Jose Rivas-Garcia
     Julia Goldman
     Matheus de Carvalho Souza
 */

#include "Rasterizer.h"

Rasterizer::Rasterizer() {};
Rasterizer::~Rasterizer() {};

bool Rasterizer::inBounds(int x, int y, const shared_ptr<Image>& image) const {
    int height(image->height());
    int width(image->width());
    return x >= 0 && y >= 0 && x < width && y < height;
};

void Rasterizer::adjustBounds(int& x0, int& x1, int& y0, int& y1, int width, int height) const {
    x0 = max<int>(0, x0);
    y0 = max<int>(0, y0);
    x1 = min<int>(width, x1);
    y1 = min<int>(height, y1);
};

void Rasterizer::setPixel(int x, int y, const Color4& c, shared_ptr<Image>& image) const {
    if (inBounds(x, y, image)) {
        image->set(x, y, c);
    }
};

Color4 Rasterizer::gradientColor(const Color4& c0, const Color4& c1, float a, int width, int height, const Point2int32& p0, const Point2int32& p1, const std::function<float (Point2int32, Point2int32, int, int)>& mix) const {
    float alpha(mix(p0,p1, width, height)); 
    return Color4(c0.lerp(c1,alpha).rgb(),a); 
};

float normalGradient(const Point2int32& p0, const Point2int32& p1, int width, int height){ 
    return float(p1.y)/(1.5f*float(height));
};

void Rasterizer::merge(const shared_ptr<Image>& q1, const shared_ptr<Image>& q2, const shared_ptr<Image>& q3, const shared_ptr<Image>& q4, shared_ptr<Image>& image) const {
    int width(image->width());
    int height(image->height());

    Thread::runConcurrently(Point2int32(0, 0), Point2int32(width, height), [&](Point2int32 pixel) {
        int x(pixel.x);
        int y(pixel.y);
        Color4 c(0, 0, 0, 0);

        switch (findQuadrant(x, y, width, height)) {
        case 1:
            q1->get(pixel, c);
            break;
        case 2:
            q2->get(Point2int32(x - (width / 2), y), c);
            break;
        case 3:
            q3->get(Point2int32(x, y - (height / 2)), c);
            break;
        case 4:
            q4->get(Point2int32(x - (width / 2), y - (height / 2)), c);
            break;
        }
        setPixel(x, y, c, image);
    });
};

int Rasterizer::findQuadrant(int x, int y, int width, int height) const {
    int a(x < (width / 2) ? 1 : 2);
    int b(a + 2);
    return y < (height / 2) ? a : b;
};

void Rasterizer::drawThickLine(const Point2int32& point1, const Point2int32& point2, const Color4& c, int halfGirth, shared_ptr<Image>& image, shared_ptr<Image>& map) const {
    Point2 c0(point1.x, point1.y);
    Point2 c1(point2.x, point2.y);
    LineSegment2D centerLine(LineSegment2D::fromTwoPoints(c0, c1));
    float r = halfGirth;

    // Bounding box coordinates
    int x0(min<float>(c0.x, c1.x) - r);
    int x1(max<float>(c0.x, c1.x) + r);
    int y0(min<float>(c0.y, c1.y) - r);
    int y1(max<float>(c0.y, c1.y) + r);
    adjustBounds(x0, x1, y0, y1, image->width(), image->height());

    const Color3 bump(Color3::white());

    // gradient divisor
    const float div(1.5f*float(image->height()));

    Thread::runConcurrently(Point2int32(x0, y0), Point2int32(x1, y1), [&](Point2int32 pixel) {
        int x(pixel.x);
        int y(pixel.y);

        Point2 P(x, y);
        float dist(fabs(centerLine.distance(P)));

        if (dist < r + 0.1f) {
            float alpha(float(y) / div);
            Color3 curCol(c.rgb().lerp(Color3::black(), alpha));
            //setPixel(x, y, Color4(curCol, 1.0f), image);
            image->set(x, y,Color4(curCol, 1.0f)); 

            Color3 curBump(bump.lerp(Color3::black(), fabs(dist / r)));
            //setPixel(x, y, Color4(curBump, 1.0f), map);
            map->set(x,y,Color4(curBump, 1.0f));

            // If we're at the end of a line segment, but not the end or start of coral
     /*       if (((x < x0 + r || x > x1 - r || y < y0 + r || y > y1 - r)) && !isEnd) {
                // Smooth out the shading

                Color4 curCol(0, 0, 0, 1);
                if (inBounds(x, y, image)) {
                    map->get(Point2int32(x, y), curCol);
                }
                setPixel(x, y, bump.max(curCol), map);
            }
            else {
                // Otherwise just draw the bump gradient normally
                setPixel(x, y, bump, map);
            }*/
        }
    });

};

void Rasterizer::drawGradiantBackground(const Color4& c0, const Color4& c1, int height, int width, shared_ptr<Image>& image) const {
    Thread::runConcurrently(Point2int32(0, 0), Point2int32(width, height), [&](Point2int32 pixel) {
        float alpha(float(pixel.y) / (1.5f*height));
        //setPixel(x, y, c0.lerp(c1, alpha), image);
        Color4 col(gradientColor(c0,c1,0.0f, width,height,Point2int32(0,0), pixel, normalGradient));
       // image->set(pixel.x, pixel.y, c0.lerp(c1, alpha));
        image->set(pixel.x, pixel.y, col);
    });
};