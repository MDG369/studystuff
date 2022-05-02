#define _USE_MATH_DEFINES
#include<math.h>
#include<stdio.h>
#include<string.h>
#include<cstdlib>
#include<ctime>
extern "C" {
#include"SDL.h"
#include"SDL_main.h"
#include "main.h"
}
#define MAX_OBJECTS 10
#define MAX_SPRITES 10
#define SCREEN_WIDTH	640
#define SCREEN_HEIGHT	480
#define CAMERA_LOWER_BOUND (-SCREEN_HEIGHT /2) // miejsce, w którym zatrzymuje siê kamera (dó³)
#define CAMERA_UPPER_BOUND SCREEN_HEIGHT // miejsce, w którym zatrzymuje siê kamera (góra)
#define JUMP_SPEED -800 // si³a skoku
#define GRAVITY 1650 // si³a gravwitacji
#define START_LIFES 3 // pocz¹tkowa liczba ¿yæ
#define MAX_JUMPS 1 // liczba dodatkowych skoków
#define GAME_SPEED 2.0 // bazowa szybkosc jednorozca
#define MAX_DASHES 1 // maksymalba liczba zrywów
#define DASH_COOLDOWN 1.5 // min. czas miêdzy zrywami
#define DASH_SPEED 6 // szybkoœæ zrywu
#define DASH_TIME 0.2 // czas trwania zrywu
#define MAX_PLATFORMS 3 // maksymalna liczba platform na ekranie
#define PLATFORMS_NUM 6 // liczba rodzajów (spriteów) platform
#define STALACTITE_NUM 1
#define MAX_STALACTITES 1
#define STAR_NUM 1
#define MAX_STARS 1
#define SPEED_INCREASE 0.05 // wspó³czynnik zmiany prêdkoœci X z czasem gry
#define FAIRIES_NUM 1 // liczba rodzajów (spriteów) wró¿ek
#define MAX_FAIRIES 1 // maksymalna liczba wró¿ek na ekranie
#define FAIRY_SPEED 0.04 // szybkoœæ wró¿ek
#define FAIRIES_CHANCE 50 // szansa na wyst¹pienie w %
#define STARS_CHANCE 33 // szansa na wyst¹pienie w %
#define RUN_FRAMES 6 // liczba klatek biegu
#define EXPLOSION_FRAMES 4 // liczba klatek eksplozji
#define FALL_FRAMES 2 //liczba klatek spadania
struct hitbox {
	double x;
	double y;
	double w;
	double h;
};
struct uniState {
	bool dead, fairyCollected, starCollected, exploded, crash;
	int lifes, dashCount, starCombo, fairyCombo, jumpCount;
	double dashCooldown, holdTime, dashTime, score, unicornY, velY, floor, speed;
	hitbox unicornHitbox;
	SDL_Surface* unicorn, * jumpSprite, * fallSprite[2], * explosionSprite[4], * explodingStarSprite[4], * run[6];
};
struct object {
	SDL_Surface* sprites[MAX_SPRITES];
	hitbox hbox[MAX_OBJECTS];
	int num[MAX_OBJECTS];
};
void DrawString(SDL_Surface *screen, int x, int y, const char *text,
                SDL_Surface *charset) {
	int px, py, c;
	SDL_Rect s, d;
	s.w = 8;
	s.h = 8;
	d.w = 8;
	d.h = 8;
	while(*text) {
		c = *text & 255;
		px = (c % 16) * 8;
		py = (c / 16) * 8;
		s.x = px;
		s.y = py;
		d.x = x;
		d.y = y;
		SDL_BlitSurface(charset, &s, screen, &d);
		x += 8;
		text++;
		};
	};


void DrawSurface(SDL_Surface *screen, SDL_Surface *sprite, int x, int y) {
	SDL_Rect dest;
	dest.x = x - sprite->w / 2;
	dest.y = y - sprite->h / 2;
	dest.w = sprite->w;
	dest.h = sprite->h;
	SDL_BlitSurface(sprite, NULL, screen, &dest);
	};


void DrawPixel(SDL_Surface *surface, int x, int y, Uint32 color) {
	int bpp = surface->format->BytesPerPixel;
	Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;
	*(Uint32 *)p = color;
	};


void DrawLine(SDL_Surface *screen, int x, int y, int l, int dx, int dy, Uint32 color) {
	for(int i = 0; i < l; i++) {
		DrawPixel(screen, x, y, color);
		x += dx;
		y += dy;
		};
	};


void DrawRectangle(SDL_Surface *screen, int x, int y, int l, int k,
                   Uint32 outlineColor, Uint32 fillColor) {
	int i;
	DrawLine(screen, x, y, k, 0, 1, outlineColor);
	DrawLine(screen, x + l - 1, y, k, 0, 1, outlineColor);
	DrawLine(screen, x, y, l, 1, 0, outlineColor);
	DrawLine(screen, x, y + k - 1, l, 1, 0, outlineColor);
	for(i = y + 1; i < y + k - 1; i++)
		DrawLine(screen, x + 1, i, l - 2, 1, 0, fillColor);
	};

