#include "CoralSceneGenerator.h"


String CoralSceneGenerator::makeCoralString(shared_ptr<Image>& color, shared_ptr<Image>& bump, shared_ptr<CoralGenerator>& coralG, float x, float y, float z, String id){

    coralG->writeCoral("branch", Color3(0, fmod(.1*x, 1), fmod(0.1*z, 1)), color, bump);
    color->save("../data-files/" + id + "-lambertian.png");
    bump->save("../data-files/" + id + "-bump.png");

    String coral = String("\ncoral_");
    coral += id
+       "= VisibleEntity {"
+"\n        model = \"starModel\";"
+"\n        frame = CFrame::fromXYZYPRDegrees("
+   (String)std::to_string(x) + ", 0," + (String)std::to_string(z) +", 0, 0, 0);"
+"\n    articulatedModelPose = UniversalMaterial::Specification { "
+"\n                lambertian = \"" + id +"-lambertian.png\";\n                bump = \"" + id +"-bump.png\"\n            };\n        };\n\n";
    return coral;
}

void CoralSceneGenerator::generateCoralScene(){
    shared_ptr<Image> color;
    shared_ptr<Image> bump;
    color = Image::create(1280, 1280, ImageFormat::RGBA8());
    bump = Image::create(1280, 1280, ImageFormat::RGBA8());
    shared_ptr<CoralGenerator> coralG(new CoralGenerator());

    TextOutput text = TextOutput("scene/massCoral.Scene.Any");
    String starCode = String("{ name = \"Coral Field\";");
    text.writeSymbol(starCode
+"\n        models = {"
+"\n            starModel = ArticulatedModel::Specification {"
+"\n            filename = \"star.off\";"
+"\n            scale = 2;"
+"\n            preprocess = {"
+"\n                setTwoSided(all(), true);"
+"\n            };"
+"\n        };"
+"\n    };\n"  
+ "\n      entities = { "
+ "\n        sun = Light { "
+ "\n            attenuation = (0, 0, 1); "
+ "\n            bulbPower = Power3(4e+005); "
+ "\n            frame = CFrame::fromXYZYPRDegrees(-15, 207, -41, -164, -77, 77);"
+ "\n            shadowMapSize = Vector2int16(2048, 2048); "
+ "\n            spotHalfAngleDegrees = 5; "
+ "\n            spotSquare = true; "
+ "\n            type = \"SPOT\"; "
+ "\n        };"
+"\n         camera = Camera {"
+"\n            frame = CFrame::fromXYZYPRDegrees(0, 0, 5);"
+"\n        };\n"); 
    for (int i = 0; i <= 10; i+=2){
        for (int j = 0; j <=10; j+=2){
            text.writeSymbol(makeCoralString(color, bump, coralG, i, 0, j, (String)std::to_string(i) + "_" + (String)std::to_string(j) +"a"));
        }
    }
    
    //String puff = makePuffball(0, 0, "8_9");
    text.writeSymbol((String)"};};");
    text.commit();
}

void CoralSceneGenerator::writeSeaScene(String& scene, int numCoralTypes, int totalCoral){
    writeModels(scene, numCoralTypes, totalCoral);
    writeEntities(scene, numCoralTypes, totalCoral);
    scene += "};";
}

void CoralSceneGenerator::writeEntities(String& scene, int numCoralTypes, int totalCoral){
    String entities("entities = {");

    // Set the Skybox
    entities += (String) "" +
    "\n        skybox = Skybox {" + 
    "\n            texture = \"cubemap/whiteroom/whiteroom-*.png\";" + 
    "\n        };" +
    "\n        camera = Camera {" +
    "\n            frame = CFrame::fromXYZYPRDegrees( -0.1, 1.4, 0, -90,-85);" +
    "\n        };" + 
    "\n        ";
    for(int i(0); i < 3; ++i){
        int fishType = Random::common().integer(0,5);
        entities += writeSchoolOfFish("fish" + (String) std::to_string(fishType), Random::common().integer(5,15));
    }

    entities += "\n    ";
    entities += "};";
    entities += "\n";

    scene += entities;
}

String CoralSceneGenerator::writeSchoolOfFish(const String& fish, int schoolNum){
    String fishSchool("");
    for (int i(0); i < schoolNum; ++i){
        float radius = .75f;
        float z = Random::common().uniform(-radius,radius);
        float phi = Random::common().uniform(0, 2*pif());
        float theta = asinf(z/radius); 
        float x = radius * cosf(theta) * cosf(phi);
        float y = radius * cosf(theta) * sinf(phi);
        fishSchool += (String) "" +
        "fish" + (String) std::to_string(schoolNum) + (String) std::to_string(i) + " = VisibleEntity {" + 
        "\n            model = \"";
        fishSchool += fish + "\";" + 
        "\n        frame = CFrame::fromXYZYPRDegrees(" + (String) std::to_string(x) + "f , ";
        fishSchool += (String) std::to_string(y) + 
                       "f , " + (String) std::to_string(z) + "f , 0.0f, 0.0f, 0.0f);" + 
        "\n        };" + 
        "\n" +
        "\n        ";
    }
    return fishSchool;
}

void CoralSceneGenerator::writeModels(String& scene, int numCoralTypes, int totalCoral){
    scene += (String) "" + 
    "models = {" +
    "\n        genericCoralModel = ArticulatedModel::Specification {" + 
    "\n            filename = \"star2.off\";" +
    "\n            scale = 2;" + 
    "\n            preprocess = {" +
    "\n                setTwoSided(all(), true);" +
    "\n            };" +
    "\n        };" +
    "\n        ";

    Array<String> allFish = FISH;
    for(int i(0); i < 6; ++i){
        int whichFish = Random::common().integer(0,14-i);
        String fish = allFish[whichFish];
        scene += (String) "" + 
        "fish" + (String) std::to_string(i) + " = ArticulatedModel::Specification {" + 
        "\n            filename = \"" + fish + "\";" + 
        "\n            scale = .001f;" + 
        "\n        };" +
        "\n        ";
        allFish.remove(whichFish);
    }
    
    scene += (String) "" + 
    "\n        shark = ArticulatedModel::Specification {" +
    "\n            filename = \"" + SHARK + "\";" +
    "\n            scale = .01f;" +
    "\n        };" +
    "\n" +
    "\n        castle = ArticulatedModel::Specification {" +
    "\n            filename = \"" + CASTLE + "\";" +
    "\n            scale = .01f;" +
    "\n        };" +
    "\n    };" +
    "\n";

}

void CoralSceneGenerator::writeScene(String& scene, String& type, String& name, int numCoralTypes, int totalCoral){
    scene = "/* -*- c++ -*- */\n{\nname = \"" + name + "\";\n\n";
    writeSeaScene(scene, numCoralTypes, totalCoral);
}