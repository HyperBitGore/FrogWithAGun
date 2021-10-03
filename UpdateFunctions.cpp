#include "Header.h"


//better way of creating attack hitbox than this garbage
bool Game::playerUpdate(SDL_Renderer* rend, Entity* p, double delta, float* pvelx, float* pvely, std::vector<Entity>& colliders, Entity* t) {
	SDL_PumpEvents();
	int mx, my;
	bool click = false;
	if ((*p).x < 0) {
		(*p).x = 0;
		(*p).xvel = 0;
	}
	else if ((*p).x+50 > 1000) {
		(*p).x = 950;
		(*p).xvel = 0;
	}
	if ((*p).y < 0) {
		(*p).y = 0;
		(*p).yvel = 0;
	}
	else if ((*p).y + 50 > 1000) {
		(*p).y = 950;
		(*p).yvel = 0;
	}
	if ((*p).health > 0) {
		if (!(*p).attacking) {
			if (SDL_GetMouseState(&mx, &my) & SDL_BUTTON(SDL_BUTTON_LEFT)) {
				(*p).attacking = true;
				Entity e = { (*p).x + 25, (*p).y + 25, 10, 10, 0, 12, false, 0 };
				if (my < (*p).y) {
					e.dodge = false;
				}
				else {
					e.dodge = true;
				}
				if (mx < (*p).x) {
					e.ctex = 1;
				}
				else {
					e.ctex = 0;
				}
				e.atkangle = (*p).atkangle;
				e.dodgecool = 0;
				float dx = mx - (*p).x + 25;
				float dy = (*p).y + 25 - my;
				//e.atkcool = getAngle(dx, dy, mx, my);
				e.atkcool = 0;
				calcSlope(e.x, e.y, mx, my, &e.xvel, &e.yvel);
				colliders.push_back(e);
				click = true;
			}
		}
		else {
			(*p).atkcool += delta;
			if ((*p).atkcool > 0.2f) {
				(*p).atkcool = 0;
				(*p).attacking = false;
			}
		}
			if (keys[SDL_SCANCODE_LSHIFT]) {
				(*t).x = (*p).x;
				(*t).y = (*p).y;
			}
			if (keys[SDL_SCANCODE_W]) {
				(*pvely) -= 3 * delta;
			}
			else if (keys[SDL_SCANCODE_S]) {
				(*pvely) += 3 * delta;
			}
			else if (keys[SDL_SCANCODE_D]) {
				(*pvelx) += 3 * delta;
			}
			else if (keys[SDL_SCANCODE_A]) {
				(*pvelx) -= 3 * delta;
			}
			if ((*pvelx) > 0.4) {
				(*pvelx) = 0.4;
			}
			else if ((*pvelx) < -0.4) {
				(*pvelx) = -0.4;
			}
			if ((*pvely) > 0.4) {
				(*pvely) = 0.4;
			}
			else if ((*pvely) < -0.4) {
				(*pvely) = -0.4;
			}
			(*p).x += (*pvelx);
			(*p).y += (*pvely);
			if (keys[SDL_SCANCODE_SPACE] && !(*p).dodge) {
				(*p).dodge = true;
				if (mx > (*p).x && mx < (*p).x + (*p).w) {
					if (my >= (*p).y) {
						(*p).y += 100;
					}
					else {
						(*p).y -= 100;
					}
				}
				else {
					if (mx > (*p).x) {
						(*p).x += 100;
					}
					else {
						(*p).x -= 100;
					}
				}
			}
			if ((*p).dodge) {
				(*p).dodgecool += delta;
				if ((*p).dodgecool > 0.3f) {
					(*p).dodge = false;
					(*p).dodgecool = 0;
				}
			}
			if ((*pvely) > 0.00006) {
				(*pvely) -= 0.0006;
			}
			else if ((*pvely) < 0.00006) {
				(*pvely) += 0.0006;
			}
			if ((*pvelx) > 0) {
				(*pvelx) -= 0.0006;
			}
			else if ((*pvelx) < 0) {
				(*pvelx) += 0.0006;
			}
		}
	else {
	(*p).dtimer += delta;
	if ((*p).dtimer > 1.0) {
		(*p).health = 10;
		(*p).dtimer = 0;
	}
	}
	std::cout << (*pvelx) << std::endl;
	return click;
}