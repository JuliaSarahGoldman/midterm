/** \file App.cpp */
#include "App.h"

// Tells C++ to invoke command-line main() function even on OS X and Win32.
G3D_START_AT_MAIN();

int main(int argc, const char* argv[]) {
    {
        G3DSpecification g3dSpec;
        g3dSpec.audio = false;
        initGLG3D(g3dSpec);
    }

    GApp::Settings settings(argc, argv);

    // Change the window and other startup parameters by modifying the
    // settings class.  For example:
    settings.window.caption = argv[0];

    // Set enable to catch more OpenGL errors
    // settings.window.debugContext     = true;

    // Some common resolutions:
    // settings.window.width            =  854; settings.window.height       = 480;
    // settings.window.width            = 1024; settings.window.height       = 768;
    settings.window.width = 1280; settings.window.height = 720;
    //settings.window.width             = 1920; settings.window.height       = 1080;
    // settings.window.width            = OSWindow::primaryDisplayWindowSize().x; settings.window.height = OSWindow::primaryDisplayWindowSize().y;
    settings.window.fullScreen = false;
    settings.window.resizable = !settings.window.fullScreen;
    settings.window.framed = !settings.window.fullScreen;

    // Set to true for a significant performance boost if your app can't render at 60fps, or if
    // you *want* to render faster than the display.
    settings.window.asynchronous = false;

    settings.hdrFramebuffer.depthGuardBandThickness = Vector2int16(64, 64);
    settings.hdrFramebuffer.colorGuardBandThickness = Vector2int16(0, 0);
    settings.dataDir = FileSystem::currentDirectory();
    settings.screenshotDirectory = "../journal/";

    settings.renderer.deferredShading = true;
    settings.renderer.orderIndependentTransparency = false;

    return App(settings).run();
}


App::App(const GApp::Settings& settings) : GApp(settings) {
}

/*void App::generateShape(int depth, Point2int32 location, float cumulativeAngle, float drawLength, float moveAngle, Array<String>& symbolBuffer, Array<Array<Point2int32>>& edgeBuffer) {
    if(depth == 0) return;

    applyRules(depth, location, cumulativeAngle, drawLength, moveAngle, symbolBuffer, vertices, edgeBuffer);
}

void App::applyRules(int depth, Point2int32 location, float cumulativeAngle, float drawLength, float moveAngle, Array<String>& symbolBuffer, Array<Array<Point2int32>>& edgeBuffer) {
    
    float angle = cumulativeAngle;

    for(int i(0); i < symbolBuffer.size(); ++i) {
        if(symbolBuffer[i] == "-") {
            angle -= moveAngle;
        }    
        else if(symbolBuffer[i] == "+") {
            angle += moveAngle;
        }    
        else if(symbolBuffer[i] == "[") {
            
        }    
        else if(symbolBuffer[i] == "]") {
            
        }    
        else if(symbolBuffer[i] == "F") {
            float radians = (angle/180.0f) * pif();
            int x = lround(cos(radians));
            int y = lround(sin(radians));

            Point2int32 point(x, y);
            Array<Point2int32> edge(location, point);
            edgeBuffer.append(edge);
            angle = cumulativeAngle;
        }    
        else if(symbolBuffer[i] == "X") {
            Array<String> applyBuffer("-", "F", "-", "X", "+");
            applyBuffer.append("F");
            applyBuffer.append("+");
            applyBuffer.append("X");

            angle = cumulativeAngle;
        }    
    }
}*/


void App::drawLine(Point2 point1, Point2 point2, Color3 c, shared_ptr<Image>& image) {
    if (point1.x == point2.x) {
        drawVLine(point1, point2, c, image);
    }
    else if (point1.y == point2.y) {
        drawHLine(point1, point2, c, image);
    }
    float slope = 1.0f * (point2.y - point1.y) / (point2.x - point1.x);
    if (fabs(slope) <= 1) {
        drawFlatLine(point1, point2, c, image);
    }
    else {
        drawSteepLine(point1, point2, c, image);
    }
}

void App::drawHLine(Point2 point1, Point2 point2, Color3 c, shared_ptr<Image>& image) {
    float x0;
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
}

