#include "controller.h"
#include <iostream>


void LoadLevel(const char *FilePath, std::vector< std::vector< int > > &walls, float &startX, float &startZ, float &startAngle)
{
    std::ifstream levelFile(FilePath);
    if (!levelFile){
        //std::cout << "No level file!" << std::endl;
    }
    else{
        std::vector <int> levelLine;
        std::string line;

        while(std::getline(levelFile, line))
        {
            levelLine.clear();
            levelLine.push_back(' ');
            for(unsigned int i = 0; i < line.length(); i++)
                levelLine.push_back(line[i]);
            levelLine.push_back(' ');
            walls.insert(walls.begin(), levelLine);
        }
        walls.erase(walls.begin());
        std::istringstream lineInput(line);
        lineInput >> startZ >> startX >> startAngle;
        startZ --;
        startX --;

        levelLine.clear();
        for(unsigned int i = 0; i < walls[walls.size() - 1].size(); i++)
            levelLine.push_back(' ');
        walls.push_back(levelLine);
        levelLine.clear();
        for(unsigned int i = 0; i < walls[0].size(); i++)
            levelLine.push_back(' ');
        walls.insert(walls.begin(), levelLine);

        levelFile.close();
    }
}

void LoadCode(std::vector< std::string > &setupCode, std::vector< std::string > &loopCode)
{
    std::string clipText = "";

    if (OpenClipboard(NULL))
    {
        HANDLE clip;
        clip = GetClipboardData(CF_TEXT);
        if (clip == 0){
            CloseClipboard();
            return;
        }
        char* data = static_cast<char*>( GlobalLock(clip) );
        if (data == 0){
            GlobalUnlock(clip);
            CloseClipboard();
            return;
        }
        clipText = (const char*)clip;

        GlobalUnlock(clip);
        CloseClipboard();
    }
    std::stringstream clipStream(clipText);
    std::string codeLine;
    while(std::getline(clipStream, codeLine))
    {
        if (codeLine == "void setup(){\r")
        {
            std::getline(clipStream, codeLine);
            while(codeLine != "}\r" && codeLine != "}")
            {
                setupCode.push_back(codeLine);
                std::getline(clipStream, codeLine);
            }
            setupCode.push_back(codeLine);
        }
        if (codeLine == "void loop(){\r")
        {
            std::getline(clipStream, codeLine);
            while(codeLine != "}\r" && codeLine != "}")
            {
                loopCode.push_back(codeLine);
                std::getline(clipStream, codeLine);
            }
            loopCode.push_back(codeLine);
        }
    }
}

unsigned int getSevenSegment(TTF_Font *font, const char *number){
    SDL_Color red = {255, 0, 0};
    SDL_Surface *textSurface = TTF_RenderUTF8_Blended(font, number, red);

    unsigned int textID;
    glGenTextures(1, &textID);
    glBindTexture(GL_TEXTURE_2D, textID);
    unsigned int format = GL_BGRA;
    if ((int)textSurface->format->BytesPerPixel == 4)
        format = GL_RGBA;
    glTexImage2D(GL_TEXTURE_2D, 0, format, textSurface->w, textSurface->h, 0,
                        GL_BGRA, GL_UNSIGNED_BYTE, textSurface->pixels);
    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    SDL_FreeSurface(textSurface);
    return textID;
}

Controller::Controller(Settings settings, double *deltaTime)
{
    m_deltaTime = deltaTime;
    m_settings = settings;
    executeTime = 0;
    running = true;

    float startX, startZ, startAngle;
    LoadLevel(m_settings.levelFile.c_str(), walls, startX, startZ, startAngle);

    LoadCode(setupCode, loopCode);
    currentLine = 0;
    isOnLoop = false;
    failedCondition = false;
    executeCode = !settings.waitOnStart;

    Uint32 fullscreenFlag;
    (m_settings.isFullscreen) ? fullscreenFlag = SDL_WINDOW_FULLSCREEN : fullscreenFlag = 0;
    m_display = new Display(m_settings.displayWidth, m_settings.displayHeight, "Hello Robot!", fullscreenFlag);

    m_camera = new Camera(glm::vec3(0.0f, 0.0f, 0.0f));
    cameraView = 0;
    cameraDistance = 2;

    m_shader = new Shader("./resources/shader.vs", "./resources/shader.fs");

    m_car = new Car((startX + 1) * m_settings.tileSize, (startZ + 1) * m_settings.tileSize, startAngle, settings);

    tile = new Drawable("./resources/floor.obj", m_settings.floorTexture.c_str());
    wall = new Drawable("./resources/floor.obj", m_settings.wallTexture.c_str());

    font = TTF_OpenFont("./resources/ttf.ttf", 32);
}

