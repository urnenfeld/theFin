
#ifndef MASTERCONTROL_H
#define MASTERCONTROL_H

#include <Urho3D/Urho3D.h>

#include "luckey.h"

namespace Urho3D {
class Node;
class Scene;
}

class Player;

class MasterControl : public Application
{
    URHO3D_OBJECT(MasterControl, Application);
public:
    MasterControl(Context* context);
    static MasterControl* GetInstance();

    Scene* GetScene() const;

    void AddPlayer();
    Player* GetPlayer(int playerId) const;
    Player* GetNearestPlayer(Vector3 pos);
    Vector< SharedPtr<Player> > GetPlayers();
    void RemovePlayer(Player *player);

    // Setup before engine initialization. Modifies the engine paramaters.
    virtual void Setup();
    // Setup after engine initialization.
    virtual void Start();
    // Cleanup after the main loop. Called by Application.
    virtual void Stop();
    void Exit();

    template <class T> void RegisterObject() { context_->RegisterFactory<T>(); }
    template <class T> void RegisterSubsystem() { context_->RegisterSubsystem(new T(context_)); }
private:
    void CreateScene();

    static MasterControl* instance_;
    Vector< SharedPtr<Player> > players_;
};

#endif // MASTERCONTROL_H
