#pragma once

#include <G4UserTrackingAction.hh>
#include <G4Track.hh>

class trkAction : public G4UserTrackingAction {
public:
    trkAction();
    ~trkAction() override;

    void PreUserTrackingAction(const G4Track *) override;
    void PostUserTrackingAction(const G4Track *) override;
};

