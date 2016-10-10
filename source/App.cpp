/** \file App.cpp */
#include "App.h"
#include "Rasterizer.h"
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
    edgeBuffer = Array<Array<Point2int32>>();
    thickBuffer = Array<float>();
}

void App::writeCoral(String bumpName, String colorName) {
    shared_ptr<Rasterizer> painter(new Rasterizer());
    shared_ptr<Image> color;
    shared_ptr<Image> bump;
    generateCrazyCoral(4, Point2int32(640, 360), -90, 200, 15, 16.0f, Array<String>("S"));

    try {
        int width = 1280;
        int height = 720;

        color = Image::create(width, height, ImageFormat::RGBA8());
        bump = Image::create(width, height, ImageFormat::RGB8());
        color->setAll(Color4(1,1,1,0));
        bump->setAll(Color3::black());

        for (int i(0); i < edgeBuffer.size(); ++i) {
            Point2int32 s = edgeBuffer[i][0];
            Point2int32 f = edgeBuffer[i][1];

            painter->drawThickLine(s, f, Color3(1,0,0), thickBuffer[i], color, bump);   

        }

        show(color);
        show(bump);
        color->save("../data-files/"+colorName+".png");
        bump->save("../data-files/"+bumpName+".png");
    }
    catch (...) {
        msgBox("Unable to load the image.");
    }
}

void App::generateFingerCoral(int depth, Point2int32 location, float cumulativeAngle, float drawLength, float moveAngle, float thick, Array<String>& symbolBuffer) {
    if (depth == 0) return;

    //void App::drawLine(Point2 point1, Point2 point2, Color3 c, shared_ptr<Image>& image) {

    applyFingerRules(depth, location, cumulativeAngle, drawLength, moveAngle, thick, symbolBuffer);
}

void App::applyFingerRules(int depth, Point2int32 location, float cumulativeAngle, float drawLength, float moveAngle, float thick, Array<String>& symbolBuffer) {
    //float angle = cumulativeAngle;
    //Point2int32 position = location;
    int isBracket = 0;
    Array<float> angles(cumulativeAngle);
    Array<Point2int32> positions(location);

    angles.resize(20);
    positions.resize(20);

    for (int i(0); i < symbolBuffer.size(); ++i) {
        if (symbolBuffer[i] == "-") {
            //angle -= moveAngle;
            angles[isBracket + 1] = angles[isBracket] - moveAngle;
        }
        else if (symbolBuffer[i] == "+") {
            //angle += moveAngle;
            angles[isBracket + 1] = angles[isBracket] + moveAngle;
        }
        else if (symbolBuffer[i] == "[") {
            ++isBracket;
            positions[isBracket] = positions[isBracket - 1];
        }
        else if (symbolBuffer[i] == "]") {
            --isBracket;

            //angle = cumulativeAngle;
            //position = location;
        }
        else if (symbolBuffer[i] == "F") {
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
        }
        else if(symbolBuffer[i] == "S") {

            Array<String> applyBuffer = Array<String>();

            applyBuffer.append("-", "[");
            applyBuffer.append("F", "X", "]", "-", "[", "-");
            applyBuffer.append("[", "F", "X", "]", "]", "+");
            applyBuffer.append("[", "F", "X", "]", "+", "[");
            applyBuffer.append("+", "[", "F", "X", "]", "]");
            applyBuffer.append("-", "[", "-", "[", "-", "[");
            applyBuffer.append("X", "F", "]", "]", "]", "+");
            applyBuffer.append("[", "+", "[", "+", "[", "X");
            applyBuffer.append("F", "]", "]", "]");

            generateFingerCoral(depth - 1, positions[isBracket], angles[isBracket], drawLength * 0.8f, moveAngle, thick * 0.7f, applyBuffer);
        }
        else if (symbolBuffer[i] == "X") {

            float count = G3D::Random::threadCommon().uniform(0.0f, 1.0);
            Array<String> applyBuffer = Array<String>();

            if (count > 0.2) {
                applyBuffer.append("-", "[");
                applyBuffer.append("F", "X", "]", "-", "[", "-");
                applyBuffer.append("[", "F", "X", "]", "]", "+");
                applyBuffer.append("[", "F", "X", "]", "+", "[");
                applyBuffer.append("+", "[", "F", "X", "]", "]");
                applyBuffer.append("[", "F", "X", "]");
            }
            else {
                applyBuffer.append("-", "[", "F", "X", "]");
                applyBuffer.append("+");
                applyBuffer.append("[");
                applyBuffer.append("F");
                applyBuffer.append("X");
                applyBuffer.append("]");
            }

            generateFingerCoral(depth - 1, positions[isBracket], angles[isBracket], drawLength * 0.8f, moveAngle, thick * 0.7f, applyBuffer);
        }
    }
}

void App::generateCrazyCoral(int depth, Point2int32 location, float cumulativeAngle, float drawLength, float moveAngle, float thick, Array<String>& symbolBuffer) {
    if (depth == 0) return;

    //void App::drawLine(Point2 point1, Point2 point2, Color3 c, shared_ptr<Image>& image) {

    applyCrazyRules(depth, location, cumulativeAngle, drawLength, moveAngle, thick, symbolBuffer);
}

