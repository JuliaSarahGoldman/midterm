#pragma once
#include <G3D/G3DAll.h>
#include "App.h"
#include "CoralGenerator.h"
#include "Rasterizer.h"
#include "CoralGenerator3D.h"

class CoralSceneGenerator {
private:
    // All Possible Fish Models

    // Tropical Fish Found @ http://www.turbosquid.com/FullPreview/Index.cfm/ID/652729
    const String FISH01 = "Models/TropicalFish.zip/TropicalFish/TropicalFish01.obj";
    const String FISH02 = "Models/TropicalFish.zip/TropicalFish/TropicalFish02.obj";
    const String FISH03 = "Models/TropicalFish.zip/TropicalFish/TropicalFish03.obj";
    const String FISH04 = "Models/TropicalFish.zip/TropicalFish/TropicalFish04.obj";
    const String FISH05 = "Models/TropicalFish.zip/TropicalFish/TropicalFish05.obj";
    const String FISH06 = "Models/TropicalFish.zip/TropicalFish/TropicalFish06.obj";
    const String FISH07 = "Models/TropicalFish.zip/TropicalFish/TropicalFish07.obj";
    const String FISH08 = "Models/TropicalFish.zip/TropicalFish/TropicalFish08.obj";
    const String FISH09 = "Models/TropicalFish.zip/TropicalFish/TropicalFish09.obj";
    const String FISH10 = "Models/TropicalFish.zip/TropicalFish/TropicalFish10.obj";
    const String FISH11 = "Models/TropicalFish.zip/TropicalFish/TropicalFish11.obj";
    const String FISH12 = "Models/TropicalFish.zip/TropicalFish/TropicalFish12.obj";
    const String FISH13 = "Models/TropicalFish.zip/TropicalFish/TropicalFish13.obj";
    const String FISH14 = "Models/TropicalFish.zip/TropicalFish/TropicalFish14.obj";
    const String FISH15 = "Models/TropicalFish.zip/TropicalFish/TropicalFish15.obj";
    
    const Array<String> FISH = {FISH01, FISH02, FISH03, FISH04, FISH05, FISH06, FISH07, FISH08, FISH09, FISH10, FISH11, FISH12, FISH13, FISH14, FISH15};

    //All Other Models
    //Castle Found @ http://www.turbosquid.com/FullPreview/Index.cfm/ID/552087
    const String CASTLE = "Models/Castle.zip/Castle/Castle.obj";
    // 
    const String CASTLE2 = "Models/Castle/Castle2.obj";
    //Shark Found @ http://www.turbosquid.com/FullPreview/Index.cfm/ID/763930
    const String SHARK = "Models/Shark.zip/Shark/Shark.obj";
    //Rocks
    const String ROCK1 = "Models/outcrops.zip/granite_outcrops/RockOutcrop1.obj";
    const String ROCK2 = "Models/outcrops.zip/granite_outcrops/RockOutcrop2.obj";
    const String ROCK3 = "Models/outcrops.zip/granite_outcrops/RockOutcrop3.obj";
    const String ROCK4 = "Models/outcrops.zip/granite_outcrops/RockOutcrop4.obj";
    const String ROCK5 = "Models/outcrops.zip/granite_outcrops/RockOutcrop5.obj";

    const String CHEST = "Models/Chest/Chest.obj";

    const String CORALDIR = "Models/Coral/Coral";

protected:
    void writeSeaScene(String& scene, int numCoralTypes, int totalCoral);
    void writeModels(String& scene, int numCoralTypes, int totalCoral);
    void writeEntities(String& scene, int numCoralTypes, int totalCoral);
    String writeSchoolOfFish(const String& Fis, int schoolNum);
    String writeBubbles(Point2& xBounds, Point2& yBounds, Point2& zBounds);

public:
    void writeScene(String& scene, String& type, String& name, int numCoralTypes, int totalCoral);
};