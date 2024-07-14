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
const Color BACKGROUND_COLOR = BLACK;
const Color TEXT_COLOR = RAYWHITE;

class Cell{
    private:
    public:
        int x=0, y=0;
        bool isAlive = false;
        bool willBeAlive = false;
        bool hasChecked = false;    
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
        
        void checkNeighbors(vector<vector<Cell>> &matrix,set<Cell*> &nextLiveCells){
            int livingCellsCount = 0;
            for(int w=x-1;w<=x+1;w++){
                for(int h=y-1;h<=y+1;h++){
                    // Neighboring Cells in all 8 directions
                    if(((int)matrix.size()) > w && w >= 0 && ((int)matrix[w].size()) > h && h >= 0 && (!(w == x && h == y))){
                        if (matrix[w][h].isAlive){
                            livingCellsCount++;
                        }
                        else if(isAlive && !matrix[w][h].hasChecked){ 
                            // If the cell is not alive and this cell is, tell the dead cell to check its neighbors
                            matrix[w][h].hasChecked = true;
                            matrix[w][h].checkNeighbors(matrix,nextLiveCells);
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

            // If this cell will be alive, add it to the set of living cells to be updated
            //if(willBeAlive)
            nextLiveCells.insert(this);

            return;
        }   

        bool update(){
            isAlive = willBeAlive;
            hasChecked = false;
            if(isAlive)
                color = WHITE;
            else
                color = BLACK;
            return isAlive;
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
    matrix[10][10].color = WHITE;
    matrix[10][11].color = WHITE;
    matrix[10][12].color = WHITE;
    matrix[10][10].isAlive = true;
    matrix[10][11].isAlive = true;
    matrix[10][12].isAlive = true;
    livingCells.insert(&matrix[10][10]);
    livingCells.insert(&matrix[10][11]);
    livingCells.insert(&matrix[10][12]);
    //matrix[10][10].checkNeighbors(matrix,livingCells);
	//--------------------------------------------------------------------------------------
    int clk = 0;
    bool paused = true;
	// Main game loop
	while (!WindowShouldClose())    // Detect window close button or ESC key
	{
	    // Update variables
	    //----------------------------------------------------------------------------------
        int mousePositionX = (GetMouseX() - xOFFSET) / SCALE;
        int mousePositionY = (GetMouseY() - yOFFSET) / SCALE;
        mousePositionX %= (int)matrix.size();
        mousePositionY %= (int)matrix[0].size();

        paused = IsKeyDown(KEY_SPACE);

        if((clk % 1000 == 0) && !paused){
            set<Cell*> updateCells;
            // All living cells should check their neighbors
            for(auto cell : livingCells){
                cell->checkNeighbors(matrix,updateCells);
            }

            // Update all cells that changed during the last cycle
            livingCells.clear();
            for(auto cell : updateCells){
                if(cell->update())
                    livingCells.insert(cell);
            }
        }   
            

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

        if(paused){
            DrawText("Paused\n     | |",SCREENWIDTH/2,0,10,TEXT_COLOR);

            if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
                //cout<<"Changed Cell"<<endl;
                Cell *cell = &matrix[mousePositionX][mousePositionY];
                if(cell->isAlive){
                    cell->willBeAlive = false;
                    livingCells.erase(cell);
                }
                else{
                    cell->willBeAlive = true;
                    livingCells.insert(cell);
                }
                cell->update();
            }
        }
        
        // Write out mouse coordinates
        string mousePosition = "x: " + to_string(mousePositionX) + " y: " + to_string(mousePositionY);
        DrawText(mousePosition.c_str(),0,0,10,TEXT_COLOR);

        EndDrawing();
        WaitTime(0.01);
        clk += 50;
	    //----------------------------------------------------------------------------------
	}

	// De-Initialization
	//--------------------------------------------------------------------------------------
	CloseWindow();        // Close window and OpenGL context
	//--------------------------------------------------------------------------------------
    
    return 0;
}