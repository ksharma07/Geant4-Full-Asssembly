#include "TrackerNeutronSD.hh"

//--------------------------------------------------------------------
TrackerNeutronSD::TrackerNeutronSD(G4String name)
	: G4VSensitiveDetector(name)
{
	totalEnergyDep = 0.;
	numInteractions = 0.;
	xPosCentroid = 0.;
	itInteracted = false;
	fromPrimaryNeutron = false;
}

//--------------------------------------------------------------------

TrackerNeutronSD::~TrackerNeutronSD()
{
}

//-----Reset the variables for each event---------------------------------------------------------------

void TrackerNeutronSD::Initialize(G4HCofThisEvent *)
{

	totalEnergyDep = 0.;
	numInteractions = 0.;
	xPosCentroid = 0.;
	itInteracted = false;
	fromPrimaryNeutron = false;
}
//--------------------------------------------------------------------

G4bool TrackerNeutronSD::ProcessHits(G4Step *aStep, G4TouchableHistory *)
{

	G4int eventID = G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID();
	auto *analysisManager = G4AnalysisManager::Instance();

	G4double energyDeposited = aStep->GetTotalEnergyDeposit(); // Energy lost to the detector material in the interaction

	const G4DynamicParticle *aParticle = aStep->GetTrack()->GetDynamicParticle();
	const G4int trackID = aStep->GetTrack()->GetTrackID();//Not used. In case of change of particke type >1 else ==1 for initial particle "neutron"
	const G4String name = aParticle->GetDefinition()->GetParticleName();
	const G4double len = aStep->GetStepLength();

	const G4int parentID = aStep->GetTrack()->GetParentID(); // Should be 0 for a primary neutron

	G4String vname = aStep->GetPreStepPoint()->GetPhysicalVolume()->GetName();//Physical placement name

	G4TrackStatus TrackStatus;
	TrackStatus = aStep->GetTrack()->GetTrackStatus();//Not used. Status of neutron track. for shielding if a neutron gets absorbed or not.

	G4ThreeVector pos = aStep->GetTrack()->GetPosition();
	//Energy Deposition
	G4double interactionEnergy = aStep->GetTotalEnergyDeposit();
	G4double deltaEnergy = aStep->GetDeltaEnergy();
	G4double initialEnergy = aStep->GetPreStepPoint()->GetKineticEnergy();

	G4double timeOfFlight = aStep->GetTrack()->GetGlobalTime();

	G4ThreeVector p_initial = aStep->GetPreStepPoint()->GetMomentumDirection();
	G4ThreeVector p_final = aStep->GetPostStepPoint()->GetMomentumDirection();

	G4double scatteringAngle = p_initial.angle(p_final);  // Angle in radians
	scatteringAngle = scatteringAngle * 180.0 / CLHEP::pi;  // Convert to degrees


	// If the current interaction had an energy deposit in the detector
	if (energyDeposited > 0)
	{
		totalEnergyDep += energyDeposited;																				// Add the energy to the current total energy of the event
		numInteractions = numInteractions + 1;																			// Add +1 to the number of total interactions in the detector for the current event
		xPosCentroid = (xPosCentroid * ((numInteractions - 1) / numInteractions)) + (double(pos[0]) / numInteractions); // Running computation of the centroid of interactions
	}

	// Check if event is a result of a primary neutron
	if ((parentID == 0) && (name == "neutron"))
	{
		fromPrimaryNeutron = true;
	}

	// Individual interactions regardless of source and material
	analysisManager->FillNtupleIColumn(0, 0, eventID);
	analysisManager->FillNtupleDColumn(0, 1, pos[0]);
	analysisManager->FillNtupleDColumn(0, 2, pos[1]);
	analysisManager->FillNtupleDColumn(0, 3, pos[2]);
	analysisManager->FillNtupleDColumn(0, 4, interactionEnergy);
	analysisManager->FillNtupleDColumn(0, 5, scatteringAngle);
	analysisManager->AddNtupleRow(0);

	//G4cout << "Detector Interaction Registered for: " << name << " inside detector volume " << vname << ", Interaction Energy: " << interactionEnergy << ", Initial Kinetic Energy: " << initialEnergy << ", Delta Energy: " << deltaEnergy << ", Position: " << pos[0] << " " << pos[1] << " " << pos[2] << G4endl;

	itInteracted = true;
	return true;
}
//--------------------------------------------------------------------

void TrackerNeutronSD::EndOfEvent(G4HCofThisEvent *HCE)
{
	
	if (totalEnergyDep > 0)
	{
		// All interactions that deposit energy
		//G4cout << "Total Deposited Energy: " << totalEnergyDep << ", Total Number of Interactions in Event: " << numInteractions << G4endl;
		auto *analysisManager = G4AnalysisManager::Instance();
		analysisManager->FillNtupleDColumn(1, 0, totalEnergyDep);
		analysisManager->FillNtupleDColumn(1, 1, xPosCentroid);
		analysisManager->FillNtupleDColumn(1, 2, timeOfFlight);
		analysisManager->AddNtupleRow(1);

		// Neutron events only
		if (fromPrimaryNeutron == true)
		{
			analysisManager->FillNtupleDColumn(2, 0, totalEnergyDep);
			analysisManager->FillNtupleDColumn(2, 1, xPosCentroid);
			analysisManager->FillNtupleDColumn(2, 2, timeOfFlight);
			analysisManager->FillNtupleDColumn(2, 3, scatteringAngle);
			analysisManager->AddNtupleRow(2);
		}
	}

	if (itInteracted == true)
	{
		G4cout << G4endl;
	}
}
