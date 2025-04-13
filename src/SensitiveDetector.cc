#include "SensitiveDetector.hh"



SensitiveDetector::SensitiveDetector(G4String name) : G4VSensitiveDetector(name) {

    fTotalEnergyDeposited = 0;
    G4cout << "SensitiveDetector constructor called for: " << name << G4endl;

}

SensitiveDetector::~SensitiveDetector(){

}


void SensitiveDetector::Initialize(G4HCofThisEvent *){

    fTotalEnergyDeposited = 0;

}


void SensitiveDetector::EndOfEvent(G4HCofThisEvent *){

    // G4cout << "Deposited energy: " << fTotalEnergyDeposited << G4endl;

}

G4bool SensitiveDetector::ProcessHits(G4Step *aStep, G4TouchableHistory * ){

    G4Track* track = aStep->GetTrack();

    if (track->GetDefinition() == G4OpticalPhoton::Definition()) {
    
        G4double photonEnergy = track->GetTotalEnergy();  // MeV
    
        G4ThreeVector pos = aStep->GetPostStepPoint()->GetPosition();
        G4double time = track->GetGlobalTime();
    
        G4cout << "Optical photon detected! Energy: "
               << photonEnergy/eV << " eV, Position: "
               << pos << ", Time: " << time/ns << " ns" << G4endl;
    
        // stop the photon
        track->SetTrackStatus(fStopAndKill);
    }

    return true;
}