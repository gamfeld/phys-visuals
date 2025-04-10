/***************************************************************************
 * Relaxation!
 *
 * Adapted from Robert H. Good's "Classical Electromagnetism" (1999)
 * 
 * Graphs potential distribution on a 2D grid via iteratively averaging values
 * with neighboring points (via Laplace's equation). Potential spreads from
 * left bound --> interior!
 *
 * Grid specifications:
 *  - The array V is 22x22: 
 *      - Columns: 0 to 21. Column 0 (left boundary) is fixed at 100 volts;
 *          column 21 (right boundary) is fixed at 0 volts.
 *      - Rows: 0 to Y+1, where Y is the number of interior rows (user input, < 21).
 *          The top boundary (row 0) and bottom boundary (row Y+1) are fixed at 0 volts.
 *  - Only the interior points (columns 1..20, rows 1..Y) are updated according to:
 *        V[x][y] = ( V[x-1][y] + V[x+1][y] + V[x][y-1] + V[x][y+1] ) / 4.0
 *
 *  Notes: 
 *  - Increasing grid height (Y) ^ the resolution at the cost of speed
 *  (likewise with boundary values*)
 *  - Only interior points are updated to ensure valid neighbor indices.
 *  - Could be tuned by applying over-relaxation factors if you'd like to mess
 *  with convergence rates
 *
 * Controls:
 * - Press any key to perform one iteration of the relaxation process.
 * - Press 'ESC' to exit.
 **************************************************************************/

#include <iostream>
#include <graphics.h>
#include <cstdlib>
#include <cstdio>

// --- Tunable Parameters --- 
// Graphics constants (adjust these to control spacing)!
const int CELL_W = 35;    // width of each cell
const int CELL_H = 15;    // height of each cell
const int X_OFF  = 30;    // left margin offset (in pixels)
const int Y_OFF  = 30;    // top margin offset (in pixels)
// --------------------------

int main() {
  int gdriver = DETECT, gmode;
  int x, y, n = 0, Y;
  char ch = ' ';
  float V[22][22] = {0};

  std::cout << "Enter grid height Y (number of interior rows, integer < 21): "; std::cin >> Y;
  if (Y <= 0 || Y > 20) {
    Y = 10;
  }
  
  // Set boundary conditions:
  // Left boundary (column 0) is fixed at 100 volts (for interior rows 1..Y)
  for (y = 0; y < 22; y++) {
    if (y >= 1 && y <= Y)
      V[0][y] = 100.0f;
    else
      V[0][y] = 0.0f;
    V[21][y] = 0.0f;  // Right boundary always 0
  }
  for (x = 0; x < 22; x++) {
    V[x][0] = 0.0f;         // Top boundary
    V[x][Y+1] = 0.0f;       // Bottom boundary
  }

  // Initialize graphics window
  initgraph(&gdriver, &gmode, nullptr);

  // Main relaxation loop (exit when ESC is pressed)
  while (ch != 27) {
    cleardevice(); 
    char buffer[70];
    for (y = 0; y < Y+2; y++) {
      for (x = 0; x < 22; x++) {
        sprintf(buffer, "%.0f", V[x][y]);
        outtextxy(X_OFF + x * CELL_W, Y_OFF + y * CELL_H, buffer);
      }
    }
    rectangle(X_OFF - 5, Y_OFF - 5, X_OFF + 22 * CELL_W + 5, Y_OFF + (Y+2) * CELL_H + 5);

    sprintf(buffer, "n = %d. Press any key (ESC to exit)", n);
    outtextxy(X_OFF, Y_OFF + (Y+2) * CELL_H + 10, buffer);
    ch = getch();
    n++;

    // Compute one relaxation iteration over interior points only:
    float newV[22][22] = {0};
    for (y = 1; y <= Y; y++) {
      for (x = 1; x <= 20; x++) {
        newV[x][y] = ( V[x-1][y] + V[x+1][y] + V[x][y-1] + V[x][y+1] ) / 4.0f;
      }
    }
    for (y = 1; y <= Y; y++) {
      for (x = 1; x <= 20; x++) {
        V[x][y] = newV[x][y];
      }
    }
  }
  closegraph();
  return 0;
}

