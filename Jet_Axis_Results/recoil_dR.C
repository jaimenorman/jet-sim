#include <iostream>

// ROOT macro to create dR_axis plots for recoil jets

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


void dRadius_recoil() {
    
    SetStyle();
    
    double pi = 3.141592653589793238462643383279502884197;
    double Rjet = 0.5;
    float ptMin = 40;
    float ptMax = 60;
    
    
    // Opening files
    TFile *pp = new TFile("output_24.03M_pp_BIpt.root");
    pp->ls();
    TFile *R = new TFile("output_R_BIpt.root");
    R->ls();
    TFile *NR = new TFile("output_24.03M_NR_BIpt.root");
    NR->ls();
    
    
    // Defining Signal
    TH3F *h_pT_pPhi_dR_pp_signal = (TH3F*)pp->Get(Form("hRecoilSignal_pT_dPhi_difaxes_R0%.0f",Rjet*10+0.1));
    TH3F *h_pT_pPhi_dR_NR_signal = (TH3F*)NR->Get(Form("hRecoilSignal_pT_dPhi_difaxes_R0%.0f",Rjet*10+0.1));
    TH3F *h_pT_pPhi_dR_R_signal = (TH3F*)R->Get(Form("hRecoilSignal_pT_dPhi_difaxes_R0%.0f",Rjet*10+0.1));
    
    // Defining Reference
    TH3F *h_pT_pPhi_dR_pp_reference = (TH3F*)pp->Get(Form("hRecoilReference_pT_dPhi_difaxes_R0%.0f",Rjet*10+0.1));
    TH3F *h_pT_pPhi_dR_NR_reference = (TH3F*)NR->Get(Form("hRecoilReference_pT_dPhi_difaxes_R0%.0f",Rjet*10+0.1));
    TH3F *h_pT_pPhi_dR_R_reference = (TH3F*)R->Get(Form("hRecoilReference_pT_dPhi_difaxes_R0%.0f",Rjet*10+0.1));
    
    // Normalisations
    
    // pp
    TH1F *hNtrigpp = (TH1F*)pp->Get("hNtrig");
    double NTrig_ref_pp = hNtrigpp->GetBinContent(1);
    double const_ref_pp = 1 / NTrig_ref_pp;
    
    double NTrig_sig_pp = hNtrigpp->GetBinContent(2);
    double const_sig_pp = 1 / NTrig_sig_pp;
    
    // NR
    TH1F *hNtrigNR = (TH1F*)NR->Get("hNtrig");
    double NTrig_ref_NR = hNtrigNR->GetBinContent(1);
    double const_ref_NR = 1 / NTrig_ref_NR;
    
    double NTrig_sig_NR = hNtrigNR->GetBinContent(2);
    double const_sig_NR = 1 / NTrig_sig_NR;
    
    // R
    TH1F *hNtrigR = (TH1F*)R->Get("hNtrig");
    double NTrig_ref_R = hNtrigR->GetBinContent(1);
    double const_ref_R = 1 / NTrig_ref_R;
    
    double NTrig_sig_R = hNtrigR->GetBinContent(2);
    double const_sig_R = 1 / NTrig_sig_R;
    
    // Creating option for Tails of distibution
    double TailOpt = 1;
    double TailMin = -1;
    double TailMax = -1;
    
    string peakOrTail;
    
    if (TailOpt==0) {
        TailMin = pi/2;
        TailMax = pi/2 + 0.6;
        peakOrTail = Form("Tail");
    }
    else if (TailOpt==1) {
        TailMin = pi - 0.6;
        TailMax = pi;
        peakOrTail = Form("Peak");
    }
    else if (TailOpt==2) {
        TailMin = pi/2;
        TailMax = pi;
    }
    
    
    
    h_pT_pPhi_dR_NR_reference->Scale(const_ref_NR);
    h_pT_pPhi_dR_NR_signal->Scale(const_sig_NR);
    
    h_pT_pPhi_dR_R_reference->Scale(const_ref_R);
    h_pT_pPhi_dR_R_signal->Scale(const_sig_R);
    
    TH3F *h_pT_pPhi_dR_pp = new TH3F("h_pT_pPhi_dR_pp", "h_pT_pPhi_dR_pp",100,0,100,50,pi/2,pi,50,0,0.8);
    h_pT_pPhi_dR_pp->Add(h_pT_pPhi_dR_pp_signal,h_pT_pPhi_dR_pp_reference,1,-1);
    
    TH3F *h_pT_pPhi_dR_NR = new TH3F("h_pT_pPhi_dR_NR", "h_pT_pPhi_dR_NR",100,0,100,50,pi/2,pi,50,0,0.8);
    h_pT_pPhi_dR_NR->Add(h_pT_pPhi_dR_NR_signal,h_pT_pPhi_dR_NR_reference,1,-1);
    
    TH3F *h_pT_pPhi_dR_R = new TH3F("h_pT_pPhi_dR_R", "h_pT_pPhi_dR_R",100,0,100,50,pi/2,pi,50,0,0.8);
    h_pT_pPhi_dR_R->Add(h_pT_pPhi_dR_R_signal,h_pT_pPhi_dR_R_reference,1,-1);
    

    // Creating canvas
    TCanvas *canvas = new TCanvas();
    canvas->SetLogy(1);
        
    
    // Defining 1D histograms
    TH1F *hdR_pp = (TH1F*)h_pT_pPhi_dR_pp->ProjectionZ("hdR_pp",h_pT_pPhi_dR_pp->GetXaxis()->FindBin(ptMin+0.1),h_pT_pPhi_dR_pp->GetXaxis()->FindBin(ptMax-0.1),h_pT_pPhi_dR_pp->GetYaxis()->FindBin(TailMin),h_pT_pPhi_dR_pp->GetYaxis()->FindBin(TailMax));
    TH1F *hdR_NR = (TH1F*)h_pT_pPhi_dR_NR->ProjectionZ("hdR_NR",h_pT_pPhi_dR_NR->GetXaxis()->FindBin(ptMin+0.1),h_pT_pPhi_dR_NR->GetXaxis()->FindBin(ptMax-0.1),h_pT_pPhi_dR_NR->GetYaxis()->FindBin(TailMin),h_pT_pPhi_dR_NR->GetYaxis()->FindBin(TailMax));
    TH1F *hdR_R = (TH1F*)h_pT_pPhi_dR_R->ProjectionZ("hdR_R",h_pT_pPhi_dR_R->GetXaxis()->FindBin(ptMin+0.1),h_pT_pPhi_dR_R->GetXaxis()->FindBin(ptMax-0.1),h_pT_pPhi_dR_R->GetYaxis()->FindBin(TailMin),h_pT_pPhi_dR_R->GetYaxis()->FindBin(TailMax));
    
    hdR_pp->Rebin(3);
    hdR_NR->Rebin(3);
    hdR_R->Rebin(3);
    
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
    TString meanLeg_pp = Form("%.4f +- %.4f", mean_pp, meanError_pp);
    TString meanLeg_R = Form("%.4f +- %.4f", mean_R, meanError_R);
    TString meanLeg_NR = Form("%.4f +- %.4f", mean_NR, meanError_NR);
    
    
    // Formatting histograms
    hdR_pp->SetTitle("");
    hdR_pp->GetYaxis()->SetTitle("#frac{1}{N_{jets}} #frac{dN_{jets}}{d#DeltaR_{axis}}");
    hdR_pp->GetXaxis()->SetTitle("#DeltaR_{axis}");
    hdR_pp->Scale(1./hdR_pp->Integral("width"));
    hdR_pp->SetMarkerStyle(kFullSquare);
    hdR_pp->SetMarkerColor(kBlue);
    hdR_pp->SetLineColor(kBlue);
    
    hdR_NR->SetMarkerColor(kGreen);
    hdR_NR->SetLineColor(kGreen);
    hdR_NR->Scale(1./hdR_NR->Integral("width"));
    hdR_NR->SetMarkerStyle(kFullTriangleUp);
    
    
    hdR_R->SetMarkerColor(kRed);
    hdR_R->SetLineColor(kRed);
    hdR_R->Scale(1./hdR_R->Integral("width"));
    hdR_R->SetMarkerStyle(kFullCircle);
    
    hdR_pp->GetXaxis()->SetRangeUser(0,0.26);
    hdR_pp->GetYaxis()->SetRangeUser(0.0005,100);
    
    
    // Drawing histograms
    hdR_pp->Draw();
    hdR_NR->Draw("SAME");
    hdR_R->Draw("SAME");
    
    
    // Legend
    TLegend *leg = new TLegend(0.2,0.2,0.6,0.45);
    hdR_pp->SetStats(0);
    leg->AddEntry(hdR_pp,Form("#splitline{pp}{mean = %s}",meanLeg_pp.Data()),"LEP");
    leg->AddEntry(hdR_NR,Form("#splitline{Recoils off}{mean = %s}",meanLeg_NR.Data()),"LEP");
    leg->AddEntry(hdR_R,Form("#splitline{Recoils on}{mean = %s}",meanLeg_R.Data()),"LEP");
    leg->SetLineColor(kWhite);
    leg->SetTextSize(0.04);
    leg->Draw("SAME");
    
    
    TString mom_range = Form("%.0f < p_{T, jet} < %.0f GeV/c",ptMin,ptMax);
    TString rad = Form("R = %.1f",Rjet);
    
    
    TLatex info;
    info.SetNDC();
    info.SetTextFont(43);
    info.SetTextSize(20);
    info.DrawLatex(0.2,0.85,"JEWEL");
    info.DrawLatex(0.4,0.85,rad);
    info.DrawLatex(0.6,0.85,mom_range);
    
    // Saving Figures
    string saveName = Form("plt_recoil__R0%.0f_%.0f_%.0f.pdf",Rjet*10,ptMin,ptMax);
    saveName.insert(11,peakOrTail);
    cout << saveName;
    //TString l(saveName);
    canvas->SaveAs(saveName.c_str());
}
