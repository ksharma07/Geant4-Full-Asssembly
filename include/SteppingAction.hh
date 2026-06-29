#ifndef STEPPINGACTION_HH
#define STEPPINGACTION_HH

#include "G4UserSteppingAction.hh"
#include "globals.hh"
#include "G4ThreeVector.hh"
#include <fstream>
#include "G4Event.hh"


class G4Step;
class EventAction;

class SteppingAction : public G4UserSteppingAction {
public:
    SteppingAction(EventAction* eventAction);
    ~SteppingAction() override;

    void UserSteppingAction(const G4Step*) override;
    
private:
    EventAction* fEventAction; // Pointer to EventAction for event-level counters
};

#endif
