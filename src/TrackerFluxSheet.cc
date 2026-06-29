#include "TrackerFluxSheet.hh"

//--------------------------------------------------------------------
TrackerFluxSheet::TrackerFluxSheet(G4String name)
	: G4VSensitiveDetector(name)
{
	itInteracted = false;
	fromPrimaryNeutron = false;
}

//--------------------------------------------------------------------

TrackerFluxSheet::~TrackerFluxSheet()
{
}

//--------------------------------------------------------------------

void TrackerFluxSheet::Initialize(G4HCofThisEvent *)
{
	itInteracted = false;
	fromPrimaryNeutron = false;
}
//--------------------------------------------------------------------

G4bool TrackerFluxSheet::ProcessHits(G4Step *aStep, G4TouchableHistory *)
{

	G4int eventID = G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID();
	auto *analysisManager = G4AnalysisManager::Instance();

	G4double energyDeposited = aStep->GetTotalEnergyDeposit();

	const G4DynamicParticle *aParticle = aStep->GetTrack()->GetDynamicParticle();
	const G4int trackID = aStep->GetTrack()->GetTrackID();
	const G4String name = aParticle->GetDefinition()->GetParticleName();
	const G4double len = aStep->GetStepLength();

	const G4int parentID = aStep->GetTrack()->GetParentID(); // Should be 0 for a primary neutron

	G4String vname = aStep->GetPreStepPoint()->GetPhysicalVolume()->GetName();

	G4TrackStatus TrackStatus;
	TrackStatus = aStep->GetTrack()->GetTrackStatus();

	G4ThreeVector pos = aStep->GetTrack()->GetPosition();
	G4double kineticEnergy = aStep->GetTrack()->GetKineticEnergy();
	G4double timeOfFlight = aStep->GetTrack()->GetGlobalTime();

	if ((parentID == 0) && (name == "neutron"))
	{
		fromPrimaryNeutron = true;
	}

	// Check that the current particle has not interacted with layer2 before
	if (itInteracted == false)
	{
		// All events
		analysisManager->FillNtupleDColumn(3, 0, kineticEnergy);
		analysisManager->FillNtupleDColumn(3, 1, pos[0]);
		analysisManager->FillNtupleDColumn(3, 2, pos[1]);
		analysisManager->FillNtupleDColumn(3, 3, pos[2]);
		analysisManager->FillNtupleDColumn(3, 4, timeOfFlight);
		analysisManager->AddNtupleRow(3);

		// Neutron events only
		if (fromPrimaryNeutron == true)
		{
			analysisManager->FillNtupleDColumn(4, 0, kineticEnergy);
			analysisManager->FillNtupleDColumn(4, 1, pos[0]);
			analysisManager->FillNtupleDColumn(4, 2, pos[1]);
			analysisManager->FillNtupleDColumn(4, 3, pos[2]);
			analysisManager->FillNtupleDColumn(4, 4, timeOfFlight);
			analysisManager->AddNtupleRow(4);
		}

		// G4cout << "Layer2 Interaction Registered for: " << name << ", Kinetic Energy: " << kineticEnergy << ", Position: " << pos[0] << " " << pos[1] << " " << pos[2] << G4endl;
	}
	itInteracted = true;
	return true;
}
//--------------------------------------------------------------------

void TrackerFluxSheet::EndOfEvent(G4HCofThisEvent *HCE)
{
}
