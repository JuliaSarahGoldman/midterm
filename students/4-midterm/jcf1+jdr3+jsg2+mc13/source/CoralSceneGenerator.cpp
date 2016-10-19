#include "CoralSceneGenerator.h"

void CoralSceneGenerator::writeSeaScene(String& scene, int numCoralTypes, int totalCoral){

}

void CoralSceneGenerator::writeEntities(String& scene, int numCoralTypes, int totalCoral){
    String entities("entities = {");

    // Set the Skybox
    entities += (String) "" +
    "\n        skybox = Skybox {" + 
//    "\n            texture = \"cubemap/whiteroom/whiteroom-*.png\";" + 
    "\n            texture = Color3(0,0,.5);" +
    "\n        };" +
    "\n" +
    "\n        camera = Camera {" +
    "\n            frame = CFrame::fromXYZYPRDegrees(0.0, 1.0, 14.0, -1.2,4.0);" +
    "\n        };" +
    "\n        light1 = Light {" +
    "\n               attenuation = (0, 0, 1);" +
    "\n                bulbPower = Power3(2000); "+
    "\n                frame = CFrame::fromXYZYPRDegrees(3.9, 6.2, 1.4, -164, -77, 77);" +
//   "\n                 m_shadowMap = NULL; "+
    "\n                spotHalfAngleDegrees = 60;"
    "\n                spotSquare = true; "+
    "\n                type = \"SPOT\"; "+
    "\n        };"+
    "\n" +
    "\n        light2 = Light {" +
    "\n               attenuation = (0, 0, 1);" +
    "\n                bulbPower = Power3(2000); "+
    "\n                frame = CFrame::fromXYZYPRDegrees(-4.1, 5.3, .5, -164, -77, 77);" +
//    "\n                m_shadowMap = NULL; "+
    "\n                spotHalfAngleDegrees = 60;"
    "\n                spotSquare = true; "+
    "\n                type = \"SPOT\"; "+
    "\n        };\n"+
    "\n        side1 = VisibleEntity {" +
    "\n            model = \"blueRectangle\";" +
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
/*    "\n        castle1 = VisibleEntity {" +
    "\n            model = \"castle\";" +
    "\n            frame = CFrame::fromXYZYPRDegrees(4.1f,-1.9f, 2.3f,3,4,5);" +
    "\n        };" +*/
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

    entities += writeBubbles(Point2(-3.5f, 3.5f), Point2(-5.5, 5.5), Point2(-.8f,2.8f));
    
    for(int i(0); i< totalCoral; ++i) {
        String index = (String) std::to_string(Random::threadCommon().integer(1,numCoralTypes));
        int modelNum;
        if(index == "7") {
            modelNum = 1;
        }else if (index == "4" || index == "6"){
             modelNum = Random::threadCommon().integer(1,3);
        }
        else{
            modelNum = Random::threadCommon().integer(1,2);
        }
        String yPos;
        if (modelNum = 1){
            yPos = ", -1.2f,";
        } else if (modelNum = 2){
            yPos = ", -.7";
        } else if (modelNum = 3){
            yPos = ", 0.3";
        }

        entities += (String) "" + 
        "\n\n    coral" + (String) std::to_string(i) + " = VisibleEntity {"+ 
        "\n        model = \"genericCoralModel" + (String) std::to_string(modelNum) +"\";" + 
        "\n        frame = CFrame::fromXYZYPRDegrees(" + (String) std::to_string(Random::threadCommon().uniform(-5.4f,5.4f)) + ", -1.2f,"+ (String) std::to_string(Random::threadCommon().uniform(-1.0f,3.0f)) + ", 0, 0, 0);" + 
        "\n        articulatedModelPose = UniversalMaterial::Specification {"  + 
        "\n            lambertian = \"" + CORALDIR + index + ".png\";" +
        "\n            bump = BumpMap::Specification {" +
        "\n                 texture =\"" + CORALDIR + index + "_Bump.png\";" +
        "\n                 settings = BumpMap::Settings {"+
        "\n                     iterations = 0;"+
        "\n                 };"+
        "\n           };"+
        "\n        };" +
        "\n    };" +
        "\n";
    }
    entities += "\n    ";
    entities += "};";
    entities += "\n";

    scene += entities;
}

