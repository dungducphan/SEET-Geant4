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

#include <iostream>
#include <filesystem>
#endif

std::string path = "/home/dphan/Documents/GitHub/SEET-Geant4/transAna";

void TransverseAna(const std::string& fileName = "Trans_1000MeV_1E7") {

    TH2D *h_RHEdep = nullptr;
    TFile* infile = nullptr;
    bool histFileExist = false;

    // Use filesystem to check if file exist
    if (!std::filesystem::exists(Form("%s/Summary/%s_HIST.root", path.c_str(), fileName.c_str()))) {
        infile = new TFile(Form("%s/Summary/%s.root", path.c_str(), fileName.c_str()), "READ");
        auto tree = (TTree *) infile->Get("Ana");
        int EventID, TrackID, PDGID;
        double x, y, z, px, py, pz, E, Edep;
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
        tree->SetBranchAddress("Edep", &Edep);

        auto NEntries = tree->GetEntries();

        // Create log x bins
        const int xlog_Nbins = 1000;
        double xlog_bins[xlog_Nbins];
        double xmin = 1E-4;
        double xmax = 1E5;
        double xlog_min = TMath::Log10(xmin);
        double xlog_max = TMath::Log10(xmax);
        double xlog_width = (xlog_max - xlog_min) / xlog_Nbins;
        for (int i = 0; i < xlog_Nbins; i++) xlog_bins[i] = TMath::Power(10, xlog_min + i * xlog_width);

        const int hlog_Nbins = 1000;
        double hlog_bins[hlog_Nbins];
        double hmin = 1;
        double hmax = 1E3;
        double hlog_min = TMath::Log10(hmin);
        double hlog_max = TMath::Log10(hmax);
        double hlog_width = (hlog_max - hlog_min) / hlog_Nbins;
        for (int i = 0; i < hlog_Nbins; i++) hlog_bins[i] = TMath::Power(10, hlog_min + i * hlog_width);


        h_RHEdep = new TH2D("h_RHEdep", ";H (um); R (um);", hlog_Nbins - 1, hlog_bins, xlog_Nbins - 1, xlog_bins);

        double h;
        for (auto i = 0; i < NEntries; i++) {
            tree->GetEntry(i);
            h = (-1. * y) - 10500;
            h_RHEdep->Fill(h, TMath::Sqrt(x * x + z * z), Edep);
        }
    } else {
        histFileExist = true;
        infile = new TFile(Form("%s/Summary/%s_HIST.root", path.c_str(), fileName.c_str()), "READ");
        h_RHEdep = (TH2D *) infile->Get("h_RHEdep");
    }

    auto c = new TCanvas("c", "c", 1200, 1200);
    c->SetMargin(0.15, 0.15, 0.15, 0.15);
    c->SetLogy();
    c->SetLogx();
    h_RHEdep->SetLineColor(kRed);
    h_RHEdep->SetLineWidth(2);
    h_RHEdep->SetFillColorAlpha(kRed, 0.3);
    h_RHEdep->Draw("COLZ");
    h_RHEdep->GetYaxis()->SetRangeUser(1E-5, 1E7);
    h_RHEdep->GetXaxis()->SetRangeUser(1, 1E3);
    h_RHEdep->GetXaxis()->CenterTitle();
    h_RHEdep->GetXaxis()->SetTitleOffset(1.2);
    h_RHEdep->GetYaxis()->CenterTitle();
    gStyle->SetOptStat(0);
    c->SaveAs(Form("%s/%s.pdf", path.c_str(), fileName.c_str()));

    if (!histFileExist) {
        auto outfile = new TFile(Form("%s/Summary/%s_HIST.root", path.c_str(), fileName.c_str()), "RECREATE");
        outfile->cd();
        h_RHEdep->Write();
        outfile->Write();
        outfile->Close();
        delete outfile;
    }

    delete h_RHEdep;
    infile->Close();
    delete infile;
    delete c;
}

#ifndef __CINT__
int main() {
    std::vector<std::string> fileList = {
//            "Trans_1000MeV_1E7",
//            "Trans_0500MeV_1E7",
//            "Trans_0200MeV_1E7",
//            "Trans_0100MeV_1E7",
//            "Trans_0050MeV_1E7",
            "Trans_0020MeV_1E7",
//            "Trans_0010MeV_1E7",
//            "Trans_0005MeV_1E7",
//            "Trans_0002MeV_1E7",
//            "Trans_0001MeV_1E7"
    };

    for (auto& file : fileList) TransverseAna(file);
    return 0;
}
#endif