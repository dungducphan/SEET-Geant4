#include <detcon.hh>
#include <SEET_SD.hh>

#include <CADMesh.hh>

#include <G4Colour.hh>
#include <G4VisAttributes.hh>

#include <TString.h>
#include <cstdlib>

detcon::detcon() : G4VUserDetectorConstruction() {}

detcon::~detcon() {
    delete logic_VDBack;
    delete logic_PinholeBlock;
}

G4VPhysicalVolume *detcon::Construct() {
    G4NistManager *nist = G4NistManager::Instance();
    G4bool checkOverlaps = true;

    // Getting the ${STL_DIR} env variable
    auto stlPath = getenv("STL_DIR");

    // World
    G4double worldSize_X = 10 * cm;
    G4double worldSize_Y = 300 * cm;
    G4double worldSize_Z = 10 * cm;
    G4Material *worldMat = nist->FindOrBuildMaterial("G4_Galactic");
    auto solidWorld = new G4Box("solidWorld", 0.5 * worldSize_X, 0.5 * worldSize_Y, 0.5 * worldSize_Z);
    auto logicWorld = new G4LogicalVolume(solidWorld, worldMat, "logicWorld");
    G4VPhysicalVolume *physWorld = new G4PVPlacement(nullptr, G4ThreeVector(), logicWorld, "physWorld", nullptr, false, 0, checkOverlaps);

    auto a = new G4RotationMatrix();
    a->rotateX(90 * deg);


    // Ceramic Packaging Layer
//    auto solid_Ceramic = new G4Tubs("solid_Ceramic", 0, 4.9 * cm, 0.5 * mm, 0, 360 * deg);
//    auto logic_Ceramic = new G4LogicalVolume(solid_Ceramic, nist->FindOrBuildMaterial("G4_BORON_CARBIDE"), "logic_Ceramic");
//    G4VPhysicalVolume *phys_Ceramic = new G4PVPlacement(a, G4ThreeVector(0, -10 * mm, 0), logic_Ceramic, "phys_Ceramic", logicWorld, false, 0, checkOverlaps);
//    auto va_Ceramic = new G4VisAttributes();
//    va_Ceramic->SetVisibility();
//    va_Ceramic->SetForceSolid();
//    va_Ceramic->SetColor(1, 0, 0, 0.5);
//    logic_Ceramic->SetVisAttributes(va_Ceramic);

    // Virtual Detector Back
    auto solid_VDBack = new G4Tubs("solid_VDBack", 0, 4.9 * cm, 0.5 * mm, 0, 360 * deg);
    logic_VDBack = new G4LogicalVolume(solid_VDBack, nist->FindOrBuildMaterial("G4_Si"), "logic_VDBack");
    G4VPhysicalVolume *phys_VDBack = new G4PVPlacement(a, G4ThreeVector(0, -11 * mm, 0), logic_VDBack, "phys_VDBack", logicWorld, false, 0, checkOverlaps);
    auto va_VDBack = new G4VisAttributes();
    va_VDBack->SetVisibility();
    va_VDBack->SetForceSolid();
    va_VDBack->SetColor(0, 1, 0, 0.5);
    logic_VDBack->SetVisAttributes(va_VDBack);

    return physWorld;
}

void detcon::ConstructSDandField() {
    auto aSD = new SEET_SD("SEET-SD");
    G4SDManager::GetSDMpointer()->AddNewDetector(aSD);
    SetSensitiveDetector(logic_VDBack, aSD);
}