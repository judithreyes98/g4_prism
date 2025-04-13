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

    if (fManager == nullptr) {
        fManager = G4AnalysisManager::Instance();
        fManager->SetVerboseLevel(1);

        fManager->SetDefaultFileType("root");

        fManager->OpenFile("../photon_data.root");

        // create ntuple
        fManager->CreateNtuple("Photons", "Detected optical photons");
        fManager->CreateNtupleDColumn("Energy_eV");
        fManager->CreateNtupleDColumn("X_mm");
        fManager->CreateNtupleDColumn("Y_mm");
        fManager->CreateNtupleDColumn("Z_mm");
        fManager->FinishNtuple();
    } else {
        G4cout << "G4AnalysisManager already initialized!" << G4endl;
    }
}

void AnalysisManager::FillPhotonData(G4double energy, G4ThreeVector pos) {
    fManager->FillNtupleDColumn(0, energy / eV);
    fManager->FillNtupleDColumn(1, pos.x() / mm);
    fManager->FillNtupleDColumn(2, pos.y() / mm);
    fManager->FillNtupleDColumn(3, pos.z() / mm);
    fManager->AddNtupleRow();
}

void AnalysisManager::Save() {
    fManager->Write();
    fManager->CloseFile();
}