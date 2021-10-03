#include "Header.h"
#undef main
bool exitf = false;
const Uint8* keys;

int main() {
	srand(time(NULL));
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) > 0) {
		std::cout << "SDL Init failed: " << SDL_GetError << std::endl;
	}
	if (!TTF_Init()) {
		std::cout << "Font init failed" << TTF_GetError << std::endl;
	}
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
		std::cout << "Error: " << Mix_GetError() << std::endl;
	}
	if (!(IMG_Init(IMG_INIT_PNG))) {
		std::cout << "Image init failed: " << IMG_GetError << std::endl;
	}
	keys = SDL_GetKeyboardState(NULL);
	SDL_Window* window = SDL_CreateWindow("Vim Jam 2", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1000, 1000, SDL_WINDOW_SHOWN);
	SDL_Renderer* rend = SDL_CreateRenderer(window, -1, 0);
	TTF_Font* font = TTF_OpenFont("DelaGothicOne-Regular.ttf", 12);
	SDL_Event e;
	double delta = 0;
	double enemyspawntimer = 0;
	double enemydif = 2.5;
	double poolcool = 0;
	double spawncool = 0;
	int dif = 1;
	int dcount = 0;
	int score = 0;
	Entity player;
	Entity spawntarget = { 400, 400, 20, 20, 100 };
	spawntarget.dodgecool = 0;
	Entity spawnpool = { 500, 500, 40, 40, 100 };
	float pvelx = 0;
	float pvely = 0;
	Game game;
	std::vector<Entity> bullets;
	std::vector<Entity> enemies;
	std::vector<Entity> spawns;
	std::vector<Entity> deads;
	SDL_Surface* playersurf = IMG_Load("player1.png");
	SDL_Texture* playertex = SDL_CreateTextureFromSurface(rend, playersurf);
	SDL_Surface* player2surf = IMG_Load("player2.png");
	SDL_Texture* player2tex = SDL_CreateTextureFromSurface(rend, player2surf);
	SDL_Surface* player3surf = IMG_Load("player3.png");
	SDL_Texture* player3tex = SDL_CreateTextureFromSurface(rend, player3surf);
	SDL_Surface* bulletsurf = IMG_Load("bullet.png");
	SDL_Texture* bullettex = SDL_CreateTextureFromSurface(rend, bulletsurf);
	SDL_Surface* poolsurf = IMG_Load("pool1.png");
	SDL_Texture* pooltex = SDL_CreateTextureFromSurface(rend, poolsurf);
	SDL_Surface* pool1surf = IMG_Load("pool2.png");
	SDL_Texture* pool1tex = SDL_CreateTextureFromSurface(rend, pool1surf);
	SDL_Surface* pool2surf = IMG_Load("pool3.png");
	SDL_Texture* pool2tex = SDL_CreateTextureFromSurface(rend, pool2surf);
	SDL_Surface* enemysurf = IMG_Load("fly1.png");
	SDL_Texture* enemytex = SDL_CreateTextureFromSurface(rend, enemysurf);
	SDL_Surface* backsurf = IMG_Load("back.png");
	SDL_Texture* backtex = SDL_CreateTextureFromSurface(rend, backsurf);
	SDL_Surface* spawnsurf = IMG_Load("spawn1.png");
	SDL_Texture* spawntex = SDL_CreateTextureFromSurface(rend, spawnsurf);
	SDL_Surface* spawn2surf = IMG_Load("spawn2.png");
	SDL_Texture* spawn2tex = SDL_CreateTextureFromSurface(rend, spawn2surf);
	SDL_Surface* deadsurf = IMG_Load("death.png");
	SDL_Texture* deadtex = SDL_CreateTextureFromSurface(rend, deadsurf);
	Mix_Chunk* edeathsound = Mix_LoadWAV("kill.wav");
	Mix_Chunk* shootsound = Mix_LoadWAV("shoot.wav");
	Mix_Chunk* spawnsound = Mix_LoadWAV("spawn.wav");
	Mix_Chunk* damagesound = Mix_LoadWAV("damage.wav");
	Mix_Chunk* deathsound = Mix_LoadWAV("death.wav");
	Mix_Chunk* backtrack = Mix_LoadWAV("frog.wav");
	spawnpool.tex = pooltex;
	spawnpool.ctex = 0;
	game.loadLevel(&player, &spawntarget);
	//SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
	Mix_VolumeChunk(backtrack, 25);
	Mix_PlayChannel(-1, backtrack, -1);
	while (!exitf) {
		while (SDL_PollEvent(&e)) {
			switch (e.type) {
			case SDL_QUIT:
				exitf = true;
				break;
			}
		}
		delta = game.getDelta();
		enemyspawntimer += delta;
		if (enemyspawntimer > enemydif) {
			int num = rand() % dif + 1;
			enemyspawntimer = 0;
			dcount++;
			if (dcount % 10 == 0) {
				dif += 1;
			}
			for (int i = 0; i < num; i++) {
				Entity e;
				int xr = rand() % 100;
				if (xr > 50) {
					e.x = 0;
				}
				else {
					e.x = 1000;
				}
				e.y = rand() % 1000;
				e.damage = 10;
				e.w = 39;
				e.h = 23;
				e.health = 100;
				if (enemydif > 0.05) {
					enemydif -= 0.005;
				}
				enemies.push_back(e);
			}
		}
		//std::cout << delta << std::endl;
		SDL_SetRenderDrawColor(rend, 0, 0, 0, 0);
		SDL_RenderClear(rend);
		int mx, my;
		SDL_PumpEvents();
		SDL_GetMouseState(&mx, &my);
		SDL_Rect back = { 0, 0, 1000, 1000 };
		SDL_RenderCopy(rend, backtex, NULL, &back);
		SDL_SetRenderDrawColor(rend, 0, 50, 255, 0);
		SDL_Rect targ = { spawntarget.x, spawntarget.y, spawntarget.w, spawntarget.h };
		SDL_RenderFillRect(rend, &targ);
		float dx = player.x - mx;
		float dy = player.y - mx;
		SDL_Point pcenter = { player.w / 2, player.h / 2 };
		float ang = game.getAngle(dx, -dy, mx, my);
		player.atkangle = ang;
		SDL_Rect prect = { player.x, player.y, player.w, player.h };
		if (player.health < 0) {
			SDL_RenderCopyEx(rend, player3tex, NULL, &prect, ang, &pcenter, SDL_FLIP_NONE);
		}
		else {
			if (player.attacking) {
				SDL_RenderCopyEx(rend, player2tex, NULL, &prect, ang, &pcenter, SDL_FLIP_NONE);
			}
			else {
				SDL_RenderCopyEx(rend, playertex, NULL, &prect, ang, &pcenter, SDL_FLIP_NONE);
			}
		}
		if (game.playerUpdate(rend, &player, delta, &pvelx, &pvely, bullets, &spawntarget)) {
			Mix_PlayChannel(-1, shootsound, 0);
		}
		SDL_Rect health = { 130, 10, player.health, 50 };
		game.drawText(rend, font, "Health", { 255, 255, 255, 0 }, 5, 10, 100, 50);
		SDL_SetRenderDrawColor(rend, 255, 50, 50, 100);
		SDL_RenderFillRect(rend, &health);
		SDL_SetRenderDrawColor(rend, 50, 255, 50, 100);
		SDL_Rect pool = { spawnpool.x, spawnpool.y, spawnpool.w, spawnpool.h };
		game.drawText(rend, font, "Spawn Pool Health", { 255, 255, 255, 0 }, 200, 10, 150, 50);
		SDL_Rect poolh = { 400 , 10, spawnpool.health, 50 };
		SDL_RenderFillRect(rend, &poolh);
		game.drawText(rend, font, "Score: ", { 255, 255, 255, 0 }, 550, 10, 100, 50);
		game.drawText(rend, font, std::to_string(score), { 255, 255, 255, 0 }, 650, 10, 100, 50);
		game.drawText(rend, font, "Property of HyperBitGore Games", { 255, 255, 255, 0 }, 10, 970, 250, 12);
		poolcool += delta;
		if (poolcool > 0.3) {
			poolcool = 0;
			spawnpool.ctex++;
			if (spawnpool.ctex > 2) {
				spawnpool.ctex = 0;
			}
			switch(spawnpool.ctex)
			{
			case 0:
				spawnpool.tex = pooltex;
				break;
			case 1:
				spawnpool.tex = pool1tex;
				break;
			case 2:
				spawnpool.tex = pool2tex;
				break;
			}
		}
		spawncool += delta;
		if (spawncool > 4.0) {
			Entity e;
			e.x = spawnpool.x;
			e.y = spawnpool.y;
			e.dodge = false;
			e.h = 20;
			e.w = 20;
			e.dtimer = 0;
			e.ctex = 0;
			e.tex = spawntex;
			e.health = 10;
			e.index = spawns.size();
			spawns.push_back(e);
			spawncool = 0;
			Mix_PlayChannel(-1, spawnsound, 0);
		}
		SDL_RenderCopy(rend, spawnpool.tex, NULL, &pool);
		for (int i = 0; i < bullets.size(); i++) {
			bullets[i].dodgecool += delta;
			if (bullets[i].dodgecool > 0.003) {
				bullets[i].x += -bullets[i].xvel;
				bullets[i].y += -bullets[i].yvel;
				bullets[i].dodgecool = 0;
			}
			SDL_Point center = {bullets[i].w / 2, bullets[i].h/2};
			SDL_Rect rect = { bullets[i].x, bullets[i].y, bullets[i].w, bullets[i].h };
			SDL_RenderCopyEx(rend, bullettex, NULL, &rect, bullets[i].atkangle, &center, SDL_FLIP_NONE);
			for (auto& j : enemies) {
				if (game.isColliding(bullets[i], j)) {
					j.health -= 100;
					score += 10;
					bullets[i].atkcool = 0.31;
					Mix_PlayChannel(-1, edeathsound, 0);
				}
			}
			if (bullets[i].y < 0 || bullets[i].y > 1000 || bullets[i].x < 0 || bullets[i].x > 1000) {
				bullets[i].atkcool = 0.31;
			}
			if (bullets[i].atkcool > 0.3f) {
				bullets.erase(bullets.begin() + i);
			}
		}
		for (int i = 0; i < enemies.size(); i++) {
			if (spawns.size() > 0) {
				if (enemies[i].x < spawns[0].x) {
					enemies[i].x += 200 * delta;
				}
				else {
					enemies[i].x -= 200 * delta;
				}
				if (enemies[i].y < spawns[0].y) {
					enemies[i].y += 200 * delta;
				}
				else {
					enemies[i].y -= 200 * delta;
				}
			}
			else if (player.health > 0) {
				if (enemies[i].x < player.x) {
					enemies[i].x += 200 * delta;
				}
				else {
					enemies[i].x -= 200 * delta;
				}
				if (enemies[i].y < player.y) {
					enemies[i].y += 200 * delta;
				}
				else {
					enemies[i].y -= 200 * delta;
				}
			}
			else {
				if (enemies[i].x < pool.x) {
					enemies[i].x += 200 * delta;
				}
				else {
					enemies[i].x -= 200 * delta;
				}
				if (enemies[i].y < pool.y) {
					enemies[i].y += 200 * delta;
				}
				else {
					enemies[i].y -= 200 * delta;
				}
			}
			if (game.isColliding(enemies[i], player)) {
				player.health--;
				Mix_PlayChannel(-1, damagesound, 0);
			}
			if (game.isColliding(enemies[i], spawnpool)) {
				spawnpool.health-=5;
				enemies[i].health = 0;
				Mix_PlayChannel(-1, damagesound, 0);
			}
			SDL_Rect rect = { enemies[i].x, enemies[i].y, enemies[i].w, enemies[i].h };
			SDL_RenderCopy(rend, enemytex, NULL, &rect);
			if (enemies[i].health <= 0) {
				Entity ex;
				ex.x = enemies[i].x;
				ex.y = enemies[i].y;
				ex.w = enemies[i].w;
				ex.h = enemies[i].h;
				ex.dodgecool = 0;
				deads.push_back(ex);
				enemies.erase(enemies.begin() + i);
			}
		}
		for (int i = 0; i < spawns.size(); i++) {
			spawns[i].dtimer += delta;
			if (spawns[i].dtimer > 0.2) {
				spawns[i].ctex++;
				if (spawns[i].ctex > 1) {
					spawns[i].ctex = 0;
				}
				switch (spawns[i].ctex) {
				case 0:
					spawns[i].tex = spawntex;
					break;
				case 1:
					spawns[i].tex = spawn2tex;
					break;
				}
				spawns[i].dtimer = 0;
			}
			if (!spawns[i].dodge) {
				if (spawns[i].x < spawntarget.x) {
					spawns[i].x += 100 * delta;
				}
				else {
					spawns[i].x -= 100 * delta;
				}
				if (spawns[i].y < spawntarget.y) {
					spawns[i].y += 100 * delta;
				}
				else {
					spawns[i].y -= 100 * delta;
				}
			}
			bool setfalse = true;
			for (auto& j : spawns) {
				if (game.isColliding(spawns[i], j) && spawns[i].index != j.index) {
					spawns[i].x += spawns[i].x - j.x;
					spawns[i].y += spawns[i].y - j.y;
					//spawns[i].dodge = true;
					//setfalse = false;
				}
			}
			if (setfalse) {
				spawns[i].dodge = false;
			}
			for (auto& j : enemies) {
				if (game.isColliding(spawns[i], j)) {
					spawns[i].health = 0;
					Mix_PlayChannel(-1, damagesound, 0);
				}
			}
			SDL_Rect rect = { spawns[i].x, spawns[i].y, spawns[i].w, spawns[i].h };
			SDL_RenderCopy(rend, spawns[i].tex, NULL, &rect);
			if (spawns[i].health <= 0) {
				spawns.erase(spawns.begin() + i);
			}
		}
		for (int i = 0; i < deads.size(); i++) {
			deads[i].dodgecool += delta;
			SDL_Rect rect = { deads[i].x, deads[i].y, deads[i].w, deads[i].h };
			SDL_RenderCopy(rend, deadtex, NULL, &rect);
			if (deads[i].dodgecool > 0.3) {
				deads.erase(deads.begin() + i);
			}
		}
		if (spawnpool.health <= 0) {
			enemies.clear();
			bullets.clear();
			spawns.clear();
			player.health = 10;
			spawnpool.health = 100;
			enemyspawntimer = 0;
			enemydif = 3.0;
			poolcool = 0;
			spawncool = 0;
			score = 0;
			dif = 1;
			dcount = 0;
			Mix_PlayChannel(-1, deathsound, 0);
			game.loadLevel(&player, &spawntarget);
		}
		SDL_RenderPresent(rend);
		if (keys[SDL_SCANCODE_ESCAPE]) {
			exitf = true;
		}
	}

	return 0;
}