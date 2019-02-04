#include "display.h"

#include <iostream>

Display::Display(int width, int height, const std::string title, Uint32 fullscreenFlag)
{
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);


    m_window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                width, height, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | fullscreenFlag);
    m_glContext = SDL_GL_CreateContext(m_window);
    if(!m_glContext)
        std::cout <<"No context"<< std::endl;
    if (glewInit() != GLEW_OK)
    {
        std::cout << "OpenGl version 3.3 or higher missing!" << std::endl;
    }

    if (TTF_Init() == -1)
        std::cout << "TTF Failed to initialize";


    SDL_SetRelativeMouseMode(SDL_TRUE);
    glEnable(GL_MULTISAMPLE);
    glEnable(GL_DEPTH_TEST);
}

void Display::Clear(float r, float g, float b, float a)
{
    glClearColor(r, g, b, a);
    glClear(GL_COLOR_BUFFER_BIT);
}

Display::~Display()
{
    SDL_GL_DeleteContext(m_glContext);
    SDL_DestroyWindow(m_window);
}

void Display::Update()
{
    SDL_GL_SwapWindow(m_window);
}
