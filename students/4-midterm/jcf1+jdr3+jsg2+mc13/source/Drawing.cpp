/** \file Drawing.cpp */
#include "Drawing.h"


Drawing::Drawing(){};
Drawing::~Drawing(){};

void Drawing::drawLine(Point2int32 point1, Point2int32 point2, Color3 c, shared_ptr<Image>& image) {
    int x0 (min<int>(point1.x, point2.x)); 
    int x1(max<int>(point1.x, point2.x)); 
    int y0(min<int>(point1.y, point2.y)); 
    int y1(max<int>(point1.y, point2.y));
    
    int dy(y1-y0); 
    int dx(x1-x0); 
    
    if(dx == 0) { 
        drawVLine(point1, point2, c, image); 
    }
    if (point1.x == point2.x) {
        drawVLine(point1, point2, c, image);
    }
    else if (point1.y == point2.y) {
        drawHLine(point1, point2, c, image);
    }
    else {

        float slope = 1.0f * (point2.y - point1.y) / (point2.x - point1.x);
        if (fabs(slope) <= 1) {
            drawFlatLine(point1, point2, c, image);
        }
        else {
            drawSteepLine(point1, point2, c, image);
        }
    }
}

void Drawing::drawHLine(Point2int32 point1, Point2int32 point2, Color3 c, shared_ptr<Image>& image) {
    float x0;
    float x1;
    x0 = min<int>(point1.x, point2.x);
    x1 = max<int>(point1.x, point2.x);

    for (int x = x0; x <= x1; ++x) {
        image->set(x, point1.y, c);
    }
}

void Drawing::drawVLine(Point2int32 point1, Point2int32 point2, Color3 c, shared_ptr<Image>& image) {
    /*  float x0;
        float x1;
        if (point2.x >= point1.x) {
            x0 = point1.x;
            x1 = point2.x;
        }
        else {
            x0 = point2.x;
            x1 = point1.x;
        }
        for (int x = x0; x <= x1; ++x) {
            image->set(x, point1.y, c);
        }
    */

    // If drawVLine is called when x coords are equal, shouldn't we be drawing wrt y at a fixed x? See Below
    float y0;
    float y1;
    y0 = min<int>(point1.y, point2.y);
    y1 = max<int>(point1.y, point2.y);

    for (int y = y0; y <= y1; ++y) {
        image->set(point1.x, y, c);
    }
    
}

void Drawing::drawFlatLine(Point2int32 point1, Point2int32 point2, Color3 c, shared_ptr<Image>& image) {
    float x0;
    float x1;
    float y0;
    float y1;
    if (point2.x >= point1.x) {
        x0 = point1.x;
        x1 = point2.x;
        y0 = point1.y;
        y1 = point2.y;
    }
    else {
        x0 = point2.x;
        x1 = point1.x;
        y0 = point2.y;
        y1 = point1.y;
    }
    float m = 1.0f * (point2.y - point1.y) / (point2.x - point1.x);
    float y = y0;
    for (int x = (int)x0; x <= x1; ++x, y += m) {
        image->set(x, (int)(y + .5f), c);
    }
}

void Drawing::drawSteepLine(Point2int32 point1, Point2int32 point2, Color3 c, shared_ptr<Image>& image) {
    float x0;
    float x1;
    float y0;
    float y1;
    if (point2.y >= point1.y) {
        x0 = point1.x;
        x1 = point2.x;
        y0 = point1.y;
        y1 = point2.y;
    }
    else {
        x0 = point2.x;
        x1 = point1.x;
        y0 = point2.y;
        y1 = point1.y;
    }
    float m = 1.0f * (point2.x - point1.x) / (point2.y - point1.y);;
    float x = x0;
    for (int y = (int)y0; y <= y1; ++y, x += m) {
        image->set((int)(x + .5f), y, c);
    }
}

