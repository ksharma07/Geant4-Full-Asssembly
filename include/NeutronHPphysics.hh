#ifndef NeutronHPphysics_h
#define NeutronHPphysics_h 1

#include "globals.hh"
#include "G4VPhysicsConstructor.hh"

class G4GenericMessenger;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class NeutronHPphysics : public G4VPhysicsConstructor
{
public:
  NeutronHPphysics(const G4String& name="neutron");
  ~NeutronHPphysics();

public:
  virtual void ConstructParticle() { };
  virtual void ConstructProcess();

public:
  void SetThermalPhysics(G4bool flag) {fThermal = flag;};

private:
  void DefineCommands();
  G4bool fThermal;
  G4GenericMessenger* fMessenger;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
