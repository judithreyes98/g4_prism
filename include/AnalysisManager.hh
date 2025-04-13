// AnalysisManager.hh
#ifndef ANALYSISMANAGER_HH
#define ANALYSISMANAGER_HH

#include "G4AnalysisManager.hh"
#include "G4SystemOfUnits.hh"

class AnalysisManager {
public:
    static AnalysisManager* Instance();
    static void Init();
    static void FillPhotonData(G4double energy, G4ThreeVector pos);
    static void Save();

private:
    AnalysisManager() {} // Private constructor
    static AnalysisManager* fInstance;
    static G4AnalysisManager* fManager;
};

#endif