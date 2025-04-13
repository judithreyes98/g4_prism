#include "DetectorConstruction.hh"
#include "DetectorMessenger.hh"

void DetectorConstruction::SetMaterial(const G4String& materialName) {

    G4cout << "SetMaterial called with: " << materialName << G4endl;

    G4Material* material = nullptr;
    G4NistManager* nist = G4NistManager::Instance();
    auto mptMat = new G4MaterialPropertiesTable();

    if (materialName == "H9KL") {

        material = new G4Material("H9KL", 2.5 * g/cm3, 1);
        G4Element* dummyElement = new G4Element("Dummy", "X", 1., 1. * g/mole);  // a G4MAterial needs to have at least one element
        material->AddElement(dummyElement, 1);

        // in order for the prism to be dispersive we need to associate an optic index to the material

        std::vector<G4double> energies; // we define a vector since we don't specify the number of elements 
        std::vector<G4double> rindices;

        for (G4double lambda = 700; lambda >= 400; lambda -= 10) { // IN DECREASING ENERGY ORDER !!
            G4double x = lambda / 1000.0; // the expresion below is in um !
            G4double n = sqrt(1 + 0.614555251/(1 - 0.0145987884/pow(x,2))
                                + 0.656775017/(1 - 0.00287769588/pow(x,2))
                                + 1.02699346/(1 - 107.653051/pow(x,2)));
            G4double E = (1.2398 / x) * eV; // energy of the photon in eV (x in um))
            energies.push_back(E);
            rindices.push_back(n);
        }

        mptMat->AddProperty("RINDEX", energies, rindices);

    } else if (materialName == "PMMA") {

        material = nist-> FindOrBuildMaterial("G4_PLEXIGLASS");

        mptMat->AddProperty("RINDEX","PMMA");

    } else if (materialName == "FusedSilica") {

        material = nist-> FindOrBuildMaterial("G4_SILICON_DIOXIDE");

        mptMat->AddProperty("RINDEX","Fused Silica");

        }

    material->SetMaterialPropertiesTable(mptMat);
    
    if (material) {
        fMaterial = material;

        if (fLogicalVolume) {
            fLogicalVolume->SetMaterial(material);
        }

        G4cout << "Material changed to: " << material->GetName() << G4endl;
    } else {
        G4cerr << "Material " << materialName << " not found!" << G4endl;
    }
}

DetectorConstruction::DetectorConstruction(G4String materialChoice) 
    : G4VUserDetectorConstruction(), 
    fMaterialChoice(materialChoice), 
    fMaterial(nullptr),
    fLogicalVolume(nullptr),
    logicDetector(nullptr) { 

        fMessenger = new DetectorMessenger(this); // Create the messenger

}


DetectorConstruction::~DetectorConstruction(){
    delete fMessenger;
}

