#pragma once

#include "SDL.h"
#include "SDL_ttf.h"

#ifndef INTERFACE_UTILS_H
#define INTERFACE_UTILS_H

// Função para renderizar o texto na tela
SDL_Texture* render_text(const char* message, SDL_Color color, SDL_Renderer* renderer, TTF_Font* font) {
    SDL_Surface* surface = TTF_RenderText_Solid(font, message, color);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    return texture;
}


#endif