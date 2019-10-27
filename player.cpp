
#include "player.h"

#include "mastercontrol.h"
#include "inputmaster.h"

Player::Player(int playerId, Context* context): Object(context),
    playerId_{playerId},
    autoPilot_{false},
    alive_{true},
    score_{0},
    multiplier_{1}
{
}

void Player::Die()
{
    alive_ = false;
}
void Player::Respawn()
{
    ResetScore();
    multiplier_ = 1;
    alive_ = true;
}

void Player::SetScore(int points)
{
    score_ = points;
}
void Player::ResetScore()
{
    SetScore(0);
}

void Player::AddScore(int points)
{
    score_ += points;
}

Controllable* Player::GetControllable()
{
    Controllable* controllable{ GetSubsystem<InputMaster>()->GetControllableByPlayer(playerId_) };
    if (controllable)

        return controllable;

    else return nullptr;
}
