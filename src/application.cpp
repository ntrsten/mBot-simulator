#include "application.h"
#include <iostream>


void ReadSettings(std::string iniFileLocation, Settings &settings, int &targetFramerate)
{
    //default values if any missing in .ini file

    settings.displayWidth = 1366;
    settings.displayHeight = 768;
    settings.isFullscreen = false;
    settings.targetFramerate = 60;
    settings.waitOnStart = true;

    settings.levelFile = "./resources/lvl.lvl";
    settings.tileSize = 3.f;
    settings.wallHeight = .5f;

    settings.carBodyModel = "./resources/body.obj";
    settings.carBodyTexture = "./resources/body.bmp";
    settings.carWheelModel = "./resources/wheel.obj";
    settings.carWheelTexture = "./resources/wheel.bmp";

    settings.floorTexture = "./resources/floor.bmp";
    settings.wallTexture = "./resources/wall.bmp";

    settings.wheelRadius = .32f;
    settings.wheelWidth = .6f;
    settings.wheelDistance = 1.12f;

    settings.bodyLength = 1.67f / 2;
    settings.bodyWidth = .90f / 2;
    settings.bodyHeight = .36f / 2;

    settings.backBodyAxelOffset = .56f;
    settings.bottomBodyAxelOffset = .15f;
    std::ifstream settingsFile(iniFileLocation.c_str());

    settings.wheelRotationPerSecond = 720;
    settings.fieldOfView = 45.0f;

    if (settingsFile)
    {
        std::string line;

        //reading of the .ini file

        while(std::getline(settingsFile, line))
        {
            if (line.find(std::string("displayWidth")) != std::string::npos){
                int firstChar = line.find(std::string("="));
                int lastChar = line.find(std::string("//"));
                std::istringstream valueString(line.substr(firstChar + 1, lastChar - firstChar - 2));
                valueString >> settings.displayWidth;
                continue;
            }
            if (line.find(std::string("displayHeight")) != std::string::npos){
                int firstChar = line.find(std::string("="));
                int lastChar = line.find(std::string("//"));
                std::istringstream valueString(line.substr(firstChar + 1, lastChar - firstChar - 2));
                valueString >> settings.displayHeight;
                continue;
            }
            if (line.find(std::string("isFullscreen")) != std::string::npos){
                int firstChar = line.find(std::string("="));
                int lastChar = line.find(std::string("//"));
                std::istringstream valueString(line.substr(firstChar + 1, lastChar - firstChar - 2));
                int value;
                valueString >> value;
                (value) ? settings.isFullscreen = true : settings.isFullscreen = false;
                continue;
            }
            if (line.find(std::string("targetFramerate")) != std::string::npos){
                int firstChar = line.find(std::string("="));
                int lastChar = line.find(std::string("//"));
                std::istringstream valueString(line.substr(firstChar + 1, lastChar - firstChar - 2));
                valueString >> settings.targetFramerate;
                continue;
            }
            if (line.find(std::string("waitOnStart")) != std::string::npos){
                int firstChar = line.find(std::string("="));
                int lastChar = line.find(std::string("//"));
                std::istringstream valueString(line.substr(firstChar + 1, lastChar - firstChar - 2));
                int value;
                valueString >> value;
                (value) ? settings.waitOnStart = true : settings.waitOnStart = false;
                continue;
            }
            if (line.find(std::string("levelFile")) != std::string::npos){
                int firstChar = line.find(std::string("="));
                int lastChar = line.find(std::string("//"));
                settings.levelFile = line.substr(firstChar + 1, lastChar - firstChar - 2);
                continue;
            }
            if (line.find(std::string("tileSize")) != std::string::npos){
                int firstChar = line.find(std::string("="));
                int lastChar = line.find(std::string("//"));
                std::istringstream valueString(line.substr(firstChar + 1, lastChar - firstChar - 2));
                float value;
                valueString >> value;
                settings.tileSize = value / 10;
                continue;
            }
            if (line.find(std::string("wallHeight")) != std::string::npos){
                int firstChar = line.find(std::string("="));
                int lastChar = line.find(std::string("//"));
                std::istringstream valueString(line.substr(firstChar + 1, lastChar - firstChar - 2));
                float value;
                valueString >> value;
                settings.wallHeight = value / 10;
                continue;
            }
            if (line.find(std::string("carBodyModel")) != std::string::npos){
                int firstChar = line.find(std::string("="));
                int lastChar = line.find(std::string("//"));
                settings.carBodyModel = line.substr(firstChar + 1, lastChar - firstChar - 2);
                continue;
            }
            if (line.find(std::string("carBodyTexture")) != std::string::npos){
                int firstChar = line.find(std::string("="));
                int lastChar = line.find(std::string("//"));
                settings.carBodyTexture = line.substr(firstChar + 1, lastChar - firstChar - 2);
                continue;
            }
            if (line.find(std::string("carWheelModel")) != std::string::npos){
                int firstChar = line.find(std::string("="));
                int lastChar = line.find(std::string("//"));
                settings.carWheelModel = line.substr(firstChar + 1, lastChar - firstChar - 2);
                continue;
            }
            if (line.find(std::string("carWheelTexture")) != std::string::npos){
                int firstChar = line.find(std::string("="));
                int lastChar = line.find(std::string("//"));
                settings.carWheelTexture = line.substr(firstChar + 1, lastChar - firstChar - 2);
                continue;
            }
            if (line.find(std::string("floorTexture")) != std::string::npos){
                int firstChar = line.find(std::string("="));
                int lastChar = line.find(std::string("//"));
                settings.floorTexture = line.substr(firstChar + 1, lastChar - firstChar - 2);
                continue;
            }
            if (line.find(std::string("wallTexture")) != std::string::npos){
                int firstChar = line.find(std::string("="));
                int lastChar = line.find(std::string("//"));
                settings.wallTexture = line.substr(firstChar + 1, lastChar - firstChar - 2);
                continue;
            }
            if (line.find(std::string("wheelRadius")) != std::string::npos){
                int firstChar = line.find(std::string("="));
                int lastChar = line.find(std::string("//"));
                std::istringstream valueString(line.substr(firstChar + 1, lastChar - firstChar - 2));
                float value;
                valueString >> value;
                settings.wheelRadius = value / 10;
                continue;
            }
            if (line.find(std::string("wheelWidth")) != std::string::npos){
                int firstChar = line.find(std::string("="));
                int lastChar = line.find(std::string("//"));
                std::istringstream valueString(line.substr(firstChar + 1, lastChar - firstChar - 2));
                float value;
                valueString >> value;
                settings.wheelWidth = value / 2;
                continue;
            }
            if (line.find(std::string("wheelDistance")) != std::string::npos){
                int firstChar = line.find(std::string("="));
                int lastChar = line.find(std::string("//"));
                std::istringstream valueString(line.substr(firstChar + 1, lastChar - firstChar - 2));
                float value;
                valueString >> value;
                settings.wheelDistance = value / 10;
                continue;
            }
            if (line.find(std::string("bodyLength")) != std::string::npos){
                int firstChar = line.find(std::string("="));
                int lastChar = line.find(std::string("//"));
                std::istringstream valueString(line.substr(firstChar + 1, lastChar - firstChar - 2));
                float value;
                valueString >> value;
                settings.bodyLength = value / 20;
                continue;
            }
            if (line.find(std::string("bodyWidth")) != std::string::npos){
                int firstChar = line.find(std::string("="));
                int lastChar = line.find(std::string("//"));
                std::istringstream valueString(line.substr(firstChar + 1, lastChar - firstChar - 2));
                float value;
                valueString >> value;
                settings.bodyWidth = value / 20;
                continue;
            }
            if (line.find(std::string("bodyHeight")) != std::string::npos){
                int firstChar = line.find(std::string("="));
                int lastChar = line.find(std::string("//"));
                std::istringstream valueString(line.substr(firstChar + 1, lastChar - firstChar - 2));
                float value;
                valueString >> value;
                settings.bodyHeight = value / 20;
                continue;
            }
            if (line.find(std::string("backBodyAxelOffset")) != std::string::npos){
                int firstChar = line.find(std::string("="));
                int lastChar = line.find(std::string("//"));
                std::istringstream valueString(line.substr(firstChar + 1, lastChar - firstChar - 2));
                float value;
                valueString >> value;
                settings.backBodyAxelOffset = value / 10;
                continue;
            }
            if (line.find(std::string("bottomBodyAxelOffset")) != std::string::npos){
                int firstChar = line.find(std::string("="));
                int lastChar = line.find(std::string("//"));
                std::istringstream valueString(line.substr(firstChar + 1, lastChar - firstChar - 2));
                float value;
                valueString >> value;
                settings.bottomBodyAxelOffset = value / 10;
                continue;
            }
            if (line.find(std::string("wheelRotationPerSecond")) != std::string::npos){
                int firstChar = line.find(std::string("="));
                int lastChar = line.find(std::string("//"));
                std::istringstream valueString(line.substr(firstChar + 1, lastChar - firstChar - 2));
                float value;
                valueString >> value;
                settings.wheelRotationPerSecond = value;
                continue;
            }
            if (line.find(std::string("fieldOfView")) != std::string::npos){
                int firstChar = line.find(std::string("="));
                int lastChar = line.find(std::string("//"));
                std::istringstream valueString(line.substr(firstChar + 1, lastChar - firstChar - 2));
                float value;
                valueString >> value;
                settings.fieldOfView = value;
                continue;
            }
        }
        settingsFile.close();
    }
}

