#include <eventAction.hh>

eventAction::eventAction() :
G4UserEventAction(),
fSignalEnergyDeposit(0),
fBackgroundEnergyDeposit(0) {}

eventAction::~eventAction() = default;

void eventAction::BeginOfEventAction(const G4Event *evt) {
    fSignalEnergyDeposit = 0;
    fBackgroundEnergyDeposit = 0;
    fSignalTracks.clear();
    fBkgdTracks.clear();
}

void eventAction::EndOfEventAction(const G4Event *evt) {
    G4int eventID = evt->GetEventID();
    G4AnalysisManager *man = G4AnalysisManager::Instance();
    man->FillNtupleIColumn(0, eventID);
    man->FillNtupleDColumn(1, fSignalEnergyDeposit);
    man->FillNtupleDColumn(2, fBackgroundEnergyDeposit);
    man->AddNtupleRow(0);
}
