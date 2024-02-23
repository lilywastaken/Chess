#ifndef DEF_INPUT
#define DEF_INPUT

#include <stdio.h>
#include <iostream>
#include <math.h>
#include <stdint.h>
#include <unistd.h>
#include <utility>

#include <SDL2/SDL.h>
#include <SDL2/SDL_keyboard.h>

using namespace std;

class Input
{
    public:

    Input();

    void updateEvents();
    bool endInput() const;
    bool getKey(const SDL_Scancode key) const;
    pair<int, int> getMouse();
    bool getClick();

    private:

    SDL_Event m_event;
    bool m_key[SDL_NUM_SCANCODES];
    bool m_endInput;
    int m_x=0, m_y=0;
    bool m_click = false;
};

#endif

