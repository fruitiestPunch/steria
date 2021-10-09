#ifndef imgs
#define imgs

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>
#include <iostream>
#include <vector>

void itemInit();
void itemQuit();

struct circle{
    int x, y, r;
};

class item{

protected:
    SDL_Renderer* ren;
    SDL_Texture* image;
    SDL_Rect pos;
    circle center;
    int oldTick;

public:
    item();
    virtual ~item();
    void setRenderer(SDL_Renderer* renderer);
    bool loadImage(std::string filename);
    void freeImage();
    void setSize(int w, int h);
    void setPos(int x, int y);
    SDL_Rect* getPos();
    void move(int x, int y);
    bool getCollision(item* other);
    bool isClicked(int x, int y);
    circle getCricleShape();
    void setCircleShape(int x, int y, int r);
    void draw(double angle);
    void draw();
    virtual void update(int tick);
    int getOldTick();
};

class animation : public item{

protected:
    int frameCount;
    std::vector <SDL_Texture*> images;
    int desiredDelta;

public:
    animation();
    virtual ~animation();
    bool addImage(std::string filePath);
    bool loadAnimation(std::string pref, std::string zbuf, std::string ext);
    void next();
    void freeImages();
    void setFPS(int FPS);
    virtual void update(int tick);
};

#endif