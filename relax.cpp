//Relaxation!
// Adapted from Robert H. Good's "Classical Electromagnetism" (1999)
// Potential distribution on 2D grid via iteratively averaging values with neighboring points.

/*********************** TUNABLE PARAMETERS ******************************
 *
 * Implements discrete relaxation procedure — computing steady-state potential distribution
 * by iteratively solving Laplace's equation on a 2D grid.
 *
 * - The Grid dimension is determined via user input (an integer Y<21).
 * - Left boundary is maintained at fixed potential of 100 units, while
 *   remaining boundaries initially set to 0 (subject to change)
 * The interior grid points are updated using the finite-difference formula:
 *      V[i][j] = (V[i-1][j] + V[i+1][j] + V[i][j-1] + V[i][j+1]) / 4.0f;   
 *
 *  Notes: 
 *  - Grid Dimensions can be determined by user input...
 *  - Increasing grid height (Y) ^ the resolution at the cost of speed
 *  (likewise with boundary values*)
 *  - Only interior points are updated to ensure valid neighbor indices.
 *  - Could be tuned by applying over-relaxation factors if you'd like to mess
 *  with convergence rates
 *
 * Controls:
 * - Press any key to perform one iteration of the relaxation process.
 * - Press 'ESC' to exit.
 *
 **************************************************************************/

#include <iostream>
#include <graphics.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main(){
    int gdriver = DETECT, gmode;
    initgraph(&gdriver, &gmode, nullptr);
    
    int n = 0, vxy, Y;
    char ch = 'd', Ych[21];
    float V[22][22] = {0};
    
    outtextxy(10, 10, (char*)"Enter A number < 21:");
    std::cin.get(Ych, 21);
    Y = atoi(Ych);
    cleardevice();
    if (Y <= 0 || Y > 20) {
        Y = 10;
    }
    
    char buffer[80];
    sprintf(buffer, "Y = %d. Press Enter or Esc.", Y);
    outtextxy(10, Y + 4, buffer);
    
    for (int j = 1; j <= Y; j++){
        V[0][j] = 100; //*
        outtextxy(4, j + 1, (char*)"100");
    }

    for (int i = 1; i <= 20; i++){
        outtextxy(3 * i + 5, 1, (char*)"0");
        outtextxy(3 * i + 5, Y + 2, (char*)"0");
    }
    
    while (ch != (char)27) {  // ESC
        for (int i = 1; i < 20; i++){
            for (int j = 1; j < Y; j++){
                V[i][j] = (V[i - 1][j] + V[i + 1][j] + V[i][j - 1] + V[i][j + 1]) / 4.0f;
            }
        }

        vxy = (int)V[10][Y / 2];
        char buf1[20];
        sprintf(buf1, "%d", vxy);
        outtextxy(3 * 10 + 5, (Y / 2) + 1, buf1);
        
        moveto(520, 15);
        lineto(51, 15);
        lineto(51, 16 * Y + 15);
        lineto(520, 16 * Y + 15);
        
        char buf2[20];
        sprintf(buf2, "n = %d", n);
        outtextxy(10, Y + 5, buf2);
        
        ch = getch();
        n++;
    }
    
    closegraph();
    return 0;
}


