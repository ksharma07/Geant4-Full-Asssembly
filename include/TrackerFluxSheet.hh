
#ifndef TrackerFluxSheet_h
#define TrackerFluxSheet_h 1

#include "G4VSensitiveDetector.hh"
#include "G4HCofThisEvent.hh"
#include "G4Step.hh"
#include "G4ThreeVector.hh"
#include "G4SDManager.hh"
#include "G4ios.hh"
#include "G4UnitsTable.hh"
#include "G4VTouchable.hh"
#include "G4VProcess.hh"
#include "G4StepStatus.hh"
#include "G4TrackStatus.hh"
#include "G4AnalysisManager.hh"
#include "G4RunManager.hh"

class TrackerFluxSheet : public G4VSensitiveDetector
{
public:
  TrackerFluxSheet(G4String);
  ~TrackerFluxSheet();

private:
  void Initialize(G4HCofThisEvent *) override;
  G4bool ProcessHits(G4Step *, G4TouchableHistory *) override;
  void EndOfEvent(G4HCofThisEvent *) override;
  G4bool itInteracted;
  G4bool fromPrimaryNeutron;
};

#endif
