#pragma once

#include <G4UserEventAction.hh>
#include <G4RunManager.hh>
#include <G4AnalysisManager.hh>

class eventAction : public G4UserEventAction {
public:
    eventAction();
    ~eventAction() override;
    void BeginOfEventAction(const G4Event*) override;
    void EndOfEventAction(const G4Event*) override;

    void AddSignalEnergyDeposit(G4double energyDep) { fSignalEnergyDeposit += energyDep; }
    void AddElectronBackgroundEnergyDeposit(G4double energyDep) { fElectronBackgroundEnergyDeposit += energyDep; }
    void AddPositronBackgroundEnergyDeposit(G4double energyDep) { fPositronBackgroundEnergyDeposit += energyDep; }
    void AddGammaBackgroundEnergyDeposit(G4double energyDep) { fGammaBackgroundEnergyDeposit += energyDep; }

private:
    G4double fSignalEnergyDeposit;
    G4double fElectronBackgroundEnergyDeposit;
    G4double fPositronBackgroundEnergyDeposit;
    G4double fGammaBackgroundEnergyDeposit;
};

