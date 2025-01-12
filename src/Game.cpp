#include "Game.h"
#include "Logger.h"
#include <SDL2/SDL.h>
#include "SDL2/SDL_timer.h"
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_video.h>
#include <SDL2/SDL_image.h>
#include <glm/glm.hpp>

Game::Game() {
    Logger::Log("Created a game instance");
}

Game::~Game() {
    m_isRunning = false;
    Logger::Log("Destroyed the game instance");
}

void Game::Initialize(int targetFps) {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        Logger::Err("Error initializing SDL");
        return;
    }

    // NOTE: The dispMode variable will also hold interesting infromation
    // like the refresh rate and the format.
    SDL_DisplayMode dispMode;
    if (SDL_GetCurrentDisplayMode(0, &dispMode) != 0) {
        Logger::Err("Error getting current display mode");
        return;
    }
    // m_windowWidth = dispMode.w;
    // m_windowHeight = dispMode.h;
    m_windowWidth = 1920;
    m_windowHeight = 1080;

    m_window = SDL_CreateWindow(
        "2D Game Engine",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        m_windowWidth,
        m_windowHeight,
        SDL_WINDOW_BORDERLESS
    );
    if (!m_window) {
        Logger::Err("Error creating SDL window");
        return;
    }

    m_renderer = SDL_CreateRenderer(
        m_window,
        -1,
        // SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
        SDL_RENDERER_ACCELERATED
    );
    if (!m_renderer) {
        Logger::Err("Error creating SDL renderer");
        return;
    }

    /*if (SDL_SetWindowFullscreen(m_window, SDL_WINDOW_FULLSCREEN) != 0) {*/
    /*    std::cerr << "Error making window fullscreen." << std::endl;*/
    /*    return;*/
    /*}*/

    m_targetMsPerFrame = 1000.0f / targetFps;
    m_msPreviousFrame = SDL_GetTicks();
    m_isRunning = true;
}

glm::vec2 playerPosition;
glm::vec2 playerVelocity;

void Game::Setup() {
    playerPosition = glm::vec2(10.0, 20.0);
    playerVelocity = glm::vec2(100.0, 0.0);
}

void Game::Run() {
    Setup();
    while (m_isRunning) {
        ProcessInput();
        Update();
        Render();
    }
}

void Game::ProcessInput() {
    SDL_Event sdlEvent;
    while (SDL_PollEvent(&sdlEvent)) {
        switch (sdlEvent.type) {
            case SDL_QUIT:
                m_isRunning = false;
            break;
            case SDL_KEYDOWN:
                if (sdlEvent.key.keysym.sym == SDLK_ESCAPE) {
                    m_isRunning = false;
                }
            break;
        }
    }
}

void Game::Update() {
    const unsigned int frameStartTime = SDL_GetTicks();
    const float deltaT = float(frameStartTime - m_msPreviousFrame) / 1000.0f;
    m_msPreviousFrame = frameStartTime;

    playerPosition += playerVelocity * deltaT;

    const unsigned int frameTime = SDL_GetTicks() - frameStartTime;
    if (frameTime >= 0 && frameTime < m_targetMsPerFrame) {
        SDL_Delay(m_targetMsPerFrame - frameTime);
    }
}

void Game::Render() {
    SDL_SetRenderDrawColor(m_renderer, 21, 21, 21, 255);
    SDL_RenderClear(m_renderer);


    /*SDL_Rect player = { .x = 10, .y = 10, .w = 20, .h = 20 };*/
    /*SDL_SetRenderDrawColor(m_renderer, 255, 255, 255, 255);*/
    /*SDL_RenderFillRect(m_renderer, &player);*/
    SDL_Surface* tank_surface = IMG_Load("./assets/images/tank-tiger-right.png");
    SDL_Texture* tank_texture = SDL_CreateTextureFromSurface(m_renderer, tank_surface);
    SDL_FreeSurface(tank_surface);

    SDL_Rect tank_rect = { .x = static_cast<int>(playerPosition.x), .y = static_cast<int>(playerPosition.y), .w = 32, .h = 32 };
    SDL_RenderCopy(m_renderer, tank_texture, NULL, &tank_rect);
    SDL_DestroyTexture(tank_texture);

    // ...

    SDL_RenderPresent(m_renderer);
}

void Game::Destroy() {
    SDL_DestroyRenderer(m_renderer);
    SDL_DestroyWindow(m_window);
    SDL_Quit();
}
