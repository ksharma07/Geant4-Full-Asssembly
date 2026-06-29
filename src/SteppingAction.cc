#include "SteppingAction.hh"
#include "EventAction.hh" 
#include "G4Step.hh"
#include "G4Track.hh"
#include "G4SystemOfUnits.hh"
#include "G4AnalysisManager.hh"
#include "G4EventManager.hh"
#include "G4Event.hh"
#include "G4TouchableHistory.hh"
#include "G4LogicalVolume.hh"
#include "DetectorMap.hh"
#include "G4PVPlacement.hh"

SteppingAction::SteppingAction(EventAction* eventAction) 
: G4UserSteppingAction(), fEventAction(eventAction)
{   
}

SteppingAction::~SteppingAction() 
{
}

void SteppingAction::UserSteppingAction(const G4Step* step)
{
    auto* analysisManager = G4AnalysisManager::Instance();

    // Get event number
    G4int eventID = G4EventManager::GetEventManager()->GetConstCurrentEvent()->GetEventID();
    G4Track* track = step->GetTrack();
    G4double tracktime = track->GetGlobalTime();
    const G4int trackID = track->GetTrackID();
    const G4int parentID = track->GetParentID();
    G4TrackStatus TrackStatus = track->GetTrackStatus();

    EventAction* eventAction = static_cast<EventAction*>(G4EventManager::GetEventManager()->GetUserEventAction());

    if (!eventAction) {
        G4cerr << "Error: EventAction not found!" << G4endl;
        return;
    }

    // Get step information
    G4ThreeVector position = step->GetPostStepPoint()->GetPosition();
    G4double energyDeposited = step->GetTotalEnergyDeposit();
    G4double energychange = step->GetPreStepPoint()->GetKineticEnergy() - step->GetPostStepPoint()->GetKineticEnergy();
    G4double time = step->GetPostStepPoint()->GetGlobalTime();//default in ns
    G4double e_initial = step->GetPreStepPoint()->GetKineticEnergy();

    G4ThreeVector p_initial = step->GetPreStepPoint()->GetMomentumDirection();
	G4ThreeVector p_final = step->GetPostStepPoint()->GetMomentumDirection();

	G4double scatteringAngle = p_initial.angle(p_final);  // Angle in radians
	scatteringAngle = scatteringAngle * 180.0 / CLHEP::pi;

    // Get logical volume name (Detector name)
    G4LogicalVolume* logicVolume = step->GetPreStepPoint()->GetTouchableHandle()->GetVolume()->GetLogicalVolume();
    G4String volumeName = logicVolume->GetName();
    G4double volume = logicVolume->GetSolid()->GetCubicVolume();

    G4VPhysicalVolume* physVol = step->GetPreStepPoint()->GetTouchableHandle()->GetVolume();
    G4String physicalName = physVol->GetName();
    /*G4cout << "##########################################"<< G4endl;
    G4cout << "Step in volume: " << volumeName << G4endl;
    G4cout << "Track Time: "<< tracktime << " Poststep Time: "<< time << G4endl;
    G4cout << "Step postpoint position (x,y,z): "<< position[0] << ", " << position[1] << ", "<< position[2] << G4endl;
    G4cout << "##########################################"<< G4endl;*/
    
    const G4DynamicParticle *aParticle = step->GetTrack()->GetDynamicParticle();
    const G4String pname = aParticle->GetDefinition()->GetParticleName();

    G4String processName = step->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName();

    //G4cout << "Particle name: " << pname << " | Volume: " << volumeName << G4endl;

    // Check if the energy deposit is significant and if it is inside a detector
    if (energychange > 0 && pname =="neutron" && parentID==0)//all the energy lost by neutron encapsulates energy transferred: Medium (Inelastic) + H/C12 knockoff(elastic)
    {
        if (volumeName == "logicLayer1") {
            // Fill for Detector1
            //fEventAction->IncrementDet1Interactions();
            //eventAction->IncrementDet1Interactions();
            //det1Interactions++;
            /*analysisManager->FillNtupleIColumn(0, eventID); // Event ID
            analysisManager->FillNtupleDColumn(1, position.x() / mm); // X position
            analysisManager->FillNtupleDColumn(2, position.y() / mm); // Y position
            analysisManager->FillNtupleDColumn(3, position.z() / mm); // Z position
            analysisManager->FillNtupleDColumn(4, energyDeposited / MeV); // Energy deposited
            analysisManager->FillNtupleDColumn(5, time / ns); // Time of interaction
            analysisManager->AddNtupleRow();*/
            G4int blockID = physVol->GetCopyNo();
            eventAction->SetLayer1ID(blockID);
            auto it = L1IDPosMap.find(blockID);
            //if(e_initial==2.45){
            eventAction->IncrementDet1Interactions(); // Increment interaction count for detector 1
            //}
            eventAction->AddDet1Energy(energychange);  // Add energy change to detector 1
            eventAction->det1Energies.push_back(energychange);
            eventAction->det1AllPosX.push_back(position[0]);
            eventAction->det1AllPosY.push_back(position[1]);
            eventAction->det1AllPosZ.push_back(position[2]);
            eventAction->det1Times.push_back(time);
            if (it != L1IDPosMap.end()) {
                G4ThreeVector idpos = it->second;
                eventAction->det1IDPosX.push_back(idpos[0]);
                eventAction->det1IDPosY.push_back(idpos[1]);
                eventAction->det1IDPosZ.push_back(idpos[2]);
            }
            // Set only if not set already
            if (eventAction->GetDet1Time() < 0.0) {//using Getter function to ensure first timestamp in the layer else = -1 and also no previous leftover values.
                if (it != L1IDPosMap.end()) {
                    G4ThreeVector pos = it->second;
                    //G4cout << "BlockID: " << blockID << " at Position: " << "X: " << pos[0] << "Y: " << pos[1] << "Z: " << pos[2] << G4endl;
                    eventAction->SetScintX(pos[0]);
                    eventAction->SetScintY(pos[1]);
                    eventAction->SetScintZ(pos[2]);
                }

                eventAction->SetDet1PositionX(position[0]);
                eventAction->SetDet1PositionY(position[1]);
                eventAction->SetDet1PositionZ(position[2]);  // Update position for detector 1
                eventAction->SetDet1Time(time);  // Update time for detector 1
                eventAction->SetScatAngle(scatteringAngle);
            }
        }
        else if (volumeName == "logicLayer2") {
            // Fill for Detector2
            //fEventAction->IncrementDet2Interactions();
            //det2Interactions++;
            //eventAction->IncrementDet2Interactions();
            /*analysisManager->FillNtupleIColumn(6, eventID); // Event ID for Detector2
            analysisManager->FillNtupleDColumn(7, position.x() / mm); // X position for Detector2
            analysisManager->FillNtupleDColumn(8, position.y() / mm); // Y position for Detector2
            analysisManager->FillNtupleDColumn(9, position.z() / mm); // Z position for Detector2
            analysisManager->FillNtupleDColumn(10, energyDeposited / MeV); // Energy deposited for Detector2
            analysisManager->FillNtupleDColumn(11, time / ns); // Time of interaction for Detector2
            analysisManager->AddNtupleRow();*/
            // Fill for Detector 1
            G4int barID = physVol->GetCopyNo();
            eventAction->SetLayer2ID(barID);
            auto it = L2IDPosMap.find(barID);
            eventAction->IncrementDet2Interactions();  // Increment interaction count for detector 2
            eventAction->AddDet2Energy(energychange);  // Add energy change to detector 2
            eventAction->det2Energies.push_back(energychange);
            eventAction->det2AllPosX.push_back(position[0]);
            eventAction->det2AllPosY.push_back(position[1]);
            eventAction->det2AllPosZ.push_back(position[2]);
            eventAction->det2Times.push_back(time);
            if (it != L2IDPosMap.end()) {
                G4ThreeVector idpos = it->second;
                eventAction->det2IDPosX.push_back(idpos[0]);
                eventAction->det2IDPosY.push_back(idpos[1]);
                eventAction->det2IDPosZ.push_back(idpos[2]);
            }

            if (eventAction->GetDet2Time() < 0.0) {//Operations for the first interaction recognised
                if (it != L2IDPosMap.end()) {
                    G4ThreeVector pos = it->second;
                    //G4cout << "BarID: " << barID << " at Position: " << "X: " << pos[0] << "Y: " << pos[1] << "Z: " << pos[2] << G4endl;
                    eventAction->SetBarX(pos[0]);
                    eventAction->SetBarY(pos[1]);
                    eventAction->SetBarZ(pos[2]);
                }
                eventAction->SetDet2PositionX(position[0]);
                eventAction->SetDet2PositionY(position[1]);
                eventAction->SetDet2PositionZ(position[2]);  
                // Update position for detector 2
                eventAction->SetDet2Time(time);  // Update time for detector 2
            }
        }
        else if (volumeName == "logicheat") { // Heat copper plate only
            eventAction->IncrementCuPtInteractions();
            eventAction->cupltEnergies.push_back(energychange);
        }
        else if(volumeName == "logicShield"){
            eventAction->IncrementShldInteractions();
            eventAction->AddShldEnergy(energychange);
        }
        else if (volumeName == "logicDetShield" || volumeName == "logicDet2Shield") {
            eventAction->IncrementDetShldInteractions();
            eventAction->AddDetShldEnergy(energychange);
            if (eventAction->GetShldPositionZ() < -2998.0) { // Only need position of first shield interaction
                eventAction->SetShldPositionZ(position[2]);
                //G4cout << "SetShldPositionZ (step) to " << position[2] << " in " << volumeName << G4endl;
            }
            if (eventAction->GetShldTime() < 0.0) {
                eventAction->SetShldTime(time);
            }
            //eventAction->SetShldPositionZ(position[2]);
        }
        else if (volumeName == "logicGamShield" || volumeName == "logicGam2Shield") {
            eventAction->IncrementGamShldInteractions();
            eventAction->AddGamShldEnergy(energychange);
            if (eventAction->GetGamPositionZ() < -2998.0) { // Only need position of first shield interaction
                eventAction->SetGamPositionZ(position[2]);
                //G4cout << "SetGamPositionZ (step) to " << position[2] << " in " << volumeName << G4endl;
            }
            if (eventAction->GetGamTime() < 0.0) {
                eventAction->SetGamTime(time);
            }
        }
        else if (volumeName == "logicRoom"){
            eventAction->IncrementWallInteractions();
        }
        else if (volumeName != "logicLayer1" && volumeName != "logicLayer2" && volumeName != "ExpHall_log" && physicalName != "OutOfWorld") { // Any volume other than air
            //fEventAction->IncrementOtherVolumes();
            eventAction->IncrementOtherVolumes();
            //otherVolumes++;
        }
    }//primary neutron dealt with..
    else if(pname =="neutron" && parentID > 0){
        //any secondary neutrons in Cu plate
        if(energychange >0){//secondary energy in det1 and det2
            if(volumeName == "logicheat" || volumeName == "logicGamShield" || volumeName == "logicGam2Shield"){
                eventAction->SecondaryInteractions();
            }
            if(volumeName == "logicLayer1"){
                eventAction->secL1Energies.push_back(energychange);
            }
            else if(volumeName == "logicLayer2"){
                eventAction->secL2Energies.push_back(energychange);
            }
        }
    }//if secondary neutron genrated and did some coincidences
    
    if (TrackStatus == fStopAndKill && pname=="neutron" && parentID == 0) { //process can be neutronInelastic or nCapture or something else
        if(volumeName != "ExpHall_log"){
            eventAction->AllKillCounter();
            std::ofstream outfile("KilledVolumeNames.txt", std::ios::app);
            if (outfile.is_open()) {
                outfile << volumeName << "\n";
                outfile.close();
            }
        }
        if (volumeName == "logicheat") {  // Primary particle killed in Cu Plate
            eventAction->CuKillCounter();
        }
        if (volumeName == "logicmtL" || volumeName == "logicTopSlab" || volumeName == "logicMiddleSlab" || volumeName == "logicBottomSlab") {  // Primary particle killed in Mount Plate
            eventAction->MtKillCounter();
        }
        if (volumeName == "logicDetShield" || volumeName == "logicDet2Shield") {  // Primary particle killed in B-HDPE Plate
            /*if (eventAction->GetShldPositionZ() < -2998.0) {
                eventAction->SetShldPositionZ(step->GetPreStepPoint()->GetPosition()[2]);
            }*/
            if (trackID == 1 && eventAction->shldprimarykills == 0) {
                eventAction->ShldKillCounter();
            }
        }
        if (volumeName == "logicGamShield" || volumeName == "logicGam2Shield") {  // Primary particle killed in Mount Plate
            /*if (eventAction->GetGamPositionZ() < -2998.0) {
                eventAction->SetGamPositionZ(step->GetPreStepPoint()->GetPosition()[2]);
            }*/
            if (trackID == 1 && eventAction->gamshldprimarykills == 0) {
                eventAction->GamShldKillCounter();
            }
        }
        if (volumeName == "logicRoom") {  // Primary particle killed in Mount Plate
            eventAction->WallKillCounter();
        }
    }//check if primary neutron killed in any volume 
    
    /*if (energychange > 0 && pname == "gamma") {
        if (volumeName == "logicGamShield" || volumeName == "logicGam2Shield") {
            eventAction->IncrementGamShldInteractions();
            eventAction->AddGamShldEnergy(energychange);
        }
    }*/
}

