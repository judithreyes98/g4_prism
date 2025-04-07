#include <iostream>

#include "G4RunManager.hh"
#include "G4MTRunManager.hh"
#include "G4UImanager.hh"
#include "G4VisManager.hh"
#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"

#include "PhysicsList.hh"
#include "DetectorConstruction.hh"
#include "ActionInitialization.hh"

int main(int argc, char** argv){

    G4UIExecutive *ui = new G4UIExecutive(argc,argv);

    #ifdef G4MULTITHREADED 
        G4MTRunManager *runManager = new G4MTRunManager;
    #else
        G4RunManager *runManager = new G4RunManager;
    #endif

    // Physics list
    runManager -> SetUserInitialization(new PhysicsList());

    // Detector Construction
    runManager -> SetUserInitialization(new DetectorConstruction());

    // Action Initialization
    runManager -> SetUserInitialization(new ActionInitialization());

    G4VisManager* visManager = new G4VisExecutive();

    visManager -> Initialize();

    G4UImanager *UImanager = G4UImanager::GetUIpointer();

    UImanager->ApplyCommand("/control/execute vis.mac");

    ui -> SessionStart();

    return 0;

}