
#ifndef FISHMASTER_H
#define FISHMASTER_H

#include "luckey.h"
#include "fish.h"


/* Classes that would be retrieving games either from disk or network
 */
class iFisher
{
public:
    virtual Vector<Fish> RetrieveFishes() = 0;
    virtual void Rescan() = 0;
    virtual unsigned Size() = 0;
};


class FishMaster : public Object
{
    URHO3D_OBJECT(FishMaster, Object);
public:
    FishMaster(Context* context);
    void RegisterFisher(iFisher &);

    Vector<Fish> RetrieveFishes();
    void Rescan();

    bool Valid() {
        return cachedFishes_.Size() > 0;
    }


    const Fish* Next();
    const Fish* Previous();
    const Fish* Current();
private:
    Vector<iFisher*> fisherTeam_;
    Vector<Fish> cachedFishes_;

    unsigned index_;
};

#endif // FISHMASTER_H
