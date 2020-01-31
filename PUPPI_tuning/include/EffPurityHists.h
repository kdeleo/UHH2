#pragma once

#include "UHH2/core/include/Hists.h"
#include "TH2F.h"
#include "UHH2/core/include/Event.h"

using namespace uhh2;
using namespace std;

namespace uhh2examples {


class EffPurityHists: public uhh2::Hists {
public:
    // use the same constructor arguments as Hists for forwarding:
    EffPurityHists(uhh2::Context & ctx, const std::string & dirname, const std::string & collection = "");

    virtual void fill(const uhh2::Event & ev) override;
    virtual ~EffPurityHists();
  protected:
    bool berror;
    double pTcut;
    const std::string collection;
    Event::Handle<vector<Jet>> h_myjets;
    Event::Handle<vector<GenJet>> h_mygenjets;

    //Jet Efficiency
    TH1F *JetEfficiency_GenJetEta,*JetEfficiency_GenJetPt,*JetEfficiency_GenJetPt_forward,*JetEfficiency_GenJetPt_central, *JetEfficiency_GenJetNPV_alljets, *JetEfficiency_GenJetNPV,*JetEfficiency_GenJetNPV_forward,*JetEfficiency_GenJetNPV_central,*JetEfficiency_GenJetEta_matchedjet,*JetEfficiency_GenJetPt_matchedjet,*JetEfficiency_GenJetPt_forward_matchedjet,*JetEfficiency_GenJetPt_central_matchedjet, *JetEfficiency_GenJetNPV_matchedjet, *JetEfficiency_GenJetNPV_alljets_matchedjet, *JetEfficiency_GenJetNPV_central_matchedjet, *JetEfficiency_GenJetNPV_forward_matchedjet;

    TH1F *JetEfficiency_GenJetNPV_alljets_central,*JetEfficiency_GenJetNPV_alljets_central_matchedjet, *JetEfficiency_GenJetNPV_alljets_forward,*JetEfficiency_GenJetNPV_alljets_forward_matchedjet;

    TH1F *JetEfficiency_GenJetNPV_alljets_0to1p3,*JetEfficiency_GenJetNPV_alljets_1p3to1p6,*JetEfficiency_GenJetNPV_alljets_1p6to2,*JetEfficiency_GenJetNPV_alljets_2to2p5,*JetEfficiency_GenJetNPV_alljets_2p5to3,*JetEfficiency_GenJetNPV_alljets_3to5;
    TH1F *JetEfficiency_GenJetNPV_alljets_0to1p3_matchedjet,*JetEfficiency_GenJetNPV_alljets_1p3to1p6_matchedjet,*JetEfficiency_GenJetNPV_alljets_1p6to2_matchedjet,*JetEfficiency_GenJetNPV_alljets_2to2p5_matchedjet,*JetEfficiency_GenJetNPV_alljets_2p5to3_matchedjet,*JetEfficiency_GenJetNPV_alljets_3to5_matchedjet;

    TH1F *JetEfficiency_GenJetNPV_0to1p3,*JetEfficiency_GenJetNPV_1p3to2p5,*JetEfficiency_GenJetNPV_2p5to3,*JetEfficiency_GenJetNPV_3to5;
    TH1F *JetEfficiency_GenJetNPV_0to1p3_matchedjet,*JetEfficiency_GenJetNPV_1p3to2p5_matchedjet,*JetEfficiency_GenJetNPV_2p5to3_matchedjet,*JetEfficiency_GenJetNPV_3to5_matchedjet;

    TH1F *JetEfficiency_GenJetPt_0to1p3,*JetEfficiency_GenJetPt_1p3to2p5,*JetEfficiency_GenJetPt_2p5to3,*JetEfficiency_GenJetPt_3to5;
    TH1F *JetEfficiency_GenJetPt_0to1p3_matchedjet,*JetEfficiency_GenJetPt_1p3to2p5_matchedjet,*JetEfficiency_GenJetPt_2p5to3_matchedjet,*JetEfficiency_GenJetPt_3to5_matchedjet;

    TH1F *JetEfficiency_GenJetEta_alljets,*JetEfficiency_GenJetPt_alljets,*JetEfficiency_GenJetPt_alljets_forward,*JetEfficiency_GenJetPt_alljets_central,*JetEfficiency_GenJetEta_alljets_matchedjet,*JetEfficiency_GenJetPt_alljets_matchedjet,*JetEfficiency_GenJetPt_alljets_forward_matchedjet,*JetEfficiency_GenJetPt_alljets_central_matchedjet;

