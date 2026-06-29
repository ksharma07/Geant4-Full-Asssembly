#ifndef RunAction_h
#define RunAction_h 1

#include "G4UserRunAction.hh"
#include "G4Run.hh"
#include "G4AnalysisManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4UnitsTable.hh"
#include "globals.hh"
#include "G4RunManager.hh"

#include "TFile.h"
#include "TTree.h"

class RunAction : public G4UserRunAction
{
public:
  RunAction();
  ~RunAction();

  virtual void BeginOfRunAction(const G4Run *run);
  virtual void EndOfRunAction(const G4Run *run);

private:
  TFile *fRootFile;  // Output ROOT file
  TTree *fTree;      // Output ROOT tree
  G4int totalEvents;
};

#endif
