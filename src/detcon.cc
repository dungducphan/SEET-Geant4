#include <detcon.hh>
#include <SEET_SD.hh>

#include <CADMesh.hh>

#include <G4Colour.hh>
#include <G4VisAttributes.hh>

#include <TString.h>
#include <cstdlib>

detcon::detcon() : G4VUserDetectorConstruction() {}

detcon::~detcon() {
    delete logic_VDFront;
    delete logic_VDBack;
    delete logic_PinholeBlock;
}

G4VPhysicalVolume *detcon::Construct() {
    G4NistManager *nist = G4NistManager::Instance();
    G4bool checkOverlaps = true;

    // Getting the ${STL_DIR} env variable
    auto stlPath = getenv("STL_DIR");

    // World
    G4double worldSize = 10 * cm;
    G4Material *worldMat = nist->FindOrBuildMaterial("G4_Galactic");
    auto solidWorld = new G4Box("solidWorld", 0.5 * worldSize, 0.5 * worldSize, 0.5 * worldSize);
    auto logicWorld = new G4LogicalVolume(solidWorld, worldMat, "logicWorld");
    G4VPhysicalVolume *physWorld = new G4PVPlacement(nullptr, G4ThreeVector(), logicWorld, "physWorld", nullptr, false, 0, checkOverlaps);

    // Pinhole Block
    auto mesh_PinholeBlock = CADMesh::TessellatedMesh::FromSTL(Form("%s/SEET-CAD-PinholeBlock.stl", stlPath));
    G4VSolid *solid_PinholeBlock = mesh_PinholeBlock->GetSolid();
    logic_PinholeBlock = new G4LogicalVolume(solid_PinholeBlock, nist->FindOrBuildMaterial("G4_W"), "logic_PinholeBlock");
    G4VPhysicalVolume *phys_PinholeBlock = new G4PVPlacement(nullptr, G4ThreeVector(0, 0, 0), logic_PinholeBlock, "phys_PinholeBlock", logicWorld, false, 0, checkOverlaps);
    auto va_PinholeBlock = new G4VisAttributes();
    va_PinholeBlock->SetVisibility();
    va_PinholeBlock->SetForceSolid();
    va_PinholeBlock->SetColor(0, 1, 0, 0.5);
    logic_PinholeBlock->SetVisAttributes(va_PinholeBlock);

    // Virtual Detector Front
    auto mesh_VDFront = CADMesh::TessellatedMesh::FromSTL(Form("%s/SEET-CAD-VirtualDetectorFront.stl", stlPath));
    G4VSolid *solid_VDFront = mesh_VDFront->GetSolid();
    logic_VDFront = new G4LogicalVolume(solid_VDFront, nist->FindOrBuildMaterial("G4_Galactic"), "logic_VDFront");
    G4VPhysicalVolume *phys_VDFront = new G4PVPlacement(nullptr, G4ThreeVector(0, 0, 0), logic_VDFront, "phys_VDFront", logicWorld, false, 0, checkOverlaps);
    auto va_VDFront = new G4VisAttributes();
    va_VDFront->SetVisibility();
    va_VDFront->SetForceSolid();
    va_VDFront->SetColor(1, 0, 0, 0.5);
    logic_VDFront->SetVisAttributes(va_VDFront);

    // Virtual Detector Back
    auto mesh_VDBack = CADMesh::TessellatedMesh::FromSTL(Form("%s/SEET-CAD-VirtualDetectorBack.stl", stlPath));
    G4VSolid *solid_VDBack = mesh_VDBack->GetSolid();
    logic_VDBack = new G4LogicalVolume(solid_VDBack, nist->FindOrBuildMaterial("G4_Galactic"), "logic_VDBack");
    G4VPhysicalVolume *phys_VDBack = new G4PVPlacement(nullptr, G4ThreeVector(0, 0, 0), logic_VDBack, "phys_VDBack", logicWorld, false, 0, checkOverlaps);
    auto va_VDBack = new G4VisAttributes();
    va_VDBack->SetVisibility();
    va_VDBack->SetForceSolid();
    va_VDBack->SetColor(0, 0, 1, 0.5);
    logic_VDBack->SetVisAttributes(va_VDBack);

    return physWorld;
}

void detcon::ConstructSDandField() {
    auto aSD = new SEET_SD("SEET-SD");
    G4SDManager::GetSDMpointer()->AddNewDetector(aSD);
    // SetSensitiveDetector(logic_VDFront, aSD);
    SetSensitiveDetector(logic_VDBack, aSD);
}