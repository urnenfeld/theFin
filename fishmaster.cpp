
#include "fishmaster.h"



FishMaster::FishMaster(Context* context)
    : Object(context)
    , fisherTeam_()
    , cachedFishes_()
    , index_(0)
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
    index_ = 0;

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


const Fish*
FishMaster::Next()
{
    if (Valid()) {
        index_ = (index_ + 1) % cachedFishes_.Size();
        return &cachedFishes_.At(index_);
    }

    return nullptr;
}


const Fish*
FishMaster::Previous()
{
    if (Valid()) {
        if (index_ == 0) {
            index_ = cachedFishes_.Size() - 1;
        } else {
            index_ = (index_ - 1) % cachedFishes_.Size();
        }

        return &cachedFishes_.At(index_);
    }

    return nullptr;
}


const Fish*
FishMaster::Current()
{
    if (Valid()) {
        return &cachedFishes_.At(index_);
    }

    return nullptr;
}
