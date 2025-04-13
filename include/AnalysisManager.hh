// AnalysisManager.hh
#ifndef ANALYSISMANAGER_HH
#define ANALYSISMANAGER_HH

#include "G4AnalysisManager.hh"
#include "G4SystemOfUnits.hh"
#include <fstream>  

class AnalysisManager {
public:
    static AnalysisManager* Instance();
    void Init();
    void FillPhotonData(G4double energy, G4ThreeVector pos);
    void Save();

private:
    AnalysisManager() {} // Private constructor
    static AnalysisManager* fInstance;
    static G4AnalysisManager* fManager;
    std::ofstream fOutputFile;
    
};

#endif