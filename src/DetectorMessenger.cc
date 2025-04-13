#include "DetectorMessenger.hh"
#include "DetectorConstruction.hh"

DetectorMessenger::DetectorMessenger(DetectorConstruction* Det) 
    : G4UImessenger(), 
    fDetectorConstruction(Det) {
    
    fPrismDir = new G4UIdirectory("/prism/");
    fPrismDir->SetGuidance("Prism detector control");

    fMaterialCmd = new G4UIcmdWithAString("/prism/setMaterial", this);
    fMaterialCmd->SetGuidance("Select material for the prism (FusedSilica, PMMA, or H9KL).");
    fMaterialCmd->SetParameterName("choice",false);
    fMaterialCmd->SetCandidates("FusedSilica PMMA H9KL");
    fMaterialCmd->AvailableForStates(G4State_Idle);
}

DetectorMessenger::~DetectorMessenger() {
    delete fMaterialCmd;
    delete fPrismDir;
}

void DetectorMessenger::SetNewValue(G4UIcommand* command, G4String newValue) {
    if (command == fMaterialCmd) {
        G4cout << "Changing material to: " << newValue << G4endl;
        fDetectorConstruction->SetMaterial(newValue);
    }
}