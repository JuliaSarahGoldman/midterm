/**
  \file App.h

  The G3D 10.00 default starter app is configured for OpenGL 4.1 and
  relatively recent GPUs.
 */
#pragma once
#include <G3D/G3DAll.h>

/** \brief Application framework. */
class App : public GApp {
protected:
    /** Called from onInit */
    void makeGUI();
    String makeTubes(Array<float>& radii, Array<float>& heights, int slices);
    String makeTube(int depth, float& radius, float height, int slices, const CoordinateFrame& coordFrame);
    String write3DCoral(int depth, float startRadius, float startHeight);
    void make3DCoral(String& coral, int depth, float radius, int height, const Matrix3& oldLeftRotation, const Matrix3& oldRightRotation);
    void createScene(String sceneName);

    void generateCrazyCoral(int depth, Point2int32 location, float cumulativeAngle, float drawLength, float moveAngle, float thick, Array<String>& symbolBuffer);
    void applyCrazyRules(int depth, Point2int32 location, float cumulativeAngle, float drawLength, float moveAngle, float thick, Array<String>& symbolBuffer);
    void writeCoral(String bumpName, String colorName, Color3 coralColor);

    void generateFingerCoral(int depth, Point2int32 location, float cumulativeAngle, float drawLength, float moveAngle, float thick, Array<String>& symbolBuffer);
    void applyFingerRules(int depth, Point2int32 location, float cumulativeAngle, float drawLength, float moveAngle, float thick, Array<String>& symbolBuffer);

    void generateThinCoral(int depth, Point2int32 location, float cumulativeAngle, float drawLength, float moveAngle, float thick, Array<String>& symbolBuffer);

    Array<Array<Point2int32>> edgeBuffer;
    Array<float> thickBuffer;

public:
    
    App(const GApp::Settings& settings = GApp::Settings());

    virtual void onInit() override;
    virtual void onAI() override;
    virtual void onNetwork() override;
    virtual void onSimulation(RealTime rdt, SimTime sdt, SimTime idt) override;
    virtual void onPose(Array<shared_ptr<Surface> >& posed3D, Array<shared_ptr<Surface2D> >& posed2D) override;

    // You can override onGraphics if you want more control over the rendering loop.
    // virtual void onGraphics(RenderDevice* rd, Array<shared_ptr<Surface> >& surface, Array<shared_ptr<Surface2D> >& surface2D) override;

    virtual void onGraphics3D(RenderDevice* rd, Array<shared_ptr<Surface> >& surface3D) override;
    virtual void onGraphics2D(RenderDevice* rd, Array<shared_ptr<Surface2D> >& surface2D) override;

    virtual bool onEvent(const GEvent& e) override;
    virtual void onUserInput(UserInput* ui) override;
    virtual void onCleanup() override;
};
