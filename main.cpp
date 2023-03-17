#include <SDL.h>
#include <iostream>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

const float gravity = .5;

int main(int argc, char* argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL could not initialize. SDL_Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    if (TTF_Init() < 0) {
        std::cerr << "SDL_ttf could not initialize. TTF_Error: " << TTF_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow("Physics Engine", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (!window) {
        std::cerr << "Window could not be created. SDL_Error: " << SDL_GetError() << std::endl;
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer) {
        std::cerr << "Renderer could not be created. SDL_Error: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    SDL_Rect squareRect;
    squareRect.x = SCREEN_WIDTH / 2;
    squareRect.y = 0;//SCREEN_HEIGHT / 2;
    squareRect.w = 20;
    squareRect.h = 20;

    float rectVelY;
    float rectVelX;

    SDL_Event event;
    bool quit = false;

    bool jump = false;
    bool isOnSurface = false;

    bool aDown = false;
    bool dDown = false;

    while (!quit) {
        while (SDL_PollEvent(&event) != 0) {
            if (event.type == SDL_QUIT) {
                quit = true;
            }

            switch( event.type ){
                case SDL_KEYDOWN:
                    if (strcmp(SDL_GetKeyName(event.key.keysym.sym), "Space") == 0){
                        jump = true;
                    }

                    if (strcmp(SDL_GetKeyName(event.key.keysym.sym), "D") == 0){
                        dDown = true;
                    }

                    if (strcmp(SDL_GetKeyName(event.key.keysym.sym), "A") == 0){
                        aDown = true;
                    }

                    break;

                case SDL_KEYUP:
                    if (strcmp(SDL_GetKeyName(event.key.keysym.sym), "D") == 0) {
                        dDown = false;
                    }
                    if (strcmp(SDL_GetKeyName(event.key.keysym.sym), "A") == 0) {
                        aDown = false;
                    }
                    break;

                default:
                    break;
            }
        }

        // Put program logic here

        // Top and bottom of screen collision detection
        if (squareRect.y + squareRect.h >= SCREEN_HEIGHT && rectVelY >= 0) {
            squareRect.y = SCREEN_HEIGHT - squareRect.h;
            isOnSurface = true;

            // Lets cube bounce after hitting ground
            rectVelY *= -0.3; // Sometimes too high of a value will cause it to bounce indefinitely, don't set this too high
            if (abs(rectVelY) < 1) {
                rectVelY = 0;
            }
        } else {
            squareRect.y += rectVelY;
            rectVelY += gravity;
            squareRect.x += rectVelX;
            isOnSurface = false;
            jump = false;

            // Lets cube bounce off of walls
            if (squareRect.x + squareRect.w >= SCREEN_WIDTH) {
                squareRect.x = SCREEN_WIDTH - squareRect.w;
                rectVelX *= -1;
            }
            if (squareRect.x <= 0) {
                squareRect.x = 0;
                rectVelX *= -1;
            }
        }

        // Prevent cube from going through top of screen
        if (squareRect.y <= 0 && rectVelY < 0) {
            rectVelY = 0;
            squareRect.y = 0;
        }

        if (jump && isOnSurface) {
            jump = false;
            rectVelY = -10;
        }

        if (aDown && isOnSurface && rectVelY == 0) {
            rectVelX = -10;
        }

        if (dDown && isOnSurface && rectVelY == 0) {
            rectVelX = 10;
        }

        if ((((dDown && aDown) || (!dDown && !aDown)) && isOnSurface) && rectVelY == 0) {
            rectVelX = 0;
        }



        // Start rendering

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderFillRect(renderer, &squareRect);

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();

    return 0;
}
