#include <item.h>
#include <ctime>
#include <cstdlib>
#include <vector>

int main(int argc, char** argv)
{
    srand(time(NULL));
    itemInit();
    //SDL_Window* win = SDL_CreateWindow("title", 0, 0, 240, 160, SDL_WINDOW_SHOWN);
    SDL_Window* win = SDL_CreateWindow("title", 0, 0, 240, 160, SDL_WINDOW_SHOWN);
    SDL_Renderer* screen = SDL_CreateRenderer(win, -1, 0);

    animation itm;
    itm.setFPS(4);
    itm.setRenderer(screen);
    itm.loadAnimation("res/wiz", "00", ".png");
    itm.setPos(30, 30);
    itm.setSize(16, 16);
    itm.setCircle(8, 8, 8);

    std::vector <item* > gems;
    int maxGems = 5;
    for(int i = 0; i < maxGems; i++)
    {
        gems.push_back(new item());
        gems[i]->setRenderer(screen);
        gems[i]->loadImage("res/gem.png");
        gems[i]->setPos(rand() % 200, rand() % 120);
        gems[i]->setSize(16, 16);
        gems[i]->setCircle(8, 8, 8);
    }

    animation arrow;
    arrow.setFPS(4);
    arrow.setRenderer(screen);
    arrow.setSize(32, 32);
    arrow.setPos(200, 120);
    arrow.setCircle(16,16, 16);
    std::string path = "res/arrow";
    std::string count = "0";
    std::string ext = ".png";
    for(int i = 0; i < 4; i++)
    {
        count[0] += 1;
        /*if(count[0] > '9')
        {
            count[-1]++;
            count[0] = '0';
        }*/
        arrow.addImage(path + count + ext);
    }

    int vX = 0;
    int vY = 0;
    bool bRun = true;
    bool arrowHidden = false;
    SDL_SetRenderDrawColor(screen, 0, 128, 128, 255);
    int fps = 30;
    int desiredDelta = 1000 / fps;
    while(bRun)
    {
        int startLoop = SDL_GetTicks();
        SDL_Event ev;
        while(SDL_PollEvent(&ev))
        {
            switch(ev.type)
            {
                case SDL_QUIT:
                    bRun = false;
                    break;
                case SDL_MOUSEBUTTONUP:
                    if(arrow.isClicked(ev.button.x, ev.button.y))
                    {
                        arrowHidden = true;
                    }
                    break;
                case SDL_KEYDOWN:
                    switch(ev.key.keysym.sym)
                    {
                        case SDLK_UP:
                            vY = -1;
                            break;
                        case SDLK_DOWN:
                            vY = 1;
                            break;
                        case SDLK_LEFT:
                            vX = -1;
                            break;
                        case SDLK_RIGHT:
                            vX = 1;
                            break;
                        case SDLK_q:
                            bRun = false;
                            break;
                    }
                    break;
                case SDL_KEYUP:
                    switch(ev.key.keysym.sym)
                    {
                        case SDLK_UP:
                            vY = 0;
                            break;
                        case SDLK_DOWN:
                            vY = 0;
                            break;
                        case SDLK_LEFT:
                            vX = 0;
                            break;
                        case SDLK_RIGHT:
                            vX = 0;
                            break;
                    }
                    break;
            }
        }

        itm.move(vX, vY);
        SDL_RenderClear(screen);
        itm.draw();
        if(!arrowHidden)
        {
            arrow.draw();
        }
        for(int i = 0; i < maxGems; i++)
        {
            if(itm.getCollision(gems[i]))
            {
                gems.erase(gems.begin() + i);
                maxGems--;
            }
            else{
                gems[i]->draw();
            }
        }

        SDL_RenderPresent(screen);
        arrow.update(startLoop);
        itm.update(startLoop);
        
        int delta = SDL_GetTicks() - startLoop;
        if(delta < desiredDelta)
        {
            SDL_Delay(desiredDelta - delta);
        }
    }
    SDL_DestroyWindow(win);
    itemQuit();
}