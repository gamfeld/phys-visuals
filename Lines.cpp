/*
 * Note: this script relies on the outdated Borland Graphics Interface (BG),
 * bundled with several Borland compilers for DOS operating systems since 1987.
 * I am using SDL_bgi as a replacement (package: libsdl-bgi-dev)
 */

/*
 * Electric Field Line Model using SDL_bgi
 *
 * Adapted from Robert H. Goods "Classical Electromagnetism", 1999
 * 
 * This script draws field lines emanating from 2 charged bodies.
 *
 * Compile with: g++ Lines.cpp -o lines -lSDL_bgi -lSDL2
 */


#include <graphics.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

int main() {
  int gdriver = DETECT, gmode;
  initgraph(&gdriver, &gmode, NULL);

  float xc = getmaxx() / 3, yc = getmaxy() / 2, th, r2, r22, r12, x, y, dx, dy, Ex, Ey;

  circle(xc, yc, 15);
  circle(xc + 100, yc, 10); // Fixed typo: yx -> yc

  for (th = M_PI / 16; th < 2 * M_PI; th += M_PI / 8) {
    r2 = 15;
    x = r2 * cos(th);
    y = r2 * sin(th);
    do {
      r22 = r2 * r2;
      r12 = (x - 100) * (x - 100) + y * y;
      Ex = 2 * x / r22 - (x - 100) / r12;
      Ey = 2 * y / r22 - y / r12;

      if (fabs(Ex) > fabs(Ey)) {
        dx = Ex / fabs(Ex);
        dy = dx * Ey / Ex;
      } else {
        dy = Ey / fabs(Ey);
        dx = dy * Ex / Ey;
      }

      x += dx;
      y += dy;
      putpixel(xc + x, yc + y, 15);
      r2 = hypot(x, y);
    } while (r12 > 144 && r2 < xc);
  }

  // Wait for user input
  sleep(5);
  closegraph();

  return 0;
}

