#include "DetectorConstruction.hh"

DetectorConstruction::DetectorConstruction(){


}

DetectorConstruction::~DetectorConstruction(){

    
}

G4VPhysicalVolume *DetectorConstruction::Construct(){ // we are defining here our main function Construct

    G4bool checkOverlaps = true;

    G4NistManager *nist = G4NistManager::Instance();

    // World material
    G4Material *worldMat = nist-> FindOrBuildMaterial("G4_AIR");

    

    // Prism material

    G4Material* h9klMat = new G4Material("H9KL", 2.5 * g/cm3, 1);
    G4Element* dummyElement = new G4Element("Dummy", "X", 1., 1. * g/mole); // a G4MAterial needs to have at least one element
    h9klMat ->AddElement(dummyElement, 1);

    // in order for the prism to be dispersive we need to associate an optic index to the material
    
    std::vector<G4double> energies; // we start by defining a vector since we don't specify the number of elements 
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

    auto mptH9KL = new G4MaterialPropertiesTable();
    mptH9KL->AddProperty("RINDEX", energies, rindices);
    
    h9klMat->SetMaterialPropertiesTable(mptH9KL);


    // Also need to define air's refractive index
    std::vector<G4double> air_energies = energies;  // Use same energies as prism
    std::vector<G4double> air_rindex(air_energies.size(), 1.0); // Air's refractive index ≈ 1.0
    
    auto mptAir = new G4MaterialPropertiesTable();
    mptAir->AddProperty("RINDEX", air_energies, air_rindex);
    worldMat->SetMaterialPropertiesTable(mptAir);


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
    auto logicPrism = new G4LogicalVolume(solidPrism, h9klMat, "LogicPrism");

    // Rotar 90 grados en X para que Z pase a ser vertical
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


    return physWorld;



}