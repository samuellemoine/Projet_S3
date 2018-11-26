#include <SDL2/SDL.h>
#include <stdio.h>

int main(){
  SDL_Window* wind = SDL_CreateWindow("Snake", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 300, 300, SDL_WINDOW_SHOWN);
  SDL_Renderer* rend = SDL_CreateRenderer(wind, -1, SDL_RENDERER_ACCELERATED);
  SDL_Surface* surf = SDL_LoadBMP("surf.bmp");
  SDL_SetRenderDrawColor(rend, 255, 255, 255, 255);
  SDL_SetColorKey(surf, SDL_TRUE, SDL_MapRGB(surf->format, 255, 0, 0));
  SDL_Texture* text = SDL_CreateTextureFromSurface(rend, surf);
  SDL_Event e;
  int playing = 1;
  while(playing){
    SDL_PollEvent(&e);
    if (e.type == SDL_QUIT) playing = 0;
    SDL_RenderClear(rend);
    SDL_RenderCopy(rend, text, NULL, NULL);
    SDL_RenderPresent(rend);
  }
  SDL_FreeSurface(surf);
  SDL_DestroyTexture(text);
  return 0;
}
