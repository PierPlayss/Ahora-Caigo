#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <iostream>
#include <vector>
#include <sstream>
#include <Windows.h>
#include <fstream>
#include <string>


#include <Windows.h>

#include "RenderWindow.hpp"
#include "Entity.hpp"

using namespace std;



RenderWindow::RenderWindow(const char* p_title, int p_w, int p_h)
	:window(NULL), renderer(NULL)
{
	window = SDL_CreateWindow(p_title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, p_w, p_h, SDL_WINDOW_SHOWN);

	if (window == NULL)
	{
		std::cout << "Window failed to init. Error: " << SDL_GetError() << std::endl;
	}

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
}

SDL_Texture* RenderWindow::loadTexture(const char* p_filePath)
{
	SDL_Texture* texture = NULL;
	texture = IMG_LoadTexture(renderer, p_filePath);

	if (texture == NULL)
		std::cout << "Failed to load texture. Error: " << SDL_GetError() << std::endl;

	return texture;
}


void RenderWindow::drawText(const char* msg, int x, int y, int r, int g, int b, int a, int size) {

	SDL_Surface* surf;
	SDL_Texture* texture;
	TTF_Font* font = TTF_OpenFont("res/fonts/kanit.otf", size);

	SDL_Color color;
	color.r = r;
	color.g = g;
	color.b = b;
	color.a = 255;
	SDL_Rect rect;
	surf = TTF_RenderText_Blended(font, msg, color);

	texture = SDL_CreateTextureFromSurface(renderer, surf);
	SDL_SetTextureAlphaMod(texture, a);
	rect.x = x;
	rect.y = y;
	rect.w = surf->w;
	rect.h = surf->h;
	TTF_CloseFont(font);
	//IMG_SavePNG(surf, "image.png");
	//SDL_SaveBMP(surf, "image.bmp");
	SDL_FreeSurface(surf);



	SDL_RenderCopy(renderer, texture, NULL, &rect);
	SDL_DestroyTexture(texture);

}

void RenderWindow::textCustom(const char* msg, std::string path, int x, int y, int r, int g, int b, int a, int size, bool center, int xoff, int yoff, int quality) {
	int width = 1280;
	int height = 720;
	SDL_Surface* surf;
	SDL_Texture* texture;
	TTF_Font* font = TTF_OpenFont(path.c_str(), size);

	SDL_Color color;
	color.r = r;
	color.g = g;
	color.b = b;
	color.a = a;
	SDL_Rect rect;


	//SDL_Color bgColor;

	switch (quality) {
	case 1: surf = TTF_RenderText_Blended(font, msg, color);
		break;
	case 2: surf = TTF_RenderText_Shaded(font, msg, color, color);
		break;
	case 0: surf = TTF_RenderText_Solid(font, msg, color);
		break;
	default:
		surf = TTF_RenderText_Solid(font, msg, color);

	}



	texture = SDL_CreateTextureFromSurface(renderer, surf);
	SDL_SetTextureAlphaMod(texture, a);

	if (center == true) {
		rect.x = width / 2 - ((surf->w) / 2) + xoff;
		rect.y = height / 2 - ((surf->h) / 2) + yoff;
		rect.w = surf->w;
		rect.h = surf->h;
	}
	else {
		rect.x = x;
		rect.y = y;
		rect.w = surf->w;
		rect.h = surf->h;
	}

	TTF_CloseFont(font);
	//IMG_SavePNG(surf, "image.png");
	//SDL_SaveBMP(surf, "image.bmp");
	SDL_FreeSurface(surf);



	SDL_RenderCopy(renderer, texture, NULL, &rect);
	SDL_DestroyTexture(texture);

}

void RenderWindow::textAlignTo(const char* msg, Entity& p_entity, int r, int g, int b, int a, int size, int alignX, int alignY, int xoff, int yoff, std::string path) {
	int width = 1280;
	int height = 720;
	int totalX = 0;
	int totalY = 0;

	SDL_Surface* surf;
	SDL_Texture* texture;

	TTF_Font* font = TTF_OpenFont(path.c_str(), size);



	SDL_Color color;
	color.r = r;
	color.g = g;
	color.b = b;
	color.a = a;
	SDL_Rect rect;
	surf = TTF_RenderText_Blended(font, msg, color);
	texture = SDL_CreateTextureFromSurface(renderer, surf);
	SDL_SetTextureAlphaMod(texture, a);

	//0= no align 1=center 2=right


	switch (alignX) {
	case 0:
		totalX += p_entity.getCurrentFrame().x;
		break;
	case 1:
		totalX = p_entity.getCurrentFrame().w / 2 - ((surf->w) / 2);
		break;
	case 2:
		totalX = p_entity.getCurrentFrame().w - ((surf->w));
		break;
	default:
		totalX += p_entity.getCurrentFrame().x;
		break;
	}

	switch (alignY) {
	case 0:
		totalY += p_entity.getCurrentFrame().y;
		break;
	case 1:
		totalY = p_entity.getCurrentFrame().h / 2 - ((surf->h) / 2);
		break;
	case 2:
		totalY = p_entity.getCurrentFrame().h - ((surf->h));
		break;
	default:
		totalY += p_entity.getCurrentFrame().y;
		break;
	}



	rect.x = totalX + xoff;
	rect.y = totalY + yoff;
	rect.w = surf->w;
	rect.h = surf->h;



	TTF_CloseFont(font);
	//IMG_SavePNG(surf, "image.png");
	//SDL_SaveBMP(surf, "image.bmp");
	SDL_FreeSurface(surf);



	SDL_RenderCopy(renderer, texture, NULL, &rect);
	SDL_DestroyTexture(texture);

}

