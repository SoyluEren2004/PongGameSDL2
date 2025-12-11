#include <iostream>
#include <SDL.h> 
#include "Ball.h"
#include <cmath> 

// sabitler
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const int PADDLE_WIDTH = 20;
const int PADDLE_HEIGHT = 100;
const int PADDLE_SPEED = 20;


int player1_score = 0;
int player2_score = 0;


// raketlerin ekran dýþýna çýkmasýný engelleyen fonksiyon
void clamp_paddle(SDL_Rect& paddle) {
    paddle.y = std::max(0, paddle.y); // y nin 0'dan aþaðý düþmesini engelle
    paddle.y = std::min(SCREEN_HEIGHT - PADDLE_HEIGHT, paddle.y); // y nin zeminden yukarý çýkmasýný engelle
}


int main(int argc, char* argv[]) {
    // ekran kartindan video baslatmasini istiyoruz 
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cout << "SDL Hata: " << SDL_GetError() << std::endl;
        return -1;
    }
    std::srand(static_cast<unsigned int>(std::time(nullptr))); // rastgele sayýlarý baþlat

    SDL_Window* window = SDL_CreateWindow("ModernPong",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    // raketler
    SDL_Rect paddle = { 50, (SCREEN_HEIGHT / 2) - (PADDLE_HEIGHT / 2), PADDLE_WIDTH, PADDLE_HEIGHT };
    SDL_Rect paddle2 = { SCREEN_WIDTH - 50 - PADDLE_WIDTH, (SCREEN_HEIGHT / 2) - (PADDLE_HEIGHT / 2), PADDLE_WIDTH, PADDLE_HEIGHT };

    Ball ball(
        Vec2(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f),
        Vec2(BALL_SPEED_X, BALL_SPEED_Y)
    );

    bool isRunning = true;
    SDL_Event event;

    // OYUN DÖNGÜSÜ
    while (isRunning) {
      
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                isRunning = false;
            }
            else if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_w) paddle.y -= PADDLE_SPEED;
                if (event.key.keysym.sym == SDLK_s) paddle.y += PADDLE_SPEED;
                if (event.key.keysym.sym == SDLK_UP) paddle2.y -= PADDLE_SPEED;
                if (event.key.keysym.sym == SDLK_DOWN) paddle2.y += PADDLE_SPEED;
            }
        }

        
        ball.Update(1.0f);

        //  raket Sýnýr Kontrolü
        clamp_paddle(paddle);
        clamp_paddle(paddle2);

        //  duvar Kontrolü (tavan ve zemin)
        if (ball.position.y < 0) {
            ball.velocity.y *= -1;
        }
        else if (ball.position.y + BALL_HEIGHT > SCREEN_HEIGHT) {
            ball.velocity.y *= -1;
        }

        // raket crpýþma kontrolü (x yön degisimi)
        if (SDL_HasIntersection(&ball.rect, &paddle)) {
            ball.velocity.x *= -1;
            ball.position.x = paddle.x + paddle.w;
        }
        else if (SDL_HasIntersection(&ball.rect, &paddle2)) {
            ball.velocity.x *= -1;
            ball.position.x = paddle2.x - BALL_WIDTH;
        }

        // gol kontrolü (ekranýn dýþýna cýkarsa)
        if (ball.position.x < 0) {
            // player 2 gol atarsa skoru arttir
            player2_score++;
            ball.Reset(); // topu sýfýrla
            std::cout << "Skor: P1: " << player1_score << " - P2: " << player2_score << std::endl;
        }
        else if (ball.position.x + BALL_WIDTH > SCREEN_WIDTH) {
            // player 1 gol atarsa gene skoru arttir
            player1_score++;
            ball.Reset(); // topu sýfýrla
            std::cout << "Skor: P1: " << player1_score << " - P2: " << player2_score << std::endl;
        }


        // RENDER
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

        for (int y = 0; y < SCREEN_HEIGHT; y += 10) {
            SDL_RenderDrawPoint(renderer, SCREEN_WIDTH / 2, y);
        }

        SDL_RenderFillRect(renderer, &paddle);
        SDL_RenderFillRect(renderer, &paddle2);
        ball.Draw(renderer);

        SDL_RenderPresent(renderer);
    }

    // TEMÝZLÝK
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}