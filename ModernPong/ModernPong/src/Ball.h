#pragma once
#include <SDL.h>  
#include "Vec2.h" 
#include <cstdlib> 

const int BALL_WIDTH = 15;
const int BALL_HEIGHT = 15;
const float BALL_SPEED_X = 0.06f;
const float BALL_SPEED_Y = 0.06f;

class Ball
{
public:
    Vec2 position;
    Vec2 velocity;
    SDL_Rect rect{};

    Ball(Vec2 position, Vec2 velocity)
        : position(position), velocity(velocity)
    {
        rect.w = BALL_WIDTH;
        rect.h = BALL_HEIGHT;
        
        Reset();
    }

    void Update(float dt)
    {
        position += velocity;

        rect.x = static_cast<int>(position.x);
        rect.y = static_cast<int>(position.y);
    }

    void Draw(SDL_Renderer* renderer)
    {
        SDL_RenderFillRect(renderer, &rect);
    }

    
    void Reset()
    {
        
        position.x = 800.0f / 2.0f - BALL_WIDTH / 2.0f;
        position.y = 600.0f / 2.0f - BALL_HEIGHT / 2.0f;

       
        float x_dir = (rand() % 2 == 0) ? 1.0f : -1.0f; 
        float y_dir = (rand() % 2 == 0) ? 1.0f : -1.0f; 

        
        velocity.x = BALL_SPEED_X * x_dir;
        velocity.y = BALL_SPEED_Y * y_dir;
    }
};