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

const int sizeOfNebulae{20};
AnimData nebulae[sizeOfNebulae]{};

for (int i = 0; i < sizeOfNebulae; i++)
{
    nebulae[i].rec.x = 0.0;
    nebulae[i].rec.y = 0.0;
    nebulae[i].rec.width = nebula.width/8;
    nebulae[i].rec.height = nebula.height/8;
    nebulae[i].pos.y = windowDimensions[1] - nebula.height/8;
    nebulae[i].frame = 0;
    nebulae[i].runningTime = 0.0;
    nebulae[i].updateTime = 0.0;
    nebulae[i].pos.x = windowDimensions[0] + i * 800;
}

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

        for (int i = 0; i < sizeOfNebulae; i++)
        {
            nebulae[i].pos.x += nebVel * dT;
        }

        // Update Scarfy Position
        scarfyData.pos.y += velocity * dT;

        for(int i = 0; i < sizeOfNebulae; i++)
        {
                nebulae[i].runningTime += dT;
            if(nebulae[i].runningTime >= nebulae[i].updateTime)
            {
                nebulae[i].runningTime = 0.0;
                    //Update Animation
                nebulae[i].rec.x = nebulae[i].frame * nebulae[i].rec.width;
                nebulae[i].frame++;
                if(nebulae[i].frame > 7)
                {
                    nebulae[i].frame = 0;
                }
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

        for (int i = 0; i < sizeOfNebulae; i ++)
        {
            DrawTextureRec(nebula, nebulae[i].rec, nebulae[i].pos, WHITE);
        }

        //Draw Scarfy
        DrawTextureRec(scarfy, scarfyData.rec, scarfyData.pos, WHITE);

        //Stop Drawing
        EndDrawing();
    }
    UnloadTexture(scarfy);
    UnloadTexture(nebula);
    CloseWindow();
}