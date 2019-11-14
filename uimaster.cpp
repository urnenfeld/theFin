
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

}


void
UIMaster::Next()
{
    const Fish* fish = GetSubsystem<FishMaster>()->Next();

    if (fish) {
        Log::Write(LOG_INFO, "Selecting fish " + fish->Name_);
        SelectFish(fish);
    } else {
        Log::Write(LOG_ERROR, "No valid fish...");
    }
}


void
UIMaster::Previous()
{
    const Fish* fish = GetSubsystem<FishMaster>()->Previous();

    if (fish) {
        Log::Write(LOG_INFO, "Selecting fish " + fish->Name_);
        SelectFish(fish);
    } else {
        Log::Write(LOG_ERROR, "No valid fish...");
    }
}


void
UIMaster::SelectFish(const Fish* fish) {

    // TODO: Move node fetching to scene creation
    Node* nameNode = scene_->GetChild("FishName", true);
    if (nameNode) {
        nameNode->GetComponent<Text3D>()->SetText(fish->Name_);
    }

    Node* descriptionNode = scene_->GetChild("FishDescription", true);
    if (descriptionNode) {
        descriptionNode->GetComponent<Text3D>()->SetText(fish->Description_);
    }

    Node* authorNode = scene_->GetChild("Author", true);
    if (authorNode) {
        authorNode->GetComponent<Text3D>()->SetText(fish->Author_);
    }

    if ((authorNode == nullptr) || (nameNode==nullptr) || (descriptionNode==nullptr)) {
        Log::Write(LOG_ERROR, "Unable to find nodes in scene...");
    }



}
