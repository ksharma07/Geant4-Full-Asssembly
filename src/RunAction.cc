#include "RunAction.hh"
#include <fstream>

RunAction::RunAction(): G4UserRunAction(), totalEvents(0)
{
  // set printing event number per each event
  // Leave run progress control to macro (/run/printProgress)
  G4RunManager::GetRunManager()->SetPrintProgress(0);

  auto *analysisManager = G4AnalysisManager::Instance();

  analysisManager->SetVerboseLevel(0);

  analysisManager->SetNtupleMerging(true);
  analysisManager->CreateNtuple("AllScintillatorDetEvents", "AllScintillatorDetEvents");
  analysisManager->CreateNtupleIColumn("eventID");
  analysisManager->CreateNtupleDColumn("posX");
  analysisManager->CreateNtupleDColumn("posY");
  analysisManager->CreateNtupleDColumn("posZ");
  analysisManager->CreateNtupleDColumn("Energy");
  analysisManager->CreateNtupleDColumn("scatteringAngle");  
  analysisManager->FinishNtuple(0);

  analysisManager->CreateNtuple("AllScintillatorDetPostEvents", "AllScintillatorDetPostEvents");
  analysisManager->CreateNtupleDColumn("totalEventEnergy");
  analysisManager->CreateNtupleDColumn("xPosCentroid");
  analysisManager->CreateNtupleDColumn("timeOfFlight");
  analysisManager->FinishNtuple(1);

  analysisManager->CreateNtuple("NeutronsScintillatorDetPostEvents", "NeutronsScintillatorDetPostEvents");
  analysisManager->CreateNtupleDColumn("totalEventEnergy");
  analysisManager->CreateNtupleDColumn("xPosCentroid");
  analysisManager->CreateNtupleDColumn("timeOfFlight");
  analysisManager->CreateNtupleDColumn("scatteringAngle");
  analysisManager->FinishNtuple(2);

  analysisManager->CreateNtuple("AllLayer2Events", "AllFluxSheetEvents");
  analysisManager->CreateNtupleDColumn("kineticEnergy");
  analysisManager->CreateNtupleDColumn("posX");
  analysisManager->CreateNtupleDColumn("posY");
  analysisManager->CreateNtupleDColumn("posZ");
  analysisManager->CreateNtupleDColumn("timeOfFlight");
  analysisManager->FinishNtuple(3);

  analysisManager->CreateNtuple("NeutronLayer2EventsOnly", "NeutronLayer2EventsOnly");
  analysisManager->CreateNtupleDColumn("kineticEnergy");
  analysisManager->CreateNtupleDColumn("posX");
  analysisManager->CreateNtupleDColumn("posY");
  analysisManager->CreateNtupleDColumn("posZ");
  analysisManager->CreateNtupleDColumn("timeOfFlight");
  analysisManager->FinishNtuple(4);
  /*
  // Open the ROOT file for output
  analysisManager->OpenFile("neutronData.root");

  // Create an ntuple with columns for both detectors
  analysisManager->CreateNtuple("NeutronData", "Neutron interaction data");
  
  // Detector 1 columns
  analysisManager->CreateNtupleDColumn("Det1E");  // Energy deposited
  analysisManager->CreateNtupleDColumn("Det1T");  // Time
  analysisManager->CreateNtupleIColumn("Det1EventID");  // Event ID
  analysisManager->CreateNtupleDColumn("Det1X");  // X position
  analysisManager->CreateNtupleDColumn("Det1Y");  // Y position
  analysisManager->CreateNtupleDColumn("Det1Z");  // Z position

  // Detector 2 columns
  analysisManager->CreateNtupleDColumn("Det2E");  // Energy deposited
  analysisManager->CreateNtupleDColumn("Det2T");  // Time
  analysisManager->CreateNtupleIColumn("Det2EventID");  // Event ID
  analysisManager->CreateNtupleDColumn("Det2X");  // X position
  analysisManager->CreateNtupleDColumn("Det2Y");  // Y position
  analysisManager->CreateNtupleDColumn("Det2Z");  // Z position

  analysisManager->FinishNtuple();
  */
  // Create the ROOT file and tree
  fRootFile = new TFile("runoutput.root", "RECREATE");
  fTree = new TTree("tree", "Event-by-Event Run Information");

  // Branches for Run information
  fTree->Branch("totalEvents", &totalEvents, "totalEvents/I");
}

RunAction::~RunAction()
{
  fRootFile->Write();
  fRootFile->Close();
  delete fRootFile;
}

void RunAction::BeginOfRunAction(const G4Run *run)
{
  auto *analysisManager = G4AnalysisManager::Instance();
  G4int runID = run->GetRunID();
  std::stringstream strRunID;
  strRunID << runID;

  // Reset killed-volume log for this run
  std::ofstream("KilledVolumeNames.txt", std::ios::trunc);

  analysisManager->OpenFile("output" + strRunID.str() + ".root");
  //analysisManager->OpenFile();

  totalEvents = run->GetNumberOfEvent();
  G4cout << "==========================================" << G4endl;
  G4cout << "Run " << run->GetRunID() << " | Total Events: " << totalEvents << G4endl;
  G4cout << "==========================================" << G4endl;
}

void RunAction::EndOfRunAction(const G4Run *run)
{
  auto *analysisManager = G4AnalysisManager::Instance();
  analysisManager->Write();
  analysisManager->CloseFile();
  G4int runID = run->GetRunID();
  G4cout << "Finishing Run: " << runID << G4endl;

  // Print run summary
  G4cout << "==========================================" << G4endl;
  G4cout << "End of Run " << run->GetRunID() << " | Total Events Processed: " << totalEvents << G4endl;
  G4cout << "==========================================" << G4endl;

  totalEvents++;
  fTree->Fill();
}
