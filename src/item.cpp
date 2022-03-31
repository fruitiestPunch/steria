#include <item.h>
#include <iostream>

void itemInit()
{
    SDL_Init(SDL_INIT_EVERYTHING);
    IMG_Init(IMG_INIT_PNG);
    TTF_Init();
}

void itemQuit()
{
    SDL_Quit();
    IMG_Quit();
    TTF_Quit();
}

item::item()
{
    pos.x = 0;
    pos.y = 0;
    image = NULL;
    pos.w = 1;
    pos.h = 1;
}

item::~item()
{
    if(image != NULL)
    {
        SDL_DestroyTexture(image);
        image = NULL;
    }
}

bool item::loadImage(std::string filename)
{
    SDL_Surface* temp = IMG_Load(filename.c_str());
    if(temp != NULL)
    {
        image = SDL_CreateTextureFromSurface(ren, temp);
        SDL_FreeSurface(temp);
        if(image != NULL)
        {
            return true;
        }
    }
    return false;
}

void item::freeImage()
{
    if(image != NULL)
    {
        SDL_DestroyTexture(image);
        image = NULL;
    }
}

void item::setRenderer(SDL_Renderer* dest)
{
    ren = dest;
}

void item::setSize(int w, int h)
{
    pos.w = w;
    pos.h = h;
}

void item::setPos(int x, int y)
{
    pos.x = x;
    pos.y = y;
}

SDL_Rect* item::getPos()
{
    return &pos;
}

void item::move(int x, int y)
{
    pos.x += x;
    pos.y += y;
}

bool item::getCollision(item* other)
{
    int dx, dy, r2;
    dx = pos.x + circ.x - (other->getPos()->x + other->getCricle().x);
    dy = pos.y + circ.y - (other->getPos()->y + other->getCricle().y);
    r2 = circ.r + other->getCricle().r;
    dx *= dx;
    dy *= dy;
    r2 *= r2;
    if(dx + dy < r2){
        return true;
    }
    return false;
}

bool item::isClicked(int x, int y)
{
    int dx, dy, r2;
    dx = pos.x + circ.x - x;
    dy = pos.y + circ.y - y;
    r2 = circ.r;
    dx *= dx;
    dy *= dy;
    r2 *= r2;
    return dx + dy < r2;    
}

void item::draw(double angle)
{
    if(image != NULL)
    {
        SDL_RenderCopyEx(ren, image, NULL, &pos, angle, NULL, SDL_FLIP_NONE);
    }
    else{
        std::cout << "Image NULL at draw(angle)\n";
    }
}

void item::draw()
{
    if(image != NULL)
    {
        SDL_RenderCopy(ren, image, NULL, &pos);
    }
    else{
        std::cout << "Image NULL at draw()\n";
    }
}

void item::update(int tick)
{
    oldTick = tick;
}

circle item::getCricle()
{
    return circ;
}

void item::setCircle(int x, int y, int r)
{
    circ.x = x;
    circ.y = y;
    circ.r = r;
}

/* ============================================
// ============== animation =================*/

animation::animation(){}

animation::~animation()
{
    this->freeImages();
}

bool animation::addImage(std::string filePath)
{
    if(loadImage(filePath))
    {
        images.push_back(image);
        return true;
    }
    return false;
}

bool animation::loadAnimation(std::string p, std::string zb, std::string ext)
{
    int leng = zb.length() - 1;
    while(addImage(p + zb + ext) && !(zb[0] > '9'))
    {
        zb[leng]++;
        for(int i = leng; i > 0; i--)
        {
            if(zb[i] > '9')
            {
                zb[i-1]++;
                zb[i] = '0';
            }
        }
    }
    if(images.size() > 0)
    {
        return true;
    }
    return false;
}

void animation::next()
{
    frameCount++;
    //image = images[frameCount % (images.size() -1)];
    image = images[frameCount % (images.size())];
}

void animation::freeImages()
{
    for(int i = 0; i < images.size(); i++)
    {
        if(images[i] != NULL)
        {
            SDL_DestroyTexture(images[i]);
            images[i] = NULL;
        }
    }
}

void animation::setFPS(int fps)
{
    desiredDelta = 1000 / fps;
}

void animation::update(int tick)
{
    if(tick - oldTick > desiredDelta)
    {
        next();
        oldTick = tick;
    }
}