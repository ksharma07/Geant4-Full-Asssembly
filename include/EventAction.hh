#ifndef EventAction_h
#define EventAction_h 1

#include "G4UserEventAction.hh"
#include "G4Event.hh"
#include "G4TrajectoryContainer.hh"
#include "G4VTrajectory.hh"
#include "G4VVisManager.hh"
#include "G4UnitsTable.hh"
#include "Randomize.hh"
#include "G4ios.hh"
#include "globals.hh"
#include "G4UnitsTable.hh"
#include <iomanip>
#include <iostream>
#include <vector>
#include <map>
#include "globals.hh"

#include "TrackerNeutronSD.hh"
#include "TFile.h"
#include "TTree.h"
#include "G4ThreeVector.hh"

using namespace std;

class G4Event;

class EventAction : public G4UserEventAction
{
public:
  EventAction();
  ~EventAction() override;

  int eventID;

  G4double primaryTheta;
  G4double primaryPhi;
  G4double primaryTime;
  //std::map<G4int, G4double> primaryThetaMap;
  //std::map<G4int, G4double> primaryPhiMap;
  G4int scintID;
  G4double scintL1X;
  G4double scintL1Y;
  G4double scintL1Z;

  G4int barID;
  G4double scintL2X;
  G4double scintL2Y;
  G4double scintL2Z;

  G4int det1Interactions;
  G4double det1Energy;
  G4double det1PositionX;
  G4double det1PositionY;
  G4double det1PositionZ;
  G4double det1Time;
  G4double det1ScatAngle;

  std::vector<G4double> det1Energies;
  std::vector<G4double> det1AllPosX;
  std::vector<G4double> det1AllPosY;
  std::vector<G4double> det1AllPosZ;
  std::vector<G4double> det1IDPosX;
  std::vector<G4double> det1IDPosY;
  std::vector<G4double> det1IDPosZ;
  std::vector<G4double> det1Times;
  void ClearL1Hits() {
    det1Energies.clear();
    det1AllPosX.clear();
    det1AllPosY.clear();
    det1AllPosZ.clear();
    det1IDPosX.clear();
    det1IDPosY.clear();
    det1IDPosZ.clear();
    det1Times.clear();
  }

  G4int det2Interactions;
  G4double det2Energy;
  G4double det2PositionX;
  G4double det2PositionY;
  G4double det2PositionZ;
  G4double det2Time;

  std::vector<G4double> det2Energies;
  std::vector<G4double> det2AllPosX;
  std::vector<G4double> det2AllPosY;
  std::vector<G4double> det2AllPosZ;
  std::vector<G4double> det2IDPosX;
  std::vector<G4double> det2IDPosY;
  std::vector<G4double> det2IDPosZ;
  std::vector<G4double> det2Times;
  void ClearL2Hits() {
    det2Energies.clear();
    det2AllPosX.clear();
    det2AllPosY.clear();
    det2AllPosZ.clear();
    det2IDPosX.clear();
    det2IDPosY.clear();
    det2IDPosZ.clear();
    det2Times.clear();
  }
  
  G4int heatInteractions;
  std::vector<G4double> cupltEnergies;
  void ClearCuHits(){
    cupltEnergies.clear();
  }

  G4int allprimarykills;
  G4int cuprimarykills;
  G4int mtprimarykills;
  G4int shldprimarykills;
  G4int gamshldprimarykills;
  G4int wallprimarykills;

  G4int n2Interactions;
  std::vector<G4double> secL1Energies;
  std::vector<G4double> secL2Energies;
  void ClearN2Hits(){
    secL1Energies.clear();
    secL2Energies.clear();
  }

  G4int shldInteractions, detshldInteractions, gamshldInteractions, wallInteractions;
  G4double shldEnergy, detshldEnergy, gamshldEnergy;
  G4double shldPositionZ, gamPositionZ;
  G4double shldTime, gamTime;
  G4int otherVolumes;