//added is how much the line is thickened by on each side
void Drawing::drawThickLine(Point2int32 point1, Point2int32 point2, Color3 c, int added, shared_ptr<Image>& image) {
    float height(image->height());
    float width(image->width());

    // Moved if statements into single for loop
    // Do this for the other functions as well! 
    for (int i = -added; i <= added; ++i) {
        if (point1.x == point2.x) {
            float x(point1.x + i);
            if (x > 0 && x < width) {
                // Changed from point1.x+i and point2.x+i to x, they're the same anyway.
                drawVLine(Point2int32(x, point1.y), Point2int32(x, point1.y), c, image);
            }
        }
        else if (point1.y == point2.y) {
            float y(point1.y + i);

            if (y > 0 && y < height) {
                // Changed from point1.y+i and point2.y+i to y, they're the same anyway.
                drawHLine(Point2int32(point1.x, y), Point2int32(point2.x, y), c, image);
            }
        }
       // else {
            float slope = 1.0f * (point2.y - point1.y) / (point2.x - point1.x);
            float x1(point1.x + i);
            float y1(point1.y + i);
            float x2(point2.x + i);
            float y2(point2.y + i);

            bool inBounds = (y1 > 0 && y1 < height) && (x1 > 0 && x1 < width) && (y2 > 0 && y2 < height) && (x2 > 0 && x2 < width);
            if (inBounds) {
                if (fabs(slope) <= 1) {
                    // Changed from point[1,2].x to point[1,2].x+i -> then to x[1,2] and y[1,2]
                    drawFlatLine(Point2int32(x1, y1), Point2int32(x2, y2), c, image);
                }
                else {
                    drawSteepLine(Point2int32(x1, y1), Point2int32(x2, y2), c, image);
                }
            }
    //    }
    }
    // OLD CODE
    /*
     if (point1.x == point2.x){
        for(int i = -added; i<=added; ++i){
            drawVLine(Point2int32(point1.x+i, point1.y), Point2int32(point2.x+i,point1.y), c, image);
        }
      }
      else if (point1.y == point2.y){
        for(int i = -added; i<=added; ++i){
            drawHLine(Point2int32(point1.x, point1.y+i), Point2int32(point2.x, point2.y+i), c, image);
        }
      }
      float slope = 1.0f * (point2.y - point1.y) / (point2.x - point1.x);
      if (fabs(slope) <= 1){
        for(int i = -added; i<=added; ++i){
            drawFlatLine(Point2int32(point1.x, point1.y+i), Point2int32(point2.x, point2.y+i), c, image);
        }
      }
      else{
        for(int i = -added; i<=added; ++i){
            drawSteepLine(Point2int32(point1.x, point1.y+i), Point2int32(point2.x, point2.y+i), c, image);
        }
      }
      */
}

void Drawing::drawGradiantBackground(Color3 c1, Color3 c2, int height, int width, shared_ptr<Image>& image) {
    Color3 current = c1;
    for (int y = 0; y < height; ++y) {
        current = current + (c2 - c1) / (height - 1);
        drawLine(Point2int32(0, y), Point2int32(width - 1, y), current, image);
    }
}

void Drawing::drawAxes(int rad, int rng, int xOff, int yOff, shared_ptr<Image>& image) {
    drawThickLine(Point2int32(0 + xOff, 10 * rng + yOff), Point2int32(20 * rad + xOff, 10 * rng + yOff), Color3(1, 0, 0), 5, image);
    drawThickLine(Point2int32(10 * rad + xOff, 0 + yOff), Point2int32(10 * rad + xOff, 20 * rng + yOff), Color3(1, 0, 0), 5, image);
    for (int x = 0; x <= 2 * rad; ++x) {
        drawThickLine(Point2int32(xOff + x * 10, yOff + rng * 10 - 2), Point2int32(xOff + x * 10, yOff + rng * 10 + 2), Color3(1, 0, 0), 0, image);
    }
    for (int y = 0; y <= 2 * rng; ++y) {
        drawThickLine(Point2int32(xOff + rad * 10 - 2, yOff + y * 10), Point2int32(xOff + rad * 10 + 2, yOff + y * 10), Color3(1, 0, 0), 0, image);
    }
}

void Drawing::drawVarGraph(bool isClock, int width, int height, int xOff, int yOff, int sign, shared_ptr<Image>& image) {
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
            drawThickLine(p1, p2, Color3(0, 0, 0),5, image);
        }
        p1 = p2;
        realX += .1;
    }
}

void Drawing::drawMyGraph(shared_ptr<Image>& image) {
    drawAxes(4, 10, 5, 5, image);
    drawVarGraph(false, 4, 10, 5, 5, 1, image);
}

void Drawing::drawClock(shared_ptr<Image>& image) {
    Point2int32 center = Point2int32(210, 65);
    drawVarGraph(true, 6, 6, 150, 5, 1, image);
    drawVarGraph(true, 6, 6, 150, 5, -1, image);
    float rotation = 0;
    int time = 0;
    Point2int32 outer = Point2int32((float)(1.0f*center.x + (50.0f * cos(rotation))), (float)(1.0f*center.y + (50.0f * sin(rotation))));
    drawLine(center, outer, Color3(1, 0, 0), image);
    if (time % 5 == 0) {
        rotation += (3.14159 / 12);
    }
}

void Drawing::drawCantorDust(float xSt, float xEn, float y, int level, shared_ptr<Image>& image) {
    drawLine(Point2int32(xSt, y), Point2int32(xEn, y), Color3(0, 0, 1), image);
    if (level > 1) {
        drawCantorDust(xSt, (xEn - xSt) / 3 + xSt, y + 10, level - 1, image);
        drawCantorDust(2.0 * (xEn - xSt) / 3 + xSt, xEn, y + 10, level - 1, image);
    }
}