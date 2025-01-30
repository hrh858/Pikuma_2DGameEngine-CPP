#include "Game.h"
#include "../Logger/Logger.h"
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

void Game::Setup() {
    // TODO: Initialize systems
    // TODO: Use the registry to create entities
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


    // TODO: Make systems update


    const unsigned int frameTime = SDL_GetTicks() - frameStartTime;
    if (frameTime >= 0 && frameTime < m_targetMsPerFrame) {
        SDL_Delay(m_targetMsPerFrame - frameTime);
    }
}

void Game::Render() {
    SDL_SetRenderDrawColor(m_renderer, 21, 21, 21, 255);
    SDL_RenderClear(m_renderer);

    // Render game objects... 

    SDL_RenderPresent(m_renderer);
}

void Game::Destroy() {
    SDL_DestroyRenderer(m_renderer);
    SDL_DestroyWindow(m_window);
    SDL_Quit();
}