String CoralSceneGenerator::writeBubbles(Point2& xBounds, Point2& yBounds, Point2& zBounds){
    String bubbles("");
    for (int i(0); i < 100; ++i){
        float radius = 1.6f;
        float z = Random::threadCommon().uniform(-radius,radius);
        float phi = Random::threadCommon().uniform(0, 2*pif());
        float theta = asinf(z/radius); 
        float x = radius * cosf(theta) * cosf(phi);
        float y = radius * cosf(theta) * sinf(phi);
        bubbles += (String) "    bubble" + (String) std::to_string(i) + " = VisibleEntity {" + 
        "\n            model = \"bubble\";" +
        "\n            frame = CFrame::fromXYZYPRDegrees(" + (String) std::to_string(x+Random::threadCommon().uniform(-3.5f,3.5f)) + "f , " + (String) std::to_string(y) + 
                       "f , " + (String) std::to_string(z+1) + "f ," + (String) std::to_string(Random::threadCommon().uniform(0,360)) + "f, " + (String) std::to_string(Random::threadCommon().uniform(-55,55))  + "f, 0.0f);" + 
        "\n        };" + 
        "\n";
    }
    return bubbles;
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
    "\n        genericCoralModel1 = ArticulatedModel::Specification {" + 
    "\n            filename = \"star.off\";" +
    //"\n            scale = 2;" + 
    "\n            preprocess = {" +
    "\n                setTwoSided(all(), true);" +
    "\n            };" +
    "\n        };" +
    "\n        " +
    "\n       genericCoralModel2 = ArticulatedModel::Specification {" + 
    "\n            filename = \"star.off\";" +
    "\n            scale = 2;" + 
    "\n            preprocess = {" +
    "\n                setTwoSided(all(), true);" +
    "\n            };" +
    "\n        };" +
    "\n        "+
    "\n       genericCoralModel3 = ArticulatedModel::Specification {" + 
    "\n            filename = \"star.off\";" +
    "\n            scale = 3;" + 
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
/*    "\n    castle = ArticulatedModel::Specification {" +
    "\n        filename = \"" + CASTLE + "\";" +
    "\n        scale = .002f;" +
    "\n    };" +*/
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
    "\n    bubble = ArticulatedModel::Specification {" +
    "\n        filename = \"ifs/sphere.ifs\";" +
    "\n        preprocess = {" +    
    "\n            setMaterial(all(), " +
    "\n                UniversalMaterial::Specification {" +
    "\n                    lambertian = Color3(0.02);" +
    "\n                    glossy = Color4(Color3(0.3), 0.8);" +
    "\n                    transmissive = Color3(0.9, 1.0, 1.0);" +
    "\n                    emissive = Color3(0.01);" +
    "\n                    etaReflect = 1.3f;" +
    "\n                    etaTransmit = 1.00f;" +
    "\n            });" +
    "\n            transformGeometry(all(), Matrix4::scale(.1f,.1f,.1f));" + 
    "\n        };" +
    "\n    };" +
    "\n" +
     "\n    blueRectangle = ArticulatedModel::Specification {" +
    "\n        filename = \"model/cube/cube.obj\";" +
    "\n        preprocess = {" +
    "\n            setMaterial(all(), " +
    "\n                UniversalMaterial::Specification {" +
    "\n                    lambertian = Color3(0, 0.807843, 0.819608);" +
//    "\n                    glossy = Color4(Color3(0.3), 0.8);" +
//    "\n                    transmissive = Color3(0.9, 1.0, 1.0);" +
//    "\n                    emissive = Color3(0.01);" +
//    "\n                    etaReflect = 1.0;" +
//    "\n                    etaTransmit = 1.03;" +
    "\n            });" +
    "\n            transformGeometry(all(), Matrix4::scale(12.6f,6.0f,0.6f));" + 
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
    writeModels(scene, numCoralTypes, totalCoral);
    writeEntities(scene, numCoralTypes, totalCoral);
    scene += "};";
}