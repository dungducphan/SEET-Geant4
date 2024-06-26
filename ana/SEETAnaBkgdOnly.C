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
std::string dataFile = "Ana_1cm_48E6";
std::string thickness = "Special";

void SEETAna() {
    gStyle->SetOptStat(0);

    // Open the file and get the tree
    auto infile = new TFile(Form("%s/%s.root", anaDirPath.c_str(), dataFile.c_str()), "READ");
    auto tree = (TTree *) infile->Get("Ana");
    auto NEntries = tree->GetEntries();

    // Prepare the histograms
    auto hElectronSpectrum          = new TH1D("hElectronSpectrum",       ";E (MeV);dN/dE (/0.5 MeV)",  100, 0, 50);
    auto hPositronSpectrum          = new TH1D("hPositronSpectrum",       ";E (Mev);dN/dE (/0.5 MeV)",  100, 0, 50);
    auto hLowEGammaSpectrum         = new TH1D("hLowEGammaSpectrum",      ";E (MeV);dN/dE",             100, 0, 1);
    auto hGammaSpectrum             = new TH1D("hGammaSpectrum",          ";E (MeV);dN/dE (/0.5 MeV)",  100, 0, 50);
    auto hGammaSpectrumWavelength   = new TH1D("hGammaSpectrumWavelength",";#lambda (nm); dN/d#lambda (/0.1 nm)", 200, 0, 20);

    auto hElectronSpectrumVersusPosition = new TH2D("hElectronSpectrumVersusPosition", ";E (MeV); R (micron)", 100, 0, 50, 127, 0, 508);
    auto hPositronSpectrumVersusPosition = new TH2D("hPositronSpectrumVersusPosition", ";E (MeV); R (micron)", 100, 0, 50, 127, 0, 508);
    auto hGammaSpectrumVersusPosition    = new TH2D("hGammaSpectrumVersusPosition",    ";E (MeV); R (micron)", 100, 0, 50, 127, 0, 508);

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
        if (TrackID == 1) continue;
        if (PDGID == 11) {
            hElectronSpectrum->Fill(E);
            hElectronSpectrumVersusPosition->Fill(E, TMath::Sqrt(x * x + z * z));
        } else if (PDGID == -11) {
            hPositronSpectrum->Fill(E);
            hPositronSpectrumVersusPosition->Fill(E, TMath::Sqrt(x * x + z * z));
        } else if (PDGID == 22) {
            if (E <= 1) hLowEGammaSpectrum->Fill(E);
            hGammaSpectrum->Fill(E);
            hGammaSpectrumWavelength->Fill(1239.842 / (E * 1E6));
            hGammaSpectrumVersusPosition->Fill(E, TMath::Sqrt(x * x + z * z));
        }
    }

    // FIXME: check the normalization
    hGammaSpectrum->Scale(10);
    hElectronSpectrum->Scale(10);
    hPositronSpectrum->Scale(10);
    double NGamma = hGammaSpectrum->Integral();
    double NElectron = hElectronSpectrum->Integral();
    double NPositron = hPositronSpectrum->Integral();

    hPositronSpectrum->Scale(50);
    hElectronSpectrum->Scale(50);
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
    hGammaSpectrum->Draw("HIST");
    hGammaSpectrum->GetXaxis()->CenterTitle();
    hGammaSpectrum->GetYaxis()->CenterTitle();
    hGammaSpectrum->GetYaxis()->SetMaxDigits(3);
    hElectronSpectrum->Draw("SAME HIST");
    hPositronSpectrum->Draw("SAME HIST");

    auto leg = new TLegend(0.45, 0.6, 0.85, 0.85);
    leg->SetBorderSize(0);
    leg->AddEntry(hElectronSpectrum, Form("50 #times Electron (%g)", NElectron), "f");
    leg->AddEntry(hPositronSpectrum, Form("50 #times Positron (%g)", NPositron), "f");
    leg->AddEntry(hGammaSpectrum, Form("Gamma (%g)", NGamma), "f");
    leg->Draw();

    canvas->SaveAs(Form("%s/%s/BackgroundOnly/SEETAna-Spectra.pdf", anaDirPath.c_str(), thickness.c_str()));

    hLowEGammaSpectrum->SetLineColor(kGreen);
    hLowEGammaSpectrum->SetFillColorAlpha(kGreen, 0.2);
    hLowEGammaSpectrum->SetLineWidth(2);
    auto LowEGammaSpectrumCanvas = new TCanvas("c1", "c1", 1200, 1200);
    LowEGammaSpectrumCanvas->SetLogy();
    hLowEGammaSpectrum->Draw();
    hLowEGammaSpectrum->GetXaxis()->CenterTitle();
    hLowEGammaSpectrum->GetYaxis()->CenterTitle();
