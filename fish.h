#ifndef FISH_H
#define FISH_H

/* Definition of a Game
 */
class Fish
{
    Fish() {}
    // As Vectors are invoking constructors and destructors...
    friend class Urho3D::Vector<Fish>;
public:
    String Name_;
    String Description_;
    String Author_;

    String Id_;
    String Path_;
    bool Present_;

    Vector<String> Screenshots_;

    bool CheckIntegrity() {
        return !Id_.Empty() && !Name_.Empty() && !Description_.Empty() && !Path_.Empty();
    }

    static Fish* FromJsonFile(Context* context, String urhofest) {
        // form Resource.cpp
        File file(context);
        if (file.Open(urhofest, FILE_READ)) {
            // from Material.cpp
            JSONFile* loadJSONFile_ = new JSONFile(context);
            if (loadJSONFile_->Load(file))
            {
                Fish* fish = new Fish();
                fish->Path_ = urhofest;

                const JSONValue& rootVal = loadJSONFile_->GetRoot();

                for (JSONObject::ConstIterator i = rootVal.Begin(); i != rootVal.End(); ++i)
                {
                    String id = i->first_;
                    if (id.Empty())
                    {
                        URHO3D_LOGWARNING(__FUNCTION__ + " ID is empty at element "+i);
                        continue;
                    } else {
                        // Log::Write(LOG_INFO, "Inspecting: " + id);

                        const JSONValue& value = i->second_;
                        if (value.IsNull())
                        {
                            URHO3D_LOGWARNING(__FUNCTION__ + " Value for "+id+" is empty");
                            continue;
                        } else {
                            // Log::Write(LOG_INFO, "Inspecting: " + value.GetString());

                            if (id == "Summary")
                                fish->Name_ = value.GetString();
                            else if (id == "Description")
                                fish->Description_ = value.GetString();
                            else if (id == "Id")
                                fish->Id_ = value.GetString();
                        }
                    }
                }

                file.Close();

                if (fish->CheckIntegrity())
                    return fish;
                else {
                    delete fish;
                    return nullptr;
                }

            } else {
                Log::Write(LOG_ERROR, "Not able to load json from urhofest");
            }
        } else {
            Log::Write(LOG_ERROR, "Not able to open file.");
        }
        file.Close();

        return nullptr;
    }
};

#endif