    TH1F *JetEfficiency_GenJetPt_alljets_0to1p3,*JetEfficiency_GenJetPt_alljets_1p3to2p5,*JetEfficiency_GenJetPt_alljets_2p5to3,*JetEfficiency_GenJetPt_alljets_3to5;
    TH1F *JetEfficiency_GenJetPt_alljets_0to1p3_matchedjet,*JetEfficiency_GenJetPt_alljets_1p3to2p5_matchedjet,*JetEfficiency_GenJetPt_alljets_2p5to3_matchedjet,*JetEfficiency_GenJetPt_alljets_3to5_matchedjet;

    TH2F *JetEfficiency_GenJetEta_genJetPt_alljets, *JetEfficiency_GenJetEta_genJetPt_alljets_matchedjet, *JetEfficiency_GenJetEta_genJetPt, *JetEfficiency_GenJetEta_genJetPt_matchedjet;


    //Jet Purity
    TH1F *JetPurity_RecoJetEta,*JetPurity_RecoJetPt,*JetPurity_RecoJetPt_central,*JetPurity_RecoJetPt_forward, *JetPurity_RecoJetNPV, *JetPurity_RecoJetNPV_forward, *JetPurity_RecoJetNPV_central,*JetPurity_RecoJetEta_matchedjet,*JetPurity_RecoJetPt_matchedjet,*JetPurity_RecoJetPt_forward_matchedjet,*JetPurity_RecoJetPt_central_matchedjet, *JetPurity_RecoJetNPV_matchedjet, *JetPurity_RecoJetNPV_central_matchedjet, *JetPurity_RecoJetNPV_forward_matchedjet;


    TH1F *JetPurity_RecoJetNPV_alljets,*JetPurity_RecoJetNPV_alljets_matchedjet,*JetPurity_RecoJetNPV_alljets_central,*JetPurity_RecoJetNPV_alljets_central_matchedjet,*JetPurity_RecoJetNPV_alljets_forward,*JetPurity_RecoJetNPV_alljets_forward_matchedjet;

    TH1F *JetPurity_RecoJetNPV_alljets_0to1p3,*JetPurity_RecoJetNPV_alljets_1p3to1p6,*JetPurity_RecoJetNPV_alljets_1p6to2,*JetPurity_RecoJetNPV_alljets_2to2p5,*JetPurity_RecoJetNPV_alljets_2p5to3,*JetPurity_RecoJetNPV_alljets_3to5;
    TH1F *JetPurity_RecoJetNPV_alljets_0to1p3_matchedjet,*JetPurity_RecoJetNPV_alljets_1p3to1p6_matchedjet,*JetPurity_RecoJetNPV_alljets_1p6to2_matchedjet,*JetPurity_RecoJetNPV_alljets_2to2p5_matchedjet,*JetPurity_RecoJetNPV_alljets_2p5to3_matchedjet,*JetPurity_RecoJetNPV_alljets_3to5_matchedjet;

    TH1F *JetPurity_RecoJetEta_alljets,*JetPurity_RecoJetPt_alljets,*JetPurity_RecoJetPt_alljets_central,*JetPurity_RecoJetPt_alljets_forward,*JetPurity_RecoJetEta_alljets_matchedjet,*JetPurity_RecoJetPt_alljets_matchedjet,*JetPurity_RecoJetPt_alljets_forward_matchedjet,*JetPurity_RecoJetPt_alljets_central_matchedjet;



    ////  diff QCD and DY
    TH1F *deltar_nearest_gen_reco;
    TH1F *matched_neutralEmEnergyFraction, *matched_neutralHadronEnergyFraction, *matched_chargedEmEnergyFraction, *matched_chargedHadronEnergyFraction;
    TH1F *unmatched_neutralEmEnergyFraction, *unmatched_neutralHadronEnergyFraction, *unmatched_chargedEmEnergyFraction, *unmatched_chargedHadronEnergyFraction;

    TH1F  *pT_genjets, *pT_recojets,  *pT_eff_recojets, *pT_purity_genjets,  *eta_genjets, *eta_recojets;

    TH1F  *pT_genjets_firstjet, *pT_recojets_firstjet,   *eta_genjets_firstjet, *eta_recojets_firstjet;
    Event::Handle<std::vector<Jet>> h_CHSjets;


    TH1F *invMass_forward, *delta_Eta12, *eta1timeseta2;
};

}
