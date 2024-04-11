#include <eventAction.hh>

eventAction::eventAction() :
G4UserEventAction(),
fSignalEnergyDeposit(0),
fElectronBackgroundEnergyDeposit(0),
fPositronBackgroundEnergyDeposit(0),
fGammaBackgroundEnergyDeposit(0) {}

eventAction::~eventAction() = default;

void eventAction::BeginOfEventAction(const G4Event *evt) {
    fSignalEnergyDeposit = 0;
    fElectronBackgroundEnergyDeposit = 0;
    fPositronBackgroundEnergyDeposit = 0;
    fGammaBackgroundEnergyDeposit = 0;
}

void eventAction::EndOfEventAction(const G4Event *evt) {
    G4int eventID = evt->GetEventID();
    G4AnalysisManager *man = G4AnalysisManager::Instance();
    man->FillNtupleIColumn(0, eventID);
    man->FillNtupleDColumn(1, fSignalEnergyDeposit);
    man->FillNtupleDColumn(2, fElectronBackgroundEnergyDeposit);
    man->FillNtupleDColumn(3, fPositronBackgroundEnergyDeposit);
    man->FillNtupleDColumn(4, fGammaBackgroundEnergyDeposit);
    man->AddNtupleRow(0);
}