void holdJump(double &holdTime, double delta) {
	holdTime += delta * 200;
}
void jump(uniState &uniState) {
	if (uniState.jumpCount < MAX_JUMPS) {
		if (uniState.holdTime < 0.9) {
			uniState.velY = JUMP_SPEED * 0.9; // minimalna wysokoœæ skoku
		}
		else if (uniState.holdTime < 1.2) {
			uniState.velY = JUMP_SPEED * uniState.holdTime;
		}
		else {
			uniState.velY = JUMP_SPEED * 1.2;
		}
		uniState.jumpCount += 1;
		uniState.holdTime = 0;
	}
}
void dash(uniState &uniState) {
	if (uniState.dashCount < MAX_DASHES && uniState.dashCooldown == 0) {
		uniState.dashCount += 1;
		uniState.dashTime = 0;
	}
}
void fairyMovement(object& fairy, uniState uniState) {
	srand(time(NULL));
	int dir = rand() % 8;
	switch (dir) { // wró¿ka ma po³owê szans na poruszenie siê w danym obiegu pêtli
	case 0:
		fairy.hbox->x += FAIRY_SPEED* uniState.speed;
		break;
	case 1:
		fairy.hbox->y -= FAIRY_SPEED * uniState.speed;
		break;
	case 2:
		fairy.hbox->y -= FAIRY_SPEED * uniState.speed;
		break;
	case 3:
		fairy.hbox->y += FAIRY_SPEED * uniState.speed;
		break;
	}
}
void randomizeCollectables(object &platforms, hitbox star[MAX_OBJECTS], int i)
{
	int X = rand() % 100;
	int Y = rand() % 150;
	int dirY = rand() % 2;
	star[i].x = 2 * SCREEN_WIDTH + X + star[i].w / 2;
	if (dirY) {
		star[i].y += Y;
	}
	if (!dirY) {
		star[i].y -= Y;
	}
	if (star[i].y <= (-SCREEN_HEIGHT + 50) / 2 || star[i].y >= SCREEN_HEIGHT * 2) {
		star[i].y = platforms.hbox[i].y - (platforms.hbox[i].h / 2) - star[i].h / 2;
	}
	for (int j = 0; j < MAX_PLATFORMS; j++) {
		bool rightSideIn = star[i].x + star[i].w / 2  >= platforms.hbox[j].x - platforms.hbox[j].w / 2;
		bool leftSideIn = star[i].x - star[i].w / 2 <= platforms.hbox[j].x + platforms.hbox[j].w / 2;
		bool downSideIn = star[i].y + star[i].h / 2 >= platforms.hbox[j].y - platforms.hbox[j].h / 2;
		bool upSideIn = star[i].y - star[i].h / 2 <= platforms.hbox[j].y + platforms.hbox[j].h / 2;
		if ((downSideIn && upSideIn) && (rightSideIn && leftSideIn)) {
			star[i].y -= Y;
		}
	}
}
int collision(uniState& uniState, hitbox& platform,  double delta) {
	// stanie na platformie
	if (uniState.unicornHitbox.y <= platform.y -platform.h/2  + 10 && uniState.unicornHitbox.x  >= platform.x - platform.w / 2 &&
		uniState.unicornHitbox.x - uniState.unicornHitbox.w <= platform.x + platform.w /2) {
		uniState.floor = platform.y - platform.h/2;
		return 1;
	}
	// zderzenie z platform¹
	else if (uniState.unicornHitbox.y - uniState.unicornHitbox.h <= platform.y + platform.h/2 && uniState.unicornHitbox.y >= platform.y - platform.h/2 && 
		uniState.unicornHitbox.x  >= platform.x -platform.w/2 && uniState.unicornHitbox.x - uniState.unicornHitbox.w + uniState.unicornHitbox.w/4 <= platform.x + platform.w/2) {
		return 2;
	}
	// brak kontaktu z platform¹
	else {
		uniState.floor = -CAMERA_LOWER_BOUND * 4;;
		return 0;
	}
}
void starCollision(uniState& uniState, object& stars, double delta, object& platforms)
{
	for (int i = 0; i < MAX_STARS; i++) {
		if (collision(uniState, stars.hbox[i], delta) == 2 && uniState.dashTime < DASH_TIME && uniState.dashTime >0) {
			randomizeCollectables(platforms, stars.hbox, i);
			uniState.score += 100 * uniState.starCombo;
			uniState.starCombo += 1;
			uniState.starCollected = false;

		}
		else if (collision(uniState, stars.hbox[i], delta) == 2) {
			uniState.speed = 0;
			uniState.crash = 1;
			uniState.velY = 0;
		}
	}
}
void fairyCollision(uniState& uniState, object& fairies, double delta, object& platforms)
{
	for (int i = 0; i < MAX_FAIRIES; i++) {
		if (collision(uniState, fairies.hbox[i], delta) == 2) {
			randomizeCollectables(platforms, fairies.hbox, i);
			uniState.score += 10 * uniState.fairyCombo;
			uniState.fairyCombo += 1;
			uniState.fairyCollected = 1;
		}
	}
}
void stalactiteCollision(uniState& uniState, object& stalactites, double delta)
{
	for (int i = 0; i < MAX_STALACTITES; i++) {
		if (collision(uniState, stalactites.hbox[i], delta) == 2) {
			uniState.crash = 1;
			uniState.speed = 0;
			uniState.velY = 0;
		}
	}
}
void platformCollision(uniState& uniState, object& platforms, double delta, double  floorTmp[3])
{
	for (int i = 0; i < MAX_PLATFORMS; i++) {
		switch (collision(uniState, platforms.hbox[i], delta)) {
		case 2:
			uniState.speed = 0;
			uniState.crash = 1;
			uniState.velY = 0;
			break;
		case 1:
			floorTmp[i] = uniState.floor;
			break;
		case 0:
			break;
		}
	}
}
void collisions(object & fairies, object &platforms,object& stalactites, uniState& uniState, object& stars, double delta ) {
	double floorTmp[MAX_PLATFORMS];
	for (int j = 0; j < MAX_PLATFORMS; j++) {
		floorTmp[j] = -CAMERA_LOWER_BOUND * 4;
	}
	platformCollision(uniState, platforms, delta, floorTmp);
	stalactiteCollision(uniState, stalactites, delta);
	starCollision(uniState, stars, delta, platforms);
	fairyCollision(uniState, fairies, delta, platforms);
	double minFloor = floorTmp[0];
	for (int i = 0 ; i < MAX_PLATFORMS; i++) {
		if (floorTmp[i] < minFloor) {
			minFloor = floorTmp[i];
		}
	}
	if (uniState.unicornY >= SCREEN_HEIGHT + 100) {
		uniState.crash = 1;
	}
	uniState.floor = minFloor;
}
double speedIncrease(double delta, uniState& uniState) {
	if (uniState.speed <= 6) {
		uniState.speed += delta * SPEED_INCREASE;
	}
	return uniState.speed;
}
void DashCheck(uniState &uniState, double delta )
{
	static bool bonusJump = true;
	static bool speedChange1 = true;
	static bool speedChange2 = true;
	// po wykonaniu zrywu
	if (uniState.dashCount > 0) {
		if (uniState.dashTime < DASH_TIME && uniState.speed !=  0) { // jeœli zryw wci¹¿ trwa i jednoro¿ec nie zderzy³ siê z przeszkod¹
			if (speedChange1) {
				uniState.speed += DASH_SPEED;
				speedChange1 = false;
			}
			uniState.dashTime += delta;
			uniState.velY = 0;
		}
		else {
			uniState.dashCooldown += delta;
			if (speedChange2 && uniState.speed != 0) {
				uniState.speed -= DASH_SPEED;
				speedChange2 = false;
			}

			if (bonusJump) {
				uniState.jumpCount = MAX_JUMPS - 1;
				bonusJump = false;
			}
		}
	}
	// odnowienie mo¿liwoœci wykonania zrywu
	if (uniState.dashCooldown >= DASH_COOLDOWN) {
		uniState.dashCooldown = 0;
		uniState.dashCount -= 1;
		bonusJump = true;
		speedChange1 = true;
		speedChange2 = true;
	}
	if (uniState.dashCount < 0) {
		uniState.dashCount = 0;
	}
}

