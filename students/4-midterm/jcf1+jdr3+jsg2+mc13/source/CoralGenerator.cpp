#include "CoralGenerator.h"
#include "App.h"
#include "Rasterizer.h"

CoralGenerator::CoralGenerator() {};
CoralGenerator::~CoralGenerator() {};

void CoralGenerator::writeCoral(String coralType, Color3 coralColor, shared_ptr<Image> &color, shared_ptr<Image> &bump) {
    shared_ptr<Rasterizer> painter(new Rasterizer());

    Array<Array<Point2int32>> edgeBuffer = Array<Array<Point2int32>>();
    Array<float> thickBuffer = Array<float>();

    if(coralType == "thin") {
        generateThinCoral(4, Point2int32(640, 600), -90, 100, 20, 16.0f, edgeBuffer, thickBuffer);
    } else if(coralType == "finger") {
        generateFingerCoral(4, Point2int32(640, 600), -90, 100, 20, 16.0f, edgeBuffer, thickBuffer);
    } else {
        generateCrazyCoral(4, Point2int32(640, 600), -90, 100, 20, 16.0f, edgeBuffer, thickBuffer);
    }

    try {
        color->setAll(Color4(1,1,1,0));
        bump->setAll(Color3::black());

        for (int i(0); i < edgeBuffer.size(); ++i) {
            Point2int32 s = edgeBuffer[i][0];
            Point2int32 f = edgeBuffer[i][1];

            painter->drawThickLine(s, f, coralColor, thickBuffer[i], color, bump);   
        }

    }
    catch (...) {
        msgBox("Unable to load the image.");
    }
}


void CoralGenerator::generateThinCoral(int depth, Point2int32& location, float cumulativeAngle, float drawLength, float moveAngle, float thick, Array<Array<Point2int32>>& edgeBuffer, Array<float>& thickBuffer) {
    
    Table<String,Array<String>> rules = Table<String,Array<String>>();

    Array<String> SBuffer("-[FXFXFXF]+[FXFXFXF]");
    rules.set("S", SBuffer);

    Array<String> XBuffer("-[FX]-[-[FX]]+[FX]+[+[FX]][FX]F");
    rules.set("X", XBuffer);

    rules.set("F", Array<String>(""));

    applyRules(depth, location, cumulativeAngle, drawLength, moveAngle, thick, "S", rules, edgeBuffer, thickBuffer);
}

void CoralGenerator::generateFingerCoral(int depth, Point2int32& location, float cumulativeAngle, float drawLength, float moveAngle, float thick, Array<Array<Point2int32>>& edgeBuffer, Array<float>& thickBuffer) {
        Table<String,Array<String>> rules = Table<String,Array<String>>();

        Array<String> SBuffer("-[FX]-[-[FX]]+[FX]+[+[FX]]+[+[+[FX]]]-[-[-[FX]]]");
        rules.set("S", SBuffer);

        Array<String> XBuffer(10);
        for(int i = 0; i < 8; ++i) {
           XBuffer.append("-[FX]-[-[FX]]+[FX]+[+[FX]]");
        }
        XBuffer.append("-[FX]-[-[FX]]+[FX]+[+[FX]]");
        XBuffer.append("-[FX]+[FX]");
        rules.set("X", XBuffer);

        rules.set("F", Array<String>(""));

        applyRules(depth, location, cumulativeAngle, drawLength, moveAngle, thick, "S", rules, edgeBuffer, thickBuffer);
}

void CoralGenerator::generateCrazyCoral(int depth, Point2int32& location, float cumulativeAngle, float drawLength, float moveAngle, float thick, Array<Array<Point2int32>>& edgeBuffer, Array<float>& thickBuffer) {
    Table<String,Array<String>> rules = Table<String,Array<String>>();

        Array<String> XBuffer(10);

        for(int i = 0; i < 6; ++i) {
           XBuffer.append("-[FX]-[-[FX]]+[FX]+[+[FX]]+[+[+[FX]]]-[-[-[FX]]]");
        }
        XBuffer.append("-[FX]-[-[FX]]+[FX]+[+[FX]]");
        XBuffer.append("-[FX]-[-[FX]]+[FX]+[+[FX]]");
        XBuffer.append("-[FX]+[FX]");
        XBuffer.append("-[FX]+[FX]");
        rules.set("S", XBuffer);
        rules.set("X", XBuffer);

        rules.set("F", Array<String>(""));


        applyRules(depth, location, cumulativeAngle, drawLength, moveAngle, thick, "S", rules, edgeBuffer, thickBuffer);
}

void CoralGenerator::applyRules(int depth, const Point2int32& location, float cumulativeAngle, float drawLength, float moveAngle, float thick, const String& symbolBuffer, const Table<String,Array<String>>& rules, Array<Array<Point2int32>>& edgeBuffer, Array<float>& thickBuffer) {

    if(depth == 0){ return; }

    int isBracket = 0;
    Array<float> angles(cumulativeAngle);
    Array<Point2int32> positions(location);

    angles.resize(20);
    positions.resize(20);

    for (int i(0); i < symbolBuffer.length(); ++i) {
        if (symbolBuffer.substr(i, 1) == "-") {
            angles[isBracket + 1] = angles[isBracket] - moveAngle;
        }
        else if (symbolBuffer.substr(i, 1) == "+") {
            angles[isBracket + 1] = angles[isBracket] + moveAngle;
        }
        else if (symbolBuffer.substr(i, 1) == "[") {
            ++isBracket;
            positions[isBracket] = positions[isBracket - 1];
        }
        else if (symbolBuffer.substr(i, 1) == "]") {
            --isBracket;
        }
        else if (symbolBuffer.substr(i, 1) == "F") {
            float randLen = drawLength * G3D::Random::threadCommon().uniform(0.5f, 1.0);
            float randAng = angles[isBracket] * G3D::Random::threadCommon().uniform(0.5f, 1.0);

            float radians = (randAng / 180.0f) * pif();
            int x = lround(cos(radians) * randLen) + positions[isBracket].x;
            int y = lround(sin(radians) * randLen) + positions[isBracket].y;

            Point2int32 point(x, y);
            Array<Point2int32> edge(positions[isBracket], point);
            edgeBuffer.append(edge);
            thickBuffer.append(thick);

            positions[isBracket] = point;

            Array<String> apply = rules["F"];
            int count = G3D::Random::threadCommon().uniform(0, apply.size());

            applyRules(depth - 1, positions[isBracket], angles[isBracket], drawLength * 0.8f, moveAngle, thick * 0.85f, apply[count], rules, edgeBuffer, thickBuffer);
        } else {

            Array<String> apply = rules[symbolBuffer.substr(i, 1)];
            int count = G3D::Random::threadCommon().uniform(0, apply.size());

            applyRules(depth - 1, positions[isBracket], angles[isBracket], drawLength * 0.8f, moveAngle, thick * 0.85f, apply[count], rules, edgeBuffer, thickBuffer);
        }
    }
}