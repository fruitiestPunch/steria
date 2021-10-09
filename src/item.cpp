#include "../include/item.h"

void itemInit(){
    SDL_Init(SDL_INIT_EVERYTHING);
    IMG_Init(IMG_INIT_PNG);
}

void itemQuit(){
    SDL_Quit();
    IMG_Quit();
}

item::item(){
    pos.x = 0;
    pos.y = 0;
    image = NULL;
    pos.w = 16;
    pos.h = 16;
}

item::~item(){
    if(image != NULL){
        SDL_DestroyTexture(image);
        image = NULL;
    }
}

void item::setRenderer(SDL_Renderer* dest){
    ren = dest;
}

bool item::loadImage(std::string filename){
    SDL_Surface* temp = IMG_Load(filename.c_str());
    if(temp != NULL){
        image = SDL_CreateTextureFromSurface(ren, temp);
        SDL_FreeSurface(temp);
        if(image != NULL){
            return true;
        }
    }
    return false;
}

void item::freeImage(){
    if(image != NULL){
        SDL_DestroyTexture(image);
        image = NULL;
    }
}

void item::setSize(int w, int h){
    pos.w = w;
    pos.h = h;
}

SDL_Rect* item::getPos(){
    return &pos;
}

void item::setPos(int x, int y){
    pos.x = x;
    pos.y = y;
}

void item::move(int x, int y){
    pos.x += x;
    pos.y += y;
}

bool item::isClicked(int x, int y){
    int dx, dy, r2;
    dx = pos.x + center.x - x;
    dy = pos.y + center.y - y;
    r2 = center.r;
    dx *= dx;
    dy *= dy;
    r2 *= r2;
    return dx + dy < r2;
}

bool item::getCollision(item* other){
    int dx, dy, r2;
    dx = pos.x + center.x - (other->getPos()->x + other->getCricleShape().x);
    dy = pos.y + center.y - (other->getPos()->y + other->getCricleShape().y);
    r2 = center.r + other->getCricleShape().r;
    dx *= dx;
    dy *= dy;
    r2 *= r2;
    if(dx + dy < r2){
        return true;
    }
    return false;
}

circle item::getCricleShape(){
    return center;
}

void item::setCircleShape(int x, int y, int r){
    center.x = x;
    center.y = y;
    center.r = r;
}

void item::draw(double angle){
    if(image != NULL){
        SDL_RenderCopyEx(ren, image, NULL, &pos, angle, NULL, SDL_FLIP_NONE);
    }else{
        std::cout << "Image is NULL at draw()\n";
    }
}

void item::draw(){
    if(image != NULL){
        SDL_RenderCopy(ren, image, NULL, &pos);
    }else{
        std::cout << "Image is NULL at draw()\n";
    }
}

void item::update(int tick){
    oldTick = tick;
}

int item::getOldTick(){
    return oldTick;
}

//################################################################
//Class Animation
//################################################################

animation::animation(){
}

animation::~animation(){
    this->freeImages();
}

bool animation::addImage(std::string filePath){
    if(loadImage(filePath)){
        images.push_back(image);
        return true;
    }
    return false;
}

bool animation::loadAnimation(std::string p, std::string zb, std::string ext){
    int len = zb.length() - 1;
    while(addImage(p + zb + ext) && !(zb[0] > '9')){
        zb[len]++;
        for(int i = len; i > 0; i--){
            if(zb[i] > '9'){
                zb[i - 1]++;
                zb[i] = 0;
            }
        }
    }
    if(images.size() > 0){
        return true;
    }
    return false;
}

void animation::next(){
    frameCount++;
    image = images[frameCount % (images.size())];
}

void animation::freeImages(){
    for(int i = 0; i < (int) images.size(); i++){
        if(images[i] != NULL){
            SDL_DestroyTexture(images[i]);
            images[i] = NULL;
        }
    }
}

void animation::setFPS(int FPS){
    desiredDelta = 1000 / FPS;
}

void animation::update(int tick){
    if(tick - oldTick > desiredDelta){
        next();
        oldTick = tick;
    }
}