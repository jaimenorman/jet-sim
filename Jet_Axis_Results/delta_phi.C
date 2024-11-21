#include <iostream>
#include <cmath>
#include <TString.h>


// ROOT macro to creat delta recoil vs delta phi plots

void SetStyle();

int ptOpt = 0; // choose pt range
int collOpt = 2; // choose collision system

double pi = M_PI;
double ptMinDphi = -1;
double ptMaxDphi = -1;
TString collScheme = "empty string";
TString BIpt = "BIpt";
TString WTA = "WTA";
double Rjet = 0.2;



void delta_phi() {

    SetStyle();

    // momentum range switch
    if (ptOpt==0) {
        ptMinDphi = 10;
        ptMaxDphi = 20;
    }
    else if (ptOpt==1) {
        ptMinDphi = 20;
        ptMaxDphi = 40;
    }
    else if (ptOpt==2) {
        ptMinDphi = 40;
        ptMaxDphi = 60;
    }

    // Converting momentum integers to strings
    TString pt_title = Form("TT{20,50} - TT{5,7}");
    TString mom_range = Form("%.0f < p_{T, jet} < %.0f GeV/c",ptMinDphi,ptMaxDphi);


    // collision system switch
    if (collOpt==0) {
        collScheme = "pp";
    }

    if (collOpt==1) {
        collScheme = "NR";
    }

    if (collOpt==2) {
        collScheme = "R";
    }

  
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------- BIpt ----------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

    // Opening file for BIpt
    TString fileName_BIpt = "output_final__BIpt.root";

    size_t collSchemePos = fileName_BIpt.Index("__");
    if (collSchemePos != TString::kNPOS) {
        collSchemePos += 1;
    }
    fileName_BIpt.Insert(collSchemePos, collScheme);
    cout << fileName_BIpt.Data() << endl;

    TFile *BIpt_file = new TFile(fileName_BIpt);
    BIpt_file->ls();


    // Defining histogram for signal
    TH2F *hPtJetDphi_signal = (TH2F*)BIpt_file->Get(Form("hPtJetDPhiSignal_R0%.0f",Rjet*10+0.1));

    // Getting normalisation value
    TH1F *hNtrig = (TH1F*)BIpt_file->Get("hNtrig");
    int gbin1 = hNtrig->GetBin(2);
    double NTrig_signal = hNtrig->GetBinContent(2);
    double const1 = 1 / NTrig_signal;

    TH1F *hdPhi_signal_BIpt = (TH1F*)hPtJetDphi_signal->ProjectionY("hdPhi_signal_BIpt",hPtJetDphi_signal->GetXaxis()->FindBin(ptMinDphi+0.1),hPtJetDphi_signal->GetXaxis()->FindBin(ptMaxDphi-0.1));
    hdPhi_signal_BIpt->Scale(const1);


    // Defining histogram for reference
    TH2F *hPtJetDphi_reference = (TH2F*)BIpt_file->Get(Form("hPtJetDPhiReference_R0%.0f",Rjet*10+0.1));

    // Getting normalisation value
    int gbin2 = hNtrig->GetBin(1);
    double NTrig_reference = hNtrig->GetBinContent(1);
    double const2 = 1 / NTrig_reference;

    TH1F *hdPhi_reference_BIpt = (TH1F*)hPtJetDphi_reference->ProjectionY("hdPhipp_reference_BIpt",hPtJetDphi_reference->GetXaxis()->FindBin(ptMinDphi+0.1),hPtJetDphi_reference->GetXaxis()->FindBin(ptMaxDphi-0.1));
    hdPhi_reference_BIpt->Scale(const2);


    // Defining the histogram dPhi vs dRecoil
    TH1F *hdPhi_dRec_BIpt = new TH1F("hdPhi_dRec_BIpt", "Delta Recoil - 10-20 GeV", 100, 0, pi);
    hdPhi_dRec_BIpt->Add(hdPhi_signal_BIpt, hdPhi_reference_BIpt,1,-1);
    hdPhi_dRec_BIpt->Rebin(4);


    // Fitting data
    TF1 *fit_BIpt = new TF1("fit_BIpt","[0]*exp((x-pi)/[1]) + [2]",pi/2,pi);
    fit_BIpt->SetLineColor(kBlue);
    fit_BIpt->SetLineStyle(1);
    fit_BIpt->SetLineWidth(2);
    fit_BIpt->SetParameters(0.009,0.01,5e-12);
    hdPhi_dRec_BIpt->Fit("fit_BIpt","R0");

    // Extracting Fit Width
    double width_BIpt = fit_BIpt->GetParameter(1);
    double width_BIpt_error = fit_BIpt->GetParError(1);
    
    // Converting Width to String
    TString width_BIpt_leg = Form("width = %f",width_BIpt);
    TString width_BIpt_error_leg = Form("width error = %f",width_BIpt_error);

//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------- WTA -----------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

    // Opening file for WTA
    TString fileName_WTA = "output_final__WTA.root";

    size_t collSchemePos_WTA = fileName_WTA.Index("__");
    if (collSchemePos_WTA != TString::kNPOS) {
        collSchemePos_WTA += 1;
    }
    fileName_WTA.Insert(collSchemePos_WTA, collScheme);
    cout << fileName_WTA.Data() << endl;

    TFile *WTA_file = new TFile(fileName_WTA);
    WTA_file->ls();


    // Defining histogram for signal
    TH2F *hPtJetDphi_signal_WTA = (TH2F*)WTA_file->Get(Form("hPtJetDPhiSignal_R0%.0f",Rjet*10+0.1));

    // Getting normalisation value
    TH1F *hNtrig_WTA = (TH1F*)WTA_file->Get("hNtrig");
    int gbin1_WTA = hNtrig_WTA->GetBin(2);
    double NTrig_signal_WTA = hNtrig_WTA->GetBinContent(2);
    double const1_WTA = 1 / NTrig_signal_WTA;

    TH1F *hdPhi_signal_WTA = (TH1F*)hPtJetDphi_signal_WTA->ProjectionY("hdPhi_signal_WTA",hPtJetDphi_signal_WTA->GetXaxis()->FindBin(ptMinDphi+0.1),hPtJetDphi_signal_WTA->GetXaxis()->FindBin(ptMaxDphi-0.1));
    hdPhi_signal_WTA->Scale(const1_WTA);


    // Defining histogram for reference
    TH2F *hPtJetDphi_reference_WTA = (TH2F*)WTA_file->Get(Form("hPtJetDPhiReference_R0%.0f",Rjet*10+0.1));

    // Getting normalisation value
    int gbin2_WTA = hNtrig_WTA->GetBin(1);
    double NTrig_reference_WTA = hNtrig_WTA->GetBinContent(1);
    double const2_WTA = 1 / NTrig_reference_WTA;

    TH1F *hdPhi_reference_WTA = (TH1F*)hPtJetDphi_reference_WTA->ProjectionY("hdPhipp_reference_WTA",hPtJetDphi_reference_WTA->GetXaxis()->FindBin(ptMinDphi+0.1),hPtJetDphi_reference_WTA->GetXaxis()->FindBin(ptMaxDphi-0.1));
    hdPhi_reference_WTA->Scale(const2_WTA);


    // Defining the histogram dPhi vs dRecoil
    TH1F *hdPhi_dRec_WTA = new TH1F("hdPhi_dRec_WTA", "Delta Recoil - 10-20 GeV", 100, 0, pi);
    hdPhi_dRec_WTA->Add(hdPhi_signal_WTA, hdPhi_reference_WTA,1,-1);
    hdPhi_dRec_WTA->Rebin(4);

    // Fitting data
    TF1 *fit_WTA = new TF1("fit_WTA","[0]*exp((x-pi)/[1]) + [2]",pi/2,pi);
    fit_WTA->SetLineColor(kRed);
    fit_WTA->SetLineStyle(1);
    fit_WTA->SetLineWidth(2);
    fit_WTA->SetParameters(0.009,0.01,5e-12);
    hdPhi_dRec_WTA->Fit("fit_WTA","R0");

    // Extracting Fit Width
    double width_WTA = fit_WTA->GetParameter(1);
    double width_WTA_error = fit_WTA->GetParError(1);
    
    // Converting Width to String
    TString width_WTA_leg = Form("width = %f",width_WTA);
    TString width_WTA_error_leg = Form("width error = %f",width_WTA_error);


//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------- Plot ----------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------


    TCanvas *canvas = new TCanvas();
    canvas->SetLogy(1);
    canvas->cd();

    hdPhi_dRec_WTA->GetXaxis()->SetRangeUser(pi/2,pi);
    hdPhi_dRec_WTA->GetYaxis()->SetRangeUser(0.0001,0.05);


    hdPhi_dRec_WTA->SetTitle("");
    hdPhi_dRec_WTA->GetXaxis()->SetTitle("#Delta#phi");
    hdPhi_dRec_WTA->GetYaxis()->SetTitle("#Delta_{Recoil}");
    hdPhi_dRec_WTA->SetMarkerColor(kRed);
    hdPhi_dRec_WTA->SetLineColor(kRed);
    hdPhi_dRec_WTA->SetMarkerStyle(kFullCircle);
    fit_WTA->SetLineColor(kRed);


    hdPhi_dRec_BIpt->SetMarkerColor(kBlue);
    hdPhi_dRec_BIpt->SetLineColor(kBlue);
    hdPhi_dRec_BIpt->SetMarkerStyle(kFullSquare);
    

    // Drawing histograms on same graph
    hdPhi_dRec_WTA->Draw("");
    hdPhi_dRec_BIpt->Draw("SAME");
    fit_WTA->Draw("SAME");
    fit_BIpt->Draw("SAME");
    
    
    
    // Formatting Legend
    hdPhi_dRec_WTA->SetStats(0);
    hdPhi_dRec_BIpt->SetStats(0);

    TLegend *leg = new TLegend(0.7,0.2,0.9,0.5);
    leg->SetLineColor(kWhite);
    leg->AddEntry(hdPhi_dRec_WTA,"WTA","LEP");
    leg->AddEntry(hdPhi_dRec_BIpt,"BIpt","LEP");
    leg->SetTextSize(0.035);
    leg->Draw("SAME");
    
    
    // TLatex
    TString Width_WTA_canvas = Form("WTA: #sigma = %.3f +- %.3f rad",width_WTA,width_WTA_error);
    TString Width_BIpt_canvas = Form("BIpt:  #sigma = %.3f +- %.3f rad",width_BIpt,width_BIpt_error);

    TLatex canvas_info;
    canvas_info.SetNDC();
    canvas_info.SetTextFont(43);
    canvas_info.SetTextSize(20);
    canvas_info.DrawLatex(0.18,0.86,"JEWEL R = 0.2");
    canvas_info.DrawLatex(0.44,0.86,collScheme);
    canvas_info.DrawLatex(0.5,0.86,mom_range);
    canvas_info.DrawLatex(0.2,0.8,"Width Returned by Fit:");
    canvas_info.DrawLatex(0.2,0.74,Width_WTA_canvas);
    canvas_info.DrawLatex(0.2,0.68,Width_BIpt_canvas);
    
    
    
    //cpp->SaveAs(Form(plt_Dphi_R0%.1f_pT%.0fto%.0f.pdf",Rjet,ptMinDphi,ptMaxDphi));


 }





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
   gStyle->SetPadLeftMargin(0.15);
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
   gStyle->SetTitleOffset(1.1,"y");
   gStyle->SetTitleOffset(1.1,"x");
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