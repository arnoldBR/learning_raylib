#include "raylib.h"
#include <stdio.h>

#define N_TILES_X 10
#define N_TILES_Y 10
#define TILE_WIDTH 70
#define TILE_HEIGTH 70

/*
    tema do workshop:
    "as coisas nao sao como parecem"
*/

typedef struct Player {
    Vector2 texturePos;
    Vector2 pos;
    Vector2 center;
    Rectangle bounds;
} Player;

typedef struct Block {
    Vector2 pos;
    Vector2 size;
    Rectangle bounds;
    bool coll;
    bool active;
} Block;

typedef struct {
    bool right;
    bool left;
    bool up;
    bool down;
} IsColliding;



void createObstacle(Rectangle, Rectangle rec2, bool coll[]);
Vector2 pointToIndex(Vector2 point);
IsColliding hasIt(Rectangle imob, Rectangle mob);

int main(void)
{
    const int screenWidth = N_TILES_X * TILE_WIDTH;
    const int screenHeight = N_TILES_Y * TILE_HEIGTH;
    InitWindow(screenWidth, screenHeight, "UFRGS");
    
    Block blocks[N_TILES_X][N_TILES_Y] = {0};
    blocks[1][1].active = 1;
    blocks[2][8].active = 1;
    blocks[4][4].active = 1;
    blocks[3][4].active = 1;
    blocks[5][4].active = 1;

    IsColliding collided;
//    blocks[N_TILES_X-1][N_TILES_Y-1].active = 1;

    //inicializacao da posicao do player
    Player hero;
    hero.pos = (Vector2){screenWidth*.5,screenHeight*.5};
    hero.bounds = (Rectangle){hero.pos.x, hero.pos.y, 32, 32};
    hero.texturePos = (Vector2){hero.pos.x, hero.pos.y-16};
    Vector2 playerIndex;

    Texture2D player_texture = LoadTexture("assets/player_1.png");

    SetTargetFPS(60);

    //bool coll[4]; //colisoes nos 4 lados de um retangulo
    int i, j;

    for(i=0; i < N_TILES_X; i++){
        for(j=0; j < N_TILES_Y; j++){
            blocks[i][j].pos.x = 0+TILE_WIDTH*i;
            blocks[i][j].pos.y = 0+TILE_HEIGTH*j;
            blocks[i][j].bounds = (Rectangle){blocks[i][j].pos.x, blocks[i][j].pos.y, TILE_WIDTH, TILE_HEIGTH};
        }
    }

    while (!WindowShouldClose()){

        playerIndex = pointToIndex(hero.center);
        char teste[30] = "boom";
        sprintf(teste, "player index: %d,%d", (int) playerIndex.x, (int) playerIndex.y);

        int collRight, collLeft, collDown, collUp = 0;
        for(i=0; i < N_TILES_X; i++){
            for(j=0; j < N_TILES_Y; j++){
                if(blocks[i][j].active){
                    //createObstacle(blocks[i][j].bounds, hero.bounds, coll);
                    collided = hasIt(blocks[i][j].bounds, hero.bounds);
                    // essas, se houver colisao, vao incrementar o lado correspondente, serao diferente de zero
                    collRight += collided.right;
                    collLeft += collided.left;
                    collUp += collided.up;
                    collDown += collided.down; 
                }
            }
        }

        // permite o movimento somente se o contador de colisao nao esta ativo
        if (IsKeyDown(KEY_RIGHT) && !collRight)//CheckCollisionRecs(hero.bounds, (Rectangle)))// !blocks[(int)playerIndex.x+1][(int)playerIndex.y].active) //coll_left
            hero.pos.x += 2;
        if (IsKeyDown(KEY_LEFT) && !collLeft) //coll_right
            hero.pos.x -= 2;
        if (IsKeyDown(KEY_DOWN) && !collDown) //coll_up
            hero.pos.y += 2;
        if (IsKeyDown(KEY_UP) && !collUp) // coll_down
            hero.pos.y -= 2;

        collRight =0; collLeft=0; collUp=0; collDown=0;

        hero.pos = (Vector2){hero.pos.x, hero.pos.y};
        hero.center = (Vector2){hero.pos.x, hero.pos.y-16};
        hero.bounds = (Rectangle){hero.pos.x, hero.pos.y, 32, 32};
        hero.texturePos = (Vector2){hero.pos.x, hero.pos.y-12};

         /*----------------------------------------------------*/
        /* DRAW                                               */
        BeginDrawing();

        ClearBackground(RAYWHITE);
        for (i=0; i < N_TILES_X; i++){
            for (j=0; j < N_TILES_Y; j++){
                DrawRectangle(blocks[i][j].pos.x, blocks[i][j].pos.y, TILE_WIDTH-1, TILE_HEIGTH-1, YELLOW);
                if(blocks[i][j].active){
                    DrawRectangle(blocks[i][j].pos.x, blocks[i][j].pos.y, TILE_WIDTH, TILE_HEIGTH, RED);
                }
                else if(!blocks[i][j].active){
                    DrawRectangle(blocks[i][j].pos.x, blocks[i][j].pos.y, TILE_WIDTH, TILE_HEIGTH, YELLOW);
                }
                DrawRectangleLines(blocks[i][j].pos.x, blocks[i][j].pos.y,TILE_WIDTH,TILE_HEIGTH,BLACK);
            }
        }

        DrawRectangleRec(hero.bounds, RED);
        DrawTexture(player_texture, hero.texturePos.x, hero.texturePos.y-16, WHITE);
        
        DrawText(teste, screenWidth*.2, 0, 32, BLUE);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}

/*
void createObstacle(Rectangle imob, Rectangle mob, bool coll[])
{
    coll[0] = CheckCollisionRecs((Rectangle){imob.x-1, imob.y, imob.width, imob.height}, mob); //left
    coll[1] = CheckCollisionRecs((Rectangle){imob.x+1, imob.y, imob.width, imob.height}, mob); //right
    coll[2] = CheckCollisionRecs((Rectangle){imob.x, imob.y-1, imob.width, imob.height}, mob); //up
    coll[3] = CheckCollisionRecs((Rectangle){imob.x, imob.y+1, imob.width, imob.height}, mob); //down
}*/

Vector2 pointToIndex(Vector2 point)
{
    return (Vector2){point.x/TILE_WIDTH, point.y/TILE_HEIGTH};
}

IsColliding hasIt(Rectangle imob, Rectangle mob)
{
    IsColliding isIt;   
    isIt.right = CheckCollisionRecs((Rectangle){imob.x-1, imob.y, imob.width, imob.height}, mob); //left
    isIt.left = CheckCollisionRecs((Rectangle){imob.x+1, imob.y, imob.width, imob.height}, mob); //right
    isIt.down = CheckCollisionRecs((Rectangle){imob.x, imob.y-1, imob.width, imob.height}, mob); //up
    isIt.up = CheckCollisionRecs((Rectangle){imob.x, imob.y+1, imob.width, imob.height}, mob); //down
    return isIt; 
}