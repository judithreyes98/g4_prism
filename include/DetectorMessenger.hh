#ifndef DETECTORMESSENGER_HH
#define DETECTORMESSENGER_HH

#include "G4UImessenger.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"
#include "globals.hh"

class DetectorConstruction;  // forward declaration
// if we include DetectorConstruction.hh here, the compiler will enter 
// an infinite loop of circular dependencies

class DetectorMessenger : public G4UImessenger {
    public:
        DetectorMessenger(DetectorConstruction*);
        virtual ~DetectorMessenger();
    
        virtual void SetNewValue(G4UIcommand*, G4String);
    
    private:
        DetectorConstruction* fDetectorConstruction;
        G4UIcmdWithAString* fMaterialCmd;
        G4UIdirectory* fPrismDir;
    };

#endif