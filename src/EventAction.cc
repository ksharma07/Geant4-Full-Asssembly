#include "EventAction.hh"
#include "G4Event.hh"
#include "G4EventManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4ios.hh"


EventAction::EventAction()
: G4UserEventAction(), det1Interactions(0), det2Interactions(0), shldInteractions(0), detshldInteractions(0), wallInteractions(0), gamshldInteractions(0) , heatInteractions(0), otherVolumes(0), n2Interactions(0), allprimarykills(0),  cuprimarykills(0), mtprimarykills(0)
{
    fRootFile = new TFile("eventoutput.root", "RECREATE");
    fTree = new TTree("Events", "Neutron Scattering Data");

    fTree->Branch("eventID",       &eventID, "eventID/I");
    fTree->Branch("primaryTheta",  &primaryTheta, "primaryTheta/D");
    fTree->Branch("primaryPhi",    &primaryPhi, "primaryPhi/D");
    fTree->Branch("primaryTime",   &primaryTime, "primaryTime/D");

    fTree->Branch("det1Interactions", &det1Interactions, "det1Interactions/I");
    fTree->Branch("det1Energy",       &det1Energy, "det1Energy/D");//summed up energy
    //First hit fill only//
    fTree->Branch("det1ScatAngle", &det1ScatAngle, "det1ScatAngle/D");
    fTree->Branch("det1Time",      &det1Time, "det1Time/D");
    fTree->Branch("scintID",       &scintID, "scintID/I");
    fTree->Branch("scintL1X",      &scintL1X);  
    fTree->Branch("scintL1Y",      &scintL1Y);
    fTree->Branch("scintL1Z",      &scintL1Z);
    fTree->Branch("det1PositionX", &det1PositionX);  
    fTree->Branch("det1PositionY", &det1PositionY);
    fTree->Branch("det1PositionZ", &det1PositionZ);
    //All hits in event lists//
    fTree->Branch("det1IDPosX",    &det1IDPosX);
    fTree->Branch("det1IDPosY",    &det1IDPosY);
    fTree->Branch("det1IDPosZ",    &det1IDPosZ);
    fTree->Branch("det1AllPosX",   &det1AllPosX);
    fTree->Branch("det1AllPosY",   &det1AllPosY);
    fTree->Branch("det1AllPosZ",   &det1AllPosZ);
    fTree->Branch("det1Energies",  &det1Energies);
    fTree->Branch("det1Times",     &det1Times);

    fTree->Branch("det2Interactions", &det2Interactions, "det2Interactions/I");
    fTree->Branch("det2Energy",       &det2Energy, "det2Energy/D");//summed up energy
    //First hit fill only//
    fTree->Branch("det2Time",      &det2Time, "det2Time/D");
    fTree->Branch("barID",         &barID, "barID/I");
    fTree->Branch("scintL2X",      &scintL2X);  
    fTree->Branch("scintL2Y",      &scintL2Y);
    fTree->Branch("scintL2Z",      &scintL2Z);
    fTree->Branch("det2PositionX", &det2PositionX);  
    fTree->Branch("det2PositionY", &det2PositionY);
    fTree->Branch("det2PositionZ", &det2PositionZ);
    //All hits in event lists//
    fTree->Branch("det2IDPosX",    &det2IDPosX);
    fTree->Branch("det2IDPosY",    &det2IDPosY);
    fTree->Branch("det2IDPosZ",    &det2IDPosZ);
    fTree->Branch("det2AllPosX",   &det2AllPosX);
    fTree->Branch("det2AllPosY",   &det2AllPosY);
    fTree->Branch("det2AllPosZ",   &det2AllPosZ);
    fTree->Branch("det2Energies",  &det2Energies);
    fTree->Branch("det2Times",     &det2Times);

    fTree->Branch("heatInteractions", &heatInteractions, "heatInteractions/I");
    fTree->Branch("shldInteractions", &shldInteractions, "shldInteractions/I");
    fTree->Branch("detshldInteractions", &detshldInteractions, "detshldInteractions/I");
    fTree->Branch("gamshldInteractions", &gamshldInteractions, "gamshldInteractions/I");
    fTree->Branch("wallInteractions", &wallInteractions, "wallInteractions/I");
    
    fTree->Branch("cupltEnergies",    &cupltEnergies);
    fTree->Branch("shldEnergy",       &shldEnergy, "shldEnergy/D");
    fTree->Branch("detshldEnergy",    &detshldEnergy, "detshldEnergy/D");
    fTree->Branch("gamshldEnergy",    &gamshldEnergy, "gamshldEnergy/D");

    fTree->Branch("shldPositionZ",    &shldPositionZ, "shldPositionZ/D");
    fTree->Branch("gamPositionZ",     &gamPositionZ, "gamPositionZ/D");
    fTree->Branch("shldTime",         &shldTime, "shldTime/D");
    fTree->Branch("gamTime",          &gamTime, "gamTime/D");

    fTree->Branch("otherVolumes",     &otherVolumes, "otherVolumes/I");

    fTree->Branch("allprimarykills", &allprimarykills, "allprimarykills/I");
    fTree->Branch("cuprimarykills",  &cuprimarykills, "cuprimarykills/I");
    fTree->Branch("mtprimarykills",  &mtprimarykills, "mtprimarykills/I");
    fTree->Branch("shldprimarykills",  &shldprimarykills, "shldprimarykills/I");
    fTree->Branch("gamshldprimarykills",  &gamshldprimarykills, "gamshldprimarykills/I");
    fTree->Branch("wallprimarykills",  &wallprimarykills, "wallprimarykills/I");

    fTree->Branch("n2Interactions",  &n2Interactions, "n2Interactions/I");//sec Cu interactions
    fTree->Branch("secL1Energies",   &secL1Energies);
    fTree->Branch("secL2Energies",   &secL2Energies);
}

