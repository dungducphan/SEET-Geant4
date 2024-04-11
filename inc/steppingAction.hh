#pragma once

#include <G4UserSteppingAction.hh>
#include <G4EventManager.hh>

class steppingAction : public G4UserSteppingAction {
public:
    steppingAction();
    ~steppingAction() override;
    void UserSteppingAction(const G4Step*) override;
};

