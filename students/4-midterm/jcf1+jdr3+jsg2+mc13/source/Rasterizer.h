/**
  \file Drawung.h
 */
 /*  John Freeman
     Jose Rivas-Garcia
     Julia Goldman
     Matheus de Carvalho Souza
 */

#pragma once
#include <G3D/G3DAll.h>

 /** \brief Application framework. */
class Rasterizer {
protected:
    bool inBounds(int x, int y, const shared_ptr<Image>& image) const;

    void setPixel(int x, int y, const Color4& c, shared_ptr<Image>& image) const; 

    void drawLine(const Point2int32& point1, const Point2int32& point2, int offset, const Color4& c, shared_ptr<Image>& image, const Color4& shade, shared_ptr<Image>& map) const;

    void drawHLine(int x0, int x1, int y, const Color4& c, shared_ptr<Image>& image, const Color4& shade, shared_ptr<Image>& map) const;

    void drawVLine(int x, int y0, int y1, const Color4& c, shared_ptr<Image>& image, const Color4& shade, shared_ptr<Image>& map) const;

    //void drawSteepLine(const Point2int32& point1, const Point2int32& point2, int offset, const Color4& c, shared_ptr<Image>& image, const Color4& shade, shared_ptr<Image>& map) const;

    void drawSteepLine(float x, float y0, float y1, float m_i, const Color4& c, shared_ptr<Image>& image, const Color4& shade, shared_ptr<Image>& map) const; 

    //void drawFlatLine(const Point2int32& point1, const Point2int32& point2, int offset, const Color4& c, shared_ptr<Image>& image, const Color4& shade, shared_ptr<Image>& map) const;

    void drawFlatLine(float y, float x0, float x1, float m, const Color4& c, shared_ptr<Image>& image, const Color4& shade, shared_ptr<Image>& map) const;

    void roundCorners(const Point2& c0, const Point2& p1, float r, float m, const Color4& c, shared_ptr<Image>& image, shared_ptr<Image>& map) const; 

public:
    void drawLine(const Point2int32& point1, const Point2int32& point2, const Color4& c, shared_ptr<Image>& image) const;

    void drawAxes(int rad, int rng, int xOff, int yOff, shared_ptr<Image>& image, shared_ptr<Image>& map) const;

    void drawVarGraph(bool isClock, int width, int height, int xOff, int yOff, int sign, shared_ptr<Image>& image) const;

    void drawMyGraph(shared_ptr<Image>& image) const;

    void drawClock(shared_ptr<Image>& image) const;

    void drawCantorDust(float xSt, float xEn, float y, int level, shared_ptr<Image>& image) const;

    void drawThickLine(const Point2int32& point1, const Point2int32& point2, const Color4& c, int thickness, shared_ptr<Image>& image, shared_ptr<Image>& map) const;

    void drawGradiantBackground(const Color4& c1, const Color4& c2, int height, int width, shared_ptr<Image>& image) const;

    Rasterizer();
    ~Rasterizer();

};