Controller::~Controller()
{
    delete m_car;
    delete m_display;
    delete m_shader;
    delete m_camera;
    delete tile;
    delete wall;
    TTF_CloseFont(font);
}

void Controller::UpdateScene()
{
    m_car->CalculateMovement(walls, *m_deltaTime);
}


float Controller::GetNextCommandFromArray(std::vector< std::string > carCode)
{
    if (carCode.size() == 0)
        return 1;
    if(failedCondition)
    {
        while(std::string(carCode[currentLine]).find(std::string("}")) == std::string::npos)
        {
            currentLine++;
        }
        failedCondition = false;
        currentLine++;
    }
    unsigned int commandLocation;

    commandLocation = std::string(carCode[currentLine]).find(std::string("move("));
    if(commandLocation != std::string::npos){
        char direction = carCode[currentLine][commandLocation + 5];
        unsigned int length = carCode[currentLine].end() - 1 - (carCode[currentLine].begin() + commandLocation + 7);
        std::istringstream speedString(std::string(carCode[currentLine]).substr(commandLocation + 7, length));
        float speed;
        speedString >> speed;
        if (speed > 255) speed = 255;
        if (speed < -255) speed = -255;

        if(direction == '1'){
            m_car->SetLWheelSpeed(speed);
            m_car->SetRWheelSpeed(speed);
        }else if(direction == '2'){
            m_car->SetLWheelSpeed(-speed);
            m_car->SetRWheelSpeed(-speed);
        }else if(direction == '3'){
            m_car->SetLWheelSpeed(-speed);
            m_car->SetRWheelSpeed( speed);
        }else if(direction == '4'){
            m_car->SetLWheelSpeed( speed);
            m_car->SetRWheelSpeed(-speed);
        }
        currentLine++;
        return 0;
    }
    commandLocation = std::string(carCode[currentLine]).find(std::string("_delay("));
    if(commandLocation != std::string::npos){
        unsigned int length = carCode[currentLine].end() - 1 - (carCode[currentLine].begin() + commandLocation + 7);
        float duration;
        std::istringstream durationString(std::string(carCode[currentLine]).substr(commandLocation + 7, length));
        durationString >> duration;
        currentLine++;
        return duration * 1000;
    }
    commandLocation = std::string(carCode[currentLine]).find(std::string("motor_9"));
    if(commandLocation != std::string::npos){
        unsigned int colonLocation = std::string(carCode[currentLine]).find(std::string(":"));
        unsigned int length = carCode[currentLine].end() - 2 - (carCode[currentLine].begin() + colonLocation + 2);
        std::istringstream speedString(std::string(carCode[currentLine]).substr(colonLocation + 2, length));
        float speed;
        speedString >> speed;
        if (speed > 255) speed = 255;
        if (speed < -255) speed = -255;
        m_car->SetLWheelSpeed(speed);
        currentLine++;
        return 0;
    }
    commandLocation = std::string(carCode[currentLine]).find(std::string("motor_10"));
    if(commandLocation != std::string::npos){
        unsigned int colonLocation = std::string(carCode[currentLine]).find(std::string(":"));
        unsigned int length = carCode[currentLine].end() - 2 - (carCode[currentLine].begin() + colonLocation + 2);
        std::istringstream speedString(std::string(carCode[currentLine]).substr(colonLocation + 2, length));
        float speed;
        speedString >> speed;
        if (speed > 255) speed = 255;
        if (speed < -255) speed = -255;
        m_car->SetRWheelSpeed(speed);
        currentLine++;
        return 0;
    }
    commandLocation = std::string(carCode[currentLine]).find(std::string("if(ir.keyPressed"));
    if(commandLocation != std::string::npos){
        unsigned int endLocation = std::string(carCode[currentLine]).find(std::string(")){"));
        unsigned int length = endLocation - commandLocation + 17;
        std::istringstream irCodeString(std::string(carCode[currentLine]).substr(commandLocation + 17, length));
        int irCode;
        irCodeString >> irCode;
        const Uint8 *keystate = SDL_GetKeyboardState(NULL);
        failedCondition = true;
        switch (irCode)
        {
        case 64:
             if (keystate[SDL_SCANCODE_UP]) failedCondition = false;
             break;
        case 25:
             if (keystate[SDL_SCANCODE_DOWN]) failedCondition = false;
             break;
        case 7:
             if (keystate[SDL_SCANCODE_LEFT]) failedCondition = false;
             break;
        case 9:
             if (keystate[SDL_SCANCODE_RIGHT]) failedCondition = false;
             break;
        case 69:
             if (keystate[SDL_SCANCODE_A]) failedCondition = false;
             break;
        case 70:
             if (keystate[SDL_SCANCODE_B]) failedCondition = false;
             break;
        case 71:
             if (keystate[SDL_SCANCODE_C]) failedCondition = false;
             break;
        case 68:
             if (keystate[SDL_SCANCODE_D]) failedCondition = false;
             break;
        case 67:
             if (keystate[SDL_SCANCODE_E]) failedCondition = false;
             break;
        case 13:
             if (keystate[SDL_SCANCODE_F]) failedCondition = false;
             break;
        case 22:
             if (keystate[SDL_SCANCODE_0]) failedCondition = false;
             break;
        case 12:
             if (keystate[SDL_SCANCODE_1]) failedCondition = false;
             break;
        case 24:
             if (keystate[SDL_SCANCODE_2]) failedCondition = false;
             break;
        case 94:
             if (keystate[SDL_SCANCODE_3]) failedCondition = false;
             break;
        case 8:
             if (keystate[SDL_SCANCODE_4]) failedCondition = false;
             break;
        case 28:
             if (keystate[SDL_SCANCODE_5]) failedCondition = false;
             break;
        case 90:
             if (keystate[SDL_SCANCODE_6]) failedCondition = false;
             break;
        case 66:
             if (keystate[SDL_SCANCODE_7]) failedCondition = false;
             break;
        case 82:
             if (keystate[SDL_SCANCODE_8]) failedCondition = false;
             break;
        case 74:
             if (keystate[SDL_SCANCODE_9]) failedCondition = false;
             break;
        }
        currentLine++;
        return 0;
    }
    commandLocation = std::string(carCode[currentLine]).find(std::string("ultrasonic"));
    if(commandLocation != std::string::npos){
        float value;
        unsigned int operatorLocation = std::string(carCode[currentLine]).find(std::string("<"));
        float ultrasonicReading = m_car->GetUltraSonicReading(walls);
        if (operatorLocation != std::string::npos)
        {
            if (commandLocation < operatorLocation)
            {
                std::istringstream valueString(std::string(carCode[currentLine]).
                                               substr(operatorLocation + 3,
                                               std::string(carCode[currentLine]).find(std::string(")){")) - operatorLocation - 3));
                valueString >> value;
                if(ultrasonicReading >= value)
                {
                    failedCondition = true;
                    currentLine++;
                    return 0;
                }
            }
            else
            {
                unsigned int startOfValue = std::string(carCode[currentLine]).find(std::string("if")) + 4;
                std::istringstream valueString(std::string(carCode[currentLine]).
                                               substr(startOfValue, operatorLocation - 2 - startOfValue));
                valueString >> value;
                if(value >= ultrasonicReading)
                {
                    failedCondition = true;
                    currentLine++;
                    return 0;
                }
            }
        }
        operatorLocation = std::string(carCode[currentLine]).find(std::string(">"));
        if (operatorLocation != std::string::npos)
        {
            if (commandLocation < operatorLocation)
            {
                std::istringstream valueString(std::string(carCode[currentLine]).
                                               substr(operatorLocation + 3,
                                               std::string(carCode[currentLine]).find(std::string(")){")) - operatorLocation - 3));
                valueString >> value;
                if(ultrasonicReading <= value)
                {
                    failedCondition = true;
                    currentLine++;
                    return 0;
                }
            }
            else
            {
                unsigned int startOfValue = std::string(carCode[currentLine]).find(std::string("if")) + 4;
                std::istringstream valueString(std::string(carCode[currentLine]).
                                               substr(startOfValue, operatorLocation - 2 - startOfValue));
                valueString >> value;
                if(value <= ultrasonicReading)
                {
                    failedCondition = true;
                    currentLine++;
                    return 0;
                }
            }
        }
        operatorLocation = std::string(carCode[currentLine]).find(std::string("=="));
        if (operatorLocation != std::string::npos)
        {
            if (commandLocation < operatorLocation)
            {
                std::istringstream valueString(std::string(carCode[currentLine]).
                                               substr(operatorLocation + 3,
                                               std::string(carCode[currentLine]).find(std::string("))){")) - operatorLocation - 3));
                valueString >> value;
                if(ultrasonicReading != value)
                {
                    failedCondition = true;
                    currentLine++;
                    return 0;
                }
            }
            else
            {
                unsigned int startOfValue = std::string(carCode[currentLine]).find(std::string("if")) + 5;
                std::istringstream valueString(std::string(carCode[currentLine]).
                                               substr(startOfValue, operatorLocation - 1 - startOfValue));
                valueString >> value;
                if(value != ultrasonicReading)
                {
                    failedCondition = true;
                    currentLine++;
                    return 0;
                }
            }
        }
        operatorLocation = std::string(carCode[currentLine]).find(std::string("=="));
        if (operatorLocation != std::string::npos)
        {
            if (commandLocation < operatorLocation)
            {
                std::istringstream valueString(std::string(carCode[currentLine]).
                                               substr(operatorLocation + 3,
                                               std::string(carCode[currentLine]).find(std::string("))){")) - operatorLocation - 3));
                valueString >> value;
                if(ultrasonicReading != value)
                {
                    failedCondition = true;
                    currentLine++;
                    return 0;
                }
            }
            else
            {
                unsigned int startOfValue = std::string(carCode[currentLine]).find(std::string("if")) + 5;
                std::istringstream valueString(std::string(carCode[currentLine]).
                                               substr(startOfValue, operatorLocation - 1 - startOfValue));
                valueString >> value;
                if(value != ultrasonicReading)
                {
                    failedCondition = true;
                    currentLine++;
                    return 0;
                }
            }
        }
    }
    if(std::string(carCode[currentLine]).find(std::string("}")) == std::string::npos)
    {
        currentLine++;
        return 1;
    }
    currentLine++;
    return 0;
}

