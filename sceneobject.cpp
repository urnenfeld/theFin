
#include "sceneobject.h"

SceneObject::SceneObject(Context *context):
    LogicComponent(context),
    initialSoundSources_{0},
    randomizer_{Random()}
{
}

void SceneObject::OnNodeSet(Node *node)
{ if (!node) return;

    while (sampleSources_.Size() < initialSoundSources_) {

        AddSoundSource();
    }
}

void SceneObject::Set(Vector3 position)
{
    node_->SetPosition(position);
    node_->SetEnabledRecursive(true);
}

void SceneObject::Disable()
{
    node_->SetEnabledRecursive(false);
}

void SceneObject::AddSoundSource()
{
    SoundSource3D* sampleSource{ node_->CreateComponent<SoundSource3D>() };
    sampleSource->SetSoundType(SOUND_EFFECT);

    sampleSources_.Push(sampleSource);
}
void SceneObject::PlaySample(Sound* sample, float gain)
{
    for (SoundSource3D* s : sampleSources_) {

        if (!s->IsPlaying()) {

            s->SetGain(gain);
            s->Play(sample);
            return;
        }
    }
    AddSoundSource();
    PlaySample(sample, gain);
}

Vector3 SceneObject::GetWorldPosition() const
{
    return node_->GetWorldPosition();
}
