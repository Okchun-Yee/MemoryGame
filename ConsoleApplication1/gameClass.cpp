#include "gameClass.h"
#include <iostream>
#include <SDL.h> 
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_messagebox.h>
#include <time.h>
#include <vector>
#include <algorithm> // std::find
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

void orignLoad(int cardNum, const char* cardImage, SDL_Renderer* renderer, SDL_Surface* imageSurface, gameClass& obj) {
    imageSurface = IMG_Load(cardImage);  // ���� �̹����� ��ü
    if (imageSurface == NULL) {
        cout << "Failed to load image " << endl;
        return;
    }
    obj.setTexture(SDL_CreateTextureFromSurface(renderer, imageSurface));

    // ī�� ��ġ�� ���� �̹��� ������
    SDL_RenderCopy(renderer, obj.getTexture(), NULL, obj.getRect());
    SDL_RenderPresent(renderer);  // ���� �������� �̹��� ǥ��
}

void gameExit(int& quit, int& cnt) {
    cout << "== WIN ==" << endl;

    SDL_Delay(1000);
    quit = 1;
    cnt = 0;  // ���� �� cnt �ʱ�ȭ
}
int gameBase(int cardNum[], const char* cardImage[], SDL_Renderer* renderer, SDL_Surface* imageSurface[], gameClass* obj) {
    cout << "== Game Start ==" << endl;

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
        //cout << cardNum[i] << endl;
        //�̹����� ���� ��� err
        if (imageSurface[i] == NULL) {
            cout << "Failed to load image " << cardImage[cardNum[i]] << ": " << IMG_GetError() << endl;
            return 0;
        }
    }

    // �ؽ�ó ����
    for (int i = 0; i < CARDSIZE; i++) {
        //��ü�� �̹��� �����ϱ�
        obj[i].setTexture(SDL_CreateTextureFromSurface(renderer, imageSurface[i]));
    }

    // �����ϰ� ��ġ�� ī�� �� (3~7��)
    int randomCheck = 3 + rand() % 5; // 3~7 �� ���� ����

    // ȭ���� 4��� (2x2)���� ������ �� ������ �����ϰ� ī�� ��ġ
    int numRows = 4; // �� ��
    int numCols = 4; // �� ��
    vector<pair<int, int>> usedPositions;  // ��ǥ�� set���� �����Ͽ� �ߺ��� ������ Ȯ��
    vector<int> selectedCard;  //���� ���� ī���� �̹����� ������� ����

    // ī�� ��ġ
    for (int i = 0; i < randomCheck; i++) {
        // �� ī�忡 ���� �������� ��ġ�� ���� ����
        int randX = rand() % numRows;
        int randY = rand() % numCols;
        cout << randX << "\t" << randY << endl;
        int retryCount = 0;
        //��ǥ �ߺ� �˻�, �ߺ��� ��� ���
        while (find(usedPositions.begin(),usedPositions.end(), make_pair(randX, randY))!= usedPositions.end()) {
            // �ߺ��� ��ǥ�� �߰ߵǸ� ���� ����
            randX = rand() % numRows;;
            randY = rand() % numCols;
            cout << randX << "\t" << randY << endl;
            retryCount++;
            if (retryCount > 16) {
                cout << "Over 16time..." << endl;
                break;
            }
        }
        // ���� ��ǥ ����
        usedPositions.push_back(make_pair(randX, randY));
        // ī���� ��ġ ����
        obj[i].setRect(randX*180, randY*180, imageSurface[0]->w, imageSurface[0]->h);
        selectedCard.push_back(cardNum[i]);   //���� ī�� �̹��� ����
        //cout << selectedCard.at(i) << endl;
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
            return 0;
        }

        obj[i].setTexture(SDL_CreateTextureFromSurface(renderer, imageSurface[i]));
        SDL_RenderCopy(renderer, obj[i].getTexture(), NULL, obj[i].getRect());
    }

    // ȭ�鿡 �� ��° ������ (���ο� �̹���)
    SDL_RenderPresent(renderer);

    updateGame(cardNum, cardImage, renderer, imageSurface, obj, selectedCard);
    return 1;
}

void updateGame(int cardNum[], const char* cardImage[],SDL_Renderer* renderer, SDL_Surface* imageSurface[], gameClass* obj, vector<int>selectedCard) {
    SDL_Event event;
    int quit = 0;
    int falseCnt = 0, maxFalseCnt = 10;
    int cnt = 0;

    while (!quit) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                quit = 1;
            }
            if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_ESCAPE) {
                    quit = 1;
                }
            }
            if (event.type == SDL_MOUSEBUTTONDOWN) {
                printf("Mouse : (%d,%d)\n", event.button.x, event.button.y);
                //���� ���� : falseCnt�� 30ȸ ���� �������, �Ѿ�� ���� ����
                if (cnt < selectedCard.size()&& falseCnt < maxFalseCnt) {
                    for (int i = 0; i < CARDSIZE; i++) {
                        SDL_Rect* rect = obj[i].getRect();  // ���� ��ü�� ��ġ�� ũ��
                        if (event.button.x >= rect->x && event.button.x <= rect->x + rect->w &&
                            event.button.y >= rect->y && event.button.y <= rect->y + rect->h) {
                            //printf("Card %d clicked\n", cardNum[i]); // ���õ� ī�� ��ȣ ���
                            if (cardNum[i] == selectedCard.at(cnt)) {
                                cout << cardNum[i] << " : " << selectedCard.at(cnt) << " true" << endl;
                                orignLoad(cardNum[i], cardImage[cardNum[i]], renderer, imageSurface[i], obj[i]);    //�̹��� ����
                                cnt++;
                                if (cnt == selectedCard.size()) gameExit(quit, cnt);
                            }
                            else {
                                falseCnt++;
                                cout << cardNum[i] << " : " << selectedCard.at(cnt) << " false" << endl;
                                cout << "false : " << falseCnt << " Time, Time Left : "<< maxFalseCnt - falseCnt << endl;
                                break;
                            }
                        }
                    }
                }
                else {
                    cout << "== Game Over ==" << endl;
                    cout << "ALL Open. See you Next Time." << endl;
                    for (int i = 0; i < CARDSIZE; i++) {
                        orignLoad(cardNum[i], cardImage[cardNum[i]], renderer, imageSurface[i], obj[i]);
                    }
                    quit = 1;
                    clear();
                }
            }
        }
    }
}