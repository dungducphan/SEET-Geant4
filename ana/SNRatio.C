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

void SNRatio() {
    auto infile = new TFile("/home/dphan/Documents/GitHub/SEET-Geant4/ana/SNRatio_96E5_1cm.root", "READ");
    auto tree = (TTree *) infile->Get("SNRatio");
    auto NEntries = tree->GetEntries();

    double SignalEdep, BackgroundBkgdEdep, ElectronPositronBackgroundBkgdEdep, GammaBackgroundBkgdEdep;
    std::vector<double> SignalEdepVec;
    std::vector<double> BackgroundBkgdEdepVec;
    std::vector<double> ElectronPositronBackgroundBkgdEdepVec;
    std::vector<double> GammaBackgroundBkgdEdepVec;
    tree->SetBranchAddress("SignalEdep", &SignalEdep);
    tree->SetBranchAddress("BackgroundBkgdEdep", &BackgroundBkgdEdep);
    tree->SetBranchAddress("ElectronPositronBackgroundBkgdEdep", &ElectronPositronBackgroundBkgdEdep);
    tree->SetBranchAddress("GammaBackgroundBkgdEdep", &GammaBackgroundBkgdEdep);

    for (auto i = 0; i < NEntries; i++) {
        tree->GetEntry(i);
        SignalEdepVec.push_back(SignalEdep);
        BackgroundBkgdEdepVec.push_back(BackgroundBkgdEdep);
        ElectronPositronBackgroundBkgdEdepVec.push_back(ElectronPositronBackgroundBkgdEdep);
        GammaBackgroundBkgdEdepVec.push_back(GammaBackgroundBkgdEdep);
    }

    infile->Close();

    auto hist_SignalEdep = new TH1D("hist_SignalEdep", ";Signal Energy Deposit (keV);dN/dE", 200, 0, 1000);
    auto hist_BackgroundBkgdEdep = new TH1D("hist_BackgroundBkgdEdep", ";Background Energy Deposit (keV);dN/dE", 200, 0, 1000);
    auto hist_ElectronPositronBackgroundBkgdEdep = new TH1D("hist_ElectronPositronBackgroundBkgdEdep", ";Electron/Positron Background Energy Deposit (keV);dN/dE", 200, 0, 1000);
    auto hist_GammaBackgroundBkgdEdep = new TH1D("hist_GammaBackgroundBkgdEdep", ";Gamma Background Energy Deposit (keV);dN/dE", 200, 0, 1000);
    for (auto i = 0; i < NEntries; i++) {
        if (SignalEdepVec[i] != 0) hist_SignalEdep->Fill(SignalEdepVec[i]);
        if (BackgroundBkgdEdepVec[i] != 0) hist_BackgroundBkgdEdep->Fill(BackgroundBkgdEdepVec[i]);
        if (ElectronPositronBackgroundBkgdEdepVec[i] != 0) hist_ElectronPositronBackgroundBkgdEdep->Fill(ElectronPositronBackgroundBkgdEdepVec[i]);
        if (GammaBackgroundBkgdEdepVec[i] != 0) hist_GammaBackgroundBkgdEdep->Fill(GammaBackgroundBkgdEdepVec[i]);
    }

    // Scale to 1e9 electrons (simulation results are for 960e6 electrons)
    hist_SignalEdep->Scale(1./0.96);
    hist_BackgroundBkgdEdep->Scale(1./0.96);
    hist_ElectronPositronBackgroundBkgdEdep->Scale(1./0.96);
    hist_GammaBackgroundBkgdEdep->Scale(1./0.96);

    // Styling
    hist_SignalEdep->SetLineColor(kRed);
    hist_SignalEdep->SetLineWidth(2);
    hist_SignalEdep->SetFillColorAlpha(kRed, 0.4);

    hist_BackgroundBkgdEdep->SetLineColor(kBlue);
    hist_BackgroundBkgdEdep->SetLineWidth(2);
    hist_BackgroundBkgdEdep->SetFillColorAlpha(kBlue, 0.4);

    hist_ElectronPositronBackgroundBkgdEdep->SetLineColor(kGreen);
    hist_ElectronPositronBackgroundBkgdEdep->SetLineStyle(kDashed);
    hist_ElectronPositronBackgroundBkgdEdep->SetLineWidth(2);
    hist_ElectronPositronBackgroundBkgdEdep->SetFillColorAlpha(kGreen, 0.4);

    hist_GammaBackgroundBkgdEdep->SetLineColor(kOrange);
    hist_GammaBackgroundBkgdEdep->SetLineStyle(kDashed);
    hist_GammaBackgroundBkgdEdep->SetLineWidth(2);
    hist_GammaBackgroundBkgdEdep->SetFillColorAlpha(kOrange, 0.4);

    // Drawing
    auto c1 = new TCanvas("c1", "c1", 1200, 1200);
    c1->SetLogy();
    hist_SignalEdep->Draw("HIST");
    hist_SignalEdep->GetXaxis()->CenterTitle();
    hist_SignalEdep->GetYaxis()->CenterTitle();
    hist_SignalEdep->GetYaxis()->SetMaxDigits(3);
    hist_SignalEdep->GetYaxis()->SetRangeUser(1, 1e7);
    hist_BackgroundBkgdEdep->Draw("SAME HIST");
    hist_ElectronPositronBackgroundBkgdEdep->Draw("SAME HIST");
    hist_GammaBackgroundBkgdEdep->Draw("SAME HIST");
    c1->SaveAs("/home/dphan/Documents/GitHub/SEET-Geant4/ana/SNRatio.pdf");

    auto outfile = new TFile("SNRatioOut.root", "RECREATE");
    outfile->cd();
    hist_SignalEdep->Write();
    hist_BackgroundBkgdEdep->Write();
    hist_ElectronPositronBackgroundBkgdEdep->Write();
    hist_GammaBackgroundBkgdEdep->Write();
    outfile->Write();
    outfile->Close();
}


#ifndef __CINT__
int main() {
    SNRatio();
    return 0;
}
#endif