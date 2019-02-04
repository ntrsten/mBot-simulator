#ifndef APPLICATION_H
#define APPLICATION_H

#include "controller.h"


class Application
{
    public:
        Application(std::string iniFileLocation);

        void Run();

        virtual ~Application();

    protected:

    private:
        int targetFramerate;
        Controller *m_controller;
        double lastTime, currentTime, deltaTime;
};

#endif // APPLICATION_H
