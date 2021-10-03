#pragma once
#include <iostream>
#include <thread>
#include <chrono>
#include <string>
#include <vector>
#include <math.h>
#include <time.h>
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <SDL_image.h>
#define PI 3.1415926535
extern const Uint8* keys;
//top down movement !
//melee combat !
//fighting on very tight walkways over something %
//Very responsive and precise dodge which can go over empty space !
//Enemies weird flesh monsters %
//enemies move slow and attack are very slow %
//hardest enemies attack at slightly shorter speed than player %
//enemies have very obvious windups %

struct Entity {
	float x;
	float y;
	int w;
	int h;
	int health;
	int damage;
	bool attacking;
	float atkcool;
	float dodgecool;
	bool dodge;
	float atkangle;
	int ctex;
	float xvel;
	float yvel;
	float dtimer;
	int index;
	SDL_Texture* tex;
};


class Game {
private:
	Uint64 LAST = 0;
	Uint64 NOW = SDL_GetPerformanceCounter();
public:
	double getDelta();
	float getAngle(int x1, int y1, int x2, int y2);
	void calcSlope(int x1, int y1, int x2, int y2, float* dx, float* dy);
	void drawText(SDL_Renderer* rend, TTF_Font* font, std::string text, SDL_Color color, int x, int y, int w, int h);
	bool playerUpdate(SDL_Renderer* rend, Entity* p, double delta, float* pvelx, float* pvely, std::vector<Entity>& colliders, Entity* t);
	void loadLevel(Entity* p, Entity* s);
	bool isColliding(Entity b, Entity e);
};