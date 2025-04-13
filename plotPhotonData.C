#include <TFile.h>
#include <TGraph.h>
#include <TCanvas.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cmath> // for std::abs

void plotPhotonData() {
    // open the CSV file
    std::ifstream file("photon_data.csv");
    
    // check if the file is open
    if (!file.is_open()) {
        std::cout << "Error opening file!" << std::endl;
        return;
    }

    // skip the header line
    std::string line;
    std::getline(file, line);

    // graph to visualize the data
    TGraph *graph = new TGraph();

    Double_t energy, x, y, z;
    Int_t i = 0;

    // read data from CSV file line by line
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string value;
        
        // read columns for energy, x, y, z
        std::getline(ss, value, ','); energy = std::stod(value);  // Energy
        std::getline(ss, value, ','); x = std::stod(value);  // X Position
        std::getline(ss, value, ','); y = std::stod(value);  // Y Position
        std::getline(ss, value, ','); z = std::stod(value);  // Z Position

        // take absolute value of the position (x)
        x = std::abs(x);
        y = std::abs(y);
        z = std::abs(z);

        // fill the graph (absolute x vs energy)
        graph->SetPoint(i++, y, energy);  // x vs energy
    }

    // close the file
    file.close();

    // create canvas
    TCanvas *canvas = new TCanvas("c1", "Position vs Energy", 800, 600);

    // title and axis labels
    graph->SetTitle("Position vs Energy;Y Position (mm);Energy (eV)");
    graph->SetMarkerStyle(20);  
    graph->SetMarkerColor(kBlue);  

    // drawing the graph
    graph->Draw("AP");

    // updating canvas to show
    canvas->Update();
}