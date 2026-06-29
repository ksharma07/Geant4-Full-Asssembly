#ifndef DETECTOR_MAP_HH
#define DETECTOR_MAP_HH

#include "globals.hh"
#include "G4ThreeVector.hh"
#include <map>

// A global map from CopyNo (int) to position (G4ThreeVector)
extern std::map<G4int, G4ThreeVector> L1IDPosMap;
extern std::map<G4int, G4ThreeVector> L2IDPosMap;

#endif
