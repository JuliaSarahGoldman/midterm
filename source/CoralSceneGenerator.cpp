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

    TextOutput text = TextOutput("scene/glassTank.Scene.Any");
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
    "\n" +
    "\n        camera = Camera {" +
    "\n            frame = CFrame::fromXYZYPRDegrees(-0.1, 1.4, 0, -90,-85);" +
    "\n        };" +
    "\n" +
    "\n        side1 = VisibleEntity {" +
    "\n            model = \"glassRectangle\";" +
    "\n            frame = CFrame::fromXYZYPRDegrees(0,0,-2,0,0,0);" +
    "\n        };" +
    "\n" +
    "\n        side2 = VisibleEntity {" +
    "\n            model = \"glassRectangle\";" +
    "\n            frame = CFrame::fromXYZYPRDegrees(0,0,4,0,0,0);" +
    "\n        };" +
    "\n" +
    "\n        bottom = VisibleEntity {" +
    "\n            model = \"glassRectangle\";" +
    "\n            frame = CFrame::fromXYZYPRDegrees(0,-2.7f,1,0,90,0);" +
    "\n        };" +
    "\n" +
    "\n        squareSide1 = VisibleEntity {" +
    "\n            model = \"glassSquare\";" +
    "\n            frame = CFrame::fromXYZYPRDegrees(6,0,1.0f,0,0,0);" +
    "\n        };" +
    "\n" +
    "\n        squareSide2 = VisibleEntity {" +
    "\n            model = \"glassSquare\";" +
    "\n            frame = CFrame::fromXYZYPRDegrees(-6,0,1.0f,0,0,0);" +
    "\n        };" +
    "\n" +    
    "\n        WATER = VisibleEntity {" +
    "\n            model = \"water\";" +
    "\n            frame = CFrame::fromXYZYPRDegrees(0,1.6f,0.9f,0,0,0);" +
    "\n        };" +
    "\n" +    
    "\n        ground = VisibleEntity {" +
    "\n            model = \"ground\";" +
    "\n            frame = CFrame::fromXYZYPRDegrees(0,-2.0f,0.9f,0,0,0);" +
    "\n        };" +
    "\n" +    
    "\n        castle1 = VisibleEntity {" +
    "\n            model = \"castle\";" +
    "\n            frame = CFrame::fromXYZYPRDegrees(4.1f,-1.9f, 2.3f,3,4,5);" +
    "\n        };" +
    "\n" +
    "\n        aRock1 = VisibleEntity {" +
    "\n            model = \"rock1\";" +
    "\n            frame = CFrame::fromXYZYPRDegrees(-3.0f,-1.7f,1.7f,0,0,0);" +
    "\n        };" +
    "\n" +    
    "\n        aRock2 = VisibleEntity {" +
    "\n            model = \"rock2\";" +
    "\n            frame = CFrame::fromXYZYPRDegrees(2.1,-1.7f,0.8f,0,0,0);" +
    "\n        };" +
    "\n" +    
    "\n        aRock3 = VisibleEntity {" +
    "\n            model = \"rock3\";" +
    "\n            frame = CFrame::fromXYZYPRDegrees(-3.2,-1.7f,5.1f,0,0,0);" +
    "\n        };" +
    "\n" +    
    "\n        aRock4 = VisibleEntity {" +
    "\n            model = \"rock4\";" +
    "\n            frame = CFrame::fromXYZYPRDegrees(-4.2,-1.7f,6.3f,135.0f,0,0);" +
    "\n        };" +
    "\n" +    
    "\n        chest1 = VisibleEntity {" +
    "\n            model = \"chest\";" +
    "\n            frame = CFrame::fromXYZYPRDegrees(-4.3,-2.0f,-0.6f,-141.0f,2.2f,9.9f);" +
    "\n        };" +
    "\n" +    
    "\n     ";

    for(int i(0); i < 3; ++i){
        int fishType = Random::threadCommon().integer(0,5);
        entities += writeSchoolOfFish("fish" + (String) std::to_string(fishType), Random::threadCommon().integer(15, 25));
    }
    
    for(int i(0); i< 8; ++i) {
        String index = (String) std::to_string(Random::threadCommon().integer(1,4));
        entities += (String) "" + 
        "\n\n    coral" + (String) std::to_string(i) + " = VisibleEntity {"+ 
        "\n        model = \"genericCoralModel\";" + 
        "\n        frame = CFrame::fromXYZYPRDegrees(" + (String) std::to_string(Random::threadCommon().uniform(-5.5f,5.5f)) + ", -1.2f,"+ (String) std::to_string(Random::threadCommon().uniform(-0.8f,2.8f)) + ", 0, 0, 0);" + 
        "\n        articulatedModelPose = UniversalMaterial::Specification {"  + 
        "\n            lambertian = \"" + CORALDIR + index + ".png\";" +
        "\n            bump = \"" + CORALDIR + index + "_Bump.png\";" +
        "\n        };" +
        "\n    };" +
        "\n";
    }
    entities += "\n    ";
    entities += "};";
    entities += "\n";

    scene += entities;
}

