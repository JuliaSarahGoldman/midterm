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
class Drawing {
public:
    void drawLine(Point2int32 point1, Point2int32 point2, Color3 c, shared_ptr<Image>& image);
    void drawHLine(Point2int32 point1, Point2int32 point2, Color3 c, shared_ptr<Image>& image);
    void drawVLine(Point2int32 point1, Point2int32 point2, Color3 c, shared_ptr<Image>& image);
    void drawSteepLine(Point2int32 point1, Point2int32 point2, Color3 c, shared_ptr<Image>& image);
    void drawFlatLine(Point2int32 point1, Point2int32 point2, Color3 c, shared_ptr<Image>& image);
    void drawAxes(int rad, int rng, int xOff, int yOff, shared_ptr<Image>& image);
    void drawVarGraph(bool isClock, int width, int height, int xOff, int yOff, int sign, shared_ptr<Image>& image);
    void drawMyGraph(shared_ptr<Image>& image);
    void drawClock(shared_ptr<Image>& image);
    void drawCantorDust(float xSt, float xEn, float y, int level, shared_ptr<Image>& image);
    void drawThickLine(Point2int32 point1, Point2int32 point2, Color3 c, int thickness, shared_ptr<Image>& image);

    Drawing();
    ~Drawing();

};

