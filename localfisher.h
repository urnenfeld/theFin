#ifndef LOCALFISHER_H
#define LOCALFISHER_H

#include <Urho3D/Urho3D.h>

#include "fishmaster.h"

class LocalFisher : public Object , public iFisher
{
    URHO3D_OBJECT(LocalFisher, Object);
public:
    LocalFisher(Context* context, const String path);

    virtual Vector<Fish> RetrieveFishes();
    virtual void Rescan();

private:
    String path_;
    Vector<Fish> fishes_;
    Vector<String> manifests_;
};

#endif // LOCALFISHER_H
