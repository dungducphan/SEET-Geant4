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
#include <TPaveText.h>
#include <TLatex.h>

#include <iostream>
#include <filesystem>
#endif

double Emax = 1000;
double Emin = 0;
int Nbins = 200;
std::string path = "/home/dphan/Documents/GitHub/SEET-Geant4/ana";
std::string filename = "SNRatio_960E6_1cm";

void SNRatio() {
    TH1D* hist_SignalEdep = nullptr;
    TH1D* hist_BackgroundBkgdEdep = nullptr;
    TH1D* hist_ElectronPositronBackgroundBkgdEdep = nullptr;
    TH1D* hist_GammaBackgroundBkgdEdep = nullptr;

    // Check if summary file exist using filesystem
    bool foundSummaryFile = std::filesystem::exists(Form("%s/%s.outhist.root", path.c_str(), filename.c_str()));
    TFile* summaryFile = nullptr;
    if (foundSummaryFile) {
        std::cout << "Summary file found." << std::endl;

        summaryFile = new TFile(Form("%s/%s.outhist.root", path.c_str(), filename.c_str()), "READ");
        hist_SignalEdep = (TH1D*) summaryFile->Get("hist_SignalEdep");
        hist_BackgroundBkgdEdep = (TH1D *) summaryFile->Get("hist_BackgroundBkgdEdep");
        hist_ElectronPositronBackgroundBkgdEdep = (TH1D *) summaryFile->Get("hist_ElectronPositronBackgroundBkgdEdep");
        hist_GammaBackgroundBkgdEdep = (TH1D *) summaryFile->Get("hist_GammaBackgroundBkgdEdep");

    } else {
        std::cout << "Summary file not found." << std::endl;

        auto infile = new TFile(Form("%s/%s.root", path.c_str(), filename.c_str()), "READ");
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

        hist_SignalEdep = new TH1D("hist_SignalEdep", ";Signal Energy Deposit (keV);dN/dE", Nbins, Emin, Emax);
        hist_BackgroundBkgdEdep = new TH1D("hist_BackgroundBkgdEdep", ";Background Energy Deposit (keV);dN/dE", Nbins, Emin, Emax);
        hist_ElectronPositronBackgroundBkgdEdep = new TH1D("hist_ElectronPositronBackgroundBkgdEdep", ";Electron/Positron Background Energy Deposit (keV);dN/dE", Nbins, Emin, Emax);
        hist_GammaBackgroundBkgdEdep = new TH1D("hist_GammaBackgroundBkgdEdep", ";Gamma Background Energy Deposit (keV);dN/dE", Nbins, Emin, Emax);
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
    }

    gStyle->SetOptStat(0);
    // Styling
    hist_SignalEdep->SetLineColor(kRed);
    hist_SignalEdep->SetLineWidth(2);
    hist_SignalEdep->SetFillColorAlpha(kRed, 0.6);

    hist_BackgroundBkgdEdep->SetLineColor(kBlue);
    hist_BackgroundBkgdEdep->SetLineWidth(2);
    hist_BackgroundBkgdEdep->SetFillColorAlpha(kBlue, 0.6);

    hist_ElectronPositronBackgroundBkgdEdep->SetLineColor(kGreen);
    hist_ElectronPositronBackgroundBkgdEdep->SetLineStyle(kDashed);
    hist_ElectronPositronBackgroundBkgdEdep->SetLineWidth(2);
    hist_ElectronPositronBackgroundBkgdEdep->SetFillColorAlpha(kGreen, 0.2);

    hist_GammaBackgroundBkgdEdep->SetLineColor(kOrange);
    hist_GammaBackgroundBkgdEdep->SetLineStyle(kDashed);
    hist_GammaBackgroundBkgdEdep->SetLineWidth(2);
    hist_GammaBackgroundBkgdEdep->SetFillColorAlpha(kOrange, 0.2);

    // Drawing
    auto c1 = new TCanvas("c1", "c1", 1200, 1200);
    hist_SignalEdep->Draw("HIST");
    hist_SignalEdep->GetXaxis()->CenterTitle();
    hist_SignalEdep->GetXaxis()->SetTitle("Energy Deposition (keV)");
    hist_SignalEdep->GetYaxis()->CenterTitle();
    hist_SignalEdep->GetYaxis()->SetTitle("dN/dE_{d}");
    hist_SignalEdep->GetYaxis()->SetMaxDigits(3);
    hist_SignalEdep->GetXaxis()->SetRangeUser(0, 500);
    hist_SignalEdep->GetYaxis()->SetRangeUser(1, 7e5);
    hist_BackgroundBkgdEdep->Draw("SAME HIST");
    hist_ElectronPositronBackgroundBkgdEdep->Draw("SAME HIST");
    hist_GammaBackgroundBkgdEdep->Draw("SAME HIST");

    auto leg = new TLegend(0.55, 0.4, 0.8, 0.7);
    leg->SetBorderSize(0);
    leg->AddEntry(hist_SignalEdep, Form("Signal"), "f");
    leg->AddEntry(hist_BackgroundBkgdEdep, Form("Total bkgd"), "f");
    leg->AddEntry(hist_ElectronPositronBackgroundBkgdEdep, Form("e^{#pm} bkgd"), "f");
    leg->AddEntry(hist_GammaBackgroundBkgdEdep, Form("#gamma bkgd"), "f");
    leg->Draw();

    auto pt = new TLatex();

    pt->DrawLatex(300,1E5,"#frac{#color[4]{B}}{#color[4]{B} + #color[2]{S}} = 4.4%");

    c1->SaveAs(Form("%s/%s.pdf", path.c_str(), filename.c_str()));

    std::cout << "Signal Energy Deposit: " << hist_SignalEdep->Integral() << std::endl;
    std::cout << "Total Background Energy Deposit: " << hist_BackgroundBkgdEdep->Integral() << std::endl;
    std::cout << "Electron/Positron Background Energy Deposit: " << hist_ElectronPositronBackgroundBkgdEdep->Integral() << std::endl;
    std::cout << "Gamma Background Energy Deposit: " << hist_GammaBackgroundBkgdEdep->Integral() << std::endl;
    std::cout << "Background Contribution: " << 100. * hist_BackgroundBkgdEdep->Integral() / (hist_SignalEdep->Integral() + hist_BackgroundBkgdEdep->Integral()) << "%." << std::endl;

    delete c1;
    delete leg;
    delete pt;
    delete hist_SignalEdep;
    delete hist_BackgroundBkgdEdep;
    delete hist_ElectronPositronBackgroundBkgdEdep;
    delete hist_GammaBackgroundBkgdEdep;

    if (foundSummaryFile) {
        summaryFile->Close();
        return;
    }

    summaryFile = new TFile(Form("%s/%s.outhist.root", path.c_str(), filename.c_str()), "RECREATE");
    summaryFile->cd();
    hist_SignalEdep->Write();
    hist_BackgroundBkgdEdep->Write();
    hist_ElectronPositronBackgroundBkgdEdep->Write();
    hist_GammaBackgroundBkgdEdep->Write();
    summaryFile->Write();
    summaryFile->Close();
}

