#include <trkAction.hh>

trkAction::trkAction() : G4UserTrackingAction() {
}

trkAction::~trkAction() = default;

void trkAction::PreUserTrackingAction(const G4Track *trk) {
}

void trkAction::PostUserTrackingAction(const G4Track *trk) {
}
