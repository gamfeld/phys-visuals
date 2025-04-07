//Field Lines!
// Adapted from Robert H. Good's "Classical Electromagnetism" (1999)
// Draws field lines emanating from 2 charged bodies.

/*********************** TUNABLE PARAMETERS ******************************
 *
 * #define CHARGE1_RADIUS 15.0f
 *   - Radius of the left (stronger) charge.
 *
 * #define CHARGE2_RADIUS 10.0f
 *   - Radius of the right (weaker) charge.
 *
 * #define CHARGE2_OFFSET 100.0f
 *   - Horizontal distance from the first charge to the second.
 *   - Increasing this spreads the charges apart.
 *
 * for (th = M_PI / 16; th < 2 * M_PI; th += M_PI / 8)
 *   - Angular spacing of initial field lines.
 *   - Decrease step for more lines (e.g., M_PI / 16 → M_PI / 32).
 *
 * Constants to leave as-is unless modifying canvas layout:
 *   xc = maxx / 3.0f;
 *   yc = maxy / 2.0f;
 *   - These center the left charge on the left third of the screen.
 *   
 * Controls:
 * - ESC exits the program (handled via getch()).
 **************************************************************************/


#include <graphics.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#define CHARGE1_RADIUS 15.0f
#define CHARGE2_RADIUS 10.0f
#define CHARGE2_OFFSET 100.0f
#define PIXEL_COLOR 15

int main() {
  int gdriver = DETECT, gmode;
  initgraph(&gdriver, &gmode, NULL);

  int maxx = getmaxx();
  int maxy = getmaxy();
  float xc = maxx / 3.0f;
  float yc = maxy / 2.0f;

  // Draw the charges (circles)
  circle((int)xc, (int)yc, (int)CHARGE1_RADIUS);
  circle((int)(xc + CHARGE2_OFFSET), (int)yc, (int)CHARGE2_RADIUS);

  // Variables for field line computation
  float th, r2, r22, r12, x, y, dx, dy, Ex, Ey;

  for (th = M_PI / 16; th < 2 * M_PI; th += M_PI / 8) {
    r2 = CHARGE1_RADIUS;
    x = r2 * cos(th);
    y = r2 * sin(th);
    do {
      r22 = r2 * r2;
      r12 = (x - CHARGE2_OFFSET) * (x - CHARGE2_OFFSET) + y * y;
      Ex = 2 * x / r22 - (x - CHARGE2_OFFSET) / r12;
      Ey = 2 * y / r22 - y / r12;

      // Going to Normalize step size for smoother tracing
      if (fabs(Ex) > fabs(Ey)) {
        dx = Ex / fabs(Ex);
        dy = dx * Ey / Ex;
      } else {
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

  //Wait for Key press lol
  getch();
  closegraph();

  return 0;
}
