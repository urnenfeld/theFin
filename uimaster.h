
#ifndef UIMASTER_H
#define UIMASTER_H

#include <Urho3D/Urho3D.h>

#include "luckey.h"

class Fish;

class UIMaster : public Object
{
    URHO3D_OBJECT(UIMaster, Object);
public:
    UIMaster(Context* context);

    void CreateScene();
    Scene* GetScene() const { return scene_; }

    void SwitchToFishes() {}
    void Next();
    void Previous();
    void SelectFish(const Fish* fish);

    void SwitchToBottomControls() {}

    void Inkove() {}

private:
    Scene* scene_;
};

#endif // UIMASTER_H
