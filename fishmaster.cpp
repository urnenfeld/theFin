
#include "fishmaster.h"



FishMaster::FishMaster(Context* context)
    : Object(context)
    , fisherTeam_()
    , cachedFishes_()
{


}

void
FishMaster::RegisterFisher(iFisher& fisher)
{
    fisherTeam_.Push(&fisher);
}


Vector<Fish>
FishMaster::RetrieveFishes()
{
    cachedFishes_.Clear();

    for (iFisher* fisher: fisherTeam_) {
        cachedFishes_.Push(fisher->RetrieveFishes());
    }

    return cachedFishes_;
}


void
FishMaster::Rescan()
{
    for (iFisher* fisher: fisherTeam_) {
        fisher->Rescan();
    }
}
