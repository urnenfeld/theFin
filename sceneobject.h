
#ifndef SCENEOBJECT_H
#define SCENEOBJECT_H

#include <Urho3D/Urho3D.h>

#include "mastercontrol.h"

class SceneObject : public LogicComponent
{
    URHO3D_OBJECT(SceneObject, Object);
public:
    SceneObject(Context* context);

    virtual void Set(Vector3 position);
    void OnNodeSet(Node *node) override;

    Vector3 GetWorldPosition() const;
protected:
    unsigned initialSoundSources_;
    Vector<SoundSource3D*> sampleSources_;
    float randomizer_;

    void Disable();

    void PlaySample(Sound *sample, float gain = 0.3f);
private:
    void AddSoundSource();
};

#endif // SCENEOBJECT_H

