
#include "HepMC/IO_GenEvent.h"
#include "HepMC/GenEvent.h"

#include "fastjet/PseudoJet.hh"
#include "fastjet/ClusterSequence.hh"
#include "fastjet/ClusterSequenceArea.hh"
#include "fastjet/tools/JetMedianBackgroundEstimator.hh"

#include "TPDGCode.h"
#include "TFile.h"
#include "TMath.h"
#include "TH1.h"
#include "TH2.h"
#include "TH3.h"
#include "TRandom3.h"

#include <iostream>
#include <vector>
#include <cmath>

using std::cout;
using std::cerr;
using std::endl;
using std::string;
using std::vector;
#include "getopt.h"

//
// h+jet analysis to run on HepMC(2) simulation files
// Takes two arguments: infile (HEPMC format) outfile base name (ROOT format)
//
// additional options: --nobkg --chargedjet|--fulljet
//
// initial code from M. Van Leeuwen, modified by J. Norman
//


int is_stable(const HepMC::GenParticle *part) {
    // copied from AliStack::IsStable()	 
    int pdg = abs(part->pdg_id());
    if(pdg>1000000000)return kTRUE;

    const Int_t kNstable = 18;
    Int_t i;


    Int_t pdgStable[kNstable] = {
        kGamma,             // Photon
        kElectron,          // Electron
        kMuonPlus,          // Muon 
        kPiPlus,            // Pion
        kKPlus,             // Kaon
        kK0Short,           // K0s
        kK0Long,            // K0l
        kProton,            // Proton 
        kNeutron,           // Neutron
        kLambda0,           // Lambda_0
        kSigmaMinus,        // Sigma Minus
        kSigmaPlus,         // Sigma Plus
        3312,               // Xsi Minus 
        3322,               // Xsi 
        3334,               // Omega
        kNuE,               // Electron Neutrino 
        kNuMu,              // Muon Neutrino
        kNuTau              // Tau Neutrino
    };

    Bool_t isStable = kFALSE;
    for (i = 0; i < kNstable; i++) {
        if (pdg == abs(pdgStable[i])) {
            isStable = kTRUE;
            break;
        }
    }

    return isStable;
}

int is_stable_charged(const HepMC::GenParticle *part) {
    // copied from AliStack::IsStable()	 
    int pdg = abs(part->pdg_id());
    if(pdg>1000000000)return kTRUE;

    const Int_t kNstableCharged = 9;
    Int_t i;


    Int_t pdgStableCharged[kNstableCharged] = {
        kElectron,          // Electron
        kMuonPlus,          // Muon 
        kPiPlus,            // Pion
        kKPlus,             // Kaon
        kProton,            // Proton 
        kSigmaMinus,        // Sigma Minus
        kSigmaPlus,         // Sigma Plus
        3312,               // Xsi Minus 
        3334                // Omega
    };

    Bool_t isStable = kFALSE;
    for (i = 0; i < kNstableCharged; i++) {
        if (pdg == abs(pdgStableCharged[i])) {
            isStable = kTRUE;
            break;
        }
    }

    return isStable;
}

int is_charged(const HepMC::GenParticle *part) {
    // 0 = check whether proton, kaon or pion, 1 = check if actually charged
    // should keep set to 1 - option 0 incorrect I think
    int charged_method = 1; 

    int abs_kf = abs(part->pdg_id());

    if(charged_method==0) {
        if (abs_kf==211 || abs_kf==321 || abs_kf==2212 || abs_kf==11 || abs_kf==13)
            return 1;
        else if (abs_kf != 22 && abs_kf!=111 && abs_kf!=130 && abs_kf!=2112 && abs_kf!=311 && abs_kf!=12 && abs_kf !=14 && abs_kf!=16)
            cout << " Unexpected particle: kf=" << abs_kf << endl;
        return 0;
    }
    else if(charged_method==1) {

        if(abs_kf==11 || abs_kf==13 || abs_kf==211 || abs_kf==321 || abs_kf==2212 || abs_kf==3222 || abs_kf==3112 || abs_kf==3312 || abs_kf==3334) 
            return 1;
        else if (abs_kf != 22 && abs_kf!=111 && abs_kf!=130 && abs_kf!=2112 && abs_kf!=311 && abs_kf!=12 && abs_kf !=14 && abs_kf!=16)
            cout << " Unexpected particle: kf=" << abs_kf << endl;
        return 0;
    }
}

