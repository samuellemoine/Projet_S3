#include "headers.h"

void timeout(int milliseconds){
    int milliseconds_since = clock() * 1000 / CLOCKS_PER_SEC;
    int end = milliseconds_since + milliseconds;
    do {
        milliseconds_since = clock() * 1000 / CLOCKS_PER_SEC;
    } while (milliseconds_since <= end);
}

void setRect(SDL_Rect *r, int x, int y, int w, int h){
  r->x = x; r->y = y; r->w=w, r->h=h;
}


void setDir(axe *currentDir, axe *dir){
  currentDir->dx = dir->dx;
  currentDir->dy = dir->dy;
}

void reset(bool *started, bool *pause, bool *gameover, bool *dirChanged, int *score, snake *head, queue *body, queue *mazeq, SDL_Rect *food, SDL_Rect** grid){
    *started = false;
    *pause = true;
    *gameover = false;
    *dirChanged = false;
    *score = 0;
    axe ini = {0, 0};
    while (queueSize(body) > 0){
      queueOut(body);
    }
    while (queueSize(mazeq) >= 0){
      queueOut(mazeq);
    }
    setRect(&head->snakeRect, grid[NBX/2][NBY/2].x, grid[NBX/2][NBY/2].y, SNAKE_WIDTH, SNAKE_HEIGHT);
    setDir(&head->dir, &ini);
    queueIn(body, &head->snakeRect);
    queueOut(body);
    *food = randFood(body, mazeq);

}

void handleMenu(queue *mazeq, bool *started, SDL_Event *event, SDL_Renderer *screen, SDL_Texture *menuTexture, int *choice, int *level, int *maze, SDL_Rect** grid){

  char** lines = allocate_Char2D(NBX, NBY);
  if(event->type == SDL_KEYDOWN){
    switch (event->key.keysym.sym){
      case SDLK_LEFT:
        *choice = !*choice;
      break;
      case SDLK_RIGHT:
        *choice = !*choice;
      break;
      case SDLK_UP:
      if (*choice){
        if (*maze > 0)  *maze -= 1;
        else  *maze = 3;
      }
      else{
        if (*level > 0)  *level -= 1;
        else  *level = 4;
      }
      break;
      case SDLK_DOWN:
      if (*choice){
        if (*maze < 3)  *maze += 1;
        else  *maze = 0;
      }
      else{
        if (*level < 4)  *level += 1;
        else  *level = 0;
      }
      break;
      case SDLK_RETURN:
        *started = true;
        switch (*maze){
          case 1:
            readMazeFile("maze1.txt", lines);
            putInMaze(mazeq, grid, lines);
          break;
	        case 2:
            randMaze(15 + rand()%15, mazeq, grid);
	        break;
          case 3:
            randMaze(33 + rand()%17, mazeq, grid);
          break;
        }
        break;
    }
  }
  free_Char2D(lines, NBY);

  SDL_Rect leftRect = (SDL_Rect) { 4 * SCREEN_WIDTH / 25, 6 * SCREEN_HEIGHT / 25, 8 * SCREEN_WIDTH / 25, 17 * SCREEN_HEIGHT / 25 };
  SDL_Rect rightRect = (SDL_Rect) { 13 * SCREEN_WIDTH / 25, 6 * SCREEN_WIDTH / 25, 8 * SCREEN_WIDTH / 25, 17 * SCREEN_WIDTH / 25 };
  SDL_Rect mazeSprite = (SDL_Rect) { (*maze * 2 + !*choice) * 160, 340, 160, 340 };
  SDL_Rect levelSprite = (SDL_Rect) { (*level * 2 + *choice) * 160, 0, 160, 340 };

  SDL_RenderClear(screen);
  SDL_RenderCopy(screen, menuTexture, &mazeSprite, &leftRect);
  SDL_RenderCopy(screen, menuTexture, &levelSprite, &rightRect);
  SDL_RenderPresent(screen);

}