float Controller::GetNextCommand()
{
    if (!isOnLoop && currentLine >= setupCode.size())
    {
        currentLine = 0;
        isOnLoop = true;
    }
    if (isOnLoop && currentLine >= loopCode.size())
    {
        currentLine = 0;
    }
    if(!isOnLoop)
        return GetNextCommandFromArray(setupCode);
    else{
        if (loopCode.size() > 1)
            return GetNextCommandFromArray(loopCode);
        else return 1;
    }
}

void Controller::PollEvent()
{
    SDL_Event event;
    while(SDL_PollEvent(&event))
    {
        switch (event.type)
        {
            case SDL_MOUSEMOTION:
                this->OnMouseEvent(event.motion.xrel, event.motion.yrel);
            break;

            case SDL_MOUSEWHEEL:
                this->OnMouseScroll(event.wheel.y);
            break;

            case SDL_QUIT:
                this->End();
            break;

            case SDL_KEYDOWN:
                if(event.key.keysym.sym == SDLK_ESCAPE)
                    this->End();
                if(event.key.keysym.sym == SDLK_RETURN)
                    executeCode = true;
                if(event.key.keysym.sym == SDLK_v)
                {
                    this->cameraView += 1;
                    if (this->cameraView > 2)
                        this->cameraView = 0;
                }
            break;
        }
    }
    this->CheckKeystates();
}