Application::Application(std::string iniFileLocation)
{
    Settings settings;
    ReadSettings(iniFileLocation, settings, targetFramerate);

    //here for testing purposes
//    std::cout << settings.backBodyAxelOffset << std::endl;
//    std::cout << settings.bodyHeight << std::endl;
//    std::cout << settings.bodyLength << std::endl;
//    std::cout << settings.bodyWidth << std::endl;
//    std::cout << settings.bottomBodyAxelOffset << std::endl;
//    std::cout << settings.carBodyModel << std::endl;
//    std::cout << settings.carBodyTexture << std::endl;
//    std::cout << settings.displayHeight << std::endl;
//    std::cout << settings.displayWidth << std::endl;
//    std::cout << settings.floorTexture << std::endl;
//    std::cout << settings.isFullscreen << std::endl;
//    std::cout << settings.levelFile << std::endl;
//    std::cout << settings.targetFramerate << std::endl;
//    std::cout << settings.tileSize << std::endl;
//    std::cout << settings.waitOnStart << std::endl;
//    std::cout << settings.wallHeight << std::endl;
//    std::cout << settings.wallTexture << std::endl;
//    std::cout << settings.wheelDistance << std::endl;
//    std::cout << settings.wheelRadius << std::endl;
//    std::cout << settings.wheelRotationPerSecond << std::endl;
//    std::cout << settings.wheelWidth << std::endl;


    m_controller = new Controller(settings, &deltaTime);
}

Application::~Application()
{
    delete m_controller;
}

void Application::Run()
{
    lastTime = SDL_GetTicks();

    double timeStepMs = 1000.f/ targetFramerate;
    double someTime, endTime;

    double timeAccumulated = 0.0f;
    int frames = 0;
    double waitTime = 1.0f;

    while(m_controller->IsRunning())
    {
        lastTime = currentTime;
        currentTime = SDL_GetTicks();
        deltaTime = currentTime - lastTime;
        timeAccumulated += deltaTime;
        if(timeAccumulated >= 1000.0f)
        {
//            std::cout << frames << std::endl;
            timeAccumulated = 0.0f;
            frames = 0;
        }
        if (waitTime > 0.0f && m_controller->executeCode)
        {
            waitTime -= deltaTime;
        }
        else if (m_controller->executeCode)
        {
            waitTime += m_controller->GetNextCommand();
        }

        m_controller->PollEvent();
        m_controller->UpdateScene();

        if (waitTime > 0)
        {
            m_controller->Draw();
            frames++;
            someTime = SDL_GetTicks() - endTime;
            if (timeStepMs > someTime)
                SDL_Delay(timeStepMs - someTime);
            endTime = SDL_GetTicks();
        }
    }
}
