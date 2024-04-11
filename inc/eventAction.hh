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
    void AddBackgroundEnergyDeposit(G4double energyDep) { fBackgroundEnergyDeposit += energyDep; }

    void AddSignalTracks(G4int trackID) { fSignalTracks.push_back(trackID); }
    void AddBkgdTracks(G4int trackID) { fBkgdTracks.push_back(trackID); }
    bool IsSignalTrack(G4int trackID) { return std::find(fSignalTracks.begin(), fSignalTracks.end(), trackID) != fSignalTracks.end(); }

private:
    G4double fSignalEnergyDeposit;
    G4double fBackgroundEnergyDeposit;
    std::vector<G4int> fSignalTracks;
    std::vector<G4int> fBkgdTracks;
};

