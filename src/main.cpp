//debug
//g++ src/*.cpp -std=c++14 -m64 -g -Wall -I include -lSDL2main -lSDL2 -lSDL2_image -o bin/debug/main && ./bin/debug/main
//release
//g++ src/*.cpp -std=c++14 -m64 -O3 -Wall -I include -lSDL2main -lSDL2 -lSDL2_image -o bin/release/main && ./bin/release/main

#include <SDL2/SDL.h>
#include "../include/item.h"
#include <ctime>    //for time in srand(time(NULL))
#include <cstdlib>  //for srand and seed
#include <vector>

int main(int argc, char** argv){
    srand(time(NULL));
    itemInit();
    SDL_Window* win = SDL_CreateWindow("steria", 1000, 500, 144, 160, SDL_WINDOW_SHOWN);
    SDL_Renderer* screen = SDL_CreateRenderer(win, -1, 0);

    animation itm;
    std::cout << itm.getOldTick() << std::endl;
    itm.setFPS(15);
    itm.setRenderer(screen);
    itm.loadAnimation("res/wiz", "00", ".png");
    itm.setPos(0, 0);
    itm.setSize(16, 16);
    itm.setCircleShape(8, 8, 8);

    std::vector <item*> gems;
    int maxGems = 5;
    for(int i = 0; i < maxGems; i++){
        gems.push_back(new item());
        gems[i]->setRenderer(screen);
        gems[i]->loadImage("res/gem.png");
        gems[i]->setPos(rand() % 128, rand() % 144);
        gems[i]->setSize(16,16);
        gems[i]->setCircleShape(8, 8, 8);
    }

    animation arrow;
    std::cout << arrow.getOldTick() << std::endl;
    arrow.setFPS(15);
    arrow.setRenderer(screen);
    arrow.setSize(16, 16);
    arrow.setPos(128, 128);
    arrow.setCircleShape(8, 8, 8);
    std::string path = "res/arrow";
    std::string count = "0";
    std::string ext = ".png";
    for(int i = 1; i <= 5; i++){
        count[0]++;
        arrow.addImage(path + count + ext);
    }

    bool arrowHidden = false;
    double ang = 0;
    //int s = 8;
    int vx = 0, vy = 0;
    bool run = true;

    SDL_SetRenderDrawColor(screen, 240, 240, 240, 255);

    int fps = 30;
    int desiredDelta = 1000 / fps;    
    while(run){
        int startingTime = SDL_GetTicks();
        SDL_Event ev;
        while(SDL_PollEvent(&ev)){
            switch (ev.type)
            {
            case SDL_QUIT:
                run = false;
                break;
            case SDL_MOUSEBUTTONUP:
                if(arrow.isClicked(ev.button.x, ev.button.y)){
                    arrowHidden = true;
                }
                break;
            case SDL_KEYDOWN:
                switch(ev.key.keysym.sym){
                    case SDLK_UP:
                        vy = -1;
                        break;
                    case SDLK_DOWN:
                        vy = +1;
                        break;
                    case SDLK_LEFT:
                        vx = -1;
                        break;
                    case SDLK_RIGHT:
                        vx = +1;
                        break;
                }
                break;
            case SDL_KEYUP:
                switch(ev.key.keysym.sym){
                    case SDLK_UP:
                        vy = 0;
                        break;
                    case SDLK_DOWN:
                        vy = 0;
                        break;
                    case SDLK_LEFT:
                        vx = 0;
                        break;
                    case SDLK_RIGHT:
                        vx = 0;
                        break;
                }
                break;
            }
        }
        //itm.setSize(s,s);
        itm.move(vx, vy);

        SDL_RenderClear(screen);
        itm.draw();
        if(!arrowHidden){
            arrow.draw();
        }
        for(int i = 0; i < maxGems; i++){
            if(itm.getCollision(gems[i])){
                gems.erase(gems.begin() + i);
                maxGems --;
            }else{
                gems[i]->draw(ang);
            }
        }

        //itm.draw(ang);
        SDL_RenderPresent(screen);
        ang++;
        //s++;
        arrow.update(startingTime);
        itm.update(startingTime);
        int delta = SDL_GetTicks() - startingTime;
        if(delta < desiredDelta){
            SDL_Delay(desiredDelta - delta);
        }
    }

    SDL_DestroyWindow(win);
    SDL_Quit();
}