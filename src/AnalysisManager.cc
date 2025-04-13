// AnalysisManager.cc
#include "AnalysisManager.hh"

G4AnalysisManager* AnalysisManager::fManager = nullptr;
AnalysisManager* AnalysisManager::fInstance = nullptr;

AnalysisManager* AnalysisManager::Instance() {
    if (fInstance == nullptr) {
        fInstance = new AnalysisManager();
    }
    return fInstance;
}

void AnalysisManager::Init() {

    // open CSV file to write
    fOutputFile.open("../photon_data.csv", std::ios::out);
    if (fOutputFile.is_open()) {
        // header
        fOutputFile << "Energy_eV,X_mm,Y_mm,Z_mm\n";
    } else {
        G4cout << "Output file is not opening!" << G4endl;
    }

}

void AnalysisManager::FillPhotonData(G4double energy, G4ThreeVector pos) {

    if (fOutputFile.is_open()) {
        fOutputFile << energy / eV << "," 
                    << pos.x() / mm << ","
                    << pos.y() / mm << ","
                    << pos.z() / mm << "\n";
                    fOutputFile.flush(); 
    }
}

void AnalysisManager::Save() {
    // close the file
    if (fOutputFile.is_open()) {
        fOutputFile.close();
    }
}