void createCollectables(object& platforms, object& collectables, uniState& uniState, int spawnChance)
{
	int chance = rand() % 100;
	if (chance <= spawnChance) {
		for (int i = 0; i < MAX_STARS; i++) {
			if (collectables.hbox[i].x < -collectables.hbox[i].w) {
				collectables.num[i]++;
				if (collectables.num[i] >= STAR_NUM) {
					collectables.num[i] = 0;
				}
				if (spawnChance == FAIRIES_CHANCE) {
					uniState.fairyCombo = 1;
				}
				else {
					uniState.starCombo = 1;
				}
				randomizeCollectables(platforms, collectables.hbox, i);
			}
		}
	}
}
void createPlatforms(object& platforms, double X, int max_platforms, int platforms_num)
{
	for (int i = 0; i < max_platforms; i++) {
		if (platforms.hbox[i].x < -platforms.hbox[i].w) {
			platforms.num[i]++;
			if (platforms.num[i] >= platforms_num) {
				platforms.num[i] = 0;
			}
			platforms.hbox[i].h = platforms.sprites[platforms.num[i]]->h;
			platforms.hbox[i].w = platforms.sprites[platforms.num[i]]->w;
			platforms.hbox[i].x = X + platforms.hbox[i].w / 2;
			if (i == 1) {
				if (platforms.num[i] % 3 == 0) {
					platforms.hbox[i].y = platforms.hbox[i - 1].y - platforms.hbox[i].h / 2 - platforms.hbox[i - 1].h / 2;
				}
				else {
					platforms.hbox[i].y = platforms.hbox[0].y - 180;
				}
			}
		}
	}
}
void setObjectsPositions(object& stars, object& platforms, object& stalactites, object& fairies)
{
	// pocz¹tkowe ustawienie poszczególnych obiektów
	stars.num[0] = 0;
	platforms.num[0] = 0;
	platforms.num[1] = 2;
	platforms.num[2] = 5;
	stalactites.num[0] = 0;
	for (int i = 0; i < MAX_PLATFORMS; i++) {
		platforms.hbox[i].h = platforms.sprites[platforms.num[i]]->h;
		platforms.hbox[i].w = platforms.sprites[platforms.num[i]]->w;
	}
	for (int i = 0; i < MAX_STALACTITES; i++) {
		stalactites.hbox[i].h = stalactites.sprites[stalactites.num[i]]->h;
		stalactites.hbox[i].w = stalactites.sprites[stalactites.num[i]]->w;
	}
	platforms.hbox[0].y = (SCREEN_HEIGHT / 2) + 40 - platforms.hbox[0].h / 2;
	platforms.hbox[0].x = 50 + platforms.hbox[0].w / 2;
	platforms.hbox[1].y = 100 - platforms.hbox[1].h / 2;
	platforms.hbox[1].x = SCREEN_WIDTH * 1.3 + platforms.hbox[1].w / 2;
	platforms.hbox[2].y = SCREEN_HEIGHT - 50 - platforms.hbox[2].h / 2;
	platforms.hbox[2].x = SCREEN_WIDTH + platforms.hbox[2].w / 2;
	stalactites.hbox[0].x = SCREEN_WIDTH * 1.2 + stalactites.hbox[0].w / 2;
	stalactites.hbox[0].y = -SCREEN_HEIGHT / 2;
	stars.hbox[0].h = stars.sprites[0]->h;
	stars.hbox[0].w = stars.sprites[0]->w;
	stars.hbox[0].x = -SCREEN_WIDTH + stars.hbox[0].w / 2;
	stars.hbox[0].y = platforms.hbox[0].y - (platforms.hbox[0].h / 2) - platforms.hbox[0].h / 2;
	fairies.hbox[0].x = SCREEN_WIDTH * 1.5;
	fairies.hbox[0].y = SCREEN_HEIGHT / 2;
	fairies.hbox[0].h = fairies.sprites[0]->h;
	fairies.hbox[0].w = fairies.sprites[0]->h;
}
void reset(object & fairies, object & stars, object &platforms, object &stalactites, int& frames, double& camY, double& fpsTimer, double& fps, int& quit, double& worldTime,
	double& backgroundX, SDL_Surface* background, uniState &uniState)
{
	uniState.exploded = false;
	uniState.fairyCombo = 1;
	uniState.starCombo = 1;
	uniState.score = 0;
	uniState.crash = false;
	uniState.dead = false;
	uniState.holdTime = 0;
	uniState.dashCount = 0;
	uniState.dashCooldown = DASH_COOLDOWN;
	uniState.dashTime = 0;
	uniState.lifes = START_LIFES;
	camY = uniState.unicornY = (SCREEN_HEIGHT - 50) / 2;
	frames = 0;
	fpsTimer = 0;
	fps = 0;
	quit = 0;
	worldTime = 0;
	uniState.speed = GAME_SPEED;
	uniState.velY = 0;
	uniState.floor = SCREEN_HEIGHT * 2;
	backgroundX = background->w / 2;
	setObjectsPositions(stars, platforms, stalactites, fairies);
	uniState.unicornHitbox.h = uniState.unicorn->h;
	uniState.unicornHitbox.w = uniState.unicorn->w;
	uniState.unicornHitbox.x = 10 + uniState.unicorn->w;
	uniState.unicornHitbox.y = uniState.unicornY;
	uniState.unicornY = platforms.hbox[0].y - uniState.unicornHitbox.h / 2;
	uniState.unicornHitbox.y = uniState.unicornY;
}
void tryAgain(object &fairies, object & stars, object &platforms, object &stalactites, uniState &uniState, int& frames, double& camY, double& fpsTimer, double& fps,
	int& quit,double& worldTime,
	 double& backgroundX, SDL_Surface* background) {
	int tmpScore = uniState.score;
	int tmpLifes = uniState.lifes;
	reset(fairies, stars, platforms, stalactites, frames, camY, fpsTimer, fps, quit, worldTime, backgroundX, background, uniState);
	uniState.score = tmpScore;
	uniState.lifes = tmpLifes;
}
void controlScheme(object & fairies, object &stars, object &platforms, object &stalactites, SDL_Event& event, bool& controlMode, bool& menu, int& frames,
	double& camY, double& fpsTimer, double& fps, int& quit, double& worldTime, double& backgroundX, SDL_Surface* background, 
	double delta,uniState &uniState)
{
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_KEYDOWN:
			if (controlMode == 0) {
				if (event.key.keysym.sym == SDLK_m) menu = 1;
				else if (event.key.keysym.sym == SDLK_d) controlMode = 1, uniState.speed = GAME_SPEED;
				else if (event.key.keysym.sym == SDLK_UP) uniState.velY = -300;
				else if (event.key.keysym.sym == SDLK_RIGHT) uniState.speed = 2;
				else if (event.key.keysym.sym == SDLK_LEFT) uniState.speed = -2;
				else if (event.key.keysym.sym == SDLK_DOWN) uniState.velY = 300;
				else if (event.key.keysym.sym == SDLK_n) 	reset(fairies, stars, platforms, stalactites, frames, camY, fpsTimer, fps, quit, worldTime, backgroundX, background, uniState);
			}
			else {
				if (event.key.keysym.sym == SDLK_m)	menu = 1;
				else if (event.key.keysym.sym == SDLK_d) controlMode = 0, uniState.speed = 0;
				else if (event.key.keysym.sym == SDLK_z) holdJump(uniState.holdTime, delta);
				else if (event.key.keysym.sym == SDLK_RETURN && uniState.dead && uniState.lifes) tryAgain(fairies, stars, platforms, stalactites, uniState, frames, camY, fpsTimer, fps, quit, worldTime, backgroundX, background);
				else if (event.key.keysym.sym == SDLK_x) dash(uniState);
				else if (event.key.keysym.sym == SDLK_n) reset(fairies, stars, platforms, stalactites, frames, camY, fpsTimer, fps, quit, worldTime, backgroundX, background, uniState);
			}
			break;
		case SDL_KEYUP:
			if (controlMode == 1) {
				if (event.key.keysym.sym == SDLK_z) jump(uniState);
			}
			else {
				if (event.key.keysym.sym == SDLK_RIGHT) uniState.speed = 0;
				else if (event.key.keysym.sym == SDLK_LEFT) uniState.speed = 0;
				else if (event.key.keysym.sym == SDLK_UP) uniState.velY = 0;
				else if (event.key.keysym.sym == SDLK_DOWN) uniState.velY = 0;
			}
			break;
		case SDL_QUIT:
			quit = 1;
			break;
		};
	};
}

