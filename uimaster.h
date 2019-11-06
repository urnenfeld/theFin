
#ifndef UIMASTER_H
#define UIMASTER_H

#include <Urho3D/Urho3D.h>

#include "luckey.h"

class UIMaster : public Object
{
    URHO3D_OBJECT(UIMaster, Object);
public:
    UIMaster(Context* context);

    void CreateScene();
    Scene* GetScene() const { return scene_; }

private:
    Scene* scene_;
};

#endif // UIMASTER_H
