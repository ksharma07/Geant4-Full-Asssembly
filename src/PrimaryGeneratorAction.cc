#include "PrimaryGeneratorAction.hh"
#include <cmath>
#include "EventAction.hh"
/*
PrimaryGeneratorAction::PrimaryGeneratorAction()
{

  n_particle = 1;
  particleGun = new G4ParticleGun(n_particle);
  KE = 2.45 * MeV;
  direction.setX(0.);
  direction.setY(0.);
  direction.setZ(1.);
  gun_position.setX(0.);
  gun_position.setY(0.);
  //G4double r = 0.75 * CLHEP::cm * std::sqrt(G4UniformRand());
  //G4double angle = 360.0 * CLHEP::deg * G4UniformRand();
  //gun_position.setX(r * std::cos(angle));
  //gun_position.setY(r * std::sin(angle));
  gun_position.setZ(-75 * CLHEP::cm);
  gun_position.setZ(-75 * CLHEP::cm);
}

PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
  delete particleGun;
}

void PrimaryGeneratorAction::GeneratePrimaries(G4Event *anEvent)
{

  particleTable = G4ParticleTable::GetParticleTable();
  //direction = G4RandomDirection(0.); //Limit random direction to hemisphere directed at collimator face
  //set angle on beam mom
  G4double theta = 1.0 * CLHEP::deg * G4UniformRand();
  G4double phi = 360.0 * CLHEP::deg * G4UniformRand();
  direction.set(std::sin(theta) * std::cos(phi), std::sin(theta) * std::sin(phi), std::cos(theta));
  //
  particleGun->SetParticleDefinition(particleTable->FindParticle("neutron"));
  particleGun->SetParticleMomentumDirection(direction);
  particleGun->SetParticleEnergy(KE);
  particleGun->SetParticlePosition(gun_position);

  // G4cout<<" +++++ Generating an event "<<G4endl;
  particleGun->GeneratePrimaryVertex(anEvent);
  // G4cout<<" +++++ Out Generate Primaries "<<G4endl;
}
*/
PrimaryGeneratorAction::PrimaryGeneratorAction()
{
  n_particle = 1;
  particleGun = new G4ParticleGun(n_particle); //one primary vertex per event
}
PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
  delete particleGun;
}

void PrimaryGeneratorAction::GeneratePrimaries(G4Event *anEvent)
{
  G4ParticleTable *particleTable = G4ParticleTable::GetParticleTable();
  
  G4ParticleDefinition *particle = particleTable->FindParticle("neutron");
  int eventID = anEvent->GetEventID();
  /*
  double collimatorR=0.75*cm;
  double X=1;
  double Y=1.1;
  while (sqrt(X*X+Y*Y)>1) {
    X=G4UniformRand();
    Y=G4UniformRand();
    std::cout<<X<<" thisIsIt "<<Y<<std::endl;
  }
  double r = std::sqrt(G4UniformRand()) * collimatorR;
  double rtheta = 2 * CLHEP::pi * G4UniformRand();

  double X = r * std::cos(rtheta);
  double Y = r * std::sin(rtheta);
  double Z = -90.0 *cm;
  G4ThreeVector pos(X,Y,Z); //starting position of particle gun
  
  
  G4ThreeVector mom(0.,0,1.00);
  particleGun->SetParticlePosition(pos);
  particleGun->SetParticleMomentumDirection(mom.unit());
  particleGun->SetParticleEnergy(2.45*MeV);
  particleGun->SetParticleDefinition(particle);
  particleGun->GeneratePrimaryVertex(anEvent);
  */

  double collimatorRadius=1.0605*cm;//5.0*cm;//
  
  EventAction *eventAction = static_cast<EventAction*>(G4EventManager::GetEventManager()->GetUserEventAction());
  G4double time = eventID * 1.0 * ns;

  //for (G4int i = 0; i < n_particle; ++i)
  //{
    double r = std::sqrt(G4UniformRand()) * collimatorRadius;
    double rtheta = 2 * CLHEP::pi * G4UniformRand();

    double X = r * std::cos(rtheta);
    double Y = r * std::sin(rtheta);
    double Z = 400.0 *cm;//-90cm
    G4ThreeVector pos(X,Y,-Z); //starting random position of particle gun in a circle of radius 0.75cm
    //G4ThreeVector pos(0.0,0.0,-Z);

    G4double theta = atan(75*cm/Z) * G4UniformRand();//this randomness ensures angle between (-theta,theta)
    //G4double theta = 0.5 * CLHEP::pi * G4UniformRand();//this randomness ensures angle between (0,90) degrees
    G4double phi = 2* CLHEP::pi * G4UniformRand();//both in radians right now
    //%%%%%%%%%%%%%%%%%%%%%%%%%
    //G4cout << "Primary theta: " << theta / CLHEP::deg << " deg, phi: " << phi / CLHEP::deg << " deg" << G4endl;
    //%%%%%%%%%%%%%%%%%%%%%%%%%
    if (eventAction) {
      eventAction->primaryTheta = theta / CLHEP::deg;
      eventAction->primaryPhi = phi / CLHEP::deg;
      eventAction->SetPrimaryTime(time);
      //G4int trackID = particle->GetTrackID();
      //eventAction->primaryThetaMap[trackID]= theta / CLHEP::deg;
      //eventAction->primaryPhiMap[trackID]= phi / CLHEP::deg;
    }
    //direction.set(std::sin(theta) * std::cos(phi), std::sin(theta) * std::sin(phi), std::cos(theta));
    //G4ThreeVector mom(direction);
    G4ThreeVector mom(0.,0,1.00); //assign starting direction of particle gun
    
    particleGun->SetParticlePosition(pos);
    particleGun->SetParticleMomentumDirection(mom.unit());
    particleGun->SetParticleEnergy(2.45*MeV);
    particleGun->SetParticleDefinition(particle);
    particleGun->SetParticleTime(time);
    particleGun->GeneratePrimaryVertex(anEvent);
  //}
}