G4VPhysicalVolume *DetectorConstruction::Construct(){ // we are defining here our main function Construct

    G4bool checkOverlaps = true;

    G4NistManager *nist = G4NistManager::Instance();

    // World material
    G4Material *worldMat = nist-> FindOrBuildMaterial("G4_AIR");

    // since we are considering an optic process, we need to assign a
    // refractive index to the material. For air it is already implemented in g4

    auto mptAir = new G4MaterialPropertiesTable();
    mptAir->AddProperty("RINDEX", "Air");

    worldMat->SetMaterialPropertiesTable(mptAir);
        

    // Prism material : deppends on the user choice
    // The prism is made of H9KL, PMMA or Silica

    auto mptMat = new G4MaterialPropertiesTable();

    G4Material* prismMat = nullptr;

    if (fMaterialChoice == "H9KL") {
        
        prismMat = new G4Material("H9KL", 2.5 * g/cm3, 1);
        G4Element* dummyElement = new G4Element("Dummy", "X", 1., 1. * g/mole);  // a G4MAterial needs to have at least one element
        prismMat->AddElement(dummyElement, 1);

        // in order for the prism to be dispersive we need to associate an optic index to the material

        std::vector<G4double> energies; // we define a vector since we don't specify the number of elements 
        std::vector<G4double> rindices;

        for (G4double lambda = 700; lambda >= 400; lambda -= 10) { // IN DECREASING ENERGY ORDER !!
            G4double x = lambda / 1000.0; // the expresion below is in um !
            G4double n = sqrt(1 + 0.614555251/(1 - 0.0145987884/pow(x,2))
                                + 0.656775017/(1 - 0.00287769588/pow(x,2))
                                + 1.02699346/(1 - 107.653051/pow(x,2)));
            G4double E = (1.2398 / x) * eV; // energy of the photon in eV (x in um))
            energies.push_back(E);
            rindices.push_back(n);
        }

        mptMat->AddProperty("RINDEX", energies, rindices);


    } else if (fMaterialChoice == "PMMA") {
        
        prismMat = nist-> FindOrBuildMaterial("G4_PLEXIGLASS");

        mptMat->AddProperty("RINDEX","PMMA");

    } else if (fMaterialChoice == "FusedSilica") {

        prismMat = nist-> FindOrBuildMaterial("G4_SILICON_DIOXIDE");

        mptMat->AddProperty("RINDEX","Fused Silica");

        
    } else {
        G4Exception("DetectorConstruction", "InvalidMaterial",
                    FatalException, "Unknown prism material");
    }

    prismMat->SetMaterialPropertiesTable(mptMat);
    

    // Defining the world volume

    G4double xWorld = 2. * m;
    G4double yWorld = 2. * m;
    G4double zWorld = 2. * m;

    G4Box *solidWorld = new G4Box("solidWorld", 0.5 * xWorld, 0.5 * yWorld, 0.5* zWorld);
    G4LogicalVolume *logicWorld = new G4LogicalVolume(solidWorld, worldMat, "logicWorld");
    G4VPhysicalVolume *physWorld = new G4PVPlacement(0, 
                                                    G4ThreeVector(0.,0.,0.), 
                                                    logicWorld, 
                                                    "physWorld", 
                                                    0, 
                                                    false, 
                                                    0, 
                                                    checkOverlaps);


    // Defining the prism to disperse the light

    G4double prism_length = 50*cm;
    G4double prism_width = 30*cm; // width along the y axis (placed in the standar orientation of axis)
    G4double prism_height = 50*cm;

    G4double dx1 = 0.5 * prism_length; // length at -dz
    G4double dx2 = 0;  // length at +dz (vertex)
    G4double dy1 = 0.5 * prism_width;
    G4double dy2 = 0.5 * prism_width;
    G4double dz = 0.5 * prism_height; 

    auto solidPrism = new G4Trd("Prism", dx1, dx2, dy1, dy2, dz);
    auto logicPrism = new G4LogicalVolume(solidPrism, prismMat, "LogicPrism");

    fLogicalVolume = logicPrism;

    // Rotation of 90 degrees to place the prism in the g4 axis definition
    auto rotation = new G4RotationMatrix();
    rotation->rotateX(90.*deg);

    G4VPhysicalVolume *physPrism =new G4PVPlacement(rotation,
                                                    G4ThreeVector(0,0,0),
                                                    logicPrism,
                                                    "Prism",
                                                    logicWorld,
                                                    false,
                                                    0,
                                                    checkOverlaps);


    // Define optical surfaces
    G4OpticalSurface* opticalSurface = new G4OpticalSurface("PrismSurface");
    opticalSurface->SetType(dielectric_dielectric);
    opticalSurface->SetFinish(polished);
    opticalSurface->SetModel(unified);

    // Create the logical border surface
    new G4LogicalBorderSurface("PrismSurface",
                            physPrism,  // Make sure to save the physical volume pointer
                            physWorld,
                            opticalSurface);


    // Define a sensitive detector to save the energy and position of the photons

    G4double det_x = 2*cm;
    G4double det_y = 40*cm;
    G4double det_z = 40*cm;

    G4Box* solidDetector = new G4Box("Detector", 0.5 * det_x, 0.5 * det_y, 0.5 * det_z);
    G4Material* detMaterial = nist->FindOrBuildMaterial("G4_AIR"); // it doesn't matter the material we use

    logicDetector = new G4LogicalVolume(solidDetector, detMaterial, "LogicDetector");

    // position behind the prism
    G4ThreeVector posDetector( prism_length/2 + det_z/2 + 1*cm, 0, 0); //with a separation of 1 cm

    new G4PVPlacement(nullptr, posDetector, logicDetector, "PhysDetector", logicWorld, false, 0, checkOverlaps);

    return physWorld;


}

void DetectorConstruction::ConstructSDandField(){

    //G4cout << "Constructing sensitive detectors..." << G4endl;

    if(logicDetector != nullptr) {


        G4SDManager *sdManager = G4SDManager::GetSDMpointer();

        SensitiveDetector *sensDet = new SensitiveDetector("SensitiveDetector");
    
        sdManager->AddNewDetector(sensDet);

        logicDetector->SetSensitiveDetector(sensDet);
        //G4cout << "Sensitive detector set for logicDetector" << G4endl;

    } else {
        G4cerr << "Error: logicDetector is null in ConstructSDandField" << G4endl;
    }
    
}
