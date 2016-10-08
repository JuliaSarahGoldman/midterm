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
    String makeTube(Array<float>& radii, Array<float>& heights, int slices);
    void createScene(String sceneName);
    void drawLine(Point2int32 point1, Point2int32 point2, Color3 c, shared_ptr<Image>& image);
    void drawHLine(Point2int32 point1, Point2int32 point2, Color3 c, shared_ptr<Image>& image);
    void drawVLine(Point2int32 point1, Point2int32 point2, Color3 c, shared_ptr<Image>& image);
    void drawSteepLine(Point2int32 point1, Point2int32 point2, Color3 c, shared_ptr<Image>& image);
    void drawFlatLine(Point2int32 point1, Point2int32 point2, Color3 c, shared_ptr<Image>& image);
    void drawGradiantBackground(Color3 c1, Color3 c2, int height, int width,  shared_ptr<Image>& image);
    void drawAxes(int rad, int rng, int xOff, int yOff, shared_ptr<Image>& image);
    void drawVarGraph(bool isClock, int width, int height, int xOff, int yOff, int sign, shared_ptr<Image>& image);
    void drawMyGraph(shared_ptr<Image>& image);
    void drawClock(shared_ptr<Image>& image);
    void drawCantorDust(float xSt, float xEn, float y, int level, shared_ptr<Image>& image);
    void drawThickLine(Point2int32 point1, Point2int32 point2, Color3 c, int thickness, shared_ptr<Image>& image);

    //void generateShape(int depth, Point2int32 location, float cumulativeAngle, float drawLength, float moveAngle, Array<String>& symbolBuffer, Array<Array<Point2int32>>& edgeBuffer);
    //String applyRules(int depth, Point2int32 location, float cumulativeAngle, float drawLength, float moveAngle, Array<String>& symbolBuffer, Array<Array<Point2int32>>& edgeBuffer);

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
