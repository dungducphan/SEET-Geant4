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

// Change these:
std::string anaDirPath = "/home/dphan/Documents/GitHub/SEET-Geant4/ana";
std::string dataFile = "Ana";

void SEETAna() {
    gStyle->SetOptStat(0);

    // Open the file and get the tree
    auto infile = new TFile(Form("%s/%s.root", anaDirPath.c_str(), dataFile.c_str()), "READ");
    auto tree = (TTree *) infile->Get("Ana");
    auto NEntries = tree->GetEntries();

    // Prepare the histograms
    auto hElectronSpectrum = new TH1D("hElectronSpectrum", ";E (MeV);dN/dE (/0.5 MeV)", 120, 0, 60);
    auto hPositronSpectrum = new TH1D("hPositronSpectrum", ";E (Mev);dN/dE (/0.5 MeV)", 120, 0, 60);
    auto hGammaSpectrum    = new TH1D("hGammaSpectrum",    ";E (MeV);dN/dE (/0.5 MeV)", 120, 0, 60);

    auto hElectronSpectrumVersusPosition = new TH2D("hElectronSpectrumVersusPosition", ";E (MeV); R (micron)", 120, 0, 60, 100, 0, 600);

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
    for (auto i = 0; i < NEntries; i++) {
        tree->GetEntry(i);
        if (PDGID == 11) {
            hElectronSpectrum->Fill(E);
            hElectronSpectrumVersusPosition->Fill(E, TMath::Sqrt(x * x + z * z));
        } else if (PDGID == -11) {
            hPositronSpectrum->Fill(E);
        } else if (PDGID == 22) {
            hGammaSpectrum->Fill(E);
        }
    }

    double NGamma = hGammaSpectrum->Integral();
    double NElectron = hElectronSpectrum->Integral();
    double NPositron = hPositronSpectrum->Integral();

    hPositronSpectrum->Scale(20);
    hElectronSpectrum->SetFillColorAlpha(kRed, 0.1);
    hPositronSpectrum->SetFillColorAlpha(kBlue, 0.1);
    hGammaSpectrum->SetFillColorAlpha(kGreen, 0.1);
    hElectronSpectrum->SetLineWidth(2);
    hPositronSpectrum->SetLineWidth(2);
    hGammaSpectrum->SetLineWidth(2);
    hElectronSpectrum->SetLineColor(kRed + 1);
    hPositronSpectrum->SetLineColor(kBlue + 1);
    hGammaSpectrum->SetLineColor(kGreen + 1);

    auto canvas = new TCanvas("c", "c", 1200, 1200);
    hGammaSpectrum->Draw();
    hGammaSpectrum->GetXaxis()->CenterTitle();
    hGammaSpectrum->GetYaxis()->CenterTitle();
    hGammaSpectrum->GetYaxis()->SetMaxDigits(3);
    hElectronSpectrum->Draw("SAME");
    hPositronSpectrum->Draw("SAME HIST");

    auto leg = new TLegend(0.55, 0.65, 0.85, 0.85);
    leg->SetBorderSize(0);
    leg->AddEntry(hElectronSpectrum, Form("Electron (%g)", NElectron), "f");
    leg->AddEntry(hPositronSpectrum, Form("20 #times Positron (%g)", NPositron), "f");
    leg->AddEntry(hGammaSpectrum, Form("Gamma (%g)", NGamma), "f");
    leg->Draw();

    canvas->SaveAs(Form("%s/All/SEETAna-Spectra.pdf", anaDirPath.c_str()));

    auto canvas2 = new TCanvas("c2", "c2", 1200, 1200);
    canvas2->SetMargin(0.15, 0.15, 0.15, 0.15);
    hElectronSpectrumVersusPosition->Draw("COLZ");
    hElectronSpectrumVersusPosition->GetXaxis()->CenterTitle();
    hElectronSpectrumVersusPosition->GetYaxis()->CenterTitle();
    canvas2->SaveAs(Form("%s/All/SEETAna-SpectraVersusPosition.pdf", anaDirPath.c_str()));

    delete canvas;
    delete canvas2;
    delete hElectronSpectrum;
    delete hPositronSpectrum;
    delete hGammaSpectrum;
    delete leg;
    delete tree;
    infile->Close();
}


void SEETAna_Log() {
    gStyle->SetOptStat(0);

    // Open the file and get the tree
    auto infile = new TFile(Form("%s/%s.root", anaDirPath.c_str(), dataFile.c_str()), "READ");
    auto tree = (TTree *) infile->Get("Ana");
    auto NEntries = tree->GetEntries();

    // Prepare the histograms
    auto hElectronSpectrum = new TH1D("hElectronSpectrum", ";E (MeV);dN/dE (/0.5 MeV)", 120, 0, 60);
    auto hPositronSpectrum = new TH1D("hPositronSpectrum", ";E (Mev);dN/dE (/0.5 MeV)", 120, 0, 60);
    auto hGammaSpectrum    = new TH1D("hGammaSpectrum",    ";E (MeV);dN/dE (/0.5 MeV)", 120, 0, 60);

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
    for (auto i = 0; i < NEntries; i++) {
        tree->GetEntry(i);
        if (PDGID == 11) {
            hElectronSpectrum->Fill(E);
        } else if (PDGID == -11) {
            hPositronSpectrum->Fill(E);
        } else if (PDGID == 22) {
            hGammaSpectrum->Fill(E);
        }
    }

    double NGamma = hGammaSpectrum->Integral();
    double NElectron = hElectronSpectrum->Integral();
    double NPositron = hPositronSpectrum->Integral();

//    hPositronSpectrum->Scale(20);
    hElectronSpectrum->SetFillColorAlpha(kRed, 0.1);
    hPositronSpectrum->SetFillColorAlpha(kBlue, 0.1);
    hGammaSpectrum->SetFillColorAlpha(kGreen, 0.1);
    hElectronSpectrum->SetLineWidth(2);
    hPositronSpectrum->SetLineWidth(2);
    hGammaSpectrum->SetLineWidth(2);
    hElectronSpectrum->SetLineColor(kRed + 1);
    hPositronSpectrum->SetLineColor(kBlue + 1);
    hGammaSpectrum->SetLineColor(kGreen + 1);

    auto canvas = new TCanvas("c", "c", 1200, 1200);
    canvas->SetLogy();
    hGammaSpectrum->Draw();
    hGammaSpectrum->GetXaxis()->CenterTitle();
    hGammaSpectrum->GetYaxis()->CenterTitle();
    hGammaSpectrum->GetYaxis()->SetMaxDigits(3);
    hGammaSpectrum->GetYaxis()->SetRangeUser(1, 1e6);
    hElectronSpectrum->Draw("SAME");
    hPositronSpectrum->Draw("SAME HIST");

    auto leg = new TLegend(0.25, 0.65, 0.55, 0.85);
    leg->SetBorderSize(0);
    leg->AddEntry(hElectronSpectrum, Form("Electron (%g)", NElectron), "f");
    leg->AddEntry(hPositronSpectrum, Form("Positron (%g)", NPositron), "f");
    leg->AddEntry(hGammaSpectrum, Form("Gamma (%g)", NGamma), "f");
    leg->Draw();
    canvas->SaveAs(Form("%s/All/SEETAna-Spectra-Log.pdf", anaDirPath.c_str()));

    delete canvas;
    delete hElectronSpectrum;
    delete hPositronSpectrum;
    delete hGammaSpectrum;
    delete leg;
    delete tree;
    infile->Close();
}


#ifndef __CINT__
int main() {
  SEETAna();
  SEETAna_Log();
  return 0;
}
#endif