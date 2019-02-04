#ifndef DISPLAY_H
#define DISPLAY_H

#include <string>
#include <SDL/SDL.h>
#include <GL/glew.h>
#include <SDL/SDL_ttf.h>

class Display
{
    public:
        Display(int width, int height, const std::string title, Uint32 fullscreenFlag);

        void Clear(float r, float g, float b, float a);
        void Update();

        virtual ~Display();

    protected:

    private:
        Display(const Display& other) {}
        void operator=(const Display& other) {}

        SDL_Window *m_window;
        SDL_GLContext m_glContext;


};

#endif // DISPLAY_H
