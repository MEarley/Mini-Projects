#include <iostream>
#include <vector>
#include <queue>
#include <set>
#include <list>
#include <math.h>
#include "raylib.h"

using namespace std;

const int SCALE = 10;
const int SCREENWIDTH = 800;
const int SCREENHEIGHT = 800;//450;
const int xOFFSET = 0; //SCREENWIDTH / 2;
const int yOFFSET = 0; //SCREENHEIGHT / 2;
const Color BACKGROUND_COLOR = RAYWHITE;

class Cell{
    private:
    public:
        int x=0, y=0;
        bool isAlive = false;
        bool willBeAlive = false;
        Color color = BLACK;
        
        Cell(){};
        ~Cell(){};   
        Cell(int x_, int y_, bool isAlive_){
            x = x_;
            y = y_;
            isAlive = isAlive_;
        }
        void randomColor(int seed){
            srand(seed);
            color = {(unsigned char)(rand() % 256),(unsigned char)(rand() % 256),(unsigned char)(rand() % 256),255};
        } 

        void checkNeighbors(vector<vector<Cell>> &matrix){
            int livingCellsCount = 0;
            for(int w=x-1;w<=x+1;w++){
                for(int h=y-1;h<=y+1;h++){
                    // Neighboring Cells in all 8 directions
                    if(((int)matrix.size()) > w && w >= 0 && ((int)matrix[w].size()) > h && h >= 0 && (!(w == x && h == y))){
                        if (matrix[w][h].isAlive){
                            livingCellsCount++;
                        }
                        else if(isAlive){ 
                            // If the cell is not alive and this cell is, tell the dead cell to check its neighbors
                            matrix[w][h].checkNeighbors(matrix);
                        }  
                    }
                }
            }

            // If this cell is alive, it will die if there are less than 2 living cells 
            //      or more than 3 living cells nearby
            if(isAlive){
                willBeAlive = !(livingCellsCount < 2 || livingCellsCount > 3);
            }
            else{
                // If this cell is dead, it will come to life if there are exactly 3 living cells nearby
                willBeAlive = livingCellsCount == 3;
            }
        }   
};


int main () {
    std::cout << "Debugging Output" << endl << endl;

    // Used for optimizing
    //clock_t time_start, time_end;

    // Initialize a matrix scaled by the amount of pixels desired on screen
    
    vector<vector<Cell>> matrix(SCREENWIDTH / SCALE,vector<Cell>(SCREENHEIGHT / SCALE));
    for(int w=0,count=0;w<(int)matrix.size();w++){
            for(int h=0;h<(int)matrix[w].size();h++){
                matrix[w][h].x = w;
                matrix[w][h].y = h;
                //matrix[w][h].randomColor(++count);
                //std::cout<<w<<" "<<h<<endl;
                if(matrix[w][h].x < 0 || matrix[w][h].y < 0){
                    std::cout<<"break"<<endl;
                    break;
                }
            }
    }
    cout<<matrix.size()<<" by "<<matrix[0].size()<<endl;


    std::cout <<endl<<endl<< "End of Debugging Output" << endl << endl;
    
    InitWindow(SCREENWIDTH, SCREENHEIGHT, "The Game of Life");
	SetTargetFPS(60);               // Set game to run at 60 frames-per-second
    set<Cell*> livingCells;
    matrix[10][10].color = GREEN;
    livingCells.insert(&matrix[10][10]);
    matrix[10][10].checkNeighbors(matrix);
	//--------------------------------------------------------------------------------------

	// Main game loop
	while (!WindowShouldClose())    // Detect window close button or ESC key
	{
	    // Update variables
	    //----------------------------------------------------------------------------------
		

        
        

	    //----------------------------------------------------------------------------------

	    // Draw
	    //----------------------------------------------------------------------------------
	    BeginDrawing();
	    ClearBackground(BACKGROUND_COLOR);
        for(int w=0;w<(int)matrix.size();w++){
            for(int h=0;h<(int)matrix[w].size();h++){
                DrawRectangle(matrix[w][h].x * SCALE,matrix[w][h].y * SCALE,SCALE,SCALE,matrix[w][h].color);
            }
        }
        
		EndDrawing();
	    //----------------------------------------------------------------------------------
	}

	// De-Initialization
	//--------------------------------------------------------------------------------------
	CloseWindow();        // Close window and OpenGL context
	//--------------------------------------------------------------------------------------
    
    return 0;
}