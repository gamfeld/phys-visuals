//Charge
// Adapted from Robert H. Good's "Classical Electromagnetism" (1999)
// EXP!

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

#include <iostream>
#include <graphics.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

using namespace std;

int main() {
  int gdriver = DETECT, gmode;
  initgraph(&gdriver, &gmode, NULL);

  int i, j;
  char ch = 'd';
  float maxx = getmaxx(), xc = maxx / 3, yc = getmaxy() / 2;
  float vc = 0, vxo, vyo;
  float x[3][200] = {0}, y[3][200] = {0}, vx[3][200] = {0}, vy[3][200] = {0};

  // Colors for left, center, right trails:
  // In BGI, RED = 4, WHITE = 15, BLUE = 1.
  int colors[3] = {4, 15, 1};

  cout.precision(2);
  cout.setf(ios::showpoint);
  cout.setf(ios::fixed);
  cout.setf(ios::showpos);

  while (ch != 27) {  // ESC key exits
    for (i = 0; i <= 2; i++) {
      for (j = 199; j >= 0; j--) {
        // Erase this pixel by drawing in background (0 = BLACK)
        putpixel((int)x[i][j], (int)(yc + y[i][j]), 0);
        putpixel((int)x[i][j], (int)(yc - y[i][j]), 0);

        if (j < 199) {
          x[i][j + 1] = x[i][j] + vx[i][j];
          y[i][j + 1] = y[i][j] + vy[i][j];
          vx[i][j + 1] = vx[i][j];
          vy[i][j + 1] = vy[i][j];
          // Draw new pixel in particle's color
          putpixel((int)x[i][j + 1], (int)(yc + y[i][j + 1]), colors[i]);
          putpixel((int)x[i][j + 1], (int)(yc - y[i][j + 1]), colors[i]);
        }
      }

      // Reset the "head" of the trail at current source x position:
      x[i][0] = xc;
      // Compute the starting velocity for this branch.
      // For i = 0, 1, 2, the local velocity components (vxo, vyo) are:
      //  i=0: left branch => vxo = -0.707, vyo = 1-0.293 = 0.707
      //  i=1: vertical   => vxo =  0.000, vyo = 1
      //  i=2: right branch => vxo =  0.707, vyo = 0.707
      vxo = (i - 1) * 0.707f;
      vyo = 1 - fabs(i - 1) * 0.293f;
      // Apply the relativistic velocity addition (lab frame transformation)
      vx[i][0] = (vxo + vc) / (1 + vc * vxo);
      vy[i][0] = vyo * sqrt(1 - vc * vc) / (1 + vc * vxo);
    }

    // Draw a horizontal center line
    line(0, (int)yc, (int)maxx, (int)yc);
    // Update the source's x position using vc.
    xc += vc;
    if (xc >= maxx) xc = 1;
    if (xc <= 0) xc = maxx - 1;

    // Display instructions in the graphics window:
    outtextxy(10, getmaxy() - 30, (char*)"Use K/M to change v/c, s to pause, ESC to exit");

    // Also, print v/c to the terminal (using carriage return to overwrite previous value).
    cout << "\rv/c = " << vc << "   ";

    // Draw velocity arrows for each particle from the new emission point.
    // The arrow is a short line representing the velocity vector.
    int arrowScale = 30;  // scale factor to make arrow visible
    for (i = 0; i <= 2; i++) {
      setcolor(colors[i]);
      line((int)x[i][0], (int)yc,
           (int)(x[i][0] + vx[i][0] * arrowScale),
           (int)(yc + vy[i][0] * arrowScale));
    }
    // Restore color to white for text and other drawing.
    setcolor(15);

    // Wait for a key press; getch() is provided by SDL_bgi
    ch = getch();
    if (ch == 'K' && vc >= -0.95f) vc -= 0.05f;
    if (ch == 'M' && vc <= 0.95f) vc += 0.05f;
    if (ch == 's') getch();

    delay(5);          // 10 ms pause
    cleardevice();      // Clear the frame for next iteration
  }

  closegraph();
  return 0;
}