float dphi(float phi1, float phi2) {
    float dphi=phi1-phi2;
    float pi = 3.14159;
    if (dphi < -pi)
        dphi+=2*pi;
    if (dphi > pi)
        dphi-=2*pi;
    /*
       if (dphi < -TMath::Pi())
       dphi+=2*TMath::Pi();
       if (dphi > TMath::Pi())
       dphi-=2*TMath::Pi();
       */
    return dphi;
}


int main(int argc, char **argv) {

    static const int debug = 0;

    // defaults, can be set with arguments --chargedjets --fuljets --nobkg
    int do_bkg = 0; // 0: no subtraction; 1: subtract onlyjet energy; 2: four momentum subtraction
    int charged_jets = 1; // 1: charged jets, 0: full jets
    int gamma_trig = 0; // gamma jet or hadron jet?
    bool add_dummies = true; // include dummies in jet reconstruction
    // h jet options
    float frac_sig = 0.5; // fraction of events to use for signal
    float pt_TTref_min = 5; // reference minimum trigger track pt
    float pt_TTref_max = 7; // reference maximum trigger track pt
    float pt_TTsig_min = 20; // signal minimum trigger track pt
    float pt_TTsig_max = 50; // signal maximum trigger track pt

    if (argc < 2) {
        cerr << "Need two arguments: infile outfile" << endl << "infile is HEPMC ascii format; outfile will be root format" << endl;
        cerr << "further option arguments: [--chargedjets|--fulljets] [--gammajet] [--nobkg]" << endl;
        return 1;
    }

    int c;

    // set some options
    int nopt_parsed = 0;
    while (1) {

        static struct option long_options[] =
        {
            /* These options set a flag. */
            {"chargedjets", no_argument,       &charged_jets, 1},
            {"fulljets",   no_argument,       &charged_jets, 0},
            {"gammajet",   no_argument,       &gamma_trig, 1},
            {"nobkg",   no_argument,       &do_bkg, 0},
            /* it is also possible to have options that do not directly set a flag
             * Not used for now */
            {0, 0, 0, 0}
        };
        /* getopt_long stores the option index here. */
        int option_index = 1;        
        c = getopt_long (argc, argv, "",
                long_options, &option_index);
        //cout << "c " << c << " option_index " << option_index << endl;
        /* Detect the end of the options. */
        if (c == -1)
            break;
        if (c != 0)
            cout << "Error parsing option: " << argv[nopt_parsed] << endl;
        nopt_parsed++;
    }

    /* Print any remaining command line arguments (not options). */
    nopt_parsed++; 
    cout << "option_index " << nopt_parsed << endl;
    if (nopt_parsed + 2 > argc)
    {
        cerr << "Need two more arguments: infile outfile" << endl << "infile is HEPMC ascii format; outfile will be root format" << endl;
        return 1;
    }

    char *inname = argv[nopt_parsed];
    // specify an input file
    HepMC::IO_GenEvent ascii_in(inname,std::ios::in);

    //
    // Make histos
    // 
    string outname(argv[nopt_parsed+1]);
    if (charged_jets)
        outname.append("_charged");
    else
        outname.append("_full");
    if (do_bkg == 0)
        outname.append("_nobkg");
    outname.append(".root");

    TString trigLabel("PtLead");
    TString trigTitle("leading hadron");
    if (gamma_trig) {
        trigLabel = "PtGamma";
        trigTitle = "gamma";
    }
    cout << "Input: " << inname << ", output " << outname << endl;


    TFile fout(outname.c_str(),"RECREATE");
    static const int nR = 4; // radii 0.2, 0.3, 0.4, 0.5
    TH2F *hPtJetEta[nR];
    TH3F *hPtLeadPtJetDPhi[nR];
    TH3F *hPtLeadPtJetKt[nR];
    TH2F *hPtJetDPhiSignal[nR];
    TH2F *hPtJetDPhiReference[nR];
    TH1F *hRecoilPtJetReference[nR];
    TH1F *hRecoilPtJetSignal[nR];
    TH1F *hPtJet[nR];
    TH1F *hEtaJet[nR];
    TH1F *hNEvent = new TH1F("hNEvent","number of events; N",1,0,1);
    hNEvent->Sumw2();
    TH1F *hNtrig = new TH1F("hNtrig", "number of triggers", 2,0,2);
    hNtrig->GetXaxis()->SetBinLabel(1,"reference") ;
    hNtrig->GetXaxis()->SetBinLabel(2,"signal"); 
    hNtrig->Sumw2();
    TH1F *hPtTrack = new TH1F("hPtTrack", "Track p_{T};p_{T}",100,0,100);
    TH1F *hEtaTrack = new TH1F("hEtaTrack", "Track #eta;#eta",20,-1,1);
    hPtTrack->Sumw2();
    hEtaTrack->Sumw2();

    TH1F *hPtLead = new TH1F(Form("h%s",trigLabel.Data()),Form("%s pt;p_{T}",trigTitle.Data()),100,0,100);
    hPtLead->Sumw2();
    TH2F *hPtTrackEta = new TH2F("hPtTrackEta","hadron pt,eta;p_{T};#eta",100,0,100,20,-1,1);
    hPtTrackEta->Sumw2();
    for (Int_t iR = 0; iR < nR; iR++) {
        Float_t jetR = 0.2 + 0.1*iR;
        char hname[256];
        char htitle[256];
        sprintf(hname,"hPtJetEta_R%02d",iR+2);
        sprintf(htitle,"jet spectrum R=%.1f;p_{T} (GeV/c);#eta",jetR);
        hPtJetEta[iR] = new TH2F(hname,htitle,150,0,150,20,-1,1);
        hPtJetEta[iR]->Sumw2();

        sprintf(hname,"h%sPtJetDPhi_R%02d",trigLabel.Data(),iR+2);
        sprintf(htitle,"%s pt vs jet vs dphi R=%.1f;p_{T,lead} (GeV/c);p_{T,jet} (GeV/c);#Delta#phi",trigTitle.Data(),jetR);
        hPtLeadPtJetDPhi[iR] = new TH3F(hname, htitle, 50,0,50,150,0,150,48,0,fastjet::pi);
        hPtLeadPtJetDPhi[iR]->Sumw2();

        sprintf(hname,"hPtJetDPhiSignal_R%02d",iR+2);
        sprintf(htitle,"jet pt vs dphi R=%.1f;p_{T,jet} (GeV/c);#Delta#phi",trigTitle.Data(),jetR);
        hPtJetDPhiSignal[iR] = new TH2F(hname, htitle, 150,0,150,100,0,fastjet::pi);
        hPtJetDPhiSignal[iR]->Sumw2();

        sprintf(hname,"hPtJetDPhiReference_R%02d",iR+2);
        sprintf(htitle,"jet pt vs dphi R=%.1f;p_{T,jet} (GeV/c);#Delta#phi",trigTitle.Data(),jetR);
        hPtJetDPhiReference[iR] = new TH2F(hname, htitle, 150,0,150,100,0,fastjet::pi);
        hPtJetDPhiReference[iR]->Sumw2();

        sprintf(hname,"hRecoilPtJetReference_R%02d",iR+2);
        sprintf(htitle,"recoil jet pt reference R=%.1f;p_{T,jet} (GeV/c)",jetR);
        hRecoilPtJetReference[iR] = new TH1F(hname, htitle, 200,-50,150);
        hRecoilPtJetReference[iR]->Sumw2();

        sprintf(hname,"hSignalPtJetReference_R%02d",iR+2);
        sprintf(htitle,"recoil jet pt signal R=%.1f;p_{T,jet} (GeV/c)",jetR);
        hRecoilPtJetSignal[iR] = new TH1F(hname, htitle, 200,-50,150);
        hRecoilPtJetSignal[iR]->Sumw2();

        sprintf(hname,"h%sPtJetKt_R%02d",trigLabel.Data(),iR+2);
        sprintf(htitle,"%s pt vs jet vs dphi R=%.1f;p_{T,lead} (GeV/c);p_{T,jet} (GeV/c);|k_{T,y}|=|p_{T,jet}sin(#delta#phi)|",trigTitle.Data(),jetR);
        hPtLeadPtJetKt[iR] = new TH3F(hname, htitle, 50,0,50,150,0,150,100,0,50);
        hPtLeadPtJetKt[iR]->Sumw2();

        sprintf(hname,"hPtJet_R%02d",iR+2);
        sprintf(htitle,"p_{T} jet spectrum R=%.1f;p_{T,jet} (GeV)",jetR);
        hPtJet[iR] = new TH1F(hname, htitle, 100,0,100);
        hPtJet[iR]->Sumw2();

        sprintf(hname,"hEtaJet_R%02d",iR+2);
        sprintf(htitle,"#eta jet spectrum R=%.1f;#eta_{jet}",jetR);
        hEtaJet[iR] = new TH1F(hname, htitle, 20,-1,1);
        hEtaJet[iR]->Sumw2();
    }

    // set up object to get random number for TT selection
    TRandom3 *rand = new TRandom3();

    // get the first event
    HepMC::GenEvent* evt = ascii_in.read_next_event();
    if (!evt) 
        cerr << "Input file not found " << inname << endl;

    Int_t iEvent = 0;

    //
    // -- Event loop --
    // loop until we run out of events
    // 
    while ( evt ) {
        // analyze the event
        if (debug)
            cout << "Event " << endl;

        hNEvent->Fill(0.5,evt->weights()[0]); // count events

        float dice = rand->Rndm();
        bool is_sig_evt;
        if(dice < frac_sig) is_sig_evt = true;
        else                is_sig_evt = false;

        // from example_UsingIterators.cc

        float pt_lead = -1;
        float phi_lead = -100;
        float max_eta = 0.9;  // ALICE track acceptance cut
        float jetR = 0.4;
        std::vector <float> phi_TT_ar;
        std::vector <float> pt_TT_ar;
        int n_TT = 0;

        int index = 0;
        std::vector <fastjet::PseudoJet> fjInputs;
        std::vector <HepMC::GenParticle*> thermal;
        // 
        // -- particle loop --
        // loop over all particles in the HepMC event. This is done to find the 'trigger', i.e.
        // the particle that a jet is recoiling from
        //
        for ( HepMC::GenEvent::particle_iterator pit = evt->particles_begin();
                pit != evt->particles_end(); ++pit ) {
            const HepMC::GenParticle *p = *pit;
            if (!p->end_vertex() && p->status()==1 && fabs(p->momentum().eta()) < max_eta) { 
                // primary (NB heavy flavour decay products probably not included)

                if (p->momentum().perp() > pt_lead && 
                        ((gamma_trig && p->pdg_id()==22) || (!gamma_trig && is_charged(p) ))) {
                    //cout << "Event " << iEvent << " found trigger PID " << p->pdg_id() << " pt " << p->momentum().perp() << " status " << p->status() << endl;
                    pt_lead = p->momentum().perp();
                    phi_lead = p->momentum().phi();
                }

                // find reference track and add information to an array
                if (!is_sig_evt && 
                        p->momentum().perp() > pt_TTref_min && p->momentum().perp() < pt_TTref_max && 
                        (!gamma_trig && is_charged(p) )) {
                    // add particles phi and pt to a vector
                    pt_TT_ar.push_back(p->momentum().perp());
                    phi_TT_ar.push_back(p->momentum().phi());
                    n_TT++;
                }
                // find signal track and add information to an array
                if (is_sig_evt && 
                        p->momentum().perp() > pt_TTsig_min && p->momentum().perp() < pt_TTsig_max && 
                        (!gamma_trig && is_charged(p) )) {
                    // add particles phi and pt to a vector
                    pt_TT_ar.push_back(p->momentum().perp());
                    phi_TT_ar.push_back(p->momentum().phi());
                    n_TT++;
                }


                // Check for jet constituents
                if (!charged_jets || is_charged(p)) {
                    hPtTrackEta->Fill(p->momentum().perp(), p->momentum().eta(),evt->weights()[0]);
                    hPtTrack->Fill(p->momentum().perp(),evt->weights()[0]);
                    hEtaTrack->Fill(p->momentum().eta(),evt->weights()[0]);

                    // create jet constituent and add to vector
                    fastjet::PseudoJet jInp(p->momentum().px(),p->momentum().py(),p->momentum().pz(),p->momentum().e());
                    jInp.set_user_index(index);
                    fjInputs.push_back(jInp);
                    index++;
                }


                // also add dummies
                if (add_dummies && p->momentum().e() < 0.11e-5) {
                    fastjet::PseudoJet jInp(p->momentum().px(),p->momentum().py(),p->momentum().pz(),p->momentum().e());
                    jInp.set_user_index(index);
                    fjInputs.push_back(jInp);
                    index++;
                }
            }

            // Find thermal momenta.
            if (p->status() == 3){
                thermal.push_back((HepMC::GenParticle*)p);
                if(debug){
                    cout << " thermal momentum particle px = " << p->momentum().px() << endl;
                }
            }

        }  // end of particle loop 

        // pick random trigger track
        float pt_TT;
        float phi_TT;
        int trig_number = -1;
        if(n_TT>0) {
            trig_number = rand->Integer(n_TT);
            pt_TT = pt_TT_ar[trig_number];
            phi_TT = phi_TT_ar[trig_number];
            if(is_sig_evt) hNtrig->Fill(1.5,evt->weights()[0]);
            else           hNtrig->Fill(0.5,evt->weights()[0]);
        }

        // Do jet finding
        fastjet::GhostedAreaSpec ghostSpec(max_eta,1,0.01);
        fastjet::Strategy               strategy = fastjet::Best;
        fastjet::RecombinationScheme    recombScheme = fastjet::BIpt_scheme;
        fastjet::AreaType areaType =   fastjet::active_area;
        fastjet::AreaDefinition areaDef = fastjet::AreaDefinition(areaType,ghostSpec);

        hPtLead->Fill(pt_lead,evt->weights()[0]);

        // loop over jet radii (R), to perform the analysis for different jet R
        for (int iR = 0; iR < nR; iR++) { 
            float jetR = 0.2+0.1*iR;
            //fastjet::RangeDefinition range(-max_eta+jetR, max_eta-jetR, 0, 2.*fastjet::pi);
            fastjet::JetDefinition jetDefCh(fastjet::antikt_algorithm, jetR,
                    recombScheme, strategy);

            fastjet::ClusterSequenceArea clustSeqCh(fjInputs, jetDefCh,areaDef);

            vector <fastjet::PseudoJet> inclusiveJetsCh = clustSeqCh.inclusive_jets();

            // Background calculation

            fastjet::JetMedianBackgroundEstimator bge;  //.......... Background Sutraction event by event
            fastjet::ClusterSequenceArea *clustSeqBG = 0;


            if (do_bkg==1) {
                fastjet::JetDefinition jetDefBG(fastjet::kt_algorithm, jetR, recombScheme, strategy);
                fastjet::Selector  BGSelector = fastjet::SelectorStrip(2*jetR);  //.......... Background Sutraction event by event

                clustSeqBG = new fastjet::ClusterSequenceArea(fjInputs, jetDefBG,areaDef);//............
                vector <fastjet::PseudoJet> BGJets = clustSeqBG->inclusive_jets();

                if (BGJets.size() > 0) {
                    try {
                        bge.set_selector(BGSelector);
                        bge.set_jets(BGJets);
                    }
                    catch (fastjet::Error e) {
                        // No bkg jet in range
                        cout << "(1) Fastjet error: " << e.message() << " caught" << endl;
                    }
                }
                else {
                    cout << "No background jets found" << endl;
                }
            }

            // loop over all jets
            for (unsigned int iJet = 0; iJet < inclusiveJetsCh.size(); iJet++) {
                if (std::abs(inclusiveJetsCh[iJet].eta()) > max_eta-jetR)
                    continue;
                // get jet information
                float dphi_jh = dphi(inclusiveJetsCh[iJet].phi(),phi_lead);
                float jet_pt = inclusiveJetsCh[iJet].perp();
                float jet_eta = inclusiveJetsCh[iJet].eta();
                //emb_jet_phi = inclusiveJetsCh[iJet].phi();
                float jet_area = clustSeqCh.area(inclusiveJetsCh[iJet]);
                if (do_bkg==1) {
                    // subtract background from jet
                    try {
                        float rho = bge.rho(inclusiveJetsCh[iJet]);
                        jet_pt -= rho*jet_area;
                    }
                    catch (fastjet::Error e) {
                        // No bkg jet in range
                        cout << "Fastjet error: " << e.message() << " caught" << endl;
                    }
                }
                else if (do_bkg==2) {
                    // 4momSub
                    fastjet::PseudoJet bkg;
                    for (unsigned int ip = 0; ip < thermal.size(); ++ip){
                        fastjet::PseudoJet p(thermal[ip]->momentum().px(),thermal[ip]->momentum().py(),thermal[ip]->momentum().pz(),thermal[ip]->momentum().e());
                        for (fastjet::PseudoJet constituent: inclusiveJetsCh[iJet].constituents()){
                            const double deltaEta = constituent.eta() - thermal[ip]->momentum().eta();
                            const double deltaPhi = constituent.phi() - thermal[ip]->momentum().phi();
                            const double deltaR = sqrt(pow(deltaEta,2) + pow(deltaPhi,2));
                            if (deltaR < 1e-5){
                                bkg += p;
                                break;
                            }
                        }
                    }
                    if (debug){
                        cout << "background 4 momentum = " << bkg.perp() << endl;
                    }
                    inclusiveJetsCh[iJet] -= bkg;
                }
                // fill histos
                hPtJetEta[iR]->Fill(jet_pt,jet_eta,evt->weights()[0]);
                hPtLeadPtJetDPhi[iR]->Fill(pt_lead,jet_pt,fabs(dphi_jh),evt->weights()[0]);
                hPtJet[iR]->Fill(jet_pt,evt->weights()[0]);
                hEtaJet[iR]->Fill(jet_eta,evt->weights()[0]);
                if (TMath::Abs(dphi_jh-TMath::Pi()) < 0.5*TMath::Pi()) {
                    float kt_jh = TMath::Abs(jet_pt*TMath::Sin(dphi_jh));
                    hPtLeadPtJetKt[iR]->Fill(pt_lead,jet_pt,kt_jh,evt->weights()[0]);
                }

                if(n_TT>0) { // found trigger track, fill h jet histos
                    float dphi_TTjet = dphi(inclusiveJetsCh[iJet].phi(),phi_TT);
                    if(is_sig_evt) { // signal
                        hPtJetDPhiSignal[iR]->Fill(jet_pt,fabs(dphi_TTjet),evt->weights()[0]);
                        if(fabs(dphi_TTjet) > TMath::Pi()-0.6)
                            hRecoilPtJetSignal[iR]->Fill(jet_pt,evt->weights()[0]);
                    }
                    else { // reference
                        hPtJetDPhiReference[iR]->Fill(jet_pt,fabs(dphi_TTjet),evt->weights()[0]);
                        if(fabs(dphi_TTjet) > TMath::Pi()-0.6)
                            hRecoilPtJetReference[iR]->Fill(jet_pt,evt->weights()[0]);
                    }
                }
            }
            if (clustSeqBG) {
                delete clustSeqBG;
                clustSeqBG = 0;
            }
        }
        if (debug)
            cout << "Leading particle pt " << pt_lead << " phi " << phi_lead << endl; 
        // delete the created event from memory
        delete evt;
        // read the next event
        ascii_in >> evt;    
        iEvent++;
        if(iEvent%1000==0) 
            cout << "run over " << iEvent << " events" << endl;
    }

    fout.Write();
    return 0;
} 
