#include <steppingAction.hh>
#include <eventAction.hh>

#include <G4Step.hh>
#include <G4VPhysicalVolume.hh>
#include <G4ParticleDefinition.hh>
#include <G4ParticleTypes.hh>
#include <G4Electron.hh>
#include <G4Positron.hh>
#include <G4Gamma.hh>
#include <G4SystemOfUnits.hh>

steppingAction::steppingAction() = default;

steppingAction::~steppingAction() = default;

void steppingAction::UserSteppingAction(const G4Step *step) {
    auto track = step->GetTrack();
    auto volume = step->GetPreStepPoint()->GetTouchableHandle()->GetVolume();
    auto evtAction = (eventAction*) G4EventManager::GetEventManager()->GetUserEventAction();

    if (volume->GetName() != "phys_VDBack") return;
    if (track->GetParticleDefinition() != G4Electron::Definition() && track->GetParticleDefinition() != G4Positron::Definition() && track->GetParticleDefinition() != G4Gamma::Definition()) return;
    auto energyDeposition = step->GetTotalEnergyDeposit() / keV;

    // Initialize the primary vectors
    auto logicalVolNameAtVertex = track->GetLogicalVolumeAtVertex()->GetName();
    if (logicalVolNameAtVertex != "logic_PhotoDiode") {
        if (track->GetTrackID() == 1) {
            evtAction->AddSignalTracks(track->GetTrackID());
        } else {
            evtAction->AddBkgdTracks(track->GetTrackID());
        }
    }

    // Find out what primary vector this step/track belongs to
    bool isSignal = evtAction->IsSignalTrack(track->GetTrackID());

    // Add secondaries if any
    auto secondaries = step->GetSecondaryInCurrentStep();
    for (auto secondary : *secondaries) {
        if (isSignal) {
            evtAction->AddSignalTracks(secondary->GetTrackID());
        } else {
            evtAction->AddBkgdTracks(secondary->GetTrackID());
        }
    }

    // Add energy deposition
    if (isSignal) {
        evtAction->AddSignalEnergyDeposit(energyDeposition);
    } else {
        evtAction->AddBackgroundEnergyDeposit(energyDeposition);
    }
}