void scrollBackground(double& backgroundX, double speed, double delta, SDL_Surface* background)
{
	backgroundX -= 20 * speed * delta;
	if (backgroundX < -SCREEN_WIDTH + background->w / 2) {
		backgroundX = background->w / 2;
	}
}

void menuFunction(bool& menu, int& quit, SDL_Surface* screen, int czarny, int czerwony, int niebieski, char  text[128], char  text2[128], SDL_Surface* charset,
	SDL_Texture* scrtex, SDL_Renderer* renderer, SDL_Event& event, bool&reset)
{
	while (menu && !quit) {
		SDL_FillRect(screen, NULL, czarny);
		DrawRectangle(screen, (SCREEN_WIDTH - 200) / 4, (SCREEN_HEIGHT - 50) / 4, (SCREEN_WIDTH + 200) / 2, (SCREEN_HEIGHT + 50) / 2, czerwony, niebieski);
		sprintf(text, "Aby wystartowac gre wcisnij enter");
		sprintf(text2, " Aby wyjsc wcisnij escape");
		DrawString(screen, screen->w / 2 - strlen(text) * 8 / 2, SCREEN_HEIGHT / 2, text, charset);
		DrawString(screen, screen->w / 2 - strlen(text2) * 8 / 2, SCREEN_HEIGHT / 2 + 20, text2, charset);
		SDL_UpdateTexture(scrtex, NULL, screen->pixels, screen->pitch);
		SDL_RenderCopy(renderer, scrtex, NULL, NULL);
		SDL_RenderPresent(renderer);
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
			case SDL_KEYDOWN:
				if (event.key.keysym.sym == SDLK_ESCAPE) quit = 1;
				if (event.key.keysym.sym == SDLK_RETURN) menu = 0, reset = 1;
				break;
			case SDL_QUIT:
				quit = 1;
				break;
			};
		}
	}
}