void RenderWindow::cleanUp()
{
	SDL_DestroyWindow(window);
}

void RenderWindow::clear()
{
	SDL_RenderClear(renderer);
}

void RenderWindow::render(Entity& p_entity, double angle, float multiplicador) {

	SDL_Point pt;
	pt.x = p_entity.getCurrentFrame().x;
	pt.y = p_entity.getCurrentFrame().y;

	SDL_Rect src;
	src.x = p_entity.getCurrentFrame().x;
	src.y = p_entity.getCurrentFrame().y;
	src.w = p_entity.getCurrentFrame().w;
	src.h = p_entity.getCurrentFrame().h;

	SDL_Rect dst;
	dst.x = p_entity.getX() * multiplicador;
	dst.y = p_entity.getY() * multiplicador;
	dst.w = p_entity.getCurrentFrame().w * multiplicador;
	dst.h = p_entity.getCurrentFrame().h * multiplicador;

	SDL_RenderCopyEx(renderer, p_entity.getTex(), &src, &dst, angle, NULL, SDL_FLIP_NONE);
}

void RenderWindow::renderFlip(Entity& p_entity, double angle, float multiplicador, bool flip) {
	SDL_RendererFlip flipType;
	SDL_Point pt;
	pt.x = p_entity.getCurrentFrame().x;
	pt.y = p_entity.getCurrentFrame().y;

	SDL_Rect src;
	src.x = p_entity.getCurrentFrame().x;
	src.y = p_entity.getCurrentFrame().y;
	src.w = p_entity.getCurrentFrame().w;
	src.h = p_entity.getCurrentFrame().h;

	SDL_Rect dst;
	dst.x = p_entity.getX() * multiplicador;
	dst.y = p_entity.getY() * multiplicador;
	dst.w = p_entity.getCurrentFrame().w * multiplicador;
	dst.h = p_entity.getCurrentFrame().h * multiplicador;

	if (flip == 1) {
		flipType = SDL_FLIP_HORIZONTAL;

	}
	else flipType = SDL_FLIP_NONE;


	SDL_RenderCopyEx(renderer, p_entity.getTex(), &src, &dst, angle, NULL, flipType);
}

void RenderWindow::render(Entity& p_entity, float multiplicador)
{
	SDL_Rect src;
	src.x = p_entity.getCurrentFrame().x;
	src.y = p_entity.getCurrentFrame().y;
	src.w = p_entity.getCurrentFrame().w;
	src.h = p_entity.getCurrentFrame().h;

	SDL_Rect dst;
	dst.x = p_entity.getX() * multiplicador;
	dst.y = p_entity.getY() * multiplicador;
	dst.w = p_entity.getCurrentFrame().w * multiplicador;
	dst.h = p_entity.getCurrentFrame().h * multiplicador;

	SDL_RenderCopy(renderer, p_entity.getTex(), &src, &dst);
}
void RenderWindow::renderCenter(Entity& p_entity, float multiplicador, int xOff, int yOff)
{
	int width = 1280;
	int height = 720;

	SDL_Rect src;
	src.x = p_entity.getCurrentFrame().x;
	src.y = p_entity.getCurrentFrame().y;
	src.w = p_entity.getCurrentFrame().w;
	src.h = p_entity.getCurrentFrame().h;

	SDL_Rect dst;
	dst.x = width / 2 - ((p_entity.getCurrentFrame().w * multiplicador) / 2) + xOff;
	dst.y = height / 2 - ((p_entity.getCurrentFrame().h * multiplicador) / 2) + yOff;
	dst.w = p_entity.getCurrentFrame().w * multiplicador;
	dst.h = p_entity.getCurrentFrame().h * multiplicador;

	SDL_RenderCopy(renderer, p_entity.getTex(), &src, &dst);
}

