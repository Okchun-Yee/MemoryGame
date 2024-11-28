#include "gameClass.h"
#include <iostream>
#include <SDL.h> 
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_messagebox.h>
#include <time.h>
#include <vector>
#include <set>
#include <Windows.h>

using namespace std;

void gameClass::setRect(int x, int y, int w, int h) {
    //rect : ȭ�� ����
    this->rect = { x, y, w, h };
}
void gameClass::setXY(int valX, int valY) {
    this->dx = valX;
    this->dy = valY;
}
void gameClass::setTexture(SDL_Texture* tex) {
    if (this->texture) {
        SDL_DestroyTexture(this->texture);
        this->texture = nullptr;
    }
    this->texture = tex;
}
void gameClass::updatePosition() {
    rect.x += dx;
    rect.y += dy;
}

void clear() {
    Sleep(500);
    system("cls");
}

void gameBase(int cardNum[], const char* cardImage[], SDL_Renderer* renderer, SDL_Surface* imageSurface[], gameClass* obj) {
    int cardtemp[CARDSIZE];
    srand(static_cast<unsigned int>(time(NULL)));  // ���� �õ� �� ���� ����

    // ������ ����� ũ�⸦ �����ϰ� ������, ���� ������ �Ұ����ϰ� ����
    for (int i = 0; i < CARDSIZE; i++) cardtemp[i] = rand() % 100;

    // ���� ũ���� ����
    for (int i = CARDSIZE - 1; i > 0; i--) {
        for (int j = 0; j < i; j++) {
            if (cardtemp[j] > cardtemp[j + 1]) {
                int temp = cardtemp[j];
                cardtemp[j] = cardtemp[j + 1];
                cardtemp[j + 1] = temp;

                temp = cardNum[j];
                cardNum[j] = cardNum[j + 1];
                cardNum[j + 1] = temp;
            }
        }
    }

    // �̹��� �ҷ�����
    for (int i = 0; i < CARDSIZE; i++) {
        imageSurface[i] = IMG_Load(cardImage[cardNum[i]]);
        if (imageSurface[i] == NULL) {
            cout << "Failed to load image " << cardImage[cardNum[i]] << ": " << IMG_GetError() << endl;
        }
    }

    // �ؽ�ó ����
    for (int i = 0; i < CARDSIZE; i++) {
        obj[i].setTexture(SDL_CreateTextureFromSurface(renderer, imageSurface[i]));
    }

    // �����ϰ� ��ġ�� ī�� �� (5~7��)
    int randomCheck = 5 + rand() % 3; // 5~7 �� ���� ����

    // ȭ���� 4��� (2x2)���� ������ �� ������ �����ϰ� ī�� ��ġ
    int numRows = 4; // �� ��
    int numCols = 4; // �� ��
    set<pair<int, int>> usedPositions;  // ��ǥ�� set���� �����Ͽ� �ߺ��� ������ Ȯ��

    // ī�� ��ġ
    for (int i = 0; i < randomCheck; i++) {
        // �� ī�忡 ���� �������� ��ġ�� ���� ����
        int randX = rand() % numRows;
        int randY = rand() % numCols;
        cout <<"pos_1 : " << randX << "\t" << randY << endl;
        int retryCount = 0;
        while (usedPositions.find(make_pair(randX, randY)) != usedPositions.end()) {
            // �ߺ��� ��ǥ�� �߰ߵǸ� ���� ����
            randX = rand() % numRows;;
            randY = rand() % numCols;
            cout << "pos_2 : " << randX << "\t" << randY << endl;
            retryCount++;
            if (retryCount > 16) {
                cout << "Too many retries, adjusting the position randomly..." << endl;
                break;
            }
        }
        // ���� ��ǥ ����
        usedPositions.insert(make_pair(randX, randY));
        // ī���� ��ġ ����
        obj[i].setRect(randX*180, randY*180, imageSurface[0]->w, imageSurface[0]->h);
        // ī�� ������
        SDL_RenderCopy(renderer, obj[i].getTexture(), NULL, obj[i].getRect());
        // ȭ�鿡 �׸���
        SDL_RenderPresent(renderer);
        SDL_Delay(1000);
    }

    //ȭ�鿡 ǥ�õ� �� ��ŭ�� ������
    for (int i = 0; i < randomCheck; i++) {
        clear();
        cout << i + 1 << "s" << endl;
    }

    // ���ο� �̹��� ������ (���÷� �ٸ� �̹��� ���)
    for (int i = 0; i < randomCheck; i++) {
        // ���ο� �̹��� �ε� �� ������
        imageSurface[i] = IMG_Load("images/free01.png");  // �ٸ� �̹����� ����
        if (imageSurface[i] == NULL) {
            cout << "Failed to load image " << cardImage[(cardNum[i] + 1) % CARDSIZE] << ": " << IMG_GetError() << endl;
        }

        obj[i].setTexture(SDL_CreateTextureFromSurface(renderer, imageSurface[i]));
        SDL_RenderCopy(renderer, obj[i].getTexture(), NULL, obj[i].getRect());
    }

    // ȭ�鿡 �� ��° ������ (���ο� �̹���)
    SDL_RenderPresent(renderer);
}




