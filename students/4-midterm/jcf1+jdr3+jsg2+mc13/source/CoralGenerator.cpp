#include "CoralGenerator.h"
#include "App.h"
#include "Rasterizer.h"

CoralGenerator::CoralGenerator() {};
CoralGenerator::~CoralGenerator() {};

void CoralGenerator::writeCoral(String coralType, Color3 coralColor, shared_ptr<Image> &color, shared_ptr<Image> &bump) {
    shared_ptr<Rasterizer> painter(new Rasterizer());

    /* JOHN: 
        I changed the code so it writes 4 faces of the coral, one in each quadrant,
        and does it in terms of the width and height of the image.
        It'd be useful to have arguments for:
        1) The thickness of the root
        2) (maybe) the initial angle and range of randomness for future angles, if that makes sense. 
        Try to abstract this more because it's hard to figure out what each constant represents.
    */

    Array<Array<Point2int32>> edgeBuffer = Array<Array<Point2int32>>();
    Array<float> thickBuffer = Array<float>();
    int width(color->width());
    int height(color->height());
    int w4(width/4); 
    if (coralType == "thin") {
        // Generate 4 faces, one in each quadrant
        generateThinCoral(5, Point2int32(w4, height/2), -90, 100, 20, 16.0f, edgeBuffer, thickBuffer);
        generateThinCoral(5, Point2int32(3*w4, height/2), -90, 100, 20, 16.0f, edgeBuffer, thickBuffer);
        generateThinCoral(5, Point2int32(w4, height-16), -90, 100, 20, 16.0f, edgeBuffer, thickBuffer);
        generateThinCoral(5, Point2int32(3*w4, height-16), -90, 100, 20, 16.0f, edgeBuffer, thickBuffer);

        //generateThinCoral(5, Point2int32(width/2, 3*height/4), -90, 100, 20, 16.0f, edgeBuffer, thickBuffer);

    }
    else if (coralType == "finger") {
        generateFingerCoral(5, Point2int32(w4, height/2), -90, 100, 20, 16.0f, edgeBuffer, thickBuffer);
        generateFingerCoral(5, Point2int32(3*w4, height/2), -90, 100, 20, 16.0f, edgeBuffer, thickBuffer);
        generateFingerCoral(5, Point2int32(w4, height-16), -90, 100, 20, 16.0f, edgeBuffer, thickBuffer);
        generateFingerCoral(5, Point2int32(3*w4,height-16), -90, 100, 20, 16.0f, edgeBuffer, thickBuffer);

        //generateFingerCoral(5, Point2int32(width/2, 3*height/4), -90, 100, 20, 16.0f, edgeBuffer, thickBuffer);
    }
    else if (coralType == "flat") {
        generateFlatCoral(6, Point2int32(w4, height/2), -90, 100, 30, 16.0f, edgeBuffer, thickBuffer);
        generateFlatCoral(6, Point2int32(3*w4, height/2), -90, 100, 30, 16.0f, edgeBuffer, thickBuffer);
        generateFlatCoral(6, Point2int32(w4, height-16), -90, 100, 30, 16.0f, edgeBuffer, thickBuffer);
        generateFlatCoral(6, Point2int32(3*w4,height-16), -90, 100, 30, 16.0f, edgeBuffer, thickBuffer);

       // generateFlatCoral(6, Point2int32(width/2, 3*height/4), -90, 100, 30, 16.0f, edgeBuffer, thickBuffer);
    }
    else if (coralType == "branch") {
        generateBranchCoral(6, Point2int32(w4, height/2), -90, 20, 15, 6.0f, edgeBuffer, thickBuffer);
        generateBranchCoral(6, Point2int32(3*w4, height/2), -90, 20, 15, 6.0f, edgeBuffer, thickBuffer);
        generateBranchCoral(6, Point2int32(w4, height-16), -90, 20, 15, 6.0f, edgeBuffer, thickBuffer);
        generateBranchCoral(6, Point2int32(3*w4,height-16), -90, 20, 15, 6.0f, edgeBuffer, thickBuffer);

        //generateBranchCoral(6, Point2int32(width/2, 3*height/4), -90, 20, 15, 6.0f, edgeBuffer, thickBuffer);
    }
    else {
        generateCrazyCoral(6, Point2int32(w4, height/2), -90, 100, 30, 16.0f, edgeBuffer, thickBuffer);
        generateCrazyCoral(6, Point2int32(3*w4, height/2), -90, 100, 30, 16.0f, edgeBuffer, thickBuffer);
        generateCrazyCoral(6, Point2int32(w4, height-16), -90, 100, 30, 16.0f, edgeBuffer, thickBuffer);
        generateCrazyCoral(6, Point2int32(3*w4, height-16), -90, 100, 30, 16.0f, edgeBuffer, thickBuffer);
        
        //generateCrazyCoral(6, Point2int32(width/2, 3*height/4), -90, 100, 30, 16.0f, edgeBuffer, thickBuffer);
    }

    try {
        color->setAll(Color4(coralColor, 0));
        bump->setAll(Color3::black());

        for (int i(0); i < edgeBuffer.size(); ++i) {
            Point2int32 s = edgeBuffer[i][0];
            Point2int32 f = edgeBuffer[i][1];

            painter->drawThickLine(s, f, coralColor, thickBuffer[i], color, bump,height/2+16);
        }

    }
    catch (...) {
        msgBox("Unable to load the image.");
    }
}


