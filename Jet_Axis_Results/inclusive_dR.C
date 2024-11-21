#include<cstdio>
#include<cstdlib>
#include<iostream>
#include "TROOT.h"
#include "TTree.h"
#include "TFile.h"
#include "TRandom.h"

// ROOT macro to create dR_axis plots for inclusive jets

void SetStyle();

 void SetStyle() {
   cout << "Setting style!" << endl;

   gStyle->Reset("Plain");
   gStyle->SetOptTitle(0);
   gStyle->SetOptStat(0);
   gStyle->SetPalette(1);
   gStyle->SetCanvasColor(10);
   //  gStyle->SetCanvasBorderMode(0);
   //  gStyle->SetFrameLineWidth(1);
   gStyle->SetFrameFillColor(kWhite);
   gStyle->SetPadColor(10);
   gStyle->SetPadTickX(1);
   gStyle->SetPadTickY(1);
   gStyle->SetPadBottomMargin(0.14);
   gStyle->SetPadLeftMargin(0.175);
   gStyle->SetPadTopMargin(0.07);
   gStyle->SetPadRightMargin(0.10);
   gStyle->SetHistLineWidth(2);
   gStyle->SetHistLineColor(kRed);
   gStyle->SetFuncWidth(2);
   gStyle->SetFuncColor(kGreen);
   //gStyle->SetLineWidth(2);
   gStyle->SetLabelSize(0.055,"xyz");
   gStyle->SetLabelOffset(0.01,"y");
   gStyle->SetLabelOffset(0.01,"x");
   gStyle->SetLabelColor(kBlack,"xyz");
   gStyle->SetTitleSize(0.06,"xyz");
   gStyle->SetTitleSize(0.06,"x");
   gStyle->SetTitleOffset(1.3,"y");
   gStyle->SetTitleOffset(1.15,"x");
   gStyle->SetTitleFillColor(kWhite);
   //  gStyle->SetTextSizePixels(30);
   gStyle->SetTextFont(42);
   gStyle->SetLegendBorderSize(0);
   gStyle->SetLegendFillColor(kWhite);
   gStyle->SetLegendFont(42);
   //gStyle->SetLineWidth(2);

   // gStyle->SetMarkerStyle(20);
   // gStyle->SetMarkerSize(1.2);
   // gStyle->SetMarkerColor(kBlack);
  }


