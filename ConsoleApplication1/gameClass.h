#pragma once
#include <iostream>
#include <SDL.h> 
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_messagebox.h>

#define WIDTH 180*4     //720
#define HEIGHT 180*4    //720
#define CARDSIZE 14

class gameClass
{
    SDL_Rect rect;
    SDL_Texture* texture;
    int dx, dy;
public:
    //�⺻ ������
    gameClass()
        : rect{ 0, 0, WIDTH, HEIGHT }, dx(0), dy(0), texture(nullptr) {}

    // ������: �ʱ�ȭ
    gameClass(SDL_Rect r, int valX, int valY, SDL_Texture* tex)
        : rect(r), dx(valX), dy(valY), texture(tex) {}

    // �Ҹ���: ���ҽ� ����
    ~gameClass() {
        if (texture) {
            SDL_DestroyTexture(texture);
            texture = nullptr;
        }
    }
    void setRect(int x, int y, int w, int h);
    void setXY(int valX, int valY);
    void setTexture(SDL_Texture* tex);
    void updatePosition();

    SDL_Texture* getTexture() {
        return texture;
    }

    SDL_Rect* getRect() {
        return &rect;
    }
};
//�Լ� ����
void gameBase(int cardNum[], const char* cardImage[], SDL_Renderer* renderer, SDL_Surface* imageSurface[], gameClass* obj);
void clear();