void Controller::OnMouseEvent(Sint32 xrel, Sint32 yrel)
{
    m_camera->ProcessMouseMovement(xrel, -yrel);
}

void Controller::OnMouseScroll(Sint32 scrollAmmount)
{
    cameraDistance -= scrollAmmount * 0.5;
    if (cameraDistance < 0.5)
        cameraDistance = 0.5;
    if (cameraDistance > 15)
        cameraDistance = 15;
}

void Controller::CheckKeystates()
{
    //camera movement
    const Uint8 *keystate = SDL_GetKeyboardState(NULL);
    if (keystate[SDL_SCANCODE_I])
        m_camera->ProcessKeyboard(FORWARD, *m_deltaTime);
    if (keystate[SDL_SCANCODE_K])
        m_camera->ProcessKeyboard(BACKWARD, *m_deltaTime);
    if (keystate[SDL_SCANCODE_J])
        m_camera->ProcessKeyboard(LEFT, *m_deltaTime);
    if (keystate[SDL_SCANCODE_L])
        m_camera->ProcessKeyboard(RIGHT, *m_deltaTime);
    if (keystate[SDL_SCANCODE_O])
        m_camera->ProcessKeyboard(UP, *m_deltaTime);
    if (keystate[SDL_SCANCODE_U])
        m_camera->ProcessKeyboard(DOWN, *m_deltaTime);
}

