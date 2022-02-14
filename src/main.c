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
    float vel_x;
    float vel_y;

} ball, paddle;

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
        if (event.key.keysym.sym == SDLK_RIGHT) // if arrow right is pressed
            paddle.vel_x += PADDLE_VEL; 
        if (event.key.keysym.sym == SDLK_LEFT) // if left arrow key is pressed
            paddle.vel_x -= PADDLE_VEL;
        break;
    case SDL_KEYUP:
        paddle.vel_x = 0;
        break;
    }

}

void update(){

    // lock and release when we get frame time.
    // while(!SDL_TICKS_PASSED(SDL_GetTicks(), last_frame_time + FRAME_TARGET_TIME));
    int time_to_wait = FRAME_TARGET_TIME - (SDL_GetTicks() - last_frame_time);

    if (time_to_wait > 0 && time_to_wait <= FRAME_TARGET_TIME)
        SDL_Delay(time_to_wait);

    // calculate delta time
    // delta time: is the amount elapsed since the last frame
    float delta_time = (SDL_GetTicks() - last_frame_time) / 1000.0f;

    // logic to keep a fixed timestep 
    last_frame_time = SDL_GetTicks(); // As soon as we initialize the init window

    // move the objects
    // incrementing the position by adding one pixel
    ball.x += ball.vel_x * delta_time;
    ball.y += ball.vel_y * delta_time;

    // TODO: Update the paddle position based on its velocity
    paddle.x += paddle.vel_x * delta_time;

    // TODO: Check for ball collision with the walls
    if (ball.x < PADDLE_BORDER_LIMIT || ball.x + ball.width >= WINDOW_WIDTH){
        ball.vel_x =  -ball.vel_x;
    }
    else if (ball.y + ball.height >= WINDOW_HEIGHT){
        ball.vel_y = -ball.vel_y;
    }
    else if (ball.y < 0) {
        // TODO: game over if no collision with paddle
        ball.vel_y = -ball.vel_y;
    }
    // TODO: Check the ball collision with the paddle
    

    // TODO: Prevent paddle from moving outside the boundaries
    if (paddle.x < PADDLE_BORDER_LIMIT){
        paddle.x = PADDLE_BORDER_LIMIT;
        paddle.vel_x = 0;
    }
    else if (paddle.x + paddle.width >= WINDOW_WIDTH)
    {
        paddle.x = WINDOW_WIDTH - paddle.width;
        paddle.vel_x = 0;
    }
    
    // TODO: Check for game over wen the ball hits the bottom part of the screen
}

void setup(){
    // Ball values
    ball.x = 20;
    ball.y = 20;
    ball.width = 15;
    ball.height = 15;
    ball.vel_x = 200;
    ball.vel_y = 80;

    // Paddle
    paddle.width = 100;
    paddle.height = 20;
    paddle.x = (WINDOW_WIDTH / 2) - (paddle.width /2);
    paddle.y = WINDOW_HEIGHT - 40;
    paddle.vel_x = 0;
    paddle.vel_y = 0;
}

void render(){
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // Start drawing our game objects
    // Draw Rectangle 
    SDL_Rect ball_rect = {ball.x, ball.y, ball.width, ball.height};
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(renderer, &ball_rect);
    // Draw paddle
    SDL_Rect paddle_rect = {paddle.x, paddle.y, paddle.width, paddle.height};
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(renderer, &paddle_rect);

    // swap the rendering buffers (render and then show all)
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