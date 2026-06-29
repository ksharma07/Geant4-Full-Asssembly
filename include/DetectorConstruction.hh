#ifndef DetectorConstruction_H
#define DetectorConstruction_H 1

#include "G4VUserDetectorConstruction.hh"
#include "Materials.hh"
#include "Sample.hh"
#include "TrackerNeutronSD.hh"
#include "TrackerFluxSheet.hh"
#include "G4PVPlacement.hh"
#include "G4GenericMessenger.hh"
#include "G4SubtractionSolid.hh"
#include <vector>
#include "G4MultiUnion.hh"
#include "CCalMaterialFactory.hh"
#include "G4NistManager.hh"

#include "G4Torus.hh"

class DetectorConstruction : public G4VUserDetectorConstruction
{
public:
  DetectorConstruction();
  ~DetectorConstruction();

  G4VPhysicalVolume *Construct();

private:

  // Logical volumes
  G4LogicalVolume *ExpHall_log;
  G4LogicalVolume *logicLayer1;
  G4LogicalVolume *logicLayer2;
  G4LogicalVolume *logicSipm;
  G4LogicalVolume *logicpcb;
  G4LogicalVolume *logicheat;
  //G4LogicalVolume *logicFluxSheet;
  G4LogicalVolume *logicFrame;
  G4LogicalVolume *logicSlab;

  //G4LogicalVolume *logicslFrame;
  //G4LogicalVolume *logicsrFrame;

  G4LogicalVolume *logicmtL;
  G4LogicalVolume *logicShield;
  G4LogicalVolume *logicShield2;

  G4LogicalVolume *logicDetShield;
  G4LogicalVolume *logicDet2Shield;
  G4LogicalVolume *logicGamShield;
  G4LogicalVolume *logicGam2Shield;

  // Physical volumes
  double L1Thickness = 1.0;
  double L4Thickness = 1.0;

  Sample *L1;
  Sample *L2;
  Sample *L3;
  Sample *L4;

  virtual void ConstructSDandField();
};

#endif
