#include "runaction.hh"

runAction::runAction() : G4UserRunAction() {
    man = G4AnalysisManager::Instance();
    man->CreateNtuple("SNRatio", "SNRatio");
    man->CreateNtupleIColumn("EventID");
    man->CreateNtupleDColumn("SignalEdep");
    man->CreateNtupleDColumn("ElectronBkgdEdep");
    man->CreateNtupleDColumn("PositronBkgdEdep");
    man->CreateNtupleDColumn("GammaBkgdEdep");
    man->FinishNtuple(0);
}

runAction::~runAction() = default;

void runAction::BeginOfRunAction(const G4Run *run) {
    man = G4AnalysisManager::Instance();
    man->OpenFile(Form("SNRatio_RunID_%03i.root", (int) run->GetRunID()));
}

void runAction::EndOfRunAction(const G4Run *) {
    man = G4AnalysisManager::Instance();
    man->Write();
    man->CloseFile();
}

