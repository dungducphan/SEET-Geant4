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

    void AddSignalEnergyDeposit(const G4double& energyDep) { fSignalEnergyDeposit += energyDep; }
    void AddBackgroundEnergyDeposit(const G4double& energyDep) { fBackgroundEnergyDeposit += energyDep; }
    void AddElectronPositronBackgroundEnergyDeposit(const G4double& energyDep) { fElectronPositronBackgroundEnergyDeposit += energyDep; }
    void AddGammaBackgroundEnergyDeposit(const G4double& energyDep) { fGammaBackgroundEnergyDeposit += energyDep; }

    void AddSignalTracks(const G4int& trackID) { fSignalTracks.insert(trackID); }
    void AddBkgdTracks(const G4int& trackID) { fBkgdTracks.insert(trackID); }
    void AddElectronPositronBkgdTracks(const G4int& trackID) { fElectronPositionBkgdTracks.insert(trackID); }
    void AddGammaBkgdTracks(const G4int& trackID) { fGammaBkgdTracks.insert(trackID); }
    [[nodiscard]] bool IsSignalTrack(const G4int& trackID) const { return std::find(fSignalTracks.begin(), fSignalTracks.end(), trackID) != fSignalTracks.end(); }
    [[nodiscard]] bool IsGammaBkgdTrack(const G4int& trackID) const { return std::find(fGammaBkgdTracks.begin(), fGammaBkgdTracks.end(), trackID) != fGammaBkgdTracks.end(); }

private:
    G4double fSignalEnergyDeposit;
    G4double fBackgroundEnergyDeposit;
    G4double fElectronPositronBackgroundEnergyDeposit;
    G4double fGammaBackgroundEnergyDeposit;
    std::set<G4int> fSignalTracks;
    std::set<G4int> fElectronPositionBkgdTracks;
    std::set<G4int> fGammaBkgdTracks;
    std::set<G4int> fBkgdTracks;

};

