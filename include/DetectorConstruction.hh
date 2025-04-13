#ifndef DETECTORCONSTRUCTION_HH
#define DETECTORCONSTRUCTION_HH

#include "G4VUserDetectorConstruction.hh"

#include "G4Box.hh"
#include "G4Trd.hh"

#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4Material.hh"
#include "G4Element.hh"

#include "G4NistManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4UnitsTable.hh"

#include "G4OpticalPhoton.hh"
#include "G4OpticalSurface.hh"
#include "G4MaterialPropertiesTable.hh"
#include "G4LogicalBorderSurface.hh"
#include "G4LogicalSkinSurface.hh"

#include "G4VisAttributes.hh"
#include "G4Color.hh"
#include "G4SDManager.hh"

#include "DetectorMessenger.hh"
#include "SensitiveDetector.hh"


class DetectorConstruction : public G4VUserDetectorConstruction {
    public:
      DetectorConstruction(G4String materialChoice = "H9KL");
      virtual ~DetectorConstruction(); // virtual function cause its already defined in the parent class 
      virtual G4VPhysicalVolume* Construct();

      void SetMaterial(const G4String& material);
  
    private:
      G4String fMaterialChoice;  // material choice to be used in the prism
      G4Material* fMaterial = nullptr;  
      G4LogicalVolume* fLogicalVolume = nullptr; 
      DetectorMessenger* fMessenger;

      G4LogicalVolume *logicDetector; 

      virtual void ConstructSDandField();
  };


#endif