
#include "uimaster.h"
#include "fishmaster.h"

#include <Urho3D/UI/Text3D.h>

UIMaster::UIMaster(Context* context) : Object(context)
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

    // scene_->CreateChild()->CreateComponent<StaticModel>()->SetModel(CACHE->GetResource<Model>("Models/Box.mdl"));

    fishNameNode_ = scene_->GetChild("FishName", true);
    fishDescriptionNode_ = scene_->GetChild("FishDescription", true);
    fishAuthorNode_= scene_->GetChild("Author", true);

    if (fishNameNode_ && fishDescriptionNode_ && fishAuthorNode_) {
        consistency_ = true;
    } else {
        Log::Write(LOG_ERROR, "Some of the nodes were not found in scene...");
        consistency_ = false;
    }
}


void
UIMaster::Next()
{
    if (mode_ == FISHING) {
        const Fish* fish = GetSubsystem<FishMaster>()->Next();

        if (fish) {
            Log::Write(LOG_INFO, "Selecting fish " + fish->Name_);
            SelectFish(fish);
        } else {
            Log::Write(LOG_ERROR, "No valid fish...");
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
            SelectFish(fish);
        } else {
            Log::Write(LOG_ERROR, "No valid fish...");
        }
    }
}


void
UIMaster::SelectFish(const Fish* fish) {

    if (consistency_) {
        fishNameNode_->GetComponent<Text3D>()->SetText(fish->Name_);
        fishDescriptionNode_->GetComponent<Text3D>()->SetText(fish->Description_);
        fishAuthorNode_->GetComponent<Text3D>()->SetText(fish->Author_);
    }
}
