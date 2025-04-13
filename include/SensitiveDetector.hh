#ifndef SENSITIVEDETECTOR_HH
#define SENSITIVEDETECTOR_HH

#include "G4VSensitiveDetector.hh"

#include "G4Step.hh"
#include "G4HCofThisEvent.hh"
#include "G4TouchableHistory.hh"
#include "globals.hh"
#include "G4OpticalPhoton.hh"

#include "G4SystemOfUnits.hh"
#include "G4UnitsTable.hh"

#include "AnalysisManager.hh"

class SensitiveDetector : public G4VSensitiveDetector{


    public:
        SensitiveDetector(G4String);
        ~SensitiveDetector();

    private:

        G4double fTotalEnergyDeposited;

        // HC : hits collection only important if you want to do analysis
        // or reconstraction within G4
        virtual void Initialize(G4HCofThisEvent *) override;
        virtual void EndOfEvent(G4HCofThisEvent *) override;

        // handles what happens to the particle in each step 
        // when it is inside of the detector

        virtual G4bool ProcessHits(G4Step *, G4TouchableHistory *) override;


};




#endif