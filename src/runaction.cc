#include "runaction.hh"

runAction::runAction() : G4UserRunAction() {
    man = G4AnalysisManager::Instance();
    man->CreateNtuple("Ana", "Ana");
    man->CreateNtupleIColumn("EventID");
    man->CreateNtupleIColumn("TrackID");
    man->CreateNtupleIColumn("PID");
    man->CreateNtupleDColumn("pX");
    man->CreateNtupleDColumn("pY");
    man->CreateNtupleDColumn("pZ");
    man->CreateNtupleDColumn("x");
    man->CreateNtupleDColumn("y");
    man->CreateNtupleDColumn("z");
    man->CreateNtupleDColumn("E");
    man->FinishNtuple(0);
}

runAction::~runAction() = default;

void runAction::BeginOfRunAction(const G4Run *run) {
    man = G4AnalysisManager::Instance();
    man->OpenFile(Form("Ana_RunID_%03i.root", (int) run->GetRunID()));
}

void runAction::EndOfRunAction(const G4Run *) {
    man = G4AnalysisManager::Instance();
    man->Write();
    man->CloseFile();
}