EventAction::~EventAction()
{
    fRootFile->Write();
    fRootFile->Close();
    delete fRootFile;
}

void EventAction::BeginOfEventAction(const G4Event *event)
{
    eventID = event->GetEventID();
    //primaryThetaMap.clear();
    //primaryPhiMap.clear();
    //primaryTheta = 0.;
    //primaryPhi = 0.;
    //Number of interactions and energy change in a detector are values **ACCUMULATED** over and event and hence initialised zero. 
    //Rest parameters are **SET** to a value only once over an event and so can be given out of range initial value.
    otherVolumes = 0;

    // Absolute start time assigned to match the primary gun SetParticleTime
    primaryTime = eventID * 1.0 * ns;

    det1Interactions = 0;
    det1Energy = 0.0;
    det1ScatAngle = -999.0;
    det1PositionX = -999.0;
    det1PositionY = -999.0;
    det1PositionZ = -999.0;
    det1Time = -1;
    
    scintID = -1;
    scintL1X = -999.0;
    scintL1Y = -999.0;
    scintL1Z = -999.0;

    barID = -1;
    scintL2X = -999.0;
    scintL2Y = -999.0;
    scintL2Z = -999.0;

    det2Interactions = 0;
    det2Energy = 0.0;
    det2PositionX = -999.0;
    det2PositionY = -999.0;
    det2PositionZ = -999.0;
    det2Time = -1;

    heatInteractions = 0;
    shldInteractions = 0;
    detshldInteractions = 0;
    gamshldInteractions = 0;
    wallInteractions =0;
    shldEnergy = 0.0;
    detshldEnergy = 0.0;
    shldPositionZ = -2999.0;
    gamPositionZ = -2999.0;
    gamshldEnergy = 0.0;
    shldTime = -1.0;
    gamTime = -1.0;

    allprimarykills= 0;
    cuprimarykills= 0;
    mtprimarykills= 0;
    n2Interactions = 0;
    shldprimarykills =0;
    gamshldprimarykills =0;
    wallprimarykills =0;

    ClearL1Hits();
    ClearL2Hits();
    ClearCuHits();
    ClearN2Hits();
}

void EventAction::IncrementDet1Interactions()
{
    det1Interactions++;
    /*G4cout << "##########################################"<< G4endl;
    G4cout << "Layer1 interactions: " << det1Interactions << G4endl;
    G4cout << "##########################################"<< G4endl;*/
}

