#include <detcon.hh>
#include <SEET_SD.hh>

#include <CADMesh.hh>

#include <G4Colour.hh>
#include <G4VisAttributes.hh>

#include <TString.h>
#include <cstdlib>

detcon::detcon() : G4VUserDetectorConstruction() {}

detcon::~detcon() {
    delete logic_PhotoDiode;
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

    auto solid_PinholeBlock = new G4Tubs("solid_PinholeBlock", 50 * um, 30 * mm, 10 * mm, 0, 360 * deg);
    logic_PinholeBlock = new G4LogicalVolume(solid_PinholeBlock, nist->FindOrBuildMaterial("G4_W"), "logic_PinholeBlock");
    G4VPhysicalVolume *phys_PinholeBlock = new G4PVPlacement(a, G4ThreeVector(0, 0, 0), logic_PinholeBlock, "phys_PinholeBlock", logicWorld, false, 0, checkOverlaps);
    auto va_PinholeBlock = new G4VisAttributes();
    va_PinholeBlock->SetVisibility();
    va_PinholeBlock->SetForceSolid();
    va_PinholeBlock->SetColor(0, 1, 0, 0.5);
    logic_PinholeBlock->SetVisAttributes(va_PinholeBlock);

    // Virtual Detector Back
    auto solid_PhotoDiode = new G4Tubs("solid_PhotoDiode", 0, 0.508 * mm, 0.1 * mm, 0, 360 * deg);
    logic_PhotoDiode = new G4LogicalVolume(solid_PhotoDiode, nist->FindOrBuildMaterial("G4_Si"), "logic_PhotoDiode");
    G4VPhysicalVolume *phys_PhotoDiode = new G4PVPlacement(a, G4ThreeVector(0, -11 * mm, 0), logic_PhotoDiode, "phys_PhotoDiode", logicWorld, false, 0, checkOverlaps);
    auto va_PhotoDiode = new G4VisAttributes();
    va_PhotoDiode->SetVisibility();
    va_PhotoDiode->SetForceSolid();
    va_PhotoDiode->SetColor(1, 0, 0, 0.5);
    logic_PhotoDiode->SetVisAttributes(va_PhotoDiode);

    return physWorld;
}

void detcon::ConstructSDandField() {
}