void App::drawVLine(Point2 point1, Point2 point2, Color3 c, shared_ptr<Image>& image) {
    /*
        float x0;
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
    if (point2.y >= point1.y) {
        y0 = point1.y;
        y1 = point2.y;
    }
    else {
        y0 = point2.y;
        y1 = point1.y;
    }
    for (int y = y0; y <= y1; ++y) {
        image->set(point1.x, y, c);
    }

}

void App::drawFlatLine(Point2 point1, Point2 point2, Color3 c, shared_ptr<Image>& image) {
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

void App::drawSteepLine(Point2 point1, Point2 point2, Color3 c, shared_ptr<Image>& image) {
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
void App::drawThickLine(Point2 point1, Point2 point2, Color3 c, int added, shared_ptr<Image>& image) {
    float height(image->height());
    float width(image->width());

    // Moved if statements into single for loop
    // Do this for the other functions as well! 
    for (int i = -added; i <= added; ++i) {
        if (point1.x == point2.x) {
            float x(point1.x + i);
            if (x > 0 && x < width) {
                // Changed from point1.x+i and point2.x+i to x, they're the same anyway.
                drawVLine(Point2(x, point1.y), Point2(x, point1.y), c, image);
            }
        }
        else if (point1.y == point2.y) {
            float y(point1.y + i);

            if (y > 0 && y < height) {
                // Changed from point1.y+i and point2.y+i to y, they're the same anyway.
                drawHLine(Point2(point1.x, y), Point2(point2.x, y), c, image);
            }
        }

        float slope = 1.0f * (point2.y - point1.y) / (point2.x - point1.x);
        float x1(point1.x + i);
        float y1(point1.y + i);
        float x2(point2.x + i);
        float y2(point2.y + i);

        bool inBounds = (y1 > 0 && y1 < height) && (x1 > 0 && x1 < width) && (y2 > 0 && y2 < height) && (x2 > 0 && x2 < width);
        if (inBounds) {
            if (fabs(slope) <= 1) {
                // Changed from point[1,2].x to point[1,2].x+i -> then to x[1,2] and y[1,2]
                drawFlatLine(Point2(x1, y1), Point2(x2, y2), c, image);
            }
            else {
                drawSteepLine(Point2(x1, y1), Point2(x2, y2), c, image);
            }
        }
    }

    // OLD CODE
    /*
     if (point1.x == point2.x){
        for(int i = -added; i<=added; ++i){
            drawVLine(Point2(point1.x+i, point1.y), Point2(point2.x+i,point1.y), c, image);
        }
      }
      else if (point1.y == point2.y){
        for(int i = -added; i<=added; ++i){
            drawHLine(Point2(point1.x, point1.y+i), Point2(point2.x, point2.y+i), c, image);
        }
      }
      float slope = 1.0f * (point2.y - point1.y) / (point2.x - point1.x);
      if (fabs(slope) <= 1){
        for(int i = -added; i<=added; ++i){
            drawFlatLine(Point2(point1.x, point1.y+i), Point2(point2.x, point2.y+i), c, image);
        }
      }
      else{
        for(int i = -added; i<=added; ++i){
            drawSteepLine(Point2(point1.x, point1.y+i), Point2(point2.x, point2.y+i), c, image);
        }
      }
      */
}

void App::drawGradiantBackground(Color3 c1, Color3 c2, int height, int width, shared_ptr<Image>& image) {
    Color3 current = c1;
    for (int y = 0; y < height; ++y) {
        current = current + (c2 - c1) / (height - 1);
        drawLine(Point2(0, y), Point2(width - 1, y), current, image);
    }
}

void App::drawAxes(int rad, int rng, int xOff, int yOff, shared_ptr<Image>& image) {
    drawThickLine(Point2(0 + xOff, 10 * rng + yOff), Point2(20 * rad + xOff, 10 * rng + yOff), Color3(1, 0, 0), 5, image);
    drawThickLine(Point2(10 * rad + xOff, 0 + yOff), Point2(10 * rad + xOff, 20 * rng + yOff), Color3(1, 0, 0), 5, image);
    for (int x = 0; x <= 2 * rad; ++x) {
        drawThickLine(Point2(xOff + x * 10, yOff + rng * 10 - 2), Point2(xOff + x * 10, yOff + rng * 10 + 2), Color3(1, 0, 0), 0, image);
    }
    for (int y = 0; y <= 2 * rng; ++y) {
        drawThickLine(Point2(xOff + rad * 10 - 2, yOff + y * 10), Point2(xOff + rad * 10 + 2, yOff + y * 10), Color3(1, 0, 0), 0, image);
    }
}

void App::drawVarGraph(bool isClock, int width, int height, int xOff, int yOff, int sign, shared_ptr<Image>& image) {
    float realX = -1 * width;
    float realY = 0;
    Point2 p1 = Point2(-1, -1);
    Point2 p2 = Point2(-1, -1);
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
        p2 = Point2(1.0f * x, pixY);
        if (p1.x != -1) {
            drawLine(p1, p2, Color3(0, 0, 0), image);
        }
        p1 = p2;
        realX += .1;
    }
}

void App::drawMyGraph(shared_ptr<Image>& image) {
    drawAxes(4, 10, 5, 5, image);
    drawVarGraph(false, 4, 10, 5, 5, 1, image);
}

void App::drawClock(shared_ptr<Image>& image) {
    Point2 center = Point2(210, 65);
    drawVarGraph(true, 6, 6, 150, 5, 1, image);
    drawVarGraph(true, 6, 6, 150, 5, -1, image);
    float rotation = 0;
    int time = 0;
    Point2 outer = Point2((float)(1.0f*center.x + (50.0f * cos(rotation))), (float)(1.0f*center.y + (50.0f * sin(rotation))));
    drawLine(center, outer, Color3(1, 0, 0), image);
    if (time % 5 == 0) {
        rotation += (3.14159 / 12);
    }
}

void App::drawCantorDust(float xSt, float xEn, float y, int level, shared_ptr<Image>& image) {
    drawLine(Point2(xSt, y), Point2(xEn, y), Color3(0, 0, 1), image);
    if (level > 1) {
        drawCantorDust(xSt, (xEn - xSt) / 3 + xSt, y + 10, level - 1, image);
        drawCantorDust(2.0 * (xEn - xSt) / 3 + xSt, xEn, y + 10, level - 1, image);
    }
}

String App::makeTube(Array<float>& radii, Array<float>& heights, int slices) {
    String tube = String("OFF\n");
    tube += format("%d %d 1\n", heights.size()*slices, (heights.size() - 1)*slices);
    for (int i = 0; i < heights.size(); ++i) {
        for (int j = 0; j < slices; ++j) {
            tube += format(STR(%f %f %f\n), radii[i] * (-sin(((2 * pif()*j) / slices))), heights[i], radii[i] * (cos((2 * pif()*j) / slices)));
        }
    }
    for (int i = 0; i < heights.size() - 1; ++i) {
        for (int j = 0; j < slices; ++j) {
            tube += format(STR(4 % d %d %d %d\n), i*slices + slices + j, i*slices + slices + (j + 1) % slices, i*slices + (j + 1) % slices, i*slices + j);
        }
    }
    return tube;
}

void App::createScene(String sceneName) {
    TextOutput output = TextOutput("scene\\" + sceneName + ".off");

    //Create the models for the scene
    String modelString(makeTube(Array<float>(1), Array<float>(1), 1));
    output.writeSymbols(modelString);

    // End the File
    output.commit(true);
}

// Called before the application loop begins.  Load data here and
// not in the constructor so that common exceptions will be
// automatically caught.
void App::onInit() {
    GApp::onInit();
    setFrameDuration(1.0f / 120.0f);

    // Call setScene(shared_ptr<Scene>()) or setScene(MyScene::create()) to replace
    // the default scene here.

    showRenderingStats = false;

    makeGUI();
    // For higher-quality screenshots:
    // developerWindow->videoRecordDialog->setScreenShotFormat("PNG");
    // developerWindow->videoRecordDialog->setCaptureGui(false);
    developerWindow->cameraControlWindow->moveTo(Point2(developerWindow->cameraControlWindow->rect().x0(), 0));
    loadScene(
        //"G3D Sponza"
        "G3D Cornell Box" // Load something simple
        //developerWindow->sceneEditorWindow->selectedSceneName()  // Load the first scene encountered 
    );
}


void App::makeGUI() {
    // Initialize the developer HUD
    createDeveloperHUD();

    debugWindow->setVisible(true);
    developerWindow->videoRecordDialog->setEnabled(true);

    GuiPane* infoPane = debugPane->addPane("Info", GuiTheme::ORNATE_PANE_STYLE);

    // Example of how to add debugging controls
    infoPane->addLabel("You can add GUI controls");
    infoPane->addLabel("in App::onInit().");
    infoPane->addButton("Exit", [this]() { m_endProgram = true; });
    infoPane->pack();

    // More examples of debugging GUI controls:
    // debugPane->addCheckBox("Use explicit checking", &explicitCheck);
    // debugPane->addTextBox("Name", &myName);
    // debugPane->addNumberBox("height", &height, "m", GuiTheme::LINEAR_SLIDER, 1.0f, 2.5f);
    // button = debugPane->addButton("Run Simulator");
    // debugPane->addButton("Generate Heightfield", [this](){ generateHeightfield(); });
    // debugPane->addButton("Generate Heightfield", [this](){ makeHeightfield(imageName, scale, "model/heightfield.off"); });

    debugWindow->pack();
    debugWindow->setRect(Rect2D::xywh(0, 0, (float)window()->width(), debugWindow->rect().height()));

    shared_ptr<G3D::Image> image;
    try {
        int width = 600;
        int height = 500;
        image = Image::create(width, height, ImageFormat::RGB32F());
        drawGradiantBackground(Color3(0, 1, 0), Color3(0, 0, 1), height, width, image);
        drawMyGraph(image);
        drawClock(image);
        drawCantorDust(20, 350, 220, 5, image);
        show(image);
    }
    catch (...) {
        msgBox("Unable to load the image.");
    }
}


// This default implementation is a direct copy of GApp::onGraphics3D to make it easy
// for you to modify. If you aren't changing the hardware rendering strategy, you can
// delete this override entirely.
void App::onGraphics3D(RenderDevice* rd, Array<shared_ptr<Surface> >& allSurfaces) {
    if (!scene()) {
        if ((submitToDisplayMode() == SubmitToDisplayMode::MAXIMIZE_THROUGHPUT) && (!rd->swapBuffersAutomatically())) {
            swapBuffers();
        }
        rd->clear();
        rd->pushState(); {
            rd->setProjectionAndCameraMatrix(activeCamera()->projection(), activeCamera()->frame());
            drawDebugShapes();
        } rd->popState();
        return;
    }

    GBuffer::Specification gbufferSpec = m_gbufferSpecification;
    extendGBufferSpecification(gbufferSpec);
    m_gbuffer->setSpecification(gbufferSpec);
    m_gbuffer->resize(m_framebuffer->width(), m_framebuffer->height());
    m_gbuffer->prepare(rd, activeCamera(), 0, -(float)previousSimTimeStep(), m_settings.hdrFramebuffer.depthGuardBandThickness, m_settings.hdrFramebuffer.colorGuardBandThickness);

    m_renderer->render(rd, m_framebuffer, scene()->lightingEnvironment().ambientOcclusionSettings.enabled ? m_depthPeelFramebuffer : shared_ptr<Framebuffer>(),
        scene()->lightingEnvironment(), m_gbuffer, allSurfaces);

    // Debug visualizations and post-process effects
    rd->pushState(m_framebuffer); {
        // Call to make the App show the output of debugDraw(...)
        rd->setProjectionAndCameraMatrix(activeCamera()->projection(), activeCamera()->frame());
        drawDebugShapes();
        const shared_ptr<Entity>& selectedEntity = (notNull(developerWindow) && notNull(developerWindow->sceneEditorWindow)) ? developerWindow->sceneEditorWindow->selectedEntity() : shared_ptr<Entity>();
        scene()->visualize(rd, selectedEntity, allSurfaces, sceneVisualizationSettings(), activeCamera());

        // Post-process special effects
        m_depthOfField->apply(rd, m_framebuffer->texture(0), m_framebuffer->texture(Framebuffer::DEPTH), activeCamera(), m_settings.hdrFramebuffer.depthGuardBandThickness - m_settings.hdrFramebuffer.colorGuardBandThickness);

        m_motionBlur->apply(rd, m_framebuffer->texture(0), m_gbuffer->texture(GBuffer::Field::SS_EXPRESSIVE_MOTION),
            m_framebuffer->texture(Framebuffer::DEPTH), activeCamera(),
            m_settings.hdrFramebuffer.depthGuardBandThickness - m_settings.hdrFramebuffer.colorGuardBandThickness);
    } rd->popState();

    // We're about to render to the actual back buffer, so swap the buffers now.
    // This call also allows the screenshot and video recording to capture the
    // previous frame just before it is displayed.
    if (submitToDisplayMode() == SubmitToDisplayMode::MAXIMIZE_THROUGHPUT) {
        swapBuffers();
    }

    // Clear the entire screen (needed even though we'll render over it, since
    // AFR uses clear() to detect that the buffer is not re-used.)
    rd->clear();

    // Perform gamma correction, bloom, and SSAA, and write to the native window frame buffer
    m_film->exposeAndRender(rd, activeCamera()->filmSettings(), m_framebuffer->texture(0), settings().hdrFramebuffer.colorGuardBandThickness.x + settings().hdrFramebuffer.depthGuardBandThickness.x, settings().hdrFramebuffer.depthGuardBandThickness.x);
}


void App::onAI() {
    GApp::onAI();
    // Add non-simulation game logic and AI code here
}


void App::onNetwork() {
    GApp::onNetwork();
    // Poll net messages here
}


void App::onSimulation(RealTime rdt, SimTime sdt, SimTime idt) {
    GApp::onSimulation(rdt, sdt, idt);

    // Example GUI dynamic layout code.  Resize the debugWindow to fill
    // the screen horizontally.
    debugWindow->setRect(Rect2D::xywh(0, 0, (float)window()->width(), debugWindow->rect().height()));
}


bool App::onEvent(const GEvent& event) {
    // Handle super-class events
    if (GApp::onEvent(event)) { return true; }

    // If you need to track individual UI events, manage them here.
    // Return true if you want to prevent other parts of the system
    // from observing this specific event.
    //
    // For example,
    // if ((event.type == GEventType::GUI_ACTION) && (event.gui.control == m_button)) { ... return true; }
    // if ((event.type == GEventType::KEY_DOWN) && (event.key.keysym.sym == GKey::TAB)) { ... return true; }
    // if ((event.type == GEventType::KEY_DOWN) && (event.key.keysym.sym == 'p')) { ... return true; }

    if ((event.type == GEventType::KEY_DOWN) && (event.key.keysym.sym == 'p')) {
        shared_ptr<DefaultRenderer> r = dynamic_pointer_cast<DefaultRenderer>(m_renderer);
        r->setDeferredShading(!r->deferredShading());
        return true;
    }

    return false;
}


void App::onUserInput(UserInput* ui) {
    GApp::onUserInput(ui);
    (void)ui;
    // Add key handling here based on the keys currently held or
    // ones that changed in the last frame.
}


void App::onPose(Array<shared_ptr<Surface> >& surface, Array<shared_ptr<Surface2D> >& surface2D) {
    GApp::onPose(surface, surface2D);

    // Append any models to the arrays that you want to later be rendered by onGraphics()
}


void App::onGraphics2D(RenderDevice* rd, Array<shared_ptr<Surface2D> >& posed2D) {
    // Render 2D objects like Widgets.  These do not receive tone mapping or gamma correction.
    Surface2D::sortAndRender(rd, posed2D);
}


void App::onCleanup() {
    // Called after the application loop ends.  Place a majority of cleanup code
    // here instead of in the constructor so that exceptions can be caught.
}
