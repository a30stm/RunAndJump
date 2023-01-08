#include "raylib.h"

struct AnimData
{
    Rectangle rec;
    Vector2 pos;
    int frame;
    float updateTime;
    float runningTime;
};

bool isOnGround(AnimData data, int windowHeight)
{
    return data.pos.y >= windowHeight - data.rec.height;
}

AnimData updateAnimData(AnimData data, float deltaTime, int maxFrame)
{
    // Update Running Time
    data.runningTime += deltaTime;
    if(data.runningTime >= data.updateTime)
    {
        data.runningTime = 0.0;
        // update animation frame
        data.rec.x = data.frame * data.rec.width;
        data.frame++;
        if(data.frame > maxFrame)
        {
            data.frame = 0;
        }
    }
    return data;
}

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

const int sizeOfNebulae{25};
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

float finishLine{nebulae[sizeOfNebulae - 1].pos.x + 500};

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

//----------------Background----------------------------
Texture2D background = LoadTexture("textures/far-buildings.png");
float bgX{};
float bgScale{3.4};

//----------------Middle----------------------------
Texture2D middleGround = LoadTexture("textures/back-buildings.png");
float mgX{};
float mgScale{3.4};

//----------------foreground----------------------------
Texture2D foreground = LoadTexture("textures/foreground.png");
float fgX{};
float fgScale{3.4};


bool collision{};
SetTargetFPS(60);
    while(!WindowShouldClose())
    {
        // delta time (time since last frame)
        const float dT{GetFrameTime()};
        //Start Drawing
        BeginDrawing();
        ClearBackground(WHITE);

//---------------Background animation and pos check--------------
        bgX -= 20 * dT;
        if (bgX <= -background.width*bgScale)
        {
            bgX = 0.0;
        }

        //Draw Background
        Vector2 bg1Pos{bgX, 0.0};
        DrawTextureEx(background, bg1Pos, 0.0, bgScale, WHITE);
        Vector2 bg2Pos{bgX + background.width*bgScale, 0.0};
        DrawTextureEx(background, bg2Pos, 0.0, bgScale, WHITE);

//---------------Middle animation and pos check--------------
        mgX -= 30 * dT;
        if (mgX <= -middleGround.width*mgScale)
        {
            mgX = 0.0;
        }

        //Draw middle
        Vector2 mg1Pos{mgX, 0.0};
        DrawTextureEx(middleGround, mg1Pos, 0.0, mgScale, WHITE);
        Vector2 mg2Pos{mgX + middleGround.width*mgScale, 0.0};
        DrawTextureEx(middleGround, mg2Pos, 0.0, mgScale, WHITE);

//---------------Foreground animation and pos check--------------
        fgX -= 40 * dT;
        if (fgX <= -foreground.width*fgScale)
        {
            fgX = 0.0;
        }

        //Draw Foreground
        Vector2 fg1Pos{fgX, 0.0};
        DrawTextureEx(foreground, fg1Pos, 0.0, fgScale, WHITE);
        Vector2 fg2Pos{fgX + foreground.width*fgScale, 0.0};
        DrawTextureEx(foreground, fg2Pos, 0.0, fgScale, WHITE);

        //Ground Check
        if(isOnGround(scarfyData, windowDimensions[1]))
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

        // Update finish line
        finishLine += nebVel * dT;

        // Update Scarfy Position
        scarfyData.pos.y += velocity * dT;

        if(!isInAir)
        {
          scarfyData = updateAnimData(scarfyData, dT, 5);  
        }

        for(int i = 0; i < sizeOfNebulae; i++)
        {
            nebulae[i] = updateAnimData(nebulae[i], dT, 7);
        }

        for(AnimData nebula : nebulae)
        {   
            float pad{50};
            Rectangle nebRec{
                nebula.pos.x + pad,
                nebula.pos.y + pad,
                nebula.rec.width - 2*pad,
                nebula.rec.height - 2*pad
            };
            Rectangle scarfyRec{
                scarfyData.pos.x,
                scarfyData.pos.y,
                scarfyData.rec.width,
                scarfyData.rec.height
            };
            if (CheckCollisionRecs(nebRec, scarfyRec))
            {
                collision = true;
            }
        }

        if(collision)
        {
            // game over
            DrawText("Game Over, Dummy!", windowDimensions[0]/4, windowDimensions[1]/2, 50, RED);
        }
        else if (scarfyData.pos.x >= finishLine)
        {
            // Win
            DrawText("Way To Go, Dummy!", windowDimensions[0]/4, windowDimensions[1]/2, 50, WHITE);
        }
        else
        {
            for (int i = 0; i < sizeOfNebulae; i ++)
            {   
            // Draw nebula
            DrawTextureRec(nebula, nebulae[i].rec, nebulae[i].pos, WHITE);
            }

            //Draw Scarfy
            DrawTextureRec(scarfy, scarfyData.rec, scarfyData.pos, WHITE);
        }

        

        //Stop Drawing
        EndDrawing();
    }
    UnloadTexture(scarfy);
    UnloadTexture(nebula);
    UnloadTexture(background);
    UnloadTexture(middleGround);
    UnloadTexture(foreground);
    CloseWindow();
}