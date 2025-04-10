/***************************************************************************
 * Dipole!!
 *
 * Adapted from Robert H. Good's "Classical Electromagnetism (1999)
 *
 * This program outputs the time-dependant EF of a radiating electric dipole in
 * a far field, using a parameterized form of the integrated dipole field
 * equation to compute sin²(theta) as a function of radius (kr), phase (wt), 
 * and an integration constant (C) - Each value of C traces a field line.
 * 
 * Notes:
 * Some other parameters you can change:
 *
 * - float fr = yc / 11.0f; Radial scaling factor for the entire diagram.
 *      - Increase for more spread-out lines, decrease for tighter packing.
 *
 * - for (C = -0.15f; C <= 1.35f; C += 0.3f); Constants of integration (C) control different field lines.
 *    - Adjust the range or step size for more or fewer lines.
 *
 * - for (kr = 0.05f; kr <= 10.0f; kr += 0.05f); Radial extent and smoothness of each line.
 *      - Decreasing the step smooths curves; increasing max kr extends them.
 *
 * - Leave as-is unless modifying canvas layout (center coords of window)*:
 *    xc = maxx / 2.0f;
 *    yc = maxy / 2.0f;
 *   (unless plotting multiple dipoles or shifting view)
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

// --- Tunable Parameters --- 
const float deltaKr = 0.05f;          // Radial step size for kr
const float maxKr     = 10.0f;        // Maximum value for kr
const float deltaC    = 0.3f;         // Step size for integration constant C
const float Cmin      = -0.15f;       // Minimum value for C
const float Cmax      =  1.35f;       // Maximum value for C
const float deltaWt   = M_PI / 64.0f; // Phase increment for each iteration
// --------------------------

int main() {
  int gdriver = DETECT, gmode;
  initgraph(&gdriver, &gmode, NULL);

  int maxx = getmaxx();
  int maxy = getmaxy();
  const float xc = maxx / 2.0f; //*
  const float yc = maxy / 2.0f; //*
  const float fr = yc / 11.0f;  // radial scaling factor of entire diagram

  float C, s2, kr, wt, x, y;
  char ch = ' ';

  wt = 0.0f; //Represents phase angle

  while (ch != 27) {  // ESC
    for (C = Cmin; C <= Cmax; C += deltaC) {
      for (kr = deltaKr; kr <= maxKr; kr += deltaKr) {
        float denom = (sin(kr - wt) / kr - cos(kr - wt));
        if (denom == 0)
          continue;
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
    outtextxy(10, maxy - 30, (char*)"Press 's' for next iteration, 'ESC' to exit");
    
    ch = getch();
    if (ch == 27) { 
      break;
    }

    wt += deltaWt;
    if (wt >= 2 * M_PI) {
      wt = 0;  
    }
    cleardevice();
  }
  closegraph();
  return 0;
}
