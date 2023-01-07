#include "raylib.h"

struct AnimData
{
    Rectangle rec;
    Vector2 pos;
    int frame;
    float updateTime;
    float runningTime;
};


int main()
{

//-----------------Window Dimensions---------------------
int windowDimensions[2];
windowDimensions[0] = 850;
windowDimensions[1] = 600;


//-----------------Initialize Window---------------------
InitWindow(windowDimensions[0], windowDimensions[1], "Run And Jump");

// Acceleration due to gravity in (pixels/s)/s
const int gravity{1000};

//-----------------nebula variables---------------------
Texture2D nebula = LoadTexture("textures/12_nebula_spritesheet.png");

    //Anim Data For Nebula
AnimData nebData{
    {0.0, 0.0, nebula.width/8, nebula.height/8},   // rectangle rec
    {windowDimensions[0], windowDimensions[1] - nebula.height/8}, // vector2 pos
    0,                                             // int frame
    1.0/12.0,                                      // float update time
    0.0                                            // float running time
    };
AnimData neb2Data{
    {0.0, 0.0, nebula.width/8, nebula.height/8},
    {windowDimensions[0] + 300, windowDimensions[1] - nebula.height/8},
    0,
    1.0/16.0,
    0.0
};

AnimData nebulae[2]{nebData, neb2Data};


    //nebula X velocity (pixels/second)
int nebVel{-450};

//-----------------scarfy variables---------------------
Texture2D scarfy = LoadTexture("textures/scarfy.png");
AnimData scarfyData;
scarfyData.rec.width = scarfy.width/6;
scarfyData.rec.height = scarfy.height;
scarfyData.rec.x = 0;
scarfyData.rec.y = 0;
scarfyData.pos.x = windowDimensions[0]/2 - windowDimensions[0]/3;
scarfyData.pos.y = windowDimensions[1] - scarfyData.rec.height;
scarfyData.frame = 0;
scarfyData.updateTime = 1.0/12.0;
scarfyData.runningTime = 0.0;




// Is Scarfy In The Air
bool isInAir{};

//Jump Velocity (pixels/second)
const int jumpVel{-600};


int velocity{-10};


SetTargetFPS(60);
    while(!WindowShouldClose())
    {
        // delta time (time since last frame)
        const float dT{GetFrameTime()};
        //Start Drawing
        BeginDrawing();
        ClearBackground(WHITE);

        //Ground Check
        if(scarfyData.pos.y >= windowDimensions[1] - scarfyData.rec.height)
        {
            // Sprite is on the ground
            velocity = 0;
            isInAir = false;
        }
        else
        {
            // Sprite is in the air
            velocity += gravity * dT;
            isInAir = true;
        }

        if(IsKeyPressed(KEY_SPACE) && !isInAir)
        {
            velocity += jumpVel;
        }

        //Update Nebula Position
        nebData.pos.x += nebVel * dT;
        neb2Data.pos.x += nebVel * dT;

        // Update Scarfy Position
        scarfyData.pos.y += velocity * dT;

        //Update Nebula Animation Frame
        nebData.runningTime += dT;
        if(nebData.runningTime >= nebData.updateTime)
        {
            nebData.runningTime = 0.0;
                //Update Animation
            nebData.rec.x = nebData.frame * nebData.rec.width;
            nebData.frame++;
            if(nebData.frame > 7)
            {
                nebData.frame = 0;
            }
        }
        neb2Data.runningTime += dT;
        if(neb2Data.runningTime >= neb2Data.updateTime)
        {
            neb2Data.runningTime = 0.0;
                //Update Animation
            neb2Data.rec.x = neb2Data.frame * neb2Data.rec.width;
            neb2Data.frame++;
            if(neb2Data.frame > 7)
            {
                neb2Data.frame = 0;
            }
        }

        //Update Scarfy Animation Frame
        scarfyData.runningTime += dT;
        if(scarfyData.runningTime >= scarfyData.updateTime && !isInAir)
        {
            scarfyData.runningTime = 0.0;
                //Update Animation
            scarfyData.rec.x = scarfyData.frame * scarfyData.rec.width;
            scarfyData.frame++;
            if(scarfyData.frame > 5)
            {
                scarfyData.frame = 0;
            }
        }

        //Draw Nebula
        DrawTextureRec(nebula, nebData.rec, nebData.pos, WHITE);
        DrawTextureRec(nebula, neb2Data.rec, neb2Data.pos, BLUE);

        //Draw Scarfy
        DrawTextureRec(scarfy, scarfyData.rec, scarfyData.pos, WHITE);

        //Stop Drawing
        EndDrawing();
    }
    UnloadTexture(scarfy);
    UnloadTexture(nebula);
    CloseWindow();
}