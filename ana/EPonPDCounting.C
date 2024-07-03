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
    // auto hEdepSecondaryElectron = new TH1D("hEdepSecondaryElectron", "",  200, 0, 100);
    // auto hEdepPrimaryElectron   = new TH1D("hEdepPrimaryElectron", "",  200, 0, 100);
    // auto hEdepElectron          = new TH1D("hEdepElectron", "",  200, 0, 100);
    // auto hEdepGamma             = new TH1D("hEdepGamma", "",  200, 0, 100);

    auto hEdepSignal          = new TH1D("hEdepSignal", "",  200, 0, 100);
    auto hEdepBackground      = new TH1D("hEdepBackground", "",  200, 0, 100);

    // Loop over the tree here
    int EventID, TrackID, PDGID;
    double Edep;
    tree->SetBranchAddress("EventID", &EventID);
    tree->SetBranchAddress("TrackID", &TrackID);
    tree->SetBranchAddress("PID", &PDGID);
    tree->SetBranchAddress("Edep", &Edep);
    for (auto i = 0; i < NEntries; i++) {
        tree->GetEntry(i);
        if (TrackID == 1) {
            hEdepSignal->Fill(Edep * 1000000);
            std::cout << "Primary electron: " << Edep << " MeV\n";
        }
        else hEdepBackground->Fill(Edep * 1000000);
    }

    auto c = new TCanvas("c", "c", 1200, 1200);
    c->SetLogy();
    hEdepSignal->SetLineColor(kBlue);
    hEdepSignal->Draw();
    hEdepBackground->SetLineColor(kRed);
    hEdepBackground->Draw("SAME");
    // c->BuildLegend();
    c->SaveAs(Form("%s/SignalVsBackground.pdf", anaDirPath.c_str()));
}

#ifndef __CINT__
int main() {
  EPonPDCounting();
  return 0;
}
#endif