void handleKeys(SDL_Event *event, const Uint8 *keyboardState, snake *head, dir *direction, bool *pause, bool *dirChanged){
  bool arrowPressed = keyboardState[SDL_SCANCODE_LEFT] || keyboardState[SDL_SCANCODE_RIGHT] || keyboardState[SDL_SCANCODE_UP] || keyboardState[SDL_SCANCODE_DOWN];
  if (event){
    printf("");
  }
  if (arrowPressed  && *pause){
        *pause = false;
  }

  if (keyboardState[SDL_SCANCODE_SPACE]){
    *pause = !(*pause);
    timeout(100);
  }

  if (keyboardState[SDL_SCANCODE_LEFT] && head->dir.dx != 1 && !*dirChanged){
    setDir(&head->dir, &direction->left);
    *dirChanged = true;
  }
  if (keyboardState[SDL_SCANCODE_RIGHT] && head->dir.dx != -1 && !*dirChanged){
    setDir(&head->dir, &direction->right);
    *dirChanged = true;
  }
  if (keyboardState[SDL_SCANCODE_UP] && head->dir.dy != 1 && !*dirChanged){
    setDir(&head->dir, &direction->up);
    *dirChanged = true;
  }
  if (keyboardState[SDL_SCANCODE_DOWN] && head->dir.dy != -1 && !*dirChanged){
    setDir(&head->dir, &direction->down);
    *dirChanged = true;
  }

}

   /* gives better precision in commands than integer division */
int indice(int coord, int size){
  int i = coord / size;
  float ratio = coord / size;
  if (ratio - i >= 0.5){
    return i + 1;
  }
  return i;
}

bool foodContact(SDL_Rect *head, SDL_Rect *food){
  if ( indice(food->x, SNAKE_WIDTH) == indice(head->x, SNAKE_WIDTH) && indice(food->y, SNAKE_HEIGHT) == indice(head->y, SNAKE_HEIGHT)){
        return true;
  }
  return false;
}


void move(snake *head, SDL_Rect ** grid, queue *body, queue *mazeq, SDL_Rect *food, bool *gameover, bool *dirChanged, int level, int *score){
  timeout(ADJUST_LEVEL - level);

  bool headMoved, eats, isInLimits;
  int tmpPosX, tmpPosY;
  int xx = -1; int yy = -1;
  SDL_Rect currentHead = queueBack(body);  /* SDL_Rect of the snake's head */

  /* update temporary positons */
  tmpPosX = head->snakeRect.x;
  tmpPosY = head->snakeRect.y;
  tmpPosX += head->dir.dx * VELOCITY;
  tmpPosY += head->dir.dy * VELOCITY;

  /* make the snake fuse through walls */
  if (tmpPosX < 0){
    head->snakeRect.x = SCREEN_WIDTH;
  }
  else if (tmpPosX >= SCREEN_WIDTH - SNAKE_WIDTH){
    head->snakeRect.x = 0;
  }
  if (tmpPosY < TOP_BAR){
    head->snakeRect.y = SCREEN_HEIGHT + TOP_BAR;
  }
  else if (tmpPosY >= SCREEN_HEIGHT + TOP_BAR - SNAKE_HEIGHT){
    head->snakeRect.y = TOP_BAR;
  }

  /* apply temporary positions whe tmpPos within limits */
  if (tmpPosX >= 0 && tmpPosX < SCREEN_WIDTH){
    head->snakeRect.x = tmpPosX;
  }
  if (tmpPosY >= TOP_BAR && tmpPosY < SCREEN_HEIGHT + TOP_BAR){
    head->snakeRect.y = tmpPosY;
  }
  /* determine position in the grid */
  xx = indice(head->snakeRect.x, SNAKE_WIDTH);
  yy = indice(head->snakeRect.y - TOP_BAR, SNAKE_HEIGHT);

  /* setting the three necessary booleans to move the snake */
  eats = foodContact(&head->snakeRect, food);
  headMoved = (indice(currentHead.x, SNAKE_WIDTH) != xx) || (indice(currentHead.y - TOP_BAR, SNAKE_HEIGHT) != yy);
  isInLimits = xx >= 0 && xx < NBX && yy >= 0 && yy < NBY;

  if (isInLimits && headMoved){
    *dirChanged = false;
    if (eats){
      queueIn(body, &grid[xx][yy]);
      *score += level + 1;
      *food = randFood(body, mazeq);
    }
    else{
      queueIn(body, &grid[xx][yy]);
      queueOut(body);
    }
    if (snakeContact(body) || mazeContact(head, mazeq)){
      *gameover = true;
    }
  }

}
