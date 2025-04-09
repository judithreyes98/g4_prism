#include "PhysicsList.hh"

PhysicsList::PhysicsList(){

    // em physics
    RegisterPhysics(new G4EmStandardPhysics());

    // optic physics
    RegisterPhysics(new G4OpticalPhysics());
}

PhysicsList::~PhysicsList(){}