void xMovement(object& platforms, uniState& uniState, double delta, object& stalactites, object& fairies, object& stars)
{
	for (int i = 0; i < MAX_PLATFORMS; i++) {
		platforms.hbox[i].x -= 150 * uniState.speed * delta;
	}
	for (int i = 0; i < MAX_STALACTITES; i++) {
		stalactites.hbox[i].x -= 150 * uniState.speed * delta;
	}
	fairies.hbox[0].x -= 150 * uniState.speed * delta;
	stars.hbox[0].x -= 150 * uniState.speed * delta;
}

void yMovement(uniState& uniState, double delta, double& camY, object& platforms, object& stalactites, object& stars, object& fairies, bool controlMode)
{
	if (controlMode != 0) {
		if (uniState.unicornY < uniState.floor) {
			uniState.velY += GRAVITY * delta;
		}
		else
		{
			uniState.velY = fmin(0, uniState.velY);
			uniState.jumpCount = 0;
		}
	}
	if (uniState.unicornY - uniState.unicornHitbox.h <= 0) {
		uniState.velY = fmax(0, uniState.velY);
	}
	if (uniState.unicornY > uniState.floor) {
		uniState.unicornY = uniState.floor;
	}
	if (uniState.unicornY <= SCREEN_HEIGHT) {
		uniState.unicornHitbox.y = uniState.unicornY;
	}
	if (camY >= CAMERA_LOWER_BOUND && camY <= CAMERA_UPPER_BOUND) { // ruch obiektami, jesli jednoro¿ec nie zbli¿a siê do krawêdzi
		for (int i = 0; i < MAX_PLATFORMS; i++) {
			platforms.hbox[i].y -= delta * uniState.velY;
		}
		for (int i = 0; i < MAX_STALACTITES; i++) {
			stalactites.hbox[i].y -= uniState.velY * delta;
		}
		stars.hbox[0].y -= delta * uniState.velY;
		fairies.hbox[0].y -= delta * uniState.velY;
	}
	else { // jesli jest blisko krawêdzi
		uniState.unicornY += delta * uniState.velY;
	}
	camY -= delta * uniState.velY;
}

void crashCheck(uniState& uniState, SDL_Surface* screen, int czerwony, int niebieski, char  text[128], SDL_Surface* charset)
{
	if (!uniState.dead && uniState.crash) {
		uniState.lifes -= 1;
		uniState.dead = true;
	}
	if (uniState.dead && uniState.lifes) {
		DrawRectangle(screen, 4, (SCREEN_HEIGHT - 16) / 2, SCREEN_WIDTH - 8, 36, czerwony, niebieski);
		sprintf(text, "Zostaly ci %i zycia, aby kontynuowac wcisnij enter", uniState.lifes);
		DrawString(screen, screen->w / 2 - strlen(text) * 8 / 2, SCREEN_HEIGHT / 2, text, charset);
	}
	if (!uniState.lifes) {
		DrawRectangle(screen, 4, (SCREEN_HEIGHT - 16) / 2, SCREEN_WIDTH - 8, 36, czerwony, niebieski);
		sprintf(text, "Nie masz juz wiecej zyc, aby zrestartowac wcisnij 'n'");
		DrawString(screen, screen->w / 2 - strlen(text) * 8 / 2, SCREEN_HEIGHT / 2, text, charset);
		sprintf(text, "Aby wrocic do menu glownego wcisnij 'm'");
		DrawString(screen, screen->w / 2 - strlen(text) * 8 / 2, (SCREEN_HEIGHT + 25) / 2, text, charset);
	}
}

void timeLapse(int& t2, double& delta, int& t1, double& worldTime, uniState& uniState, double& fpsTimer, double& fps, int& frames)
{
	t2 = SDL_GetTicks();
	delta = (t2 - t1) * 0.001;
	t1 = t2;
	worldTime += delta;
	if (!uniState.crash) {
		uniState.score += delta * 25;
	}
	if (uniState.speed < GAME_SPEED * 6 && uniState.speed != 0) {
		uniState.speed = speedIncrease(delta, uniState);
	}
	fpsTimer += delta;
	if (fpsTimer > 0.5) {
		fps = frames * 2;
		frames = 0;
		fpsTimer -= 0.5;
	};
}

void explosion(double& explTime, double delta, int& explCount, SDL_Surface* screen, SDL_Surface* explosionSprite[4], double x, double y, bool &exploded)
{
	explTime += delta;
	if (explTime >= 0.1) {
		explCount += 1;
		explTime = 0;
	}
	if (explCount <= 3) {
		DrawSurface(screen, explosionSprite[explCount], x, y);
	}
	else {
		exploded = 1;
		explTime = 0;
		explCount = 0;
	}
}

