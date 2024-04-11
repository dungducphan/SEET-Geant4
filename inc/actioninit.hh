#include <G4VUserActionInitialization.hh>

#include <generator.hh>
#include <runaction.hh>
#include <eventAction.hh>
#include <steppingAction.hh>

class actioninit : public G4VUserActionInitialization {
public:
    actioninit();

    ~actioninit() override;

    void Build() const override;

    void BuildForMaster() const override;
};