  void BeginOfEventAction(const G4Event *) override;
  void EndOfEventAction(const G4Event *) override;

  void IncrementDet1Interactions(); 
  void IncrementDet2Interactions(); 
  void IncrementShldInteractions();
  void IncrementDetShldInteractions();
  void IncrementGamShldInteractions();
  void IncrementWallInteractions();
  void IncrementCuPtInteractions();
  void IncrementOtherVolumes();

  void AllKillCounter();
  void CuKillCounter();
  void MtKillCounter();
  void ShldKillCounter();
  void GamShldKillCounter();
  void WallKillCounter();
  void SecondaryInteractions();

  // Getter functions if need to call these values outside eventaction like in stepping fn for time
  G4double GetDet1Energy() const { return det1Energy; }
  G4double GetDet2Energy() const { return det2Energy; }
  G4double GetDetShldEnergy() const { return detshldEnergy; }
  G4double GetGamShldEnergy() const { return shldEnergy; }
  G4double GetShldEnergy() const { return gamshldEnergy; }
  G4double GetShldPositionZ() const { return shldPositionZ; }
  G4double GetGamPositionZ() const { return gamPositionZ; }
  G4double GetShldTime() const { return shldTime; }
  G4double GetGamTime() const { return gamTime; }
  //G4ThreeVector GetDet1Position() const { return det1Position; }
  //G4ThreeVector GetDet2Position() const { return det2Position; }
  G4double GetDet1Time() const { return det1Time; }
  G4double GetDet2Time() const { return det2Time; }
 
  // Setter functions
  void AddDet1Energy(G4double energy) { det1Energy += energy; }
  void AddDet2Energy(G4double energy) { det2Energy += energy; }
  void AddShldEnergy(G4double energy) { shldEnergy += energy; }
  void AddDetShldEnergy(G4double energy) { detshldEnergy += energy; }
  void AddGamShldEnergy(G4double energy) { gamshldEnergy += energy; }

  void SetLayer1ID(G4int ID) { scintID = ID; }
  void SetScintX(G4double posx) { scintL1X = posx; }
  void SetScintY(G4double posy) { scintL1Y = posy; }
  void SetScintZ(G4double posz) { scintL1Z = posz; }

  void SetLayer2ID(G4int ID) { barID = ID; }
  void SetBarX(G4double posx) { scintL2X = posx; }
  void SetBarY(G4double posy) { scintL2Y = posy; }
  void SetBarZ(G4double posz) { scintL2Z = posz; }

  void SetPrimaryTheta(G4double val) { primaryTheta = val; }
  void SetPrimaryPhi(G4double val) { primaryPhi = val; }
  void SetPrimaryTime(G4double val) { primaryTime = val; }
  void SetDet1PositionX(G4double pos1x) { det1PositionX = pos1x; }
  void SetDet1PositionY(G4double pos1y) { det1PositionY = pos1y; }
  void SetDet1PositionZ(G4double pos1z) { det1PositionZ = pos1z; }
  void SetDet2PositionX(G4double pos2x) { det2PositionX = pos2x; }
  void SetDet2PositionY(G4double pos2y) { det2PositionY = pos2y; }
  void SetDet2PositionZ(G4double pos2z) { det2PositionZ = pos2z; }
  void SetShldPositionZ(G4double poshz) { shldPositionZ = poshz; }
  void SetGamPositionZ(G4double posgz) { gamPositionZ = posgz; }
  void SetShldTime(G4double time) { shldTime = time; }
  void SetGamTime(G4double time) { gamTime = time; }
  void SetDet1Time(G4double time) { det1Time = time; }
  void SetScatAngle(G4double angle) { det1ScatAngle = angle; }
  void SetDet2Time(G4double time) { det2Time = time; }
  /*G4int GetDet1Interactions() { return det1Interactions; }
  G4int GetDet2Interactions() { return det2Interactions; }
  G4int GetOtherVolumes() { return otherVolumes; }*/
private: 
  TFile* fRootFile;
  TTree* fTree;
};

#endif
