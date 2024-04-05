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
std::string dataFile = "Ana_5cm_1E7";
std::string thickness = "W5cm";

void SEETAna() {
    gStyle->SetOptStat(0);

    // Open the file and get the tree
    auto infile = new TFile(Form("%s/%s.root", anaDirPath.c_str(), dataFile.c_str()), "READ");
    auto tree = (TTree *) infile->Get("Ana");
    auto NEntries = tree->GetEntries();

    // Prepare the histograms
    auto hElectronSpectrum          = new TH1D("hElectronSpectrum",       ";E (MeV);dN/dE (/0.5 MeV)",  100, 0, 50);
    auto hPositronSpectrum          = new TH1D("hPositronSpectrum",       ";E (Mev);dN/dE (/0.5 MeV)",  100, 0, 50);
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
            hGammaSpectrum->Fill(E);
            hGammaSpectrumWavelength->Fill(1239.842 / (E * 1E6));
            hGammaSpectrumVersusPosition->Fill(E, TMath::Sqrt(x * x + z * z));
        }
    }

    double NGamma = hGammaSpectrum->Integral();
    double NElectron = hElectronSpectrum->Integral();
    double NPositron = hPositronSpectrum->Integral();

    hPositronSpectrum->Scale(20);
    hElectronSpectrum->Scale(20);
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
    hElectronSpectrum->Draw("SAME HIST");
    hPositronSpectrum->Draw("SAME HIST");

    auto leg = new TLegend(0.55, 0.65, 0.85, 0.85);
    leg->SetBorderSize(0);
    leg->AddEntry(hElectronSpectrum, Form("20 #times Electron (%g)", NElectron), "f");
    leg->AddEntry(hPositronSpectrum, Form("20 #times Positron (%g)", NPositron), "f");
    leg->AddEntry(hGammaSpectrum, Form("Gamma (%g)", NGamma), "f");
    leg->Draw();

    canvas->SaveAs(Form("%s/BackgroundOnly/SEETAna-Spectra.pdf", anaDirPath.c_str()));

    hGammaSpectrumWavelength->SetFillColorAlpha(kGreen, 0.1);
    hGammaSpectrumWavelength->SetLineWidth(2);
    auto spectrumWavelength = new TCanvas("c1", "c1", 1200, 1200);
    spectrumWavelength->SetLogy();
    hGammaSpectrumWavelength->Draw();
    hGammaSpectrumWavelength->GetXaxis()->CenterTitle();
    hGammaSpectrumWavelength->GetYaxis()->CenterTitle();
    hGammaSpectrumWavelength->GetYaxis()->SetRangeUser(1, 1e5);
    hGammaSpectrumWavelength->GetYaxis()->SetMaxDigits(3);
    spectrumWavelength->SaveAs(Form("%s/%s/BackgroundOnly/SEETAna-SpectraWavelength.pdf", anaDirPath.c_str(), thickness.c_str()));

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
    delete spectrumWavelength;
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