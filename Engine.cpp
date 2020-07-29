#include "framework.h"
#include "Settings.h"
#include "EngineBase.h"
#include "Engine.h"
#include "Level.h"

double Engine::offset = 0;

Engine::Engine()
{
    // Initilize level
    level = new Level();
    AddGameObject(level);

    // Initialize character
    character = new Character();
    AddGameObject(character);

    leftPressed = false;
    rightPressed = false;
    spacePressed = false;
}

Engine::~Engine()
{
}

void Engine::KeyUp(WPARAM wParam)
{
    // sets and resets key presses
    if (wParam == VK_LEFT)
        leftPressed = false;
    if (wParam == VK_RIGHT)
        rightPressed = false;
    if (wParam == VK_SPACE)
        spacePressed = false;
}

void Engine::KeyDown(WPARAM wParam)
{
    // sets and resets key presses
    if (wParam == VK_LEFT)
        leftPressed = true;
    if (wParam == VK_RIGHT)
        rightPressed = true;
    if (wParam == VK_SPACE)
        spacePressed = true;
}

void Engine::Logic(double elapsedTime)
{
    EngineBase::Logic(elapsedTime);

    // This is the logic part of the engine. Add your game logic here.
    // It runs every frame and it receives the elapsed time between the last frame and this one, in seconds.
    // Use this value for a smooth and consistent movement, regardless of the CPU or graphics speed

    // key inputs are sent to character
    character->GoesLeft = leftPressed;
    character->GoesRight = rightPressed;

    // If we collide with the tiles of the level
    CollisionDistances cd = level->CharacterCollides(character);
    if (cd.bottom > 0)
    {
        // if it's a bottom collision, we stop the character from falling
        character->StopFalling(cd.bottom);
        if (spacePressed)
        { // if the user pressed the jump key, the character jumps
            character->Jump(true);
            spacePressed = false;
        }
    }
    if (cd.left > 0)
    {
        // if it's a left collision
        character->StopMovingLeft(cd.left);
    }
    if (cd.right > 0)
    {
        // if it's a right collision
        character->StopMovingRight(cd.right);
    }
    if (cd.top > 0)
    {
        // if it's a top collision
        character->BounceTop();
    }

    // Change the display offset based on character position, but clamp it to the limits
    Engine::offset = character->GetPosition().x - RESOLUTION_X / 2;
    if (Engine::offset < 0)
        Engine::offset = 0;
    if (Engine::offset > LEVEL_WIDTH* TILE_WIDTH - RESOLUTION_X)
        Engine::offset = LEVEL_WIDTH * TILE_WIDTH - RESOLUTION_X;
}