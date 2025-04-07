

//Dipole!
#include <graphics.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <math.h>

#ifdef __linux__
// Linux does not support conio.h functions like kbhit(), so we need an alternative
#include <SDL2/SDL.h>
#endif

int main() {
  int gdriver = DETECT, gmode;
  initgraph(&gdriver, &gmode, (char*)"");
  
  char ch = 'd';
  float xc = getmaxx() / 2, yc = getmaxy() / 2, fr = yc / 11, C, s2, kr, wt, x, y;

  while (ch != (char)27) {  // Loop until ESC is pressed
    for (wt = 0; wt < 2 * M_PI; wt += M_PI / 64) {
      for (kr = 0.05; kr <= 10; kr += 0.05) {
        s2 = fabs(C / (sin(kr - wt) / kr - cos(kr - wt)));
        if (s2 < 1) {
          x = fr * kr * sqrt(s2);
          y = fr * kr * sqrt(1 - s2);
          putpixel(xc + x, yc + y, 15);
          putpixel(xc - x, yc - y, 15);
          putpixel(xc + x, yc - y, 15);
          putpixel(xc - x, yc + y, 15);
        }
      }
    }

#ifdef __linux__
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) {
        ch = 27; // Simulate ESC key
      } else if (event.type == SDL_KEYDOWN) {
        ch = event.key.keysym.sym;
      }
    }
#else
    if (kbhit()) {
      ch = getch();
    }
#endif

    delay(20);
    cleardevice();

    if (ch == (char)27) {
      break;
    }
  }

  closegraph();
  return 0;
}

