#ifndef Materials_H
#define Materials_H 1

#include "G4Material.hh"

class Materials
{
public:
  Materials();
  ~Materials();

  G4Material *FindMaterial(G4String);

private:
  // Elements
  G4Element *elementH;
  G4Element *elementD;
  G4Element *elementO;
  G4Element *elementN;
  G4Element *elementS;
  G4Element *elementFe;
  G4Element *elementCa;
  G4Element *elementSi;
  G4Element *elementLi;
  G4Element *elementAl;
  G4Element *elementPb;
  G4Element *elementC;
  G4Element *elementB;
  G4Element *elB10;
  G4Element *elB11;
  G4Isotope *deuter;
  G4Isotope *B10;
  G4Isotope *B11;
  G4Element *elementW;

  // Materials
  G4Material *vacuum;
  G4Material *water;
  G4Material *lead;
  G4Material *iron;
  G4Material *silicon;
  G4Material *heavywater;
  G4Material *air;
  G4Material *concrete;
  G4Material *carbon;
  G4Material *boron;
  G4Material *aluminium;
  G4Material *glassFiber;
  G4Material *epoxyResin;
  G4Material *FR4;
  G4Material *polyEth;
  G4Material *boratedPE;
  G4Material *lithiumPE;
  G4Material *boratedW;
  G4Material *ABS;
  G4Material *SiO2;
  G4Material *ABS_GF;

  G4Material *eljen230;
  G4Material *eljen232q;
  G4Material *boricAcid;

  G4double temperature, pressure, density;
};

#endif
