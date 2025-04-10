/***************************************************************************
 * Charge!
 * 
 * Adapted from Robert H. Good's "Classical Electromagnetism" (1999)
 *
 * This program uses a parameterized form of the integrated dipole field
 * equation to compute sin²(theta) as a function of radius (kr), phase (wt), and an 
 * integration constant (C), for electric field lines (using "quasi-photons"
 * emitted from a moving charge). 
 *
 * Each C val traces a distinct field line - as phase advances (representing
 * time), field pattern evolves frame-by-frame.
 *
 * Branch mapping:
 *  i = 0: left branch  -> vxo = -INITIAL_VX_SCALE, vyo = 1 - INITIAL_VYO_SCALE
 *  i = 1: center branch-> vxo =  0,                vyo = 1
 *  i = 2: right branch -> vxo = +INITIAL_VX_SCALE, vyo = 1 - INITIAL_VYO_SCALE
 *
 * Notes:
 *  - For each branch, the trail is updated (calculated from the end of the tail towards the head).
 *  - Right now, the velocity arrow is a bit broken not gonna lie.
 *  - Source's horizontal position is updated via the vc param.
 *  - Initial source position is placed at 1/3 window width, vertically centered.
 *  - Hoz. center line added for reference
 *
 * Controls: 
 * - Press 's' or any key to step forward in phase frame (wt+=π/64).
 * - Press 'ESC' to exit.
 **************************************************************************/

#include <iostream>
#include <graphics.h>
#include <cmath>
#include <cstdlib>
#include <cstdio>
#include <unistd.h>

// --- Tunable Parameters ---
const int NUM_BRANCHES = 3;   // number of branches (trails)
const int TRAIL_LENGTH = 200; // trail length

// Initial velocity parameters for each branch.
// For branch 0 (left): vxo = -0.707, for branch 1 (center): 0, branch 2 (right): +0.707.
const float INITIAL_VX_SCALE = 0.707f;
// The corresponding vyo for left/right branches are computed as: vyo = 1 - 0.293 = 0.707,
// while the center branch uses vyo = 1.
const float INITIAL_VYO_SCALE = 0.293f;

// Colors for branches (BGI color codes):
const int COLOR_LEFT   = 4;   // RED
const int COLOR_CENTER = 15;  // WHITE
const int COLOR_RIGHT  = 1;   // BLUE
int colors[NUM_BRANCHES] = { COLOR_LEFT, COLOR_CENTER, COLOR_RIGHT };
const int ARROW_SCALE = 30;   // scale factor for drawing velocity arrow
// --------------------------

int main() {
  int gdriver = DETECT, gmode;
  initgraph(&gdriver, &gmode, nullptr);
  
  int maxx = getmaxx();
  int maxy = getmaxy();
  float xc = maxx / 3.0f;
  float yc = maxy / 2.0f;

  // Simulation parameters:
  float vc = 0.0f;  // v/c (charge's speed as a fraction of light speed)
  float vxo, vyo;   // local (branch-specific) velocity components

  // Arrays to hold trail positions and velocities.
  float x[NUM_BRANCHES][TRAIL_LENGTH] = {0};
  float y[NUM_BRANCHES][TRAIL_LENGTH] = {0};
  float vx[NUM_BRANCHES][TRAIL_LENGTH] = {0};
  float vy[NUM_BRANCHES][TRAIL_LENGTH] = {0};

  std::cout.precision(2);
  std::cout.setf(std::ios::showpoint);
  std::cout.setf(std::ios::fixed);
  std::cout.setf(std::ios::showpos);
      // draw center line!:
    line(0, (int)yc, maxx, (int)yc);
  char ch = 'd'; 
  while (ch != 27) {
    for (int i = 0; i < NUM_BRANCHES; i++) {
      for (int j = TRAIL_LENGTH - 1; j >= 0; j--) {
         putpixel((int)x[i][j], (int)(yc + y[i][j]), 0);
        putpixel((int)x[i][j], (int)(yc - y[i][j]), 0);

        if (j < TRAIL_LENGTH - 1) {
          x[i][j + 1] = x[i][j] + vx[i][j];
          y[i][j + 1] = y[i][j] + vy[i][j];
          vx[i][j + 1] = vx[i][j];
          vy[i][j + 1] = vy[i][j];
          putpixel((int)x[i][j + 1], (int)(yc + y[i][j + 1]), colors[i]);
          putpixel((int)x[i][j + 1], (int)(yc - y[i][j + 1]), colors[i]);
        }
      }
      x[i][0] = xc;

      // Compute starting local velocity components for branch i.
      vxo = (i - 1) * INITIAL_VX_SCALE;
      vyo = 1 - std::fabs(i - 1) * INITIAL_VYO_SCALE;

      vx[i][0] = (vxo + vc) / (1 + vc * vxo);
      vy[i][0] = vyo * std::sqrt(1 - vc * vc) / (1 + vc * vxo);
    }
    
    // draw center line!:
    line(0, (int)yc, maxx, (int)yc);

    xc += vc;
    if (xc >= maxx) xc = 1;
    if (xc <= 0) xc = maxx - 1;

    outtextxy(10, maxy - 30, (char*)"Use K/M to change v/c, s to pause, ESC to exit");

    std::cout << "\rv/c = " << vc << "   ";

    // Draw velocity arrow for each branch (short line showing velocity vector).
    for (int i = 0; i < NUM_BRANCHES; i++) {
      setcolor(colors[i]);
      line((int)x[i][0], (int)yc,
           (int)(x[i][0] + vx[i][0] * ARROW_SCALE),
           (int)(yc + vy[i][0] * ARROW_SCALE));
    }
    setcolor(COLOR_CENTER);

    ch = getch();
    if (ch == 'K' && vc >= -0.95f) vc -= 0.05f;
    if (ch == 'M' && vc <= 0.95f) vc += 0.05f;
    if (ch == 's') getch();

    delay(2);          // Small pause (2 ms) between frames.
    cleardevice();     // Clear the frame for the next iteration.
  }

  closegraph();
  return 0;
}
