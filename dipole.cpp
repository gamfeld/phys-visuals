//Dipole!!
// Adapted from Robert H. Good's "Classical Electromagnetism" (1999)
// Time-dependant EF of radiating electric dipole in far field.

/*********************** TUNABLE PARAMETERS ******************************
 * 
 * float fr = yc / 11.0f; Radial scaling factor for the entire diagram.
 *   - Increase for more spread-out lines, decrease for tighter packing.
 *
 * for (C = -0.15f; C <= 1.35f; C += 0.3f); Constants of integration (C) control different field lines.
 *   - Adjust the range or step size for more or fewer lines.
 *
 * for (kr = 0.05f; kr <= 10.0f; kr += 0.05f); Radial extent and smoothness of each line.
 *   - Decreasing the step smooths curves; increasing max kr extends them.
 *
 * Constants to leave as-is unless modifying canvas layout:
 *   xc = maxx / 2.0f;
 *   yc = maxy / 2.0f;
 *   - Keep these centered (unless plotting multiple dipoles or shifting view)
 *
 * This program uses a parameterized form of the integrated dipole field
 * equation to compute sin²(theta) as a function of radius (kr), phase (wt), and an integration constant (C).
 * Each value of C traces a distinct field line. As the phase advances (representing time), the field pattern evolves frame-by-frame.
 *
 * Controls:
 * - Press 's' or any key to step forward in phase frame (wt+=π/64).
 * - Press 'ESC' to exit.
 **************************************************************************/

#include <graphics.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

int main() {

  int gdriver = DETECT, gmode;
  initgraph(&gdriver, &gmode, NULL);

  int maxx = getmaxx();
  int maxy = getmaxy();
  float xc = maxx / 2.0f;
  float yc = maxy / 2.0f;
  float fr = yc / 11.0f; // HERE - Radial scaling
  float C, s2, kr, wt, x, y;
  char ch = ' ';

  // Initialize phase
  wt = 0.0f;

  // draw one frame per phase value, then wait for a key press.
  while (ch != 27) {  // 27 is ASCII for ESC
    for (C = -0.15f; C <= 1.35f; C += 0.3f) {
      for (kr = 0.05f; kr <= 10.0f; kr += 0.05f) {
        float denom = (sin(kr - wt) / kr - cos(kr - wt));
        if (denom == 0) {
          continue;  
        }
        s2 = fabs(C / denom);
        if (s2 <= 1) {
          x = fr * kr * sqrt(s2);
          y = fr * kr * sqrt(1 - s2);
          putpixel((int)(xc + x), (int)(yc + y), 15);
          putpixel((int)(xc - x), (int)(yc - y), 15);
          putpixel((int)(xc + x), (int)(yc - y), 15);
          putpixel((int)(xc - x), (int)(yc + y), 15);
        }
      }
    }

    // Draw instructions in a box near the bottom of the screen:
    outtextxy(10, maxy - 30, (char*)"Press 's' for next iteration, 'ESC' to exit");

    ch = getch();
    if (ch == 27) { // EXIT!
      break;
    }
    wt += M_PI / 64;
    if (wt >= 2 * M_PI) {
      wt = 0;  // Wrap around after full cycle!
    }

    cleardevice();
  }

  closegraph();
  return 0;
}

