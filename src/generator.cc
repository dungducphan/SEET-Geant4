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

    sps->GetEneDist()->SetEnergyDisType("Gauss"); // Mono, Lin, Pow, Exp, Gaus, Brem, BBody, Cdg (cosmic diffuse gamma), User, Arb, Epn (energy per nucleon)
    sps->GetEneDist()->SetMonoEnergy(90 * MeV);
    sps->GetEneDist()->SetBeamSigmaInE(4 * MeV);

    fRandom = new TRandom3();
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
    double r = static_cast<double>(rand()) / RAND_MAX;
    double theta = static_cast<double>(rand()) / RAND_MAX;
    theta = theta * TMath::TwoPi();
    G4double x = r * TMath::Sin(theta) * 0. * um;
    G4double z = r * TMath::Cos(theta) * 0. * um;

    return {x, 148 * cm, z};
}

G4ThreeVector generator::RandomizedDirection() {
    double rdm1 = fRandom->Gaus(0, 2);
    double y_mom = -1. * TMath::Cos(rdm1 * mrad);
    double tmp = TMath::Sin(rdm1 * mrad);
    double rdm2 = static_cast<double>(rand()) / RAND_MAX;
    rdm2 = rdm2 * TMath::TwoPi();
    double x_mom = TMath::Sin(rdm2 * rad) * tmp;
    double z_mom = TMath::Cos(rdm2 * rad) * tmp;

    return {x_mom, y_mom, z_mom};
}