//    hLowEGammaSpectrum->GetYaxis()->SetRangeUser(1, 1e5);
    hLowEGammaSpectrum->GetYaxis()->SetMaxDigits(3);
    LowEGammaSpectrumCanvas->SaveAs(Form("%s/%s/BackgroundOnly/SEETAna-LowEGamma.pdf", anaDirPath.c_str(), thickness.c_str()));

    auto canvas2 = new TCanvas("c2", "c2", 1200, 1200);
    canvas2->SetMargin(0.15, 0.15, 0.15, 0.15);
    hElectronSpectrumVersusPosition->Draw("COLZ");
    hElectronSpectrumVersusPosition->GetXaxis()->CenterTitle();
    hElectronSpectrumVersusPosition->GetYaxis()->CenterTitle();
    canvas2->SaveAs(Form("%s/%s/BackgroundOnly/SEETAna-ElectronSpectraVersusPosition.pdf", anaDirPath.c_str(), thickness.c_str()));

    auto canvas3 = new TCanvas("c3", "c3", 1200, 1200);
    canvas3->SetMargin(0.15, 0.15, 0.15, 0.15);
    hPositronSpectrumVersusPosition->Draw("COLZ");
    hPositronSpectrumVersusPosition->GetXaxis()->CenterTitle();
    hPositronSpectrumVersusPosition->GetYaxis()->CenterTitle();
    canvas3->SaveAs(Form("%s/%s/BackgroundOnly/SEETAna-PositronSpectraVersusPosition.pdf", anaDirPath.c_str(), thickness.c_str()));

    auto canvas4 = new TCanvas("c4", "c4", 1200, 1200);
    canvas4->SetMargin(0.15, 0.15, 0.15, 0.15);
    hGammaSpectrumVersusPosition->Draw("COLZ");
    hGammaSpectrumVersusPosition->GetXaxis()->CenterTitle();
    hGammaSpectrumVersusPosition->GetYaxis()->CenterTitle();
    canvas4->SaveAs(Form("%s/%s/BackgroundOnly/SEETAna-GammaSpectraVersusPosition.pdf", anaDirPath.c_str(), thickness.c_str()));

    delete canvas;
    delete LowEGammaSpectrumCanvas;
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
    auto hElectronSpectrum = new TH1D("hElectronSpectrum", ";E (MeV);dN/dE (/0.5 MeV)", 100, 0, 50);
    auto hPositronSpectrum = new TH1D("hPositronSpectrum", ";E (Mev);dN/dE (/0.5 MeV)", 100, 0, 50);
    auto hGammaSpectrum    = new TH1D("hGammaSpectrum",    ";E (MeV);dN/dE (/0.5 MeV)", 100, 0, 50);

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
        if (TrackID == 1) continue;
        if (PDGID == 11) {
            hElectronSpectrum->Fill(E);
        } else if (PDGID == -11) {
            hPositronSpectrum->Fill(E);
        } else if (PDGID == 22) {
            hGammaSpectrum->Fill(E);
        }
    }

    // FIXME: check the normalization
    hGammaSpectrum->Scale(10);
    hElectronSpectrum->Scale(10);
    hPositronSpectrum->Scale(10);
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
    hGammaSpectrum->Draw("HIST");
    hGammaSpectrum->GetXaxis()->CenterTitle();
    hGammaSpectrum->GetYaxis()->CenterTitle();
    hGammaSpectrum->GetYaxis()->SetMaxDigits(3);
    hGammaSpectrum->GetYaxis()->SetRangeUser(1, 1e7);
    hElectronSpectrum->Draw("SAME HIST");
    hPositronSpectrum->Draw("SAME HIST");

    auto leg = new TLegend(0.5, 0.6, 0.85, 0.87);
    leg->SetBorderSize(0);
    leg->AddEntry(hElectronSpectrum, Form("Electron (%g)", NElectron), "f");
    leg->AddEntry(hPositronSpectrum, Form("Positron (%g)", NPositron), "f");
    leg->AddEntry(hGammaSpectrum, Form("Gamma (%g)", NGamma), "f");
    leg->Draw();
    canvas->SaveAs(Form("%s/%s/BackgroundOnly/SEETAna-Spectra-Log.pdf", anaDirPath.c_str(), thickness.c_str()));

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