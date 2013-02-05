#include "GameManager.h"
#include "GameState.h"
#include "WorldManager.h"
#include <LibZerkish.h>

GameManager::GameManager(GameState* gameState)
{
  this->gameState = gameState;
}

void GameManager::Initialize()
{
  graphics = gameState->GetGraphics(); 

}

void GameManager::PauseGame()
{

}

void GameManager::StartGame()
{

}

void GameManager::ResumeGame()
{

}

void GameManager::Update(double delta)
{

}

void GameManager::Draw()
{

}