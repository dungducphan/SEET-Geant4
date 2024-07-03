#ifndef __CINT__
#include <TTree.h>
#include <TFile.h>
#include <TMath.h>
#include <TCanvas.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TGraph.h>
#include <TROOT.h>
#include <TStyle.h>
#include <TLegend.h>
#endif

#include <iostream>

// Change these:
std::string anaDirPath = "/home/dphan/Documents/GitHub/SEET-Geant4/ana";
std::string dataFile = "Ana";
std::string thickness = "14mm";

void EPonPDCounting() {
    gStyle->SetOptStat(0);

    // Open the file and get the tree
    auto infile = new TFile(Form("%s/%s.root", anaDirPath.c_str(), dataFile.c_str()), "READ");
    auto tree = (TTree *) infile->Get("Ana");
    auto NEntries = tree->GetEntries();

    // Prepare the histograms
    auto hElectronSpectrum          = new TH1D("hElectronSpectrum",       ";E (MeV);dN/dE (/0.5 MeV)",  200, 0, 100);
    auto hPositronSpectrum          = new TH1D("hPositronSpectrum",       ";E (Mev);dN/dE (/0.5 MeV)",  200, 0, 100);

    // Loop over the tree here
    int EventID, TrackID, PDGID;
    double x, y, z, px, py, pz, E;
    tree->SetBranchAddress("EventID", &EventID);
    tree->SetBranchAddress("TrackID", &TrackID);
    tree->SetBranchAddress("PID", &PDGID);
    tree->SetBranchAddress("x", &x);
    tree->SetBranchAddress("y", &y);
    tree->SetBranchAddress("z", &z);
    tree->SetBranchAddress("pX", &px);
    tree->SetBranchAddress("pX", &py);
    tree->SetBranchAddress("pZ", &pz);
    tree->SetBranchAddress("E", &E);
    int nElectrons = 0;
    int nPositrons = 0;
    for (auto i = 0; i < NEntries; i++) {
        tree->GetEntry(i);
        if (PDGID == 11) {
            hElectronSpectrum->Fill(E);
            nElectrons++;
        } else if (PDGID == -11) {
            hPositronSpectrum->Fill(E);
            nPositrons++;
        } else continue;
    }

    auto c = new TCanvas("c", "c", 1200, 1200);
    c->SetLogy();
    hElectronSpectrum->SetLineColor(kBlue);
    hElectronSpectrum->Draw();
    hPositronSpectrum->SetLineColor(kRed);
    hPositronSpectrum->Draw("SAME");
    c->BuildLegend();
    c->SaveAs(Form("%s/EPonPDCounting.pdf", anaDirPath.c_str()));

    std::cout << "EP: " << (nElectrons + nPositrons) << std::endl;
    std::cout << "Charge: " << (nElectrons + nPositrons) * 1.6E-19 * 1E12 << " pC." << std::endl;
}

#ifndef __CINT__
int main() {
  EPonPDCounting();
  return 0;
}
#endif