void drawUnicorn(uniState& uniState, double& explTime, double delta, int& explCount, SDL_Surface* screen, SDL_Surface* explosionSprite[4], double worldTime
)
{
	if (!uniState.exploded && uniState.crash) {
		explosion(explTime, delta, explCount, screen, explosionSprite, 10 + uniState.unicorn->w / 2, uniState.unicornY - uniState.unicorn->h / 2, uniState.exploded);
	}
	if (!uniState.crash) {
		if (uniState.unicornY == uniState.floor || uniState.dashTime < DASH_TIME && uniState.dashTime >0) {
			DrawSurface(screen, uniState.run[int(worldTime * 2 * uniState.speed) % 6], 10 + uniState.unicorn->w / 2, uniState.unicornY - uniState.unicorn->h / 2);
		}
		else if (uniState.velY < 0) {
			DrawSurface(screen, uniState.jumpSprite, 10 + uniState.unicorn->w / 2, uniState.unicornY - uniState.unicorn->h / 2);
		}
		else if (uniState.velY > 10 && uniState.velY < 100) {
			DrawSurface(screen, uniState.fallSprite[1], 10 + uniState.unicorn->w / 2, uniState.unicornY - uniState.unicorn->h / 2);
		}
		else if (uniState.velY >= 100) {
			DrawSurface(screen, uniState.fallSprite[0], 10 + uniState.unicorn->w / 2, uniState.unicornY - uniState.unicorn->h / 2);
		}
		else {
			DrawSurface(screen, uniState.run[0], 10 + uniState.unicorn->w / 2, uniState.unicornY - uniState.unicorn->h / 2);
		}
	}
}

void drawObjects(SDL_Surface* screen, object& platforms, object& stalactites, object& fairies, object& stars)
{
	for (int i = 0; i < MAX_PLATFORMS; i++) {
		DrawSurface(screen, platforms.sprites[platforms.num[i]], platforms.hbox[i].x, platforms.hbox[i].y);
	}
	for (int i = 0; i < MAX_STALACTITES; i++) {
		DrawSurface(screen, stalactites.sprites[stalactites.num[i]], stalactites.hbox[i].x, stalactites.hbox[i].y);
	}
	DrawSurface(screen, fairies.sprites[0], fairies.hbox[0].x, fairies.hbox[0].y);
	DrawSurface(screen, stars.sprites[0], stars.hbox[0].x, stars.hbox[0].y);
}

void drawCollectables(uniState& uniState, char  text[128], SDL_Surface* screen, SDL_Surface* charset, double& starCollTime, double delta, int& starCollCount,
	SDL_Surface* explodingStarSprite[4], double& fairyCollTime)
{
	if (!uniState.starCollected) {
		sprintf(text, "+%.d", (100 * (uniState.starCombo - 1)));
		DrawString(screen, 20 + uniState.unicorn->w, uniState.unicornY - uniState.unicorn->h - 50, text, charset);
		explosion(starCollTime, delta, starCollCount, screen, explodingStarSprite, 20 + uniState.unicorn->w, uniState.unicornY - uniState.unicorn->h, uniState.starCollected);

	}
	if (uniState.fairyCollected) {
		fairyCollTime += delta;
		if (fairyCollTime <= 0.33) {
			sprintf(text, "+%.d", (10 * (uniState.fairyCombo - 1)));
			DrawString(screen, 20 + uniState.unicorn->w, uniState.unicornY - uniState.unicorn->h - 50, text, charset);
		}
		else {
			uniState.fairyCollected = 0;
			fairyCollTime = 0;
		}
	}
}

void drawBackground(SDL_Surface* screen, int czarny, SDL_Surface* background, double backgroundX)
{
	SDL_FillRect(screen, NULL, czarny);
	DrawSurface(screen, background, backgroundX, SCREEN_HEIGHT / 2);
}

void drawLifes(uniState& uniState, SDL_Surface* screen, SDL_Surface* heartSprites)
{
	for (int i = 0; i < uniState.lifes; i++) {
		DrawSurface(screen, heartSprites, (100 + i * 2 * heartSprites->w) / 2, 0 + heartSprites->h / 2);
	}
}

void drawScore(char  text[128], uniState& uniState, SDL_Surface* screen, int zielony, int czarny, SDL_Surface* charset)
{
	sprintf(text, "SCORE = %.lf", uniState.score);
	DrawRectangle(screen, (SCREEN_WIDTH - 115) / 2, 4, 115, 36, zielony, czarny);
	DrawString(screen, screen->w / 2 - strlen(text) * 8 / 2, 10, text, charset);
}

void freeSurfaces( SDL_Surface* explosionSprite[4], SDL_Surface* explodingStarSprite[4],
	SDL_Surface* heartSprites, object& platforms, object& stars, object& stalactites, object& fairies, SDL_Surface* background, uniState& uniState, SDL_Surface* charset,
	SDL_Surface* screen, SDL_Texture* scrtex, SDL_Window* window, SDL_Renderer* renderer)
{
	for (int i = 0; i < RUN_FRAMES; i++) {
		SDL_FreeSurface(uniState.run[i]);
	}
	for (int i = 0; i < EXPLOSION_FRAMES; i++) {
		SDL_FreeSurface(explosionSprite[i]);
		SDL_FreeSurface(explodingStarSprite[i]);
	}
	SDL_FreeSurface(uniState.jumpSprite);
	for (int i = 0; i < FALL_FRAMES; i++) {
		SDL_FreeSurface(uniState.fallSprite[i]);
	}
	SDL_FreeSurface(heartSprites);
	for (int i = 0; i < PLATFORMS_NUM; i++) {
		SDL_FreeSurface(platforms.sprites[i]);
	}
	for (int i = 0; i < STAR_NUM; i++) {
		SDL_FreeSurface(stars.sprites[i]);
	}
	SDL_FreeSurface(stalactites.sprites[0]);
	SDL_FreeSurface(fairies.sprites[0]);
	SDL_FreeSurface(background);
	SDL_FreeSurface(uniState.unicorn);
	SDL_FreeSurface(charset);
	SDL_FreeSurface(screen);
	SDL_DestroyTexture(scrtex);
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
}

