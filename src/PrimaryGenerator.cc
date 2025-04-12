#include "PrimaryGenerator.hh"

PrimaryGenerator::PrimaryGenerator(){

    fParticleGun = new G4ParticleGun(1);

    // Particle position
    G4double x = -1. * m;
    G4double y = -0.8 * m;
    G4double z = 0. * m;

    G4ThreeVector pos(x, y, z);

    // Particle direction

    G4double px = 1. ; 
    G4double py = 0.8 ; 
    G4double pz = 0. ;  

    G4ThreeVector mom(px,py,pz);

    // Particle type

    G4ParticleTable *particleTable = G4ParticleTable::GetParticleTable();
    G4ParticleDefinition *particle = particleTable -> FindParticle("opticalphoton");

    fParticleGun -> SetParticlePosition(pos);
    fParticleGun -> SetParticleMomentumDirection(mom);
    fParticleGun -> SetParticleDefinition(particle);

    // if we want to generate optical photons, we neet to asociate a polarization to them

    // we choose an arbitrary polarization direction for the photons, since we don't need any
    // in this case the choice is to set it perpendicular to the position

    G4ThreeVector normal(1., 0., 0.);
    if (pos == normal) normal = G4ThreeVector(0., 1., 0.);

    G4ThreeVector polarization = pos.cross(normal);
    polarization = polarization.unit();

    fParticleGun->SetParticlePolarization(polarization);

}


PrimaryGenerator::~PrimaryGenerator(){

    delete fParticleGun;
}

void PrimaryGenerator::GeneratePrimaries(G4Event *anEvent){

    // we set a different energy (wavelength) for each photon, within the visible range
    G4double lambda_nm = 400. + G4UniformRand() * (700. - 400.);  // 400–700 nm
    G4double energy = (1240. / lambda_nm) * eV; // E = hc/λ

    fParticleGun->SetParticleEnergy(energy);

    // Create vertex
    fParticleGun -> GeneratePrimaryVertex(anEvent);
    
}