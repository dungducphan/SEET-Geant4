#include "generator.hh"

generator::generator() : G4VUserPrimaryGeneratorAction(), fGeneralParticleSource(nullptr) {
    // Seed the random number generator
    srand(static_cast<unsigned int>(time(nullptr)));

    fGeneralParticleSource = new G4GeneralParticleSource();

    G4SingleParticleSource *sps = fGeneralParticleSource->GetCurrentSource();
    G4ParticleDefinition *particle = G4ParticleTable::GetParticleTable()->FindParticle("e-");

    sps->SetParticleDefinition(particle);
    sps->SetNumberOfParticles(1);

    sps->GetPosDist()->SetPosDisType("Point");
    sps->GetPosDist()->ConfineSourceToVolume("NULL");

    sps->GetEneDist()->SetEnergyDisType("Mono"); // Mono, Lin, Pow, Exp, Gaus, Brem, BBody, Cdg (cosmic diffuse gamma), User, Arb, Epn (energy per nucleon)
    sps->GetEneDist()->SetMonoEnergy(1 * GeV);
}

generator::~generator() {
    delete fGeneralParticleSource;
}

void generator::GeneratePrimaries(G4Event *anEvent) {
    fGeneralParticleSource->GetCurrentSource()->GetPosDist()->SetCentreCoords(RandomizedPosition());
    fGeneralParticleSource->GetCurrentSource()->GetAngDist()->SetParticleMomentumDirection(RandomizedDirection());
    fGeneralParticleSource->GeneratePrimaryVertex(anEvent);
}

G4ThreeVector generator::RandomizedPosition() {
    return {0, 145 * cm, 0};
}

G4ThreeVector generator::RandomizedDirection() {
    return {0, -1, 0};
}