void RenderWindow::renderAlignTo(Entity& p_entity, Entity& p_entity2, float multiplicador, int alignX, int alignY, int xoff, int yoff) {

	int totalX = 0, totalY = 0;


	SDL_Rect src;
	src.x = p_entity.getCurrentFrame().x;
	src.y = p_entity.getCurrentFrame().y;
	src.w = p_entity.getCurrentFrame().w;
	src.h = p_entity.getCurrentFrame().h;



	switch (alignX) {
	case 0:
		totalX = p_entity2.getCurrentFrame().x;
		break;
	case 1:
		totalX = p_entity2.getCurrentFrame().w / 2 - ((p_entity.getCurrentFrame().w) / 2);
		break;
	case 2:
		totalX = p_entity2.getCurrentFrame().w - (p_entity.getCurrentFrame().w);

		break;
	default:
		totalX = p_entity2.getCurrentFrame().x;
		break;
	}

	switch (alignY) {
	case 0:
		totalY = p_entity2.getCurrentFrame().y;
		break;
	case 1:
		totalY = p_entity2.getCurrentFrame().h / 2 - (p_entity.getCurrentFrame().h / 2);
		break;
	case 2:
		totalY = p_entity2.getCurrentFrame().h - ((p_entity.getCurrentFrame().h));
		break;
	default:
		totalY = p_entity2.getCurrentFrame().y;
		break;
	}


	SDL_Rect dst;


	dst.x = totalX + xoff;
	dst.y = totalY + yoff;
	dst.w = p_entity.getCurrentFrame().w * multiplicador;
	dst.h = p_entity.getCurrentFrame().h * multiplicador;

	SDL_RenderCopy(renderer, p_entity.getTex(), &src, &dst);

}

void RenderWindow::render(Entity& p_entity, SDL_Rect& rect)
{
	SDL_Rect src;
	src.x = rect.x;
	src.y = rect.y;
	src.w = rect.w;
	src.h = rect.h;

	SDL_Rect dst;
	dst.x = p_entity.getX();
	dst.y = p_entity.getY();
	dst.w = p_entity.getCurrentFrame().w;
	dst.h = p_entity.getCurrentFrame().h / 4;

	SDL_RenderCopy(renderer, p_entity.getTex(), &src, &dst);
}


void RenderWindow::animation(Entity& p_entity, SDL_Rect& rect, SDL_Rect& pos) {

	SDL_Rect dst;
	dst.x = rect.x;
	dst.y = rect.y;
	dst.w = rect.w;
	dst.h = rect.h;

	SDL_Rect final;
	final.x = pos.x;
	final.y = pos.y;
	final.w = pos.w;
	final.h = pos.h;


	SDL_RenderCopy(renderer, p_entity.getTex(), &dst, &final);
}

void RenderWindow::backgroundColor(int r, int g, int b, int a) {
	SDL_SetRenderDrawColor(renderer, r, g, b, a);
}

void RenderWindow::display()
{
	SDL_RenderPresent(renderer);
}

void RenderWindow::ToggleFullscreen() {
	Uint32 FullscreenFlag = SDL_WINDOW_FULLSCREEN;
	bool IsFullscreen = SDL_GetWindowFlags(window) & FullscreenFlag;
	SDL_SetWindowFullscreen(window, IsFullscreen ? 0 : FullscreenFlag);
	SDL_ShowCursor(IsFullscreen);
}

void RenderWindow::rectangle(int x, int y, int w, int h, int r, int g, int b, int a, bool filled) {
	//SDL_RenderClear(renderer);

	SDL_Rect rect;
	rect.x = x;
	rect.y = y;
	rect.w = w;
	rect.h = h;

	SDL_SetRenderDrawColor(renderer, r, g, b, a);
	if (filled == true) {

		SDL_RenderFillRect(renderer, &rect);

	}
	else {
		SDL_RenderDrawRect(renderer, &rect);
	}

	SDL_SetRenderDrawColor(renderer, 0, 0, 0, a);
	SDL_RenderPresent(renderer);


}
float RenderWindow::easyInOut(int startPos, int endPos, float duration, int& currentStep, bool &animationOver) {

	//float startPos = -90, endPos = 200;
	//float currentPos = startPos;
	const int FPS = 60;
	float change;
	float t = 0;
	//float duration = 1;
	//int currentStep = 0;
	int totalSteps = duration * FPS;


	if (currentStep < totalSteps) {

		t = (currentStep * 1.0) / totalSteps;

		if (t <= 0.5) {
			change = 2.0 * t * t;
			change = startPos + (change * (endPos - startPos)) / 1;

		}
		else {
			t -= 0.5;
			change = 2.0 * t * (1.0 - t) + 0.5;
			change = startPos + (change * (endPos - startPos)) / 1;

		}



		currentStep++;
		return change;
	}
	else {
		animationOver = true;
	}

}


