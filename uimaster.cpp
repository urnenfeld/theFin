#include <unistd.h>


#include "uimaster.h"
#include "fishmaster.h"
#include "effectmaster.h"
#include "mastercontrol.h"


#include <Urho3D/UI/Text3D.h>

UIMaster::UIMaster(Context* context)
    : Object(context)
    , mode_(FISHING)
    , fishNameNode_(nullptr)
    , fishDescriptionNode_(nullptr)
    , fishAuthorNode_(nullptr)
    , fishNumber_(nullptr)
    , consistency_(false)
{
}


void
UIMaster::CreateScene()
{
    scene_ = new Scene(context_);
    scene_->CreateComponent<Octree>();
    scene_->CreateComponent<PhysicsWorld>();
    scene_->CreateComponent<DebugRenderer>();

    // Load Scene
    SharedPtr<File> file = CACHE->GetFile("Scenes/theFinScene.xml");
    if (file) {
        scene_->LoadXML(*file);
    } else {
        Log::Write(LOG_ERROR, "Scene could not be loaded");
    }

    //Light
    Node* lightNode{ scene_->CreateChild("Light") };
    lightNode->SetPosition(Vector3(2.0f, 3.0f, 1.0f));
    lightNode->CreateComponent<Light>();

    //Camera
    Node* cameraNode{ scene_->CreateChild("Camera") };
    cameraNode->SetPosition(Vector3::UP * 3 + Vector3::BACK * 5.0f);
    cameraNode->LookAt(Vector3::ZERO);
    Camera* camera{ cameraNode->CreateComponent<Camera>() };
    RENDERER->SetViewport(0, new Viewport(context_, scene_, camera));

    // Scening
    fishNameNode_ = scene_->GetChild("FishName", true);
    fishDescriptionNode_ = scene_->GetChild("FishDescription", true);
    fishAuthorNode_= scene_->GetChild("Author", true);
    fishNumber_ = scene_->GetChild("FishNumber", true);

    if (fishNameNode_ && fishDescriptionNode_ && fishAuthorNode_ && fishNumber_) {
        consistency_ = true;
    } else {
        Log::Write(LOG_ERROR, "Some of the nodes were not found in scene...");
        consistency_ = false;
    }

    // Set intial status
    const Fish* fish = GetSubsystem<FishMaster>()->Current();

    if (fish) {
        Log::Write(LOG_INFO, "Initial selection ok " + fish->Name_);
        SelectFish(fish);
    } else {
        Log::Write(LOG_ERROR, "No valid fish for initial selection ...");
    }
}


void
UIMaster::Next()
{
    if (mode_ == FISHING) {
        const Fish* fish = GetSubsystem<FishMaster>()->Next();

        if (fish) {
            Log::Write(LOG_INFO, "Selecting fish " + fish->Name_);
            rotation_ += 90.0f;
            SelectFish(fish);
        } else {
            Log::Write(LOG_ERROR, "No valid fish for selecting...");
        }
    }
}


void
UIMaster::Previous()
{
    if (mode_ == FISHING) {
        const Fish* fish = GetSubsystem<FishMaster>()->Previous();

        if (fish) {
            Log::Write(LOG_INFO, "Selecting fish " + fish->Name_);
            rotation_ -= 90.0f;
            SelectFish(fish);
        } else {
            Log::Write(LOG_ERROR, "No valid fish for selecting...");
        }
    }
}


void
UIMaster::SelectFish(const Fish* fish) {

    if (consistency_) {
        fishNameNode_->GetComponent<Text3D>()->SetText(fish->Name_);
        fishDescriptionNode_->GetComponent<Text3D>()->SetText(fish->Description_);
        fishAuthorNode_->GetComponent<Text3D>()->SetText(fish->Author_);

        String count = "[" + String(GetSubsystem<FishMaster>()->CurrentFishIndex()+1) + "/" + String(GetSubsystem<FishMaster>()->TotalFishes()) + "]";
        fishNumber_->GetComponent<Text3D>()->SetText(count);

        Node* boxEffect = scene_->GetChild("UrhoBox", true);
        if (boxEffect) {
            GetSubsystem<EffectMaster>()->RotateTo(boxEffect, Quaternion(0, rotation_, 0), 1.0f);
        }
    }
}


void
UIMaster::Invoke() {
    if (mode_ == FISHING) {
        const Fish* fish = GetSubsystem<FishMaster>()->Current();
        if (fish != nullptr) {
            int unistd_error;
            const String delegationSelector = "/tmp/finNextLaunch";

            // Inform the other world what do we want to launch by:
            // A symlink and a success exit code.
            // As theFin must die and free ALL resources to the fish...
            // The script called from the init process should check for the
            // EXIS_SUCCESS and launch the fish or shutdown in case of EXIT_FAILURE
            if (!(GetSubsystem<FileSystem>()->FileExists(delegationSelector)) || GetSubsystem<FileSystem>()->Delete(delegationSelector)) {

                // Get actual environment from IDE...
                char* local_fisher_path = getenv("LOCAL_FISH_PATH");
                if (local_fisher_path) {
                    unistd_error = symlink("/bin/ls", delegationSelector.CString());
                } else {
                    const String link = "/usr/bin/"+fish->Id_+"-launcher";
                    unistd_error= symlink(link.CString(), delegationSelector.CString());
                }

                if (unistd_error == 0) {
                    ErrorExit("Launching Fish ... "+fish->Id_+"-launcher", EXIT_SUCCESS);
                    MC->Exit();
                } else {
                    Log::Write(LOG_ERROR, "Launch delegation did not work!(Symlink)");
                }
            } else {
                Log::Write(LOG_ERROR, "Launch delegation did not work!(Deletion)");
            }

        } else {
            Log::Write(LOG_ERROR, "No valid fish for invoking...");
        }
    }
}
