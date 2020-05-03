#include "localfisher.h"


LocalFisher::LocalFisher(Context* context, const String path)
    : Object(context)
    , iFisher()
{
    path_ = path + "/usr/share/Urho3D";

    Rescan();
}


Vector<Fish> LocalFisher::RetrieveFishes()
{
    return fishes_;
}


void LocalFisher::Rescan()
{
    fishes_.Clear();

    Log::Write(LOG_INFO, "Checking for manifests in : " + path_);
/* RegisterPath() affects finding remaining resources
    if (!GetSubsystem<FileSystem>()->CheckAccess(path_)) {
        Log::Write(LOG_ERROR, "Cannot access : " + path_);
    } else {
        GetSubsystem<FileSystem>()->RegisterPath(path_);
    }
*/
    if (GetSubsystem<FileSystem>()->DirExists(path_)) {
        GetSubsystem<FileSystem>()->ScanDir(manifests_, path_, ".json", SCAN_FILES, true);
        if (manifests_.Empty()) {
            Log::Write(LOG_WARNING, "No Manifests found");
        } else {
            for (String manifest : manifests_) {
                if (manifest.Contains("urhofest.json")) {
                    manifest = path_ + "/" + manifest;
                    Log::Write(LOG_INFO, "Inspecting fish... " + manifest);

                    Fish* fish = Fish::FromJsonFile(context_, manifest);
                    if (fish != nullptr) {
                        Log::Write(LOG_INFO, "Registering Fish : " + fish->Name_);
                        fishes_.Push(*fish);
                    } else {
                        Log::Write(LOG_ERROR, "Malformed manifest for : " + manifest);
                    }
                }
            }
        }
    } else {
        Log::Write(LOG_ERROR, "Path for manifests not found: " + path_);
    }
}
