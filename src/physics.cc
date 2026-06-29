#include "physics.hh"
#include "NeutronHPphysics.hh"
MyPhysicsList::MyPhysicsList()
{
  RegisterPhysics (new G4EmStandardPhysics());
  RegisterPhysics(new G4DecayPhysics());
  RegisterPhysics(new NeutronHPphysics("NeutronHP"));
  RegisterPhysics(new G4OpticalPhysics());
  //RegisterPhysics(new G4HadronElasticPhysics());
  // RegisterPhysics(new G4HadronPhysicsQBBC());
  //RegisterPhysics(new G4NeutronTrackingCut());
}

MyPhysicsList::~MyPhysicsList()
{
  
}
