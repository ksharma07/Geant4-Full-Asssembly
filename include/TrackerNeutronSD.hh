
#ifndef TrackerNeutronSD_h
#define TrackerNeutronSD_h 1

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

class TrackerNeutronSD : public G4VSensitiveDetector
{
public:
  TrackerNeutronSD(G4String);
  ~TrackerNeutronSD();

private:
  void Initialize(G4HCofThisEvent *) override;
  G4bool ProcessHits(G4Step *, G4TouchableHistory *) override;
  void EndOfEvent(G4HCofThisEvent *) override;
  G4double totalEnergyDep;
  G4double xPosCentroid;
  G4double numInteractions;
  G4double timeOfFlight;
  G4double scatteringAngle;
  G4bool itInteracted;
  G4bool fromPrimaryNeutron;
  G4ThreeVector p_initial;
  G4ThreeVector p_final;
  G4bool initialSet = false;
};

#endif
