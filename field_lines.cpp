/***************************************************************************
 * Field Lines!
 *
 * Adapted from Robert H. Good's "Classical Electromagnetism" (1999)
 *
 * Draws field lines emanating from 2 charged bodies (static instance).
 *
 * Notes:
 *  - Field lines are initially set via:
 *      for (th = M_PI / 16; th < 2 * M_PI; th += M_PI / 8)
 *    (Decrease the angular step for more field lines.)
 *  - Constants for canvas layout (computed from the window size):
 *     - xc = maxx / 3.0f;  Centers the left charge on the left third of the screen.
 *     - yc = maxy / 2.0f;  Centers the charges vertically.
 *
 * Controls:
 * - Press 'ESC' to exit.
 **************************************************************************/

#include <graphics.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

// --- Tunable parameters ---
const float CHARGE1_RADIUS = 15.0f;   // radius of the left (stronger) charge
const float CHARGE2_RADIUS = 10.0f;   // radius of the right (weaker) charge
const float CHARGE2_OFFSET = 100.0f;  // horizontal offset from charge 1 to charge 2
const int   PIXEL_COLOR    = 15;     
// --------------------------

int main() {
  int gdriver = DETECT, gmode;
  initgraph(&gdriver, &gmode, NULL);

  int maxx = getmaxx();
  int maxy = getmaxy();

  // center positions for drawing charges:
  float xc = maxx / 3.0f;
  float yc = maxy / 2.0f;

  circle((int)xc, (int)yc, (int)CHARGE1_RADIUS);
  circle((int)(xc + CHARGE2_OFFSET), (int)yc, (int)CHARGE2_RADIUS);

  float th, r2, r22, r12, x, y, dx, dy, Ex, Ey;

  for (th = M_PI / 16; th < 2 * M_PI; th += M_PI / 8) { //*
    r2 = CHARGE1_RADIUS;
    x = r2 * cos(th);
    y = r2 * sin(th);
    do {
      r22 = r2 * r2;
      r12 = (x - CHARGE2_OFFSET) * (x - CHARGE2_OFFSET) + y * y;
      Ex = 2 * x / r22 - (x - CHARGE2_OFFSET) / r12;
      Ey = 2 * y / r22 - y / r12;

      // Normalizing step size for smoother tracing!
      if (fabs(Ex) > fabs(Ey)) {
        dx = Ex / fabs(Ex);
        dy = dx * Ey / Ex;
      }
      else {
        dy = Ey / fabs(Ey);
        dx = dy * Ex / Ey;
      }
      x += dx;
      y += dy;
      putpixel((int)(xc + x), (int)(yc + y), PIXEL_COLOR);
      r2 = hypot(x, y);
    } while (r12 > 144 && r2 < xc);
  }

  outtextxy(10, maxy - 30, (char*)"Press 'ESC' to exit");
  getch();

  closegraph();
  return 0;
}
