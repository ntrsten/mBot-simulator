#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "display.h"
#include "car.h"
#include "camera.h"

#include "windows.h"


class Controller
{
    public:
        Controller(Settings m_settings, double *deltaTime);

        void PollEvent();
        void UpdateScene();
        void OnMouseEvent(Sint32 xrel, Sint32 yrel);
        void OnMouseScroll(Sint32 scrollAmmount);
        void CheckKeystates();
        void PlaceCamera();

        float GetNextCommand();
        float GetNextCommandFromArray(std::vector< std::string > carCode);

        void Draw();
        void DrawLevel();
        void DrawText(const char * text);

        bool IsRunning();
        void End();

        bool executeCode;

        virtual ~Controller();

    protected:

    private:
        bool running;

        double *m_deltaTime;

        Settings m_settings;

        Display *m_display;

        Camera *m_camera;
        unsigned int cameraView;
        float cameraDistance;

        Shader *m_shader;
        Car *m_car;
        Drawable *tile, *wall;
        std::vector< std::vector< int > > walls;

        double executeTime;
        unsigned int currentLine;
        bool isOnLoop, failedCondition;
        std::vector< std::string > setupCode, loopCode;

        TTF_Font *font;
};

#endif // CONTROLLER_H