void checkSurfaces(SDL_Surface* charset ,SDL_Surface* explosionSprite[4], SDL_Surface* explodingStarSprite[4],
	SDL_Surface* heartSprites, object& platforms, object& stars, object& stalactites, object& fairies, SDL_Surface* background, uniState& uniState, SDL_Surface* screen, SDL_Texture* scrtex,
	SDL_Window* window, SDL_Renderer* renderer)
{
	if (charset == NULL) {
		freeSurfaces(explosionSprite, explodingStarSprite, heartSprites, platforms, stars, stalactites, fairies, background, uniState, charset, screen, scrtex, window, renderer);
	}
	if (uniState.jumpSprite == NULL) {
		freeSurfaces(explosionSprite, explodingStarSprite, heartSprites, platforms, stars, stalactites, fairies, background, uniState, charset, screen, scrtex, window, renderer);
	}
	for (int i = 0; i < FALL_FRAMES; i++) {
		if (uniState.fallSprite[i] == NULL)
			freeSurfaces(explosionSprite, explodingStarSprite, heartSprites, platforms, stars, stalactites, fairies, background, uniState, charset, screen, scrtex, window, renderer);
	}
	for (int i = 0; i < RUN_FRAMES; i++) {
		if (uniState.run[i] == NULL)
			freeSurfaces(explosionSprite, explodingStarSprite, heartSprites, platforms, stars, stalactites, fairies, background, uniState, charset, screen, scrtex, window, renderer);
	}
	for (int i = 0; i < EXPLOSION_FRAMES; i++) {
		if (explosionSprite[i] == NULL) {
			freeSurfaces(explosionSprite, explodingStarSprite, heartSprites, platforms, stars, stalactites, fairies, background, uniState, charset, screen, scrtex, window, renderer);
		}
		if (explodingStarSprite[i] == NULL) {
			freeSurfaces(explosionSprite, explodingStarSprite, heartSprites, platforms, stars, stalactites, fairies, background, uniState, charset, screen, scrtex, window, renderer);
		}
	}
	if (fairies.sprites[0] == NULL) {
		freeSurfaces(explosionSprite, explodingStarSprite, heartSprites, platforms, stars, stalactites, fairies, background, uniState, charset, screen, scrtex, window, renderer);
	}
	if (stalactites.sprites[0] == NULL) {
		freeSurfaces(explosionSprite, explodingStarSprite, heartSprites, platforms, stars, stalactites, fairies, background, uniState, charset, screen, scrtex, window, renderer);
	}
	if (uniState.unicorn == NULL) {
		freeSurfaces(explosionSprite, explodingStarSprite, heartSprites, platforms, stars, stalactites, fairies, background, uniState, charset, screen, scrtex, window, renderer);
	}
	for (int i = 0; i < PLATFORMS_NUM; i++) {
		if (platforms.sprites[i] == NULL)
			freeSurfaces(explosionSprite, explodingStarSprite, heartSprites, platforms, stars, stalactites, fairies, background, uniState, charset, screen, scrtex, window, renderer);
	}
	if (background == NULL) {
		freeSurfaces(explosionSprite, explodingStarSprite, heartSprites, platforms, stars, stalactites, fairies, background, uniState, charset, screen, scrtex, window, renderer);
	}
	if (heartSprites == NULL) {
		freeSurfaces(explosionSprite, explodingStarSprite, heartSprites, platforms, stars, stalactites, fairies, background, uniState, charset, screen, scrtex, window, renderer);
	}
	if (stars.sprites[0] == NULL)
		freeSurfaces(explosionSprite, explodingStarSprite, heartSprites, platforms, stars, stalactites, fairies, background, uniState, charset, screen, scrtex, window, renderer);
}
// main
#ifdef __cplusplus
extern "C"
#endif
void initializeSurfaces(object& fairies, object& stars, object& platforms, SDL_Surface* explosionSprite[4], SDL_Surface* explodingStarSprite[4],
	 object& stalactites, SDL_Surface*& charset, uniState& uniState, SDL_Surface*& background, SDL_Surface*& heartSprites)
{
	fairies.sprites[0] = SDL_LoadBMP("fairy.bmp");
	stars.sprites[0] = SDL_LoadBMP("star.bmp");
	platforms.sprites[0] = SDL_LoadBMP("platform2.bmp");
	platforms.sprites[1] = SDL_LoadBMP("platform1.bmp");
	platforms.sprites[2] = SDL_LoadBMP("platform3.bmp");
	platforms.sprites[3] = SDL_LoadBMP("platform4.bmp");
	platforms.sprites[4] = SDL_LoadBMP("platform5.bmp");
	platforms.sprites[5] = SDL_LoadBMP("platform6.bmp");
	uniState.run[0] = SDL_LoadBMP("run1.bmp");
	uniState.run[1] = SDL_LoadBMP("run2.bmp");
	uniState.run[2] = SDL_LoadBMP("run3.bmp");
	uniState.run[3] = SDL_LoadBMP("run4.bmp");
	uniState.run[4] = SDL_LoadBMP("run5.bmp");
	uniState.run[5] = SDL_LoadBMP("run6.bmp");
	explosionSprite[0] = SDL_LoadBMP("explosion0.bmp");
	explosionSprite[1] = SDL_LoadBMP("explosion1.bmp");
	explosionSprite[2] = SDL_LoadBMP("explosion2.bmp");
	explosionSprite[3] = SDL_LoadBMP("explosion3.bmp");
	explodingStarSprite[0] = SDL_LoadBMP("explodingStar1.bmp");
	explodingStarSprite[1] = SDL_LoadBMP("explodingStar2.bmp");
	explodingStarSprite[2] = SDL_LoadBMP("explodingStar3.bmp");
	explodingStarSprite[3] = SDL_LoadBMP("explodingStar4.bmp");
	uniState.jumpSprite = SDL_LoadBMP("jump4.bmp");
	uniState.fallSprite[0] = SDL_LoadBMP("fall1.bmp");
	uniState.fallSprite[1] = SDL_LoadBMP("fall2.bmp");
	stalactites.sprites[0] = SDL_LoadBMP("stalactite.bmp");
	charset = SDL_LoadBMP("cs8x8.bmp");
	uniState.unicorn = SDL_LoadBMP("unicorn2.bmp");
	background = SDL_LoadBMP("background.bmp");
	heartSprites = SDL_LoadBMP("heart.bmp");
	stars.sprites[0] = SDL_LoadBMP("star.bmp");
}
int main(int argc, char **argv) {
	int t1, t2, quit, frames, rc;
	double delta, worldTime, fpsTimer, fps, backgroundX, camY;
	double explTime = 0;
	double starCollTime = 0;
	double fairyCollTime = 0;
	int explCount = 0;
	int starCollCount = 0;
	int fairyCollCount = 0;
	bool menu = 1;
	bool start = 0;
	bool controlMode = 1;
	uniState uniState;
	uniState.starCollected = 1;
	SDL_Event event;
	SDL_Surface* screen, * charset;
	SDL_Surface* background, * heartSprites, *explosionSprite[EXPLOSION_FRAMES], *explodingStarSprite[EXPLOSION_FRAMES];
	SDL_Texture *scrtex;
	SDL_Window *window;
	SDL_Renderer *renderer;
	object platforms, stalactites, stars, fairies;
	if(SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		printf("SDL_Init error: %s\n", SDL_GetError());
		return 1;
		}

	// tryb pe³noekranowy / fullscreen mode
	rc = SDL_CreateWindowAndRenderer(0, 0, SDL_WINDOW_FULLSCREEN_DESKTOP,
	                                 &window, &renderer);
	//rc = SDL_CreateWindowAndRenderer(SCREEN_WIDTH, SCREEN_HEIGHT, 0,
	//                                 &window, &renderer);
	if(rc != 0) {
		SDL_Quit();
		printf("SDL_CreateWindowAndRenderer error: %s\n", SDL_GetError());
		return 1;
	};
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
	SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_SetWindowTitle(window, "JEDNOROZEC");
	screen = SDL_CreateRGBSurface(0, SCREEN_WIDTH, SCREEN_HEIGHT, 32,
	                              0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);
	scrtex = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888,
	                           SDL_TEXTUREACCESS_STREAMING,
	                           SCREEN_WIDTH, SCREEN_HEIGHT);

	SDL_ShowCursor(SDL_DISABLE);
	initializeSurfaces(fairies, stars, platforms, explosionSprite, explodingStarSprite, stalactites, charset, uniState, background, heartSprites);
	checkSurfaces(charset, explosionSprite, explodingStarSprite, heartSprites, platforms, stars, stalactites, fairies, background, uniState, screen, scrtex, window, renderer);
	SDL_SetColorKey(charset, true, 0x000000);
	char text[128];
	char text2[128];
	int czarny = SDL_MapRGB(screen->format, 0x00, 0x00, 0x00);
	int bialy = SDL_MapRGB(screen->format, 0xff, 0xff, 0xff);
	int zielony = SDL_MapRGB(screen->format, 0x00, 0xFF, 0x00);
	int czerwony = SDL_MapRGB(screen->format, 0xFF, 0x00, 0x00);
	int niebieski = SDL_MapRGB(screen->format, 0x11, 0x11, 0xCC);
	reset(fairies, stars, platforms, stalactites, frames, camY, fpsTimer, fps, quit, worldTime, backgroundX, background, uniState);
	while (!quit) {
		menuFunction(menu, quit, screen, czarny, czerwony, niebieski, text, text2, charset, scrtex, renderer, event, start);
		if (start) {
			reset(fairies, stars, platforms, stalactites, frames, camY, fpsTimer, fps, quit, worldTime,backgroundX, background, uniState);
			start = 0;
		}
		t1 = SDL_GetTicks();
		while (!menu) {
			srand(time(NULL));
			timeLapse(t2, delta, t1, worldTime, uniState, fpsTimer, fps, frames);
			drawBackground(screen, czarny, background, backgroundX);
			drawLifes(uniState, screen, heartSprites);
			drawScore(text, uniState, screen, zielony, czarny, charset);
			drawUnicorn(uniState, explTime, delta, explCount, screen, explosionSprite, worldTime);
			drawObjects(screen, platforms, stalactites, fairies, stars);
			drawCollectables(uniState, text, screen, charset, starCollTime, delta, starCollCount, explodingStarSprite, fairyCollTime);
			DashCheck(uniState, delta);
			fairyMovement(fairies, uniState);
			xMovement(platforms, uniState, delta, stalactites, fairies, stars);
			yMovement(uniState, delta, camY, platforms, stalactites, stars, fairies, controlMode);
			scrollBackground(backgroundX, uniState.speed, delta, background);
			collisions(fairies, platforms, stalactites, uniState, stars,  delta);
			createPlatforms(platforms, SCREEN_WIDTH, MAX_PLATFORMS, PLATFORMS_NUM);
			createPlatforms(stalactites, SCREEN_WIDTH * 1.5, MAX_STALACTITES, STALACTITE_NUM);
			createCollectables(platforms, stars, uniState, STARS_CHANCE);
			createCollectables(platforms, fairies, uniState, FAIRIES_CHANCE);
			crashCheck(uniState, screen, czerwony, niebieski, text, charset);
			controlScheme(fairies, stars, platforms, stalactites, event, controlMode, menu, frames, camY, fpsTimer, fps, quit, worldTime, backgroundX, background, delta, uniState);
			frames++;
			SDL_UpdateTexture(scrtex, NULL, screen->pixels, screen->pitch);
			SDL_RenderCopy(renderer, scrtex, NULL, NULL);
			SDL_RenderPresent(renderer);
		};
	}
	freeSurfaces(explosionSprite, explodingStarSprite, heartSprites, platforms, stars, stalactites, fairies, background, uniState, charset, screen, scrtex, window, renderer);
	return 0;
}