void Controller::DrawLevel()
{
    for(unsigned int i = 1; i < walls.size() - 1; i++)
    {
        for(unsigned int j = 1; j < walls[i].size() - 1; j++)
        {
            if(walls[i][j] == HORIZONTAL_WALL)
            {
                wall->Draw(m_shader,glm::vec3((float(i) - 0.5f) * m_settings.tileSize, m_settings.wallHeight / 2, (float(j)) * m_settings.tileSize), 0, 90, glm::vec3(0.0f, 0.0f, 90.0f), glm::vec3(m_settings.wallHeight, 1.0f, m_settings.tileSize));
            }
            if(walls[i][j] == VERTICAL_WALL)
            {
                wall->Draw(m_shader,glm::vec3(float(i) * m_settings.tileSize, m_settings.wallHeight / 2, (float(j) - 0.5f) * m_settings.tileSize), 90, 90, glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(m_settings.wallHeight, 1.0f, m_settings.tileSize));
            }
            if(walls[i][j] == BOTH_WALLS)
            {
                wall->Draw(m_shader,glm::vec3((float(i) - 0.5f) * m_settings.tileSize, m_settings.wallHeight / 2, (float(j)) * m_settings.tileSize), 0, 90, glm::vec3(0.0f, 0.0f, 90.0f), glm::vec3(m_settings.wallHeight, 1.0f, m_settings.tileSize));
                wall->Draw(m_shader,glm::vec3(float(i) * m_settings.tileSize, m_settings.wallHeight / 2, (float(j) - 0.5f) * m_settings.tileSize), 90, 90, glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(m_settings.wallHeight, 1.0f, m_settings.tileSize));
            }
            if((i != walls.size() - 1) && (j!= walls[i].size() - 1))
                tile->Draw(m_shader,glm::vec3(float(i) * m_settings.tileSize, -0.05f, float(j) * m_settings.tileSize), 90, 0, glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(m_settings.tileSize, 1.0f, m_settings.tileSize));
        }
    }
}

void Controller::PlaceCamera()
{
    switch(cameraView){
        case 0:
            m_camera->SetCamera(glm::vec3(m_car->x + (cameraDistance * 5) * cos(glm::radians(180 - m_car->carAngle)),
                                          m_car->y + (cameraDistance * 3),
                                          m_car->z + (cameraDistance * 5) * sin(glm::radians(180 - m_car->carAngle))),
                                -1 * m_car->carAngle, -20);
            break;
        case 1:
            break;
        case 2:
            m_camera->SetCamera(glm::vec3(m_car->x,
                                          m_car->y + m_settings.bodyHeight + m_settings.wheelRadius + m_settings.bottomBodyAxelOffset,
                                          m_car->z),
                                -1 * m_car->carAngle, 0);
            break;
    }

}

void Controller::DrawText(const char * text)
{
    unsigned int textTexture = getSevenSegment(font, text);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_NONE);
    Drawable textBox(textTexture);
    textBox.Draw(m_shader,
              glm::vec3(m_camera->Position.x + m_camera->Front.x + m_camera->Up.x * 0.3 + m_camera->Right.x * 0.5,
                                  m_camera->Position.y + m_camera->Front.y + m_camera->Up.y * 0.3 + m_camera->Right.y * 0.5,
                                  m_camera->Position.z + m_camera->Front.z + m_camera->Up.z * 0.3 + m_camera->Right.z * 0.5),
              -m_camera->Yaw, m_camera->Pitch, m_camera->Right, glm::vec3(1.0f, 1.0f, 1.0f));
    glDisable(GL_BLEND);
}

void Controller::Draw()
{
    m_shader->Use();

    m_display->Clear(0.5f,0.5f,0.5f,1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    PlaceCamera();

    glm::mat4 projection = glm::perspective(glm::radians(m_settings.fieldOfView), float(m_settings.displayWidth)/float(m_settings.displayHeight), 0.1f, 1000.0f);
    m_shader->setMat4("projection", projection);

    glm::mat4 view = m_camera->GetViewMatrix();
    m_shader->setMat4("view", view);

    m_car->Draw(m_shader);
    DrawLevel();

    std::stringstream ss;
    ss << m_car->GetUltraSonicReading(walls);
    std::string ultrasonicValueString(ss.str());

    DrawText(ultrasonicValueString.substr(0, 5).c_str());

    m_display->Update();
}

bool Controller::IsRunning()
{
    return running;
}

void Controller::End()
{
    running = false;
}
