//
// Created by dphan on 2/22/23.
//

#include <SEET_SD.hh>
#include <G4SystemOfUnits.hh>
#include <TString.h>
#include <G4RunManager.hh>

SEET_SD::SEET_SD(const G4String &name) : G4VSensitiveDetector(name) {}

SEET_SD::~SEET_SD() = default;

G4bool SEET_SD::ProcessHits(G4Step * aStep, G4TouchableHistory *) {

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
    G4double E = aStep->GetPreStepPoint()->GetKineticEnergy() / MeV;

    // Get position
    G4ThreeVector pos = aStep->GetPreStepPoint()->GetPosition();
    G4double x = pos.x() / um;
    G4double y = pos.y() / um;
    G4double z = pos.z() / um;

    // Get momentum
    G4ThreeVector mom = aStep->GetPreStepPoint()->GetMomentum();
    G4double px = mom.x() / MeV;
    G4double py = mom.y() / MeV;
    G4double pz = mom.z() / MeV;

    G4double tof = aStep->GetPreStepPoint()->GetGlobalTime();
    G4int det_id = aStep->GetPreStepPoint()->GetTouchable()->GetCopyNumber();

    G4AnalysisManager *man = G4AnalysisManager::Instance();
    man->FillNtupleIColumn(0, event_id);
    man->FillNtupleIColumn(1, track_id);
    man->FillNtupleIColumn(2, pdg);
    man->FillNtupleDColumn(3, px);
    man->FillNtupleDColumn(4, py);
    man->FillNtupleDColumn(5, pz);
    man->FillNtupleDColumn(6, x);
    man->FillNtupleDColumn(7, y);
    man->FillNtupleDColumn(8, z);
    man->FillNtupleDColumn(9, E);
    man->AddNtupleRow(0);

    aStep->GetTrack()->SetTrackStatus(fStopAndKill);

    return true;
}