
#ifndef PLAYER_H
#define PLAYER_H

#include <Urho3D/Urho3D.h>
#include "mastercontrol.h"

class Controllable;

class Player : public Object
{
    URHO3D_OBJECT(Player, Object);
public:
    Player(int playerId, Context* context);

    Controllable *GetControllable();

    int GetPlayerId() const { return playerId_; }
    void AddScore(int points);
    unsigned GetScore() const { return score_; }
    void Die();
    void Respawn();
    void ResetScore();

    bool IsAlive() const noexcept { return alive_; }
    bool IsHuman() const noexcept { return !autoPilot_; }
private:
    int playerId_;
    bool autoPilot_;
    bool alive_;

    unsigned score_;
    int multiplier_;

    void SetScore(int points);
};

#endif // PLAYER_H
