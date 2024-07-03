#include "SEET_Edep_SD.h"

#include <G4SystemOfUnits.hh>
#include <TString.h>
#include <G4RunManager.hh>

SEET_Edep_SD::SEET_Edep_SD(const G4String &name) : G4VSensitiveDetector(name) {}

SEET_Edep_SD::~SEET_Edep_SD() = default;

G4bool SEET_Edep_SD::ProcessHits(G4Step * aStep, G4TouchableHistory *) {

    // Get event number
    G4int event_id = G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID();

    // Get particle type
    G4ParticleDefinition *particle = aStep->GetTrack()->GetDefinition();
    G4int pdg = particle->GetPDGEncoding();

    // Get track ID
    G4int track_id = aStep->GetTrack()->GetTrackID();

    // Kill if not an electron, positron, or gamma
    if (particle != G4Electron::ElectronDefinition() && particle != G4Positron::PositronDefinition() && particle != G4Gamma::GammaDefinition()) {
        aStep->GetTrack()->SetTrackStatus(fStopAndKill);
        return false;
    }

    // Get kinetic energy
    G4double edep = aStep->GetTotalEnergyDeposit() / MeV;

    G4AnalysisManager *man = G4AnalysisManager::Instance();
    man->FillNtupleIColumn(0, event_id);
    man->FillNtupleIColumn(1, track_id);
    man->FillNtupleIColumn(2, pdg);
    man->FillNtupleDColumn(3, edep);
    man->AddNtupleRow(0);

    return true;
}