#include <iostream>

// ROOT macro to compare the delta R_axis in the peak and tail of the delta phi distributions

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
   gStyle->SetPadLeftMargin(0.178);
   gStyle->SetPadTopMargin(0.07);
   gStyle->SetPadRightMargin(0.10);
   gStyle->SetHistLineWidth(2);
   gStyle->SetHistLineColor(kRed);
   gStyle->SetFuncWidth(2);
   gStyle->SetFuncColor(kGreen);
   //gStyle->SetLineWidth(2);
   gStyle->SetLabelSize(0.05,"xyz");
   gStyle->SetLabelOffset(0.01,"y");
   gStyle->SetLabelOffset(0.01,"x");
   gStyle->SetLabelColor(kBlack,"xyz");
   gStyle->SetTitleSize(0.06,"xyz");
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

void comparison_PT_dR() {     // Comparing the peaks and tails dR distributions
    
    SetStyle();
        
    // Variable Definitions
    double ptMin = -1;
    double ptMax = -1;
    double pi = 3.141592653589793238462643383279502884197;
    double Rjet = 0.2;
    double const_sig = -1;
    double const_ref = -1;
    double NTrig_ref;
    double NTrig_sig;
    double Peak_min = pi - 0.6;
    double Peak_max = pi;
    double Tail_min = pi/2;
    double Tail_max = pi/2 + 0.6;
    double mean_peak;
    double mean_peak_error;
    double mean_tail;
    double mean_tail_error;
    
    // Options
    int ptOpt = 2;
    int collOpt = 2;
    
    // Creating option for pt range
    if (ptOpt==0) {
        ptMin = 10;
        ptMax = 20;
    }
    else if (ptOpt==1) {
        ptMin = 20;
        ptMax = 40;
    }
    else if (ptOpt==2) {
        ptMin = 40;
        ptMax = 60;
    }
    
    // Converting momentum integers to strings
    TString pt_title = Form("TT{20,50} - TT{5,7}");
    TString mom_range = Form("%.0f < p_{T, jet} < %.0f GeV/c",ptMin,ptMax);
    TString rad = Form("R = %.1f",Rjet);
    TString collScheme;
    
    
    // Opening files
    TFile *pp = new TFile("output_final_pp_BIpt.root");
    pp->ls();
    TFile *R = new TFile("output_final_Recoils_on_BIpt.root");
    R->ls();
    TFile *NR = new TFile("output_final_Recoils_off_BIpt.root");
    NR->ls();
    
    TH3F *h_pT_Phi_dR_signal = new TH3F("h_pT_Phi_dR", "Title",100,0,100,50,pi/2,pi,50,0,0.8);
    TH3F *h_pT_Phi_dR_reference = new TH3F("h_pT_Phi_dR", "Title",100,0,100,50,pi/2,pi,50,0,0.8);

    // Histogram definitions for if statements
    TH1F *hNTrig;
    TH3F *h_pT_Phi_dR;
    TH1F *hdR_tail;
    TH1F *hdR_peak;

    
    // Creating canvas
    TCanvas *canvas = new TCanvas();
    canvas->SetLogy(1);
    
    
    
    if (collOpt==0) { // pp
        collScheme = Form("pp");
        
        TH3F *h_pT_Phi_dR_signal = (TH3F*)pp->Get(Form("hRecoilSignal_pT_dPhi_difaxes_R0%.0f",Rjet*10+0.1));
        TH3F *h_pT_Phi_dR_reference = (TH3F*)pp->Get(Form("hRecoilReference_pT_dPhi_difaxes_R0%.0f",Rjet*10+0.1));
        
        hNTrig = (TH1F*)pp->Get("hNtrig");
        NTrig_ref = hNTrig->GetBinContent(1);
        const_ref = 1 / NTrig_ref;
        
        NTrig_sig = hNTrig->GetBinContent(2);
        const_sig = 1 / NTrig_sig;
        
        h_pT_Phi_dR_reference->Scale(const_ref);
        h_pT_Phi_dR_signal->Scale(const_sig);
        
        h_pT_Phi_dR = new TH3F("h_pT_Phi_dR", "h_pT_Phi_dR",100,0,100,50,pi/2,pi,50,0,0.8);
        h_pT_Phi_dR->Add(h_pT_Phi_dR_signal,h_pT_Phi_dR_reference,1,-1);
        
        
        // Defining 1D histograms
        hdR_tail = (TH1F*)h_pT_Phi_dR->ProjectionZ("hdR_tail",h_pT_Phi_dR->GetXaxis()->FindBin(ptMin+0.1),h_pT_Phi_dR->GetXaxis()->FindBin(ptMax-0.1),h_pT_Phi_dR->GetYaxis()->FindBin(Tail_min),h_pT_Phi_dR->GetYaxis()->FindBin(Tail_max));
        
        hdR_peak = (TH1F*)h_pT_Phi_dR->ProjectionZ("hdR_peak",h_pT_Phi_dR->GetXaxis()->FindBin(ptMin+0.1),h_pT_Phi_dR->GetXaxis()->FindBin(ptMax-0.1),h_pT_Phi_dR->GetYaxis()->FindBin(Peak_min),h_pT_Phi_dR->GetYaxis()->FindBin(Peak_max));
        
        // Get mean
        mean_tail = hdR_tail->GetMean();
        mean_tail_error = hdR_tail->GetMeanError();
        mean_peak = hdR_peak->GetMean();
        mean_peak_error = hdR_peak->GetMeanError();
    }
    
    else if (collOpt==1) { // NR
        collScheme = Form("NR");
        
        TH3F *h_pT_Phi_dR_signal = (TH3F*)NR->Get(Form("hRecoilSignal_pT_dPhi_difaxes_R0%.0f",Rjet*10+0.1));
        TH3F *h_pT_Phi_dR_reference = (TH3F*)NR->Get(Form("hRecoilReference_pT_dPhi_difaxes_R0%.0f",Rjet*10+0.1));
        
        hNTrig = (TH1F*)NR->Get("hNtrig");
        NTrig_ref = hNTrig->GetBinContent(1);
        const_ref = 1 / NTrig_ref;
        
        NTrig_sig = hNTrig->GetBinContent(2);
        const_sig = 1 / NTrig_sig;
        
        h_pT_Phi_dR_reference->Scale(const_ref);
        h_pT_Phi_dR_signal->Scale(const_sig);
        
        h_pT_Phi_dR = new TH3F("h_pT_Phi_dR", "h_pT_Phi_dR",100,0,100,50,pi/2,pi,50,0,0.8);
        h_pT_Phi_dR->Add(h_pT_Phi_dR_signal,h_pT_Phi_dR_reference,1,-1);
        
        
        // Defining 1D histograms
        
        hdR_tail = (TH1F*)h_pT_Phi_dR->ProjectionZ("hdR_tail",h_pT_Phi_dR->GetXaxis()->FindBin(ptMin+0.1),h_pT_Phi_dR->GetXaxis()->FindBin(ptMax-0.1),h_pT_Phi_dR->GetYaxis()->FindBin(Tail_min),h_pT_Phi_dR->GetYaxis()->FindBin(Tail_max));
        
        hdR_peak = (TH1F*)h_pT_Phi_dR->ProjectionZ("hdR_peak",h_pT_Phi_dR->GetXaxis()->FindBin(ptMin+0.1),h_pT_Phi_dR->GetXaxis()->FindBin(ptMax-0.1),h_pT_Phi_dR->GetYaxis()->FindBin(Peak_min),h_pT_Phi_dR->GetYaxis()->FindBin(Peak_max));
        
        // Get mean
        mean_tail = hdR_tail->GetMean();
        mean_tail_error = hdR_tail->GetMeanError();
        mean_peak = hdR_peak->GetMean();
        mean_peak_error = hdR_peak->GetMeanError();
    }
    
    else if (collOpt==2) { // R
        collScheme = Form("R");
        
        TH3F *h_pT_Phi_dR_signal = (TH3F*)R->Get(Form("hRecoilSignal_pT_dPhi_difaxes_R0%.0f",Rjet*10+0.1));
        TH3F *h_pT_Phi_dR_reference = (TH3F*)R->Get(Form("hRecoilReference_pT_dPhi_difaxes_R0%.0f",Rjet*10+0.1));
        
        hNTrig = (TH1F*)R->Get("hNtrig");
        NTrig_ref = hNTrig->GetBinContent(1);
        const_ref = 1 / NTrig_ref;
        
        NTrig_sig = hNTrig->GetBinContent(2);
        const_sig = 1 / NTrig_sig;
        
        h_pT_Phi_dR_reference->Scale(const_ref);
        h_pT_Phi_dR_signal->Scale(const_sig);
        
        h_pT_Phi_dR = new TH3F("h_pT_Phi_dR", "h_pT_Phi_dR",100,0,100,50,pi/2,pi,50,0,0.8);
        h_pT_Phi_dR->Add(h_pT_Phi_dR_signal,h_pT_Phi_dR_reference,1,-1);
        
        // Defining 1D histograms
        
        hdR_tail = (TH1F*)h_pT_Phi_dR->ProjectionZ("hdR_tail",h_pT_Phi_dR->GetXaxis()->FindBin(ptMin+0.1),h_pT_Phi_dR->GetXaxis()->FindBin(ptMax-0.1),h_pT_Phi_dR->GetYaxis()->FindBin(Tail_min),h_pT_Phi_dR->GetYaxis()->FindBin(Tail_max));
        
        hdR_peak = (TH1F*)h_pT_Phi_dR->ProjectionZ("hdR_peak",h_pT_Phi_dR->GetXaxis()->FindBin(ptMin+0.1),h_pT_Phi_dR->GetXaxis()->FindBin(ptMax-0.1),h_pT_Phi_dR->GetYaxis()->FindBin(Peak_min),h_pT_Phi_dR->GetYaxis()->FindBin(Peak_max));
         
        // Get mean
        mean_tail = hdR_tail->GetMean();
        mean_tail_error = hdR_tail->GetMeanError();
        mean_peak = hdR_peak->GetMean();
        mean_peak_error = hdR_peak->GetMeanError();
    }
    
    // Converting Mean to String for Legend
    TString meanLeg_tail = Form("Tail: %.3f +- %.3f", mean_tail, mean_tail_error);
    TString meanLeg_peak = Form("Peak: %.4f +- %.4f", mean_peak, mean_peak_error);
    
    
    // Formatting histograms
    hdR_tail->Rebin(2);
    hdR_tail->SetTitle("");
    hdR_tail->GetYaxis()->SetTitle("#frac{1}{N_{jets}} #frac{dN_{jets}}{d#DeltaR_{axis}}");
    hdR_tail->GetXaxis()->SetTitle("#DeltaR_{axis}");
    hdR_tail->Scale(1./hdR_tail->Integral("width"));
    hdR_tail->SetMarkerStyle(kFullSquare);
    hdR_tail->SetMarkerColor(kBlue);
    hdR_tail->SetLineColor(kBlue);
    hdR_tail->GetXaxis()->SetRangeUser(0,0.12);
    hdR_tail->GetYaxis()->SetRangeUser(0.00005,200);
    
    
    hdR_peak->Rebin(2);
    hdR_peak->Scale(1./hdR_peak->Integral("width"));
    hdR_peak->SetMarkerStyle(kFullCircle);
    hdR_peak->SetMarkerColor(kRed);
    hdR_peak->SetLineColor(kRed);
    
    // Draw histogram
    hdR_tail->Draw();
    hdR_peak->Draw("SAME");
    
    // Legend
    TLegend *leg = new TLegend(0.2,0.2,0.6,0.45);
    hdR_tail->SetStats(0);
    leg->AddEntry(hdR_tail,Form("#splitline{Tail}{mean = %s}",meanLeg_tail.Data()),"LEP");
    leg->AddEntry(hdR_peak,Form("#splitline{Peak}{mean = %s}",meanLeg_peak.Data()),"LEP");
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
    
    double entries = hdR_tail->GetEntries();
    cout << "entries = " << entries << endl;
    
    // Saving Figures
    string saveName = Form("plt__PeakTailComp_R0%.0f_%.0f_%.0f.pdf",Rjet*10,ptMin,ptMax);
    saveName.insert(4,collScheme);
    cout << saveName;
    canvas->SaveAs(saveName.c_str());
}
