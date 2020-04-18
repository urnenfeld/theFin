
#ifndef UIMASTER_H
#define UIMASTER_H

#include <Urho3D/Urho3D.h>

#include "luckey.h"

class Fish;

class UIMaster : public Object
{
    enum {
        FISHING,
        OPTIONS
    } mode_;

    URHO3D_OBJECT(UIMaster, Object);
public:
    UIMaster(Context* context);

    void CreateScene();
    Scene* GetScene() const { return scene_; }

    void Next();
    void Previous();

    void SwitchToControls() { mode_ = OPTIONS; }
    void SwitchToFishes() { mode_ = FISHING; }

    void Invoke();

private:
    void SelectFish(const Fish* fish);

    Scene* scene_;

    Node* fishNameNode_;
    Node* fishDescriptionNode_;
    Node* fishAuthorNode_;
    Node* fishNumber_;

    float rotation_ = 90.0f;
    bool consistency_;
};

#endif // UIMASTER_H