String CoralSceneGenerator::writeSchoolOfFish(const String& fish, int schoolNum){
    String fishSchool("");
    for (int i(0); i < schoolNum; ++i){
        float radius = 1.6f;
        float z = Random::threadCommon().uniform(-radius,radius);
        float phi = Random::threadCommon().uniform(0, 2*pif());
        float theta = asinf(z/radius); 
        float x = radius * cosf(theta) * cosf(phi);
        float y = radius * cosf(theta) * sinf(phi);
        fishSchool += (String) "    fish" + (String) std::to_string(schoolNum) + (String) std::to_string(i) + " = VisibleEntity {" + 
        "\n            model = \"";
        fishSchool += fish + "\";" + 
        "\n        frame = CFrame::fromXYZYPRDegrees(" + (String) std::to_string(x+Random::threadCommon().uniform(-3.5f,3.5f)) + "f , ";
        fishSchool += (String) std::to_string(y) + 
                       "f , " + (String) std::to_string(z+1) + "f ," + (String) std::to_string(Random::threadCommon().uniform(0,360)) + "f, " + (String) std::to_string(Random::threadCommon().uniform(-55,55))  + "f, 0.0f);" + 
        "\n        };" + 
        "\n";
    }
    return fishSchool;
}

void CoralSceneGenerator::writeModels(String& scene, int numCoralTypes, int totalCoral){
    scene += (String) "" + 
    "models = {" +
    "\n        genericCoralModel = ArticulatedModel::Specification {" + 
    "\n            filename = \"star.off\";" +
    //"\n            scale = 2;" + 
    "\n            preprocess = {" +
    "\n                setTwoSided(all(), true);" +
    "\n            };" +
    "\n        };" +
    "\n        ";

    Array<String> allFish = FISH;
    for(int i(0); i < 6; ++i){
        int whichFish = Random::threadCommon().integer(0,14-i);
        String fish = allFish[whichFish];
        scene += (String) "\n" + 
        "fish" + (String) std::to_string(i) + " = ArticulatedModel::Specification {" + 
        "\n            filename = \"" + fish + "\";" + 
        "\n            scale = .001f;" + 
        "\n        };" +
        "\n" +
        "\n        ";
        allFish.remove(whichFish);
    }
    
    scene += (String) "" + 
    "\n    shark = ArticulatedModel::Specification {" +
    "\n        filename = \"" + SHARK + "\";" +
    "\n        scale = .01f;" +
    "\n    };" +
    "\n" +
    "\n    castle = ArticulatedModel::Specification {" +
    "\n        filename = \"" + CASTLE + "\";" +
    "\n        scale = .002f;" +
    "\n    };" +
    "\n" +    
    "\n    chest = ArticulatedModel::Specification {" +
    "\n        filename = \"" + CHEST + "\";" +
    "\n        scale = 1.0f;" +
    "\n    };" +
    "\n" +    
    "\n    water = ArticulatedModel::Specification {" +
    "\n        filename = \"water.obj\";" +
    "\n        preprocess = {" +
    "\n            transformGeometry(all(), Matrix4::scale(5.8f,1.0f,2.8f));" + 
    "\n        };" +
    "\n    };" +
    "\n" +
    "\n    glassRectangle = ArticulatedModel::Specification {" +
    "\n        filename = \"model/cube/cube.obj\";" +
    "\n        preprocess = {" +
    "\n            setMaterial(all(), " +
    "\n                UniversalMaterial::Specification {" +
    "\n                    lambertian = Color3(0.02);" +
    "\n                    glossy = Color4(Color3(0.3), 0.8);" +
    "\n                    transmissive = Color3(0.9, 1.0, 1.0);" +
    "\n                    emissive = Color3(0.01);" +
    "\n                    etaReflect = 1.0;" +
    "\n                    etaTransmit = 1.03;" +
    "\n            });" +
    "\n            transformGeometry(all(), Matrix4::scale(12.6f,6.0f,0.6f));" + 
    "\n        };" +
    "\n    };" +
    "\n" +
    "\n    glassSquare = ArticulatedModel::Specification {" +
    "\n        filename = \"model/cube/cube.obj\";" +
    "\n        preprocess = {" +
    "\n            setMaterial(all(), " +
    "\n                UniversalMaterial::Specification {" +
    "\n                    lambertian = Color3(0.02);" +
    "\n                    glossy = Color4(Color3(0.3), 0.8);" +
    "\n                    transmissive = Color3(0.9, 1.0, 1.0);" +
    "\n                    emissive = Color3(0.01);" +
    "\n                    etaReflect = 1.0;" +
    "\n                    etaTransmit = 1.03;" +
    "\n            });" +
    "\n            transformGeometry(all(), Matrix4::scale(0.6f,6.0f,6.0f));" + 
    "\n        };" +
    "\n    };" +
    "\n" +    
    "\n    ground = ArticulatedModel::Specification {" +
    "\n        filename = \"model/cube/cube.obj\";" +
    "\n        preprocess = {" +
    "\n            transformGeometry(all(), Matrix4::scale(11.6f,0.6f,5.6f));" + 
    "\n            setMaterial(all()," +
    "\n                UniversalMaterial::Specification {" +
    "\n                    lambertian = \"ground.jpg\";" +
    "\n                    bump = \"groundBump.jpg\";" +
    "\n            });" +
    "\n        };" +
    "\n    };" +
    "\n" +
    "\n    rock1 = ArticulatedModel::Specification {" +
    "\n        filename = \"" + ROCK5 +"\";" +
    "\n        preprocess = {" +
    "\n            transformGeometry(all(), Matrix4::scale(0.1f,0.1f,0.1f));" + 
    "\n        };" +
    "\n    };" +
    "\n" +
    "\n    rock2 = ArticulatedModel::Specification {" +
    "\n        filename = \"" + ROCK4 +"\";" +
    "\n        preprocess = {" +
    "\n            transformGeometry(all(), Matrix4::scale(0.045f,0.045f,0.045f));" + 
    "\n        };" +
    "\n    };" +
    "\n" +
    "\n    rock3 = ArticulatedModel::Specification {" +
    "\n        filename = \"" + ROCK2 +"\";" +
    "\n        preprocess = {" +
    "\n            transformGeometry(all(), Matrix4::scale(0.1f,0.1f,0.1f));" + 
    "\n        };" +
    "\n    };" +
    "\n" +
    "\n    rock4 = ArticulatedModel::Specification {" +
    "\n        filename = \"" + ROCK1 +"\";" +
    "\n        preprocess = {" +
    "\n            transformGeometry(all(), Matrix4::scale(0.1f,0.1f,0.1f));" + 
    "\n        };" +
    "\n    };" +
    "\n" +
    "\n};" +
    "\n\n";


}

void CoralSceneGenerator::writeScene(String& scene, String& type, String& name, int numCoralTypes, int totalCoral){
    scene = "/* -*- c++ -*- */\n{\nname = \"" + name + "\";\n\n";
    writeSeaScene(scene, numCoralTypes, totalCoral);
}