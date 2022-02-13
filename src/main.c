#include <stdio.h>
#include <SDL2/SDL.h> // SDL files
#include <stdbool.h>
#include "./constants.h"

int game_is_running = FALSE;
SDL_Window* window = NULL; // because we want to access in the main as well
SDL_Renderer* renderer = NULL;

int last_frame_time = 0;

struct ball
{
    float x;
    float y;
    float width;
    float height;
} ball;

int initilize_window(void){
    // creates an SDL window
    /*
    
    1. Initialize SDL (check if initialization is successful)
    2. Create a window (check same as 1.)
    3. Create the renderer (SDL has to render to display everything)
        follow same checks as in step 1 and 2.
    
    */
    
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0){
        // 0 represents that the initialize was a success
        fprintf(stderr, "Error initializing SDL.\n");
        return FALSE; // if it does not work
    }
    window = SDL_CreateWindow("First window", 
                              SDL_WINDOWPOS_CENTERED, // centre of the screen
                              SDL_WINDOWPOS_CENTERED,
                              WINDOW_WIDTH,
                              WINDOW_HEIGHT,
                              SDL_WINDOW_BORDERLESS
                            );
    if (!window){
        fprintf(stderr, "Error creating SDL window. \n");
        return FALSE;
    }
    // Rederer to actually render the window (SDL)
    renderer = SDL_CreateRenderer(window, -1, 0);
    if (!renderer){
        fprintf(stderr, "Error creating SDL Renderer. \n");
        return FALSE;
    }

    // If everything is working return success
    return TRUE;
}

void process_input(){
    // Calls event that tells it to process
    SDL_Event event;
    SDL_PollEvent(&event);

    switch (event.type)
    {
    case SDL_QUIT: // The cross button at the right top of the window
        game_is_running = FALSE;
        break;
    case SDL_KEYDOWN:
        if (event.key.keysym.sym == SDLK_ESCAPE) // press escape to quit
            game_is_running = FALSE;
        break;
    }

}

void update(){

    // lock and release when we get frame time.
    while(!SDL_TICKS_PASSED(SDL_GetTicks(), last_frame_time + FRAME_TARGET_TIME));

    // calculate delta time
    // delta time: is the amount elapsed since the last frame
    float delta_time = (SDL_GetTicks() - last_frame_time) / 1000.0f;

    // logic to keep a fixed timestep 
    last_frame_time = SDL_GetTicks(); // As soon as we initialize the init window

    // move the objects
    // incrementing the position by adding one pixel
    ball.x += 30 * delta_time;
    ball.y += 20 * delta_time;
}

void setup(){
    ball.x = 20;
    ball.y = 20;
    ball.width = 15;
    ball.height = 15;
}

void render(){
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // Start drawing our game objects
    // Draw Rectangle 
    SDL_Rect ball_rect = {ball.x, ball.y, ball.width, ball.height};
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(renderer, &ball_rect);

    // swap the rendering buffers
    SDL_RenderPresent(renderer);
}

void destroy_window(){
    // Destroy everything in reverse order.
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

int main() {
    game_is_running = initilize_window();

    setup(); 

    while(game_is_running){
        process_input();
        update();
        render();
    }

    destroy_window(); //clean up after we exit the while loop

    return 0;
}