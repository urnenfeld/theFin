
#include "effectmaster.h"
#include "inputmaster.h"
#include "player.h"
#include "spawnmaster.h"
#include "fishmaster.h"
#include "uimaster.h"

#include "localfisher.h"

#include "mastercontrol.h"

URHO3D_DEFINE_APPLICATION_MAIN(MasterControl);

MasterControl* MasterControl::instance_ = NULL;

MasterControl* MasterControl::GetInstance()
{
    return MasterControl::instance_;
}

MasterControl::MasterControl(Context *context):
    Application(context)
{
    instance_ = this;
}

void MasterControl::Setup()
{
    SetRandomSeed(TIME->GetSystemTime());

    engineParameters_[EP_LOG_NAME] = GetSubsystem<FileSystem>()->GetAppPreferencesDir("luckey", "logs") + "theFin2.log";
    engineParameters_[EP_WINDOW_TITLE] = "theFin2";
    engineParameters_[EP_WINDOW_ICON] = "Textures/UrhoIcon.png";
    engineParameters_[EP_WORKER_THREADS] = false;
    engineParameters_[EP_RESOURCE_PATHS] = "Data;CoreData;Resources;";
}
void MasterControl::Start()
{
    RegisterSubsystem<EffectMaster>();
    RegisterSubsystem<InputMaster>();
    RegisterSubsystem<SpawnMaster>();
    RegisterSubsystem<FishMaster>();
    RegisterSubsystem<UIMaster>();

    LocalFisher* lf = new LocalFisher(context_, "/tmp");
    GetSubsystem<FishMaster>()->RegisterFisher(*lf);

    if (GRAPHICS)
        ENGINE->SetMaxFps(GRAPHICS->GetRefreshRate());

    GetSubsystem<UIMaster>()->CreateScene();
}


Scene*
MasterControl::GetScene() const
{
    return GetSubsystem<UIMaster>()->GetScene();
}


void MasterControl::Stop()
{
    engine_->DumpResources(true);
}


void MasterControl::Exit()
{
    engine_->Exit();
}


Vector<SharedPtr<Player> > MasterControl::GetPlayers()
{
    return players_;
}
Player* MasterControl::GetPlayer(int playerId) const
{
    for (Player* p : players_) {

        if (p->GetPlayerId() == playerId){
            return p;
        }
    }
    return nullptr;
}
Player* MasterControl::GetNearestPlayer(Vector3 pos)
{
    Player* nearest{};
    for (Player* p : players_){
        if (p->IsAlive()){

            if (!nearest
                    || (LucKey::Distance(GetSubsystem<InputMaster>()->GetControllableByPlayer(p->GetPlayerId())->GetWorldPosition(), pos) <
                        LucKey::Distance(GetSubsystem<InputMaster>()->GetControllableByPlayer(nearest->GetPlayerId())->GetWorldPosition(), pos)))
            {
                nearest = p;
            }
        }
    }
    return nearest;
}
