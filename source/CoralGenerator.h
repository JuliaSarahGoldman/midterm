#pragma once
#include <G3D/G3DAll.h>

 /** \brief Application framework. */
class CoralGenerator {
protected:
    
    void applyRules(int depth, const Point2int32& location, float cumulativeAngle, float drawLength, float minAngle, float maxAngle, float thick, float thickMult, float drawMult, const String& symbolBuffer, const Table<String,Array<String>>& rules, Array<Array<Point2int32>>& edgeBuffer, Array<float>& thickBuffer);
    void generateFingerCoral(int depth, Point2int32& location, float cumulativeAngle, float drawLength, float minAngle, float maxAngle, float thick, Array<Array<Point2int32>>& edgeBuffer, Array<float>& thickBuffer);
    void generateThinCoral(int depth, Point2int32& location, float cumulativeAngle, float drawLength, float minAngle, float maxAngle, float thick, Array<Array<Point2int32>>& edgeBuffer, Array<float>& thickBuffer);
    void generateCrazyCoral(int depth, Point2int32& location, float cumulativeAngle, float drawLength, float minAngle, float maxAngle, float thick, Array<Array<Point2int32>>& edgeBuffer, Array<float>& thickBuffer);
    void generateFlatCoral(int depth, Point2int32& location, float cumulativeAngle, float drawLength, float minAngle, float maxAngle, float thick, Array<Array<Point2int32>>& edgeBuffer, Array<float>& thickBuffer);
    void generateBranchCoral(int depth, Point2int32& location, float cumulativeAngle, float drawLength, float minAngle, float maxAngle, float thick, Array<Array<Point2int32>>& edgeBuffer, Array<float>& thickBuffer);
    void generateTallCoral(int depth, Point2int32& location, float cumulativeAngle, float drawLength, float minAngle, float maxAngle, float thick, Array<Array<Point2int32>>& edgeBuffer, Array<float>& thickBuffer);

public:
    
    void writeCoral(String coralType, Color3 coralColor, shared_ptr<Image>& color, shared_ptr<Image>& bump, float initialThickness, float minAngle, float maxAngle);
    CoralGenerator();
    ~CoralGenerator();

};

