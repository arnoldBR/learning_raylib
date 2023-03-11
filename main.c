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
    Rectangle bounds;
} Player;

typedef struct Block {
    Vector2 pos;
    Vector2 size;
    Rectangle bounds;
    bool coll;
    bool active;
} Block;

void createObstacle(Rectangle, Rectangle rec2, bool coll[]);

int main(void)
{
    const int screenWidth = N_TILES_X * TILE_WIDTH;
    const int screenHeight = N_TILES_Y * TILE_HEIGTH;
    InitWindow(screenWidth, screenHeight, "UFRGS");
    
    Block blocks[N_TILES_X][N_TILES_Y] = {0};
    blocks[1][1].active = 1;
    blocks[5][8].active = 1;
    blocks[7][3].active = 1;

    //inicializacao da posicao do player
    Player hero;
    hero.pos = (Vector2){screenWidth*.5,screenHeight*.5};
    hero.bounds = (Rectangle){hero.pos.x, hero.pos.y, 32, 32};
    hero.texturePos = (Vector2){hero.pos.x, hero.pos.y-16};

    Texture2D player_texture = LoadTexture("assets/player_1.png");

    SetTargetFPS(60);

    bool coll[4]; //colisoes nos 4 lados de um retangulo
    int i, j;

    while (!WindowShouldClose())
    {
        for(i=0; i < N_TILES_X; i++)
        {
            for(j=0; j < N_TILES_Y; j++)
            {
                if(blocks[i][j].active)
                {
                    blocks[i][j].pos.x = 0+TILE_WIDTH*i;
                    blocks[i][j].pos.y = 0+TILE_HEIGTH*j;
                    blocks[i][j].bounds = (Rectangle){blocks[i][j].pos.x, blocks[i][j].pos.y, TILE_WIDTH, TILE_HEIGTH};
                    createObstacle(blocks[i][j].bounds, hero.bounds, coll);
               }
            }
        }
        
        if (IsKeyDown(KEY_RIGHT) && !coll[0]) //coll_left
            hero.pos.x += 2;
        if (IsKeyDown(KEY_LEFT) && !coll[1]) //coll_right
            hero.pos.x -= 2;
        if (IsKeyDown(KEY_DOWN) && !coll[2]) //coll_up
            hero.pos.y += 2;
        if (IsKeyDown(KEY_UP) && !coll[3]) // coll_down
            hero.pos.y -= 2;

        hero.pos = (Vector2){hero.pos.x, hero.pos.y};
        hero.bounds = (Rectangle){hero.pos.x, hero.pos.y, 32, 32};
        hero.texturePos = (Vector2){hero.pos.x, hero.pos.y-12};

        BeginDrawing();

        for (i=0; i < N_TILES_X; i++){
            for (j=0; j < N_TILES_Y; j++){
                if(blocks[i][j].active){
                    DrawRectangle(blocks[i][j].pos.x, blocks[i][j].pos.y, TILE_WIDTH, TILE_HEIGTH, RED);
                    //createObstacle(blocks[i][j].bounds, hero.bounds, coll);
                }
                else if(!blocks[i][j].active){
                    DrawRectangle(blocks[i][j].pos.x, blocks[i][j].pos.y, TILE_WIDTH, TILE_HEIGTH, WHITE);
                }
            }
        }

        ClearBackground(RAYWHITE);
        DrawRectangleRec(hero.bounds, RED);
        DrawTexture(player_texture, hero.texturePos.x, hero.texturePos.y-16, WHITE);
        
        EndDrawing();
    }

    CloseWindow();
    return 0;
}

void createObstacle(Rectangle imob, Rectangle mob, bool coll[])
{
    coll[0] = CheckCollisionRecs((Rectangle){imob.x-1, imob.y, imob.width, imob.height}, mob); //left
    coll[1] = CheckCollisionRecs((Rectangle){imob.x+1, imob.y, imob.width, imob.height}, mob); //right
    coll[2] = CheckCollisionRecs((Rectangle){imob.x, imob.y-1, imob.width, imob.height}, mob); //up
    coll[3] = CheckCollisionRecs((Rectangle){imob.x, imob.y+1, imob.width, imob.height}, mob); //down
}