void CoralGenerator::generateThinCoral(int depth, Point2int32& location, float cumulativeAngle, float drawLength, float moveAngle, float thick, Array<Array<Point2int32>>& edgeBuffer, Array<float>& thickBuffer) {

    Table<String, Array<String>> rules;

    //Array<String> SBuffer("-[FXFXFXF]+[FXFXFXF]");
    Array<String> SBuffer("-[FXFXF]+[FXFF]");
    rules.set("S", SBuffer);

    //Array<String> XBuffer("-[FX]-[-[FX]]+[FX]+[+[FX]][FX]F");
    Array<String> XBuffer("-[FFXF]+[FXF][FXFF]F");
    rules.set("X", XBuffer);

    rules.set("F", Array<String>("FF"));

    applyRules(depth, location, cumulativeAngle, drawLength, moveAngle, thick, 0.65f, 0.8f, "S", rules, edgeBuffer, thickBuffer);
}

void CoralGenerator::generateFlatCoral(int depth, Point2int32& location, float cumulativeAngle, float drawLength, float moveAngle, float thick, Array<Array<Point2int32>>& edgeBuffer, Array<float>& thickBuffer) {

    Table<String, Array<String>> rules;

    //Array<String> SBuffer("-[FY]+[FY]++[FY]--[FFYF]+++[FFYF]---[FY]");
    Array<String> SBuffer("-[FY]+[FY]++[FY]");
    rules.set("S", SBuffer);

    //Array<String> YBuffer("-[FFFX]+[FFFX]++[FFFX]--[FFXFF]+++[FFXFF]---[FFFX]");
    Array<String> YBuffer("++[FX]", "--[FX]");
    rules.set("Y", YBuffer);

    Array<String> XBuffer("FF", "", "+[FXF]");
    rules.set("X", XBuffer);

    rules.set("F", Array<String>(""));

    applyRules(depth, location, cumulativeAngle, drawLength, moveAngle, thick, 0.9f, 0.9f, "S", rules, edgeBuffer, thickBuffer);
}

void CoralGenerator::generateBranchCoral(int depth, Point2int32& location, float cumulativeAngle, float drawLength, float moveAngle, float thick, Array<Array<Point2int32>>& edgeBuffer, Array<float>& thickBuffer) {

    Table<String, Array<String>> rules;

    /*Array<String> XBuffer("+[FX]F", "-[FX]F", "+[FX]-[FX]", "F");
    rules.set("X", XBuffer);*/

    Array<String> XBuffer("+[FFFFFX]-[FFFFFX]");
    rules.set("X", XBuffer);

    Array<String> YBuffer("+[FFXFFFX]");
    rules.set("Y", YBuffer);

    Array<String> ZBuffer("-[FFFXFFX]");
    rules.set("Z", ZBuffer);

    rules.set("F", Array<String>(""));

    applyRules(depth, location, cumulativeAngle, drawLength, moveAngle, thick, 0.8f, 0.8f, "FX+++[YF]---[ZF]", rules, edgeBuffer, thickBuffer);
}

void CoralGenerator::generateFingerCoral(int depth, Point2int32& location, float cumulativeAngle, float drawLength, float moveAngle, float thick, Array<Array<Point2int32>>& edgeBuffer, Array<float>& thickBuffer) {
    Table<String, Array<String>> rules;

    Array<String> SBuffer("-[FX]--[FX]+[FX]++[FX]+++[FX]---[FX]");
    rules.set("S", SBuffer);

    Array<String> XBuffer;
    for (int i = 0; i < 8; ++i) {
        XBuffer.append("-[FX]--[FX]+[FX]++[FX]");
    }
    //XBuffer.append("-[FX]--[FX]+[FX]++[FX]");
    XBuffer.append("-[FX]+[FX]");
    rules.set("X", XBuffer);

    rules.set("F", Array<String>(""));

    applyRules(depth, location, cumulativeAngle, drawLength, moveAngle, thick, 0.75f, 0.85f, "S", rules, edgeBuffer, thickBuffer);
}

