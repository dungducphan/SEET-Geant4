#include <steppingAction.hh>
#include <eventAction.hh>

#include <G4Step.hh>
#include <G4VPhysicalVolume.hh>
#include <G4ParticleDefinition.hh>
#include <G4ParticleTypes.hh>
#include <G4Electron.hh>
#include <G4Positron.hh>
#include <G4Gamma.hh>

steppingAction::steppingAction() = default;

steppingAction::~steppingAction() = default;

void steppingAction::UserSteppingAction(const G4Step *step) {
    auto track = step->GetTrack();
    auto volume = step->GetPreStepPoint()->GetTouchableHandle()->GetVolume();
    auto evtAction = (eventAction*) G4EventManager::GetEventManager()->GetUserEventAction();

    if (volume->GetName() != "phys_VDBack") return;
    if (track->GetParticleDefinition() != G4Electron::Definition() && track->GetParticleDefinition() != G4Positron::Definition() && track->GetParticleDefinition() != G4Gamma::Definition()) return;

    auto energyDeposition = step->GetTotalEnergyDeposit();
    if (track->GetParticleDefinition() == G4Electron::Definition()) {
        if (track->GetTrackID() == 1) {
            evtAction->AddSignalEnergyDeposit(energyDeposition);
        } else {
            evtAction->AddElectronBackgroundEnergyDeposit(energyDeposition);
        }
    } else {
        if (track->GetParticleDefinition() == G4Gamma::Definition()) {
            evtAction->AddGammaBackgroundEnergyDeposit(energyDeposition);
        } else {
            evtAction->AddPositronBackgroundEnergyDeposit(energyDeposition);
        }
    }
}
