
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
        printf("Selecting fish %s\n", fish->Name_.CString());
        SelectFish(fish);
    } else {
        printf("No Valid fish\n");
    }
}


void
UIMaster::Previous()
{
    const Fish* fish = GetSubsystem<FishMaster>()->Previous();

    if (fish) {
        printf("Selecting fish %s\n", fish->Name_.CString());
        SelectFish(fish);
    } else {
        printf("No Valid fish\n");
    }
}


void
UIMaster::SelectFish(const Fish* fish) {

    Node* nameNode = scene_->GetChild("FishName", true);
    if (nameNode) {
        nameNode->GetComponent<Text3D>()->SetText(fish->Name_);
    }
}