void CoralGenerator::generateCrazyCoral(int depth, Point2int32& location, float cumulativeAngle, float drawLength, float moveAngle, float thick, Array<Array<Point2int32>>& edgeBuffer, Array<float>& thickBuffer) {
    Table<String, Array<String>> rules;

    Array<String> XBuffer;

    for (int i = 0; i < 6; ++i) {
        XBuffer.append("-[FX]-[-[FX]]+[FX]+[+[FX]]+[+[+[FX]]]-[-[-[FX]]]");
    }
    XBuffer.append("-[FX]-[-[FX]]+[FX]+[+[FX]]");
    XBuffer.append("-[FX]-[-[FX]]+[FX]+[+[FX]]");
    XBuffer.append("-[FX]+[FX]");
    XBuffer.append("-[FX]+[FX]");
    rules.set("S", XBuffer);
    rules.set("X", XBuffer);

    rules.set("F", Array<String>(""));


    applyRules(depth, location, cumulativeAngle, drawLength, moveAngle, thick, 0.65f, 0.8f, "S", rules, edgeBuffer, thickBuffer);
}

void CoralGenerator::applyRules(int depth, const Point2int32& location, float cumulativeAngle, float drawLength, float moveAngle, float thick, const float thickMult, const float drawMult, const String& symbolBuffer, const Table<String, Array<String>>& rules, Array<Array<Point2int32>>& edgeBuffer, Array<float>& thickBuffer) {

    if (depth == 0) { return; }

    int isBracket = 0;
    Array<float> angles;
    Array<Point2int32> positions;

    angles.resize(20);
    positions.resize(20);

    angles.setAll(cumulativeAngle);
    positions.setAll(location);

    for (int i(0); i < symbolBuffer.length(); ++i) {

        //debugPrintf("%s, %f, %f\n", symbolBuffer.substr(i, 1), angles[isBracket], angles[isBracket + 1]);

        if (symbolBuffer.substr(i, 1) == "-") {
            angles[isBracket + 1] = angles[isBracket + 1] - moveAngle;
        }
        else if (symbolBuffer.substr(i, 1) == "+") {
            angles[isBracket + 1] = angles[isBracket + 1] + moveAngle;
        }
        else if (symbolBuffer.substr(i, 1) == "[") {
            ++isBracket;
            positions[isBracket] = positions[isBracket - 1];
        }
        else if (symbolBuffer.substr(i, 1) == "]") {
            angles[isBracket] = cumulativeAngle;
            --isBracket;
        }
        else if (symbolBuffer.substr(i, 1) == "F") {
            float randLen = drawLength * G3D::Random::threadCommon().uniform(0.7f, 1.0);
            float randAng = angles[isBracket] * G3D::Random::threadCommon().uniform(0.5f, 1.0f);

            //debugPrintf("%f %f\n", angles[isBracket], randAng);

            float radians = (randAng / 180.0f) * pif();
            int x = lround(cos(radians) * randLen) + positions[isBracket].x;
            int y = lround(sin(radians) * randLen) + positions[isBracket].y;

            Point2int32 point(x, y);
            Array<Point2int32> edge(positions[isBracket], point);
            edgeBuffer.append(edge);
            thickBuffer.append(thick);

            positions[isBracket] = point;

            Array<String> apply = rules["F"];
            int count = G3D::Random::threadCommon().uniform(0, apply.size() - 1);

            applyRules(depth - 1, positions[isBracket], angles[isBracket], drawLength * drawMult, moveAngle, thick, thickMult, drawMult, apply[count], rules, edgeBuffer, thickBuffer);
        }
        else if (symbolBuffer.substr(i, 1) != "\n") {

            String str = symbolBuffer.substr(i, 1);
            Array<String> apply = rules[str];
            int count = G3D::Random::threadCommon().uniform(0, apply.size() - 1);

            applyRules(depth - 1, positions[isBracket], angles[isBracket], drawLength * drawMult, moveAngle, thick * thickMult, thickMult, drawMult, apply[count], rules, edgeBuffer, thickBuffer);
        }
    }
}