void App::applyCrazyRules(int depth, Point2int32 location, float cumulativeAngle, float drawLength, float moveAngle, float thick, Array<String>& symbolBuffer) {
    //float angle = cumulativeAngle;
    //Point2int32 position = location;
    int isBracket = 0;
    Array<float> angles(cumulativeAngle);
    Array<Point2int32> positions(location);

    angles.resize(20);
    positions.resize(20);

    for (int i(0); i < symbolBuffer.size(); ++i) {
        if (symbolBuffer[i] == "-") {
            //angle -= moveAngle;
            angles[isBracket + 1] = angles[isBracket] - moveAngle;
        }
        else if (symbolBuffer[i] == "+") {
            //angle += moveAngle;
            angles[isBracket + 1] = angles[isBracket] + moveAngle;
        }
        else if (symbolBuffer[i] == "[") {
            ++isBracket;
            positions[isBracket] = positions[isBracket - 1];
        }
        else if (symbolBuffer[i] == "]") {
            --isBracket;

            //angle = cumulativeAngle;
            //position = location;
        }
        else if (symbolBuffer[i] == "F") {
            float randLen = drawLength * G3D::Random::threadCommon().uniform(0.5f, 1.0);


            float radians = (angles[isBracket] / 180.0f) * pif();
            int x = lround(cos(radians) * randLen) + positions[isBracket].x;
            int y = lround(sin(radians) * randLen) + positions[isBracket].y;

            Point2int32 point(x, y);
            Array<Point2int32> edge(positions[isBracket], point);
            edgeBuffer.append(edge);
            thickBuffer.append(thick);

            positions[isBracket] = point;
        }
        else if (symbolBuffer[i] == "X" || symbolBuffer[i] == "S") {

            float count = G3D::Random::threadCommon().uniform(0.0f, 1.0);
            Array<String> applyBuffer = Array<String>();

            if (count > 0.6f) {
                applyBuffer.append("-", "[");
                applyBuffer.append("F", "X", "]", "-", "[", "-");
                applyBuffer.append("[", "F", "X", "]", "]", "+");
                applyBuffer.append("[", "F", "X", "]", "+", "[");
                applyBuffer.append("+", "[", "F", "X", "]", "]");
                applyBuffer.append("-", "[", "-", "[", "-", "[");
                applyBuffer.append("X", "F", "]", "]", "]", "+");
                applyBuffer.append("[", "+", "[", "+", "[", "X");
                applyBuffer.append("F", "]", "]", "]");
            }
            else if (count > 0.2) {
                applyBuffer.append("-", "[");
                applyBuffer.append("F", "X", "]", "-", "[", "-");
                applyBuffer.append("[", "F", "X", "]", "]", "+");
                applyBuffer.append("[", "F", "X", "]", "+", "[");
                applyBuffer.append("+", "[", "F", "X", "]", "]");
                applyBuffer.append("[", "F", "X", "]");
            }
            else {
                applyBuffer.append("-", "[", "F", "X", "]");
                applyBuffer.append("+");
                applyBuffer.append("[");
                applyBuffer.append("F");
                applyBuffer.append("X");
                applyBuffer.append("]");
            }

            /*Array<String> applyBuffer("F", "-", "[", "[", "X");
            applyBuffer.append("]");
            applyBuffer.append("+");
            applyBuffer.append("[");
            applyBuffer.append("X");
            applyBuffer.append("]");
            applyBuffer.append("]");
            applyBuffer.append("+");
            applyBuffer.append("[");
            applyBuffer.append("F");
            applyBuffer.append("]");
            applyBuffer.append("[");
            applyBuffer.append("+");
            applyBuffer.append("[");
            applyBuffer.append("F");
            applyBuffer.append("X");
            applyBuffer.append("]");
            applyBuffer.append("]");
            applyBuffer.append("-");
            applyBuffer.append("[");
            applyBuffer.append("X");
            applyBuffer.append("]");*/

            generateCrazyCoral(depth - 1, positions[isBracket], angles[isBracket], drawLength * 0.8f, moveAngle, thick * 0.7f, applyBuffer);
        }
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
    writeCoral("test-bump","test-lambertian");
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
    shared_ptr<G3D::Image> map;
    try {
        int width = 600;
        int height = 500;
        image = Image::create(width, height, ImageFormat::RGB32F());
        map = Image::create(width, height, ImageFormat::RGB32F());
        image->setAll(Color4(1, 1, 1, 0));
        map->setAll(Color4(1, 1, 1, 0));

        shared_ptr<Rasterizer> painter(new Rasterizer());
        /*

                //painter->drawGradiantBackground(Color3(0, 1, 0), Color3(0, 0, 1), height, width, image);
                // painter->drawThickLine(Point2int32(0, 0), Point2int32(599, 499), Color3(1, 0, 0), 5, image);
                painter->drawLine(Point2int32(300, 0), Point2int32(300, 499), Color3(1, 0, 0), image);
                painter->drawLine(Point2int32(0, 250), Point2int32(599, 250), Color3(1, 0, 0), image);
                // Flat diagonals
                painter->drawLine(Point2int32(0, 0), Point2int32(599, 499), Color3(1, 0, 0), image); // Negative
                painter->drawLine(Point2int32(0, 499), Point2int32(599, 0), Color3(1, 0, 0), image); //Positive
                //Steep diagonals
                painter->drawLine(Point2int32(0, 0), Point2int32(100, 499), Color3(1, 0, 0), image); // Negative
                painter->drawLine(Point2int32(100, 499), Point2int32(200, 0), Color3(1, 0, 0), image); // Positive

                painter->drawThickLine(Point2int32(300, 0), Point2int32(300, 499), Color3(1, 0, 0),5, image, map);
                painter->drawThickLine(Point2int32(0, 250), Point2int32(599, 250), Color3(1, 0, 0),5, image, map);
                // Flat diagonals
                painter->drawThickLine(Point2int32(0, 0), Point2int32(599, 499), Color3(1, 0, 0),5, image, map); // Negative
                painter->drawThickLine(Point2int32(0, 499), Point2int32(599, 0), Color3(1, 0, 0),5, image, map); //Positive
                //Steep diagonals
                painter->drawThickLine(Point2int32(0, 0), Point2int32(100, 499), Color3(1, 0, 0),5, image, map); // Negative
                painter->drawThickLine(Point2int32(100, 499), Point2int32(200, 0), Color3(1, 0, 0),5, image, map); // Positive
                */
                //Really Flat diagonals
                painter->drawThickLine(Point2int32(0,250), Point2int32(599, 300), Color3(1,0,0), 5, image, map);
                /*
                //Really Steep diagonals
                painter->drawThickLine(Point2int32(400,0), Point2int32(410, 499), Color3(1,0,0), 5, image, map);

               // drawMyGraph(image);
                //drawClock(image);
                //drawCantorDust(20, 350, 220, 5, image);

                */
/*
        painter->drawThickLine(Point2int32(300, 0), Point2int32(300, 499), Color3(1, 0, 0), 5, image, map);
        painter->drawThickLine(Point2int32(0, 250), Point2int32(599, 250), Color3(1, 0, 0), 5, image, map);

        painter->drawThickLine(Point2int32(150, 0), Point2int32(450, 499), Color3(1, 0, 0), 5, image, map);
        painter->drawThickLine(Point2int32(450, 0), Point2int32(150, 499), Color3(1, 0, 0), 5, image, map);

        painter->drawThickLine(Point2int32(0, 125), Point2int32(599, 375), Color3(1, 0, 0), 5, image, map);
        painter->drawThickLine(Point2int32(0, 375), Point2int32(599, 125), Color3(1, 0, 0), 5, image, map);


        painter->drawThickLine(Point2int32(0, 0), Point2int32(599, 499), Color3(1, 0, 0), 5, image, map);
        painter->drawThickLine(Point2int32(0, 499), Point2int32(599, 0), Color3(1, 0, 0), 5, image, map);

        painter->drawThickLine(Point2int32(225, 0), Point2int32(375, 499), Color3(1, 0, 0), 5, image, map);
        painter->drawThickLine(Point2int32(375, 0), Point2int32(225, 499), Color3(1, 0, 0), 5, image, map);

*/
        painter->drawThickLine(Point2int32(300, 50), Point2int32(300, 449), Color3(1, 0, 0), 5, image, map);
        painter->drawThickLine(Point2int32(50, 250), Point2int32(549, 250), Color3(1, 0, 0), 5, image, map);

        painter->drawThickLine(Point2int32(150, 50), Point2int32(450, 449), Color3(1, 0, 0), 5, image, map);
        painter->drawThickLine(Point2int32(450, 50), Point2int32(150, 449), Color3(1, 0, 0), 5, image, map);

        painter->drawThickLine(Point2int32(50, 125), Point2int32(549, 375), Color3(1, 0, 0), 5, image, map);
        painter->drawThickLine(Point2int32(50, 375), Point2int32(549, 125), Color3(1, 0, 0), 5, image, map);


        painter->drawThickLine(Point2int32(50, 50), Point2int32(549, 449), Color3(1, 0, 0), 5, image, map);
        painter->drawThickLine(Point2int32(50, 449), Point2int32(549, 50), Color3(1, 0, 0), 5, image, map);

        painter->drawThickLine(Point2int32(225, 50), Point2int32(375, 449), Color3(1, 0, 0), 5, image, map);
        painter->drawThickLine(Point2int32(375, 50), Point2int32(225, 449), Color3(1, 0, 0), 5, image, map);


        show(image);
        show(map);
        image->convert(ImageFormat::RGB8());
        map->convert(ImageFormat::RGB8());
        image->save("../data-files/tubes.png");
        map->save("../data-files/tubesMap.png");
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
