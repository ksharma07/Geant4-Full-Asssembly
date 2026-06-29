// PROGRAM neutron_cross_sections

#include "G4RunManager.hh"
#include "G4MTRunManager.hh"
#include "G4RunManagerFactory.hh"
#include "G4UImanager.hh"
#include "G4UIExecutive.hh"
#include "G4VisExecutive.hh"
#include "DetectorConstruction.hh"
#include "PrimaryGeneratorAction.hh"
#include "EventAction.hh"
#include "RunAction.hh"
#include "ActionInitialization.hh"
#include "Shielding.hh"
#include "physics.hh"
#include <cstdlib>

int main(int argc, char **argv)
{

  // Set directory containing the materials.cms database file
  setenv("CCAL_GLOBALPATH", "./dataglobal", true);

  // Seed the random number generator manually
  G4long myseed = time(NULL);
  CLHEP::HepRandom::setTheSeed(myseed);
  G4cout << " Seed set to  " << myseed << G4endl;

#ifdef G4MULTITHREADED
  G4int nThreads = 0;
#endif

  // Default number of threads
  nThreads = 1;

  // Construct the default run manager
  auto runManager = G4RunManagerFactory::CreateRunManager(G4RunManagerType::Default);
#ifdef G4MULTITHREADED
  if (nThreads > 0)
  {
    runManager->SetNumberOfThreads(nThreads);
  }
#endif

  runManager->SetUserInitialization(new DetectorConstruction());
  //runManager->SetUserInitialization(new Shielding());
  runManager->SetUserInitialization(new MyPhysicsList());
  runManager->SetUserInitialization(new ActionInitialization());

  G4UIExecutive *session = nullptr;
  if (argc == 1) // Define UI session for interactive mode.
  {
    session = new G4UIExecutive(argc, argv);
  }

  auto *visManager = new G4VisExecutive;
  visManager->Initialize();

  // get the pointer to the UI manager and set verbosities
  auto *UI = G4UImanager::GetUIpointer();

  if (session) // Define UI session for interactive mode.
  {

    //   Customize the G4UIXm menubar with a macro file :
    UI->ApplyCommand("/control/execute gui/gui.mac");
    UI->ApplyCommand("/control/execute gui/init_vis.mac");

    session->SessionStart();
    delete session;
  }
  else // Batch mode
  {
    G4String command = "/control/execute ";
    G4String fileName = argv[1];
    UI->ApplyCommand(command + fileName);
  }

  delete visManager;
  delete runManager;
  return 0;
}
