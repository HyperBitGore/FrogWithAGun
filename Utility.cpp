#include "Header.h"


double Game::getDelta() {
	double delta = 0;
	LAST = NOW;
	NOW = SDL_GetPerformanceCounter();
	delta = (double)((NOW - LAST) * 1000 / (double)SDL_GetPerformanceFrequency());
	delta = delta * 0.001;
	return delta;
}
void Game::drawText(SDL_Renderer* rend, TTF_Font* font, std::string text, SDL_Color color, int x, int y, int w, int h) {
	SDL_Surface* surf = TTF_RenderText_Solid(font, text.c_str(), color);
	SDL_Texture* texture = SDL_CreateTextureFromSurface(rend, surf);
	SDL_Rect rect = { x, y, w, h };
	SDL_RenderCopy(rend, texture, NULL, &rect);

	SDL_FreeSurface(surf);
	SDL_DestroyTexture(texture);
}
bool Game::isColliding(Entity b, Entity e) {
	if (b.x < e.x + e.w && b.x + b.w > e.x && b.y < e.y + e.h && b.y + b.h > e.y) {
		return true;
	}
	return false;
}
float Game::getAngle(int x1, int y1, int x2, int y2) {
	float angle = -90 + atan2(y1 - y2, x1 - x2) * (180 / PI);
	return angle >= 0 ? angle : 360 + angle;
}
void Game::calcSlope(int x1, int y1, int x2, int y2, float* dx, float* dy) {
	int steps = std::max(abs(x1 - x2), abs(y1 - y2));
	if (steps == 0) {
		*dx = *dy = 0;
		return;
	}
	*dx = (x1 - x2);
	*dx /= steps;

	*dy = (y1 - y2);
	*dy /= steps;
}
void Game::loadLevel(Entity* p, Entity* s) {
	(*p).x = 400.0;
	(*p).y = 400.0;
	(*p).h = 50;
	(*p).w = 50;
	(*p).health = 10;
	(*p).attacking = false;
	(*p).damage = 12;
	(*p).atkcool = 0;
	(*p).dodgecool = 0;
	(*p).dodge = true;
	(*p).atkangle = 0;
	(*p).dtimer = 0;
	(*s).x = 400;
	(*s).y = 400;
	(*s).w = 20;
	(*s).h = 20;
	(*s).dodgecool = 0;
	(*s).health = 100;
}
