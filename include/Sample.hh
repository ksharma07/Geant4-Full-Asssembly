#ifndef Sample_H
#define Sample_H 1

#include "G4Material.hh"
#include "Materials.hh"
#include "G4Tubs.hh"
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"
#include "G4ThreeVector.hh"
#include "G4PVPlacement.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"
#include "G4RotationMatrix.hh"
#include "G4Transform3D.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4NistManager.hh"
#include "CCalMaterialFactory.hh"

class Sample
{
public:
  G4LogicalVolume *expHall_log;

  Sample(G4LogicalVolume *, Materials *, std::string, double, double, double, double, double, double, G4String, G4Colour);
  ~Sample();

  G4VPhysicalVolume *Construct();
  void setRmin(G4double);
  void setRmax(G4double);
  void setLength(G4double);
  void setMaterial(G4String);
  G4LogicalVolume *GetSampleLog() { return Sample_log; }
  void Report();

private:
  // dimensions
  G4double BTrmin;
  G4double BTrmax;
  G4double BTDz;
  G4double BTSPhi;
  G4double BTDPhi;
  G4Colour objColour;

  // materials
  Materials *materials;
  G4Material *SampleMaterial;

  // default position
  G4RotationMatrix NoRot;
  G4ThreeVector *Pos0;

  // the tube
  G4Tubs *Sample_tube;

  // logical volume
  G4LogicalVolume *Sample_log;

  // physical volume
  G4VPhysicalVolume *Sample_phys;
};

#endif
