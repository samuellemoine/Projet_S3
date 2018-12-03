#include "headers.h"
int power(int x, int n){
    if (n == 0) return 1;
    return x * power(x, n - 1);
}

void setPixel(SDL_Surface* surf, Uint8 r, Uint8 g, Uint8 b, Uint8 a, int x, int y, int w){
    Uint32 *pixels;
    SDL_LockSurface(surf);
    pixels = surf->pixels;
    pixels[x * w + y] = SDL_MapRGBA(surf->format, r, g, b, a);
    SDL_UnlockSurface(surf);
}

void drawCircle(SDL_Surface* surf, Uint8 r, Uint8 g, Uint8 b, Uint8 a, int x, int y, int w, int h, int ray){
  int i, j;
  int sum, sq;
  for (i = 0; i < h; i++){
    for (j = 0; j < w; j++){
      sum = power(j - x, 2) + power(i - y, 2);
      sq = power(ray, 2);
      if (sum <= sq) setPixel(surf, r, g, b, a, i, j, w);
    }
  }
}
SDL_Texture* circleTexture(SDL_Renderer* screen, Uint8 r, Uint8 g, Uint8 b, Uint8 a, int x, int y, int w, int h, int ray){
  SDL_Surface* s = SDL_CreateRGBSurfaceWithFormat(0, w, h, 32, SDL_PIXELFORMAT_RGBA8888);
  drawCircle(s, r, g, b, a, x, y, w, h, ray);
  SDL_Texture* t = SDL_CreateTextureFromSurface(screen, s);
  SDL_FreeSurface(s);
  return t;
}