void dRadius() {

    SetStyle();    

    // Options
    float ptMin = 40;   // 10-20 20-40 40-60
    float ptMax = 60;
    float radius = 5;   // 2 or 5

    // String Conversion
    TString mom_range = Form("%.0f < p_{T, jet} < %.0f GeV/c",ptMin,ptMax);
    TString rad = Form("R = 0.%.0f",radius);
    
    // Creating canvas
    TCanvas *c =new TCanvas();
    c->SetLogy(1);
    //c->Divide(3,1,0.001,0.001,0);
    
    
    
    // Opening files
    TFile *pp = new TFile("output_24.03M_pp_BIpt.root");
    pp->ls();
    TFile *Rec = new TFile("output_R_BIpt.root");
    Rec->ls();
    TFile *NoRec = new TFile("output_24.03M_NR_BIpt.root");
    NoRec->ls();
    
    // Defining 2D histograms for projection
    TString histName;
    if(radius==2) histName = "hdJetRadpT_R02";
    if(radius==5) histName = "hdJetRadpT_R05";

    TH2F *hdJetRadpT_pp = (TH2F*)pp->Get(histName);
    TH2F *hdJetRadpT_R = (TH2F*)Rec->Get(histName);
    TH2F *hdJetRadpT_NR = (TH2F*)NoRec->Get(histName);
    
    
    // Defining 1D histograms
    TH1F *hdR_pp = (TH1F*)hdJetRadpT_pp->ProjectionY("hdR_pp",hdJetRadpT_pp->GetXaxis()->FindBin(ptMin+0.1),hdJetRadpT_pp->GetXaxis()->FindBin(ptMax-0.1));
    
    TH1F *hdR_R = (TH1F*)hdJetRadpT_R->ProjectionY("hdR_R",hdJetRadpT_R->GetXaxis()->FindBin(ptMin+0.1),hdJetRadpT_R->GetXaxis()->FindBin(ptMax-0.1));
    
    TH1F *hdR_NR = (TH1F*)hdJetRadpT_NR->ProjectionY("hdR_NR",hdJetRadpT_NR->GetXaxis()->FindBin(ptMin+0.1),hdJetRadpT_NR->GetXaxis()->FindBin(ptMax-0.1));
    
    
    // Get Mean
    double mean_pp = hdR_pp->GetMean();
    double meanError_pp = hdR_pp->GetMeanError();
    cout << fixed << setprecision(4) << "Mean pp = " << mean_pp << " +- " << meanError_pp << endl;
    
    double mean_R = hdR_R->GetMean();
    double meanError_R = hdR_R->GetMeanError();
    cout << fixed << setprecision(4) << "Mean R = " << mean_R << " +- " << meanError_R << endl;
    
    double mean_NR = hdR_NR->GetMean();
    double meanError_NR = hdR_NR->GetMeanError();
    cout << fixed << setprecision(4) << "Mean NR = " << mean_NR << " +- " << meanError_NR << endl;
    
    // Converting Mean to String for Legend
    TString meanLeg_pp = Form("pp: %.4f +- %.4f", mean_pp, meanError_pp);
    TString meanLeg_R = Form("R: %.4f +- %.4f", mean_R, meanError_R);
    TString meanLeg_NR = Form("NR: %.4f +- %.4f", mean_NR, meanError_NR);
    
    
    // Formatting histograms
    hdR_pp->SetTitle("");
    hdR_pp->GetYaxis()->SetTitle("#frac{1}{N_{jets}} #frac{dN_{jets}}{d#DeltaR_{axis}}");
    hdR_pp->GetXaxis()->SetTitle("#DeltaR_{axis}");
    hdR_pp->Scale(1./hdR_pp->Integral("width"));
    hdR_pp->SetMarkerStyle(kFullSquare);
    hdR_pp->SetMarkerColor(kBlue);
    hdR_pp->SetLabelSize(0.055);
    hdR_pp->SetTitleSize(0.06);
    
    hdR_R->SetTitle("");
    hdR_R->GetYaxis()->SetTitle("");
    hdR_R->GetXaxis()->SetTitle("");
    hdR_R->SetMarkerColor(kRed);
    hdR_R->SetLineColor(kRed);
    hdR_R->Scale(1./hdR_R->Integral("width"));
    hdR_R->SetMarkerStyle(kFullCircle);
    
    hdR_NR->SetTitle("");
    hdR_NR->GetYaxis()->SetTitle("");
    hdR_NR->GetXaxis()->SetTitle("");
    hdR_NR->SetMarkerColor(kGreen);
    hdR_NR->SetLineColor(kGreen);
    hdR_NR->Scale(1./hdR_NR->Integral("width"));
    hdR_NR->SetMarkerStyle(kFullTriangleUp);
    
    // Rebinning histograms
    //hdR_pp->Rebin(2);
    //hdR_R->Rebin(2);
    //hdR_NR->Rebin(2);
    
    hdR_pp->GetXaxis()->SetRangeUser(0,0.26);
    hdR_pp->GetYaxis()->SetRangeUser(0.01,55);
    
    // Drawing histograms
    hdR_pp->Draw();
    hdR_R->Draw("SAME");
    hdR_NR->Draw("SAME");
    
    
    // Legend
    TLegend *leg = new TLegend(0.2,0.2,0.6,0.45);
    hdR_pp->SetStats(0);
    leg->AddEntry(hdR_pp,Form("#splitline{pp}{mean = %s}",meanLeg_pp.Data()),"LEP");
    leg->AddEntry(hdR_NR,Form("#splitline{Recoils off}{mean = %s}",meanLeg_NR.Data()),"LEP");
    leg->AddEntry(hdR_R,Form("#splitline{Recoils on}{mean = %s}",meanLeg_R.Data()),"LEP");
    leg->SetLineColor(kWhite);
    leg->SetTextSize(0.04);
    leg->Draw("SAME");


    TLatex info;
    info.SetNDC();
    info.SetTextFont(43);
    info.SetTextSize(25);
    info.DrawLatex(0.2,0.85,"JEWEL");
    info.DrawLatex(0.38,0.85,rad);
    info.DrawLatex(0.56,0.85,mom_range);
    

    // Saving Figures
    TString saveName = Form("plt_R0%.0f_%.0f_%.0f.pdf",radius,ptMin,ptMax);
    c->SaveAs(saveName);
}

