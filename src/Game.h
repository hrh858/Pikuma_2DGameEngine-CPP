#ifndef GAME_H
#define GAME_H
#include <SDL2/SDL.h>

class Game {
    private:
     SDL_Window* m_window;
     SDL_Renderer* m_renderer;
     bool m_isRunning;

    public:
        Game();
        ~Game();
        void Initialize();
        void Setup();
        void Run();
        void ProcessInput();
        void Update();
        void Render();
        void Destroy();

        int m_windowWidth;
        int m_windowHeight;
};

#endif