void SNRatioLog() {
    TH1D* hist_SignalEdep = nullptr;
    TH1D* hist_BackgroundBkgdEdep = nullptr;
    TH1D* hist_ElectronPositronBackgroundBkgdEdep = nullptr;
    TH1D* hist_GammaBackgroundBkgdEdep = nullptr;

    // Check if summary file exist using filesystem
    bool foundSummaryFile = std::filesystem::exists(Form("%s/%s.outhist.root", path.c_str(), filename.c_str()));
    TFile* summaryFile = nullptr;
    if (foundSummaryFile) {
        std::cout << "Summary file found." << std::endl;

        summaryFile = new TFile(Form("%s/%s.outhist.root", path.c_str(), filename.c_str()), "READ");
        hist_SignalEdep = (TH1D*) summaryFile->Get("hist_SignalEdep");
        hist_BackgroundBkgdEdep = (TH1D *) summaryFile->Get("hist_BackgroundBkgdEdep");
        hist_ElectronPositronBackgroundBkgdEdep = (TH1D *) summaryFile->Get("hist_ElectronPositronBackgroundBkgdEdep");
        hist_GammaBackgroundBkgdEdep = (TH1D *) summaryFile->Get("hist_GammaBackgroundBkgdEdep");

    } else {
        std::cout << "Summary file not found." << std::endl;

        auto infile = new TFile(Form("%s/%s.root", path.c_str(), filename.c_str()), "READ");
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

        hist_SignalEdep = new TH1D("hist_SignalEdep", ";Signal Energy Deposit (keV);dN/dE", Nbins, Emin, Emax);
        hist_BackgroundBkgdEdep = new TH1D("hist_BackgroundBkgdEdep", ";Background Energy Deposit (keV);dN/dE", Nbins, Emin, Emax);
        hist_ElectronPositronBackgroundBkgdEdep = new TH1D("hist_ElectronPositronBackgroundBkgdEdep", ";Electron/Positron Background Energy Deposit (keV);dN/dE", Nbins, Emin, Emax);
        hist_GammaBackgroundBkgdEdep = new TH1D("hist_GammaBackgroundBkgdEdep", ";Gamma Background Energy Deposit (keV);dN/dE", Nbins, Emin, Emax);
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
    }

    gStyle->SetOptStat(0);
    // Styling
    hist_SignalEdep->SetLineColor(kRed);
    hist_SignalEdep->SetLineWidth(2);
    hist_SignalEdep->SetFillColorAlpha(kRed, 0.6);

    hist_BackgroundBkgdEdep->SetLineColor(kBlue);
    hist_BackgroundBkgdEdep->SetLineWidth(2);
    hist_BackgroundBkgdEdep->SetFillColorAlpha(kBlue, 0.6);

    hist_ElectronPositronBackgroundBkgdEdep->SetLineColor(kGreen);
    hist_ElectronPositronBackgroundBkgdEdep->SetLineStyle(kDashed);
    hist_ElectronPositronBackgroundBkgdEdep->SetLineWidth(2);
    hist_ElectronPositronBackgroundBkgdEdep->SetFillColorAlpha(kGreen, 0.2);

    hist_GammaBackgroundBkgdEdep->SetLineColor(kOrange);
    hist_GammaBackgroundBkgdEdep->SetLineStyle(kDashed);
    hist_GammaBackgroundBkgdEdep->SetLineWidth(2);
    hist_GammaBackgroundBkgdEdep->SetFillColorAlpha(kOrange, 0.2);

    // Drawing
    auto c1 = new TCanvas("c1", "c1", 1200, 1200);
    c1->SetLogy();
    hist_SignalEdep->Draw("HIST");
    hist_SignalEdep->GetXaxis()->CenterTitle();
    hist_SignalEdep->GetXaxis()->SetTitle("Energy Deposition (keV)");
    hist_SignalEdep->GetYaxis()->CenterTitle();
    hist_SignalEdep->GetYaxis()->SetTitle("dN/dE_{d}");
    hist_SignalEdep->GetYaxis()->SetMaxDigits(3);
    hist_SignalEdep->GetYaxis()->SetRangeUser(1, 1e6);
    hist_BackgroundBkgdEdep->Draw("SAME HIST");
    hist_ElectronPositronBackgroundBkgdEdep->Draw("SAME HIST");
    hist_GammaBackgroundBkgdEdep->Draw("SAME HIST");

    auto leg = new TLegend(0.55, 0.4, 0.8, 0.7);
    leg->SetBorderSize(0);
    leg->AddEntry(hist_SignalEdep, Form("Signal"), "f");
    leg->AddEntry(hist_BackgroundBkgdEdep, Form("Total bkgd"), "f");
    leg->AddEntry(hist_ElectronPositronBackgroundBkgdEdep, Form("e^{#pm} bkgd"), "f");
    leg->AddEntry(hist_GammaBackgroundBkgdEdep, Form("#gamma bkgd"), "f");
    leg->Draw();

    auto pt = new TLatex();

    pt->DrawLatex(300,1E5,"#frac{#color[4]{B}}{#color[4]{B} + #color[2]{S}} = 4.4%");

    c1->SaveAs(Form("%s/%s_log.pdf", path.c_str(), filename.c_str()));

    std::cout << "Signal Energy Deposit: " << hist_SignalEdep->Integral() << std::endl;
    std::cout << "Total Background Energy Deposit: " << hist_BackgroundBkgdEdep->Integral() << std::endl;
    std::cout << "Electron/Positron Background Energy Deposit: " << hist_ElectronPositronBackgroundBkgdEdep->Integral() << std::endl;
    std::cout << "Gamma Background Energy Deposit: " << hist_GammaBackgroundBkgdEdep->Integral() << std::endl;
    std::cout << "Background Contribution: " << 100. * hist_BackgroundBkgdEdep->Integral() / (hist_SignalEdep->Integral() + hist_BackgroundBkgdEdep->Integral()) << "%." << std::endl;

    delete c1;
    delete leg;
    delete pt;
    delete hist_SignalEdep;
    delete hist_BackgroundBkgdEdep;
    delete hist_ElectronPositronBackgroundBkgdEdep;
    delete hist_GammaBackgroundBkgdEdep;

    if (foundSummaryFile) {
        summaryFile->Close();
        return;
    }

    summaryFile = new TFile(Form("%s/%s.outhist.root", path.c_str(), filename.c_str()), "RECREATE");
    summaryFile->cd();
    hist_SignalEdep->Write();
    hist_BackgroundBkgdEdep->Write();
    hist_ElectronPositronBackgroundBkgdEdep->Write();
    hist_GammaBackgroundBkgdEdep->Write();
    summaryFile->Write();
    summaryFile->Close();
}


#ifndef __CINT__
int main() {
    SNRatio();
    SNRatioLog();
    return 0;
}
#endif