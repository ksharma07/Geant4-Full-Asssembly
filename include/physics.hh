#ifndef PHYSICS_HH
#define PHYSICS_HH
#include "G4VModularPhysicsList.hh"
#include "G4EmStandardPhysics.hh"
#include "G4DecayPhysics.hh"
#include "NeutronHPphysics.hh"
#include "G4OpticalPhysics.hh"
//#include "G4NeutronTrackingCut.hh"
//#include "G4HadronElasticPhysics.hh"

class MyPhysicsList : public G4VModularPhysicsList
{
public:
  MyPhysicsList();
  ~MyPhysicsList();
};
#endif