// Increment method for Layer 2 interactions
void EventAction::IncrementDet2Interactions()
{
    det2Interactions++;
    /*G4cout << "##########################################"<< G4endl;
    G4cout << "Layer2 interactions: " << det2Interactions << G4endl;
    G4cout << "##########################################"<< G4endl;*/
}

void EventAction::IncrementShldInteractions()
{
    shldInteractions++;
    /*G4cout << "##########################################"<< G4endl;
    G4cout << "Shield interactions: " << shldInteractions << G4endl;
    G4cout << "##########################################"<< G4endl;*/
}

void EventAction::IncrementDetShldInteractions()
{
    detshldInteractions++;
    /*G4cout << "##########################################"<< G4endl;
    G4cout << "Detector Shield interactions: " << detshldInteractions << G4endl;
    G4cout << "##########################################"<< G4endl;*/
}

void EventAction::IncrementGamShldInteractions()
{
    gamshldInteractions++;
    /*G4cout << "##########################################"<< G4endl;
    G4cout << "Gamma Shield interactions: " << gamshldInteractions << G4endl;
    G4cout << "##########################################"<< G4endl;*/
}

void EventAction::IncrementWallInteractions()
{
    wallInteractions++;
}

void EventAction::IncrementCuPtInteractions()
{
    heatInteractions++;
}

// Increment method for interactions in other volumes
void EventAction::IncrementOtherVolumes()
{
    otherVolumes++;
    /*G4cout << "##########################################"<< G4endl;
    G4cout << "Other interactions: " << otherVolumes << G4endl;
    G4cout << "##########################################"<< G4endl;*/
}

void EventAction::AllKillCounter()
{
    allprimarykills++;
}

void EventAction::CuKillCounter()
{
    cuprimarykills++;
}

void EventAction::MtKillCounter()
{
    mtprimarykills++;
}

void EventAction::ShldKillCounter()
{
    shldprimarykills++;
}

void EventAction::GamShldKillCounter()
{
    gamshldprimarykills++;
}

void EventAction::WallKillCounter()
{
    wallprimarykills++;
}

void EventAction::SecondaryInteractions()
{
    n2Interactions++;
}

void EventAction::EndOfEventAction(const G4Event *evt)
{
    // Print event summary
    /*G4cout << "==========================================" << G4endl;
    G4cout << "Saving to tree: theta = " << primaryTheta << " deg, phi = " << primaryPhi << " deg" << G4endl;
    G4cout << "Event "           << evt->GetEventID() << " | Layer1: " << det1Interactions << " | Layer2: " << det2Interactions << " | Copper Plate: " << heatInteractions << " | Other Volumes: " << otherVolumes << " | Shield: "<< shldInteractions << G4endl;
    G4cout << "det1Energy:     " << det1Energy    << "Time: " << det1Time / CLHEP::ns << " ns"<< ", det2Energy: " << det2Energy << "Time: " << det2Time / CLHEP::ns << G4endl;
    G4cout << "det1position:   " << det1PositionX << " " << det1PositionY << " " << det1PositionZ << G4endl;
    G4cout << "scint1position: " << scintL1X      << " " << scintL1Y      << " " << scintL1Z << G4endl;
    G4cout << "det2position:   " << det2PositionX << " " << det2PositionY << " " << det2PositionZ << G4endl;
    G4cout << "scint2position: " << scintL2X      << " " << scintL2Y      << " " << scintL2Z << G4endl;
    G4cout << "Scattering angle from layer 1: " << det1ScatAngle << G4endl;
    G4cout << "Shield Energy: "  << shldEnergy << G4endl;
    G4cout << "Detector Shield Energy: "  << detshldEnergy << " Interactions: " << detshldInteractions << G4endl;
    G4cout << "Gamma Shield Energy: "  << gamshldEnergy << " Interactions: " << gamshldInteractions << G4endl;
    G4cout << "==========================================" << G4endl;*/
    
    // Progress print handled via /run/printProgress in macro
    fTree->Fill();
}
