#pragma once

#include "UHH2/core/include/Hists.h"
#include "TH2F.h"
#include "UHH2/core/include/Event.h"

using namespace uhh2;
using namespace std;

namespace uhh2examples {


class PuppiJetHists: public uhh2::Hists {
public:
    // use the same constructor arguments as Hists for forwarding:
    PuppiJetHists(uhh2::Context & ctx, const std::string & dirname, const std::string & collection="");

    virtual void fill(const uhh2::Event & ev) override;
    virtual ~PuppiJetHists();
  protected:
    bool berror;
    double pTcut;
    const std::string collection;
    Event::Handle<vector<Jet>> h_myjets;
    Event::Handle<vector<GenJet>> h_mygenjets;

    //Jet pT scale
    TH2F *JetPtScale_Eta0to1p3,*JetPtScale_Eta1p3to2,*JetPtScale_Eta2to2p5,*JetPtScale_Eta2p5to3,*JetPtScale_Eta3to10;
    TH2F *JetPtResponse_Eta0to1p3,*JetPtResponse_Eta1p3to2,*JetPtResponse_Eta2to2p5,*JetPtResponse_Eta2p5to3,*JetPtResponse_Eta3to10;
    TH2F *JetPtResponse_Eta0to1p3_PU50,*JetPtResponse_Eta1p3to2_PU50,*JetPtResponse_Eta2to2p5_PU50,*JetPtResponse_Eta2p5to3_PU50,*JetPtResponse_Eta3to10_PU50;
    TH2F *JetPtResponse_Eta0to1p3_PU40,*JetPtResponse_Eta1p3to2_PU40,*JetPtResponse_Eta2to2p5_PU40,*JetPtResponse_Eta2p5to3_PU40,*JetPtResponse_Eta3to10_PU40;
    TH2F *NeutralMultiplicity_vs_mu_Eta3to5_pt17to20, *NeutralMultiplicity_vs_mu_unmatched_Eta3to5_pt17to20, *NeutralMultiplicity_vs_mu_matched_Eta3to5_pt17to20;
    TH2F *NeutralMultiplicity_vs_mu_Eta3to5_pt15to17, *NeutralMultiplicity_vs_mu_unmatched_Eta3to5_pt15to17, *NeutralMultiplicity_vs_mu_matched_Eta3to5_pt15to17;
    TH2F *NeutralMultiplicity_vs_mu_Eta3to5_pt20to23, *NeutralMultiplicity_vs_mu_unmatched_Eta3to5_pt20to23, *NeutralMultiplicity_vs_mu_matched_Eta3to5_pt20to23;
    TH2F *NeutralMultiplicity_vs_mu_Eta3to5_pt27to30, *NeutralMultiplicity_vs_mu_unmatched_Eta3to5_pt27to30, *NeutralMultiplicity_vs_mu_matched_Eta3to5_pt27to30;
    TH2F *NeutralMultiplicity_vs_mu_Eta0to2p5_pt15to17, *NeutralMultiplicity_vs_mu_unmatched_Eta0to2p5_pt15to17, *NeutralMultiplicity_vs_mu_matched_Eta0to2p5_pt15to17;
    TH2F *NeutralMultiplicity_vs_mu_Eta0to2p5_pt20to23, *NeutralMultiplicity_vs_mu_unmatched_Eta0to2p5_pt20to23, *NeutralMultiplicity_vs_mu_matched_Eta0to2p5_pt20to23;
    TH2F *NeutralMultiplicity_vs_mu_Eta0to2p5_pt27to30, *NeutralMultiplicity_vs_mu_unmatched_Eta0to2p5_pt27to30, *NeutralMultiplicity_vs_mu_matched_Eta0to2p5_pt27to30;
    TH2F *NeutralMultiplicity_vs_mu_Eta0to2p5_pt17to20, *NeutralMultiplicity_vs_mu_unmatched_Eta0to2p5_pt17to20, *NeutralMultiplicity_vs_mu_matched_Eta0to2p5_pt17to20;

    TH2F *NeutralPuppiMultiplicity_vs_mu_Eta3to5_pt17to20, *NeutralPuppiMultiplicity_vs_mu_unmatched_Eta3to5_pt17to20, *NeutralPuppiMultiplicity_vs_mu_matched_Eta3to5_pt17to20;
    TH2F *NeutralPuppiMultiplicity_vs_mu_Eta3to5_pt15to17, *NeutralPuppiMultiplicity_vs_mu_unmatched_Eta3to5_pt15to17, *NeutralPuppiMultiplicity_vs_mu_matched_Eta3to5_pt15to17;
    TH2F *NeutralPuppiMultiplicity_vs_mu_Eta3to5_pt20to23, *NeutralPuppiMultiplicity_vs_mu_unmatched_Eta3to5_pt20to23, *NeutralPuppiMultiplicity_vs_mu_matched_Eta3to5_pt20to23;
    TH2F *NeutralPuppiMultiplicity_vs_mu_Eta3to5_pt27to30, *NeutralPuppiMultiplicity_vs_mu_unmatched_Eta3to5_pt27to30, *NeutralPuppiMultiplicity_vs_mu_matched_Eta3to5_pt27to30;
    TH2F *NeutralPuppiMultiplicity_vs_mu_Eta0to2p5_pt15to17, *NeutralPuppiMultiplicity_vs_mu_unmatched_Eta0to2p5_pt15to17, *NeutralPuppiMultiplicity_vs_mu_matched_Eta0to2p5_pt15to17;
    TH2F *NeutralPuppiMultiplicity_vs_mu_Eta0to2p5_pt20to23, *NeutralPuppiMultiplicity_vs_mu_unmatched_Eta0to2p5_pt20to23, *NeutralPuppiMultiplicity_vs_mu_matched_Eta0to2p5_pt20to23;
    TH2F *NeutralPuppiMultiplicity_vs_mu_Eta0to2p5_pt27to30, *NeutralPuppiMultiplicity_vs_mu_unmatched_Eta0to2p5_pt27to30, *NeutralPuppiMultiplicity_vs_mu_matched_Eta0to2p5_pt27to30;
    TH2F *NeutralPuppiMultiplicity_vs_mu_Eta0to2p5_pt17to20, *NeutralPuppiMultiplicity_vs_mu_unmatched_Eta0to2p5_pt17to20, *NeutralPuppiMultiplicity_vs_mu_matched_Eta0to2p5_pt17to20;


    TH2F *JetPtResponse_Eta3to5_pt17to20_vsNeutralMultiplicity, *JetPtResponse_Eta0to2p5_pt17to20_vsNeutralMultiplicity,*JetPtResponse_Eta3to5_pt15to17_vsNeutralMultiplicity, *JetPtResponse_Eta0to2p5_pt15to17_vsNeutralMultiplicity,*JetPtResponse_Eta3to5_pt20to23_vsNeutralMultiplicity, *JetPtResponse_Eta0to2p5_pt20to23_vsNeutralMultiplicity,*JetPtResponse_Eta3to5_pt27to30_vsNeutralMultiplicity, *JetPtResponse_Eta0to2p5_pt27to30_vsNeutralMultiplicity;

    TH2F *JetPtResponse_Eta3to5_pt17to20_vsNeutralMultiplicity_mu_10to20, *JetPtResponse_Eta0to2p5_pt17to20_vsNeutralMultiplicity_mu_10to20,*JetPtResponse_Eta3to5_pt15to17_vsNeutralMultiplicity_mu_10to20, *JetPtResponse_Eta0to2p5_pt15to17_vsNeutralMultiplicity_mu_10to20,*JetPtResponse_Eta3to5_pt20to23_vsNeutralMultiplicity_mu_10to20, *JetPtResponse_Eta0to2p5_pt20to23_vsNeutralMultiplicity_mu_10to20,*JetPtResponse_Eta3to5_pt27to30_vsNeutralMultiplicity_mu_10to20, *JetPtResponse_Eta0to2p5_pt27to30_vsNeutralMultiplicity_mu_10to20;

    TH2F *JetPtResponse_Eta3to5_pt17to20_vsNeutralMultiplicity_mu_50to60, *JetPtResponse_Eta0to2p5_pt17to20_vsNeutralMultiplicity_mu_50to60,*JetPtResponse_Eta3to5_pt15to17_vsNeutralMultiplicity_mu_50to60, *JetPtResponse_Eta0to2p5_pt15to17_vsNeutralMultiplicity_mu_50to60,*JetPtResponse_Eta3to5_pt20to23_vsNeutralMultiplicity_mu_50to60, *JetPtResponse_Eta0to2p5_pt20to23_vsNeutralMultiplicity_mu_50to60,*JetPtResponse_Eta3to5_pt27to30_vsNeutralMultiplicity_mu_50to60, *JetPtResponse_Eta0to2p5_pt27to30_vsNeutralMultiplicity_mu_50to60;


    TH2F *JetPtResponse_Eta3to5_pt17to20_vsmu, *JetPtResponse_Eta0to2p5_pt17to20_vsmu,*JetPtResponse_Eta3to5_pt15to17_vsmu, *JetPtResponse_Eta0to2p5_pt15to17_vsmu,*JetPtResponse_Eta3to5_pt20to23_vsmu, *JetPtResponse_Eta0to2p5_pt20to23_vsmu,*JetPtResponse_Eta3to5_pt27to30_vsmu, *JetPtResponse_Eta0to2p5_pt27to30_vsmu;

    TH2F  *JetPtResponse_Eta2p5to3p0_pt17to20_vsmu, *JetPtResponse_Eta2p5to3p0_pt15to17_vsmu, *JetPtResponse_Eta2p5to3p0_pt20to23_vsmu, *JetPtResponse_Eta2p5to3p0_pt27to30_vsmu;


    TH1F*  JetPtResponse_Eta3to3,*Matching_eff;
    TH1F *JetEfficiency_GenJetNPV_0to1p3,*JetEfficiency_GenJetNPV_1p3to2p5,*JetEfficiency_GenJetNPV_2p5to3,*JetEfficiency_GenJetNPV_3to5;
    TH1F *JetEfficiency_GenJetNPV_alljets_0to1p3,*JetEfficiency_GenJetNPV_alljets_1p3to2p5,*JetEfficiency_GenJetNPV_alljets_2p5to3,*JetEfficiency_GenJetNPV_alljets_3to5;

    TH1F *JetEfficiency_GenJetNPV_0to1p3_matchedjet,*JetEfficiency_GenJetNPV_1p3to2p5_matchedjet,*JetEfficiency_GenJetNPV_2p5to3_matchedjet,*JetEfficiency_GenJetNPV_3to5_matchedjet;

    TH1F *JetEfficiency_GenJetNPV_alljets_0to1p3_matchedjet,*JetEfficiency_GenJetNPV_alljets_1p3to2p5_matchedjet,*JetEfficiency_GenJetNPV_alljets_2p5to3_matchedjet,*JetEfficiency_GenJetNPV_alljets_3to5_matchedjet;

    TH1F *JetEfficiency_GenJetPt_0to1p3,*JetEfficiency_GenJetPt_1p3to2p5,*JetEfficiency_GenJetPt_2p5to3,*JetEfficiency_GenJetPt_3to5;

    TH1F *JetEfficiency_GenJetPt_0to1p3_matchedjet,*JetEfficiency_GenJetPt_1p3to2p5_matchedjet,*JetEfficiency_GenJetPt_2p5to3_matchedjet,*JetEfficiency_GenJetPt_3to5_matchedjet;

    TH2F *ChargedHadronEnergyFraction,*ChargedEMEnergyFraction,*NeutralHadronEnergyFraction,*NeutralEMEnergyFraction;

    //Jet Efficiency
    TH1F *JetEfficiency_GenJetEta,*JetEfficiency_GenJetPt,*JetEfficiency_GenJetPt_forward,*JetEfficiency_GenJetPt_central, *JetEfficiency_GenJetNPV_alljets, *JetEfficiency_GenJetNPV,*JetEfficiency_GenJetNPV_forward,*JetEfficiency_GenJetNPV_central,*JetEfficiency_GenJetEta_matchedjet,*JetEfficiency_GenJetPt_matchedjet,*JetEfficiency_GenJetPt_forward_matchedjet,*JetEfficiency_GenJetPt_central_matchedjet, *JetEfficiency_GenJetNPV_matchedjet, *JetEfficiency_GenJetNPV_alljets_matchedjet, *JetEfficiency_GenJetNPV_central_matchedjet, *JetEfficiency_GenJetNPV_forward_matchedjet;

    TH1F *JetEfficiency_GenJetNPV_alljets_central,*JetEfficiency_GenJetNPV_alljets_central_matchedjet, *JetEfficiency_GenJetNPV_alljets_forward,*JetEfficiency_GenJetNPV_alljets_forward_matchedjet;

    //Jet Purity
    TH1F *JetPurity_RecoJetEta,*JetPurity_RecoJetPt,*JetPurity_RecoJetPt_central,*JetPurity_RecoJetPt_forward, *JetPurity_RecoJetNPV, *JetPurity_RecoJetNPV_forward, *JetPurity_RecoJetNPV_central,*JetPurity_RecoJetEta_matchedjet,*JetPurity_RecoJetPt_matchedjet,*JetPurity_RecoJetPt_forward_matchedjet,*JetPurity_RecoJetPt_central_matchedjet, *JetPurity_RecoJetNPV_matchedjet, *JetPurity_RecoJetNPV_central_matchedjet, *JetPurity_RecoJetNPV_forward_matchedjet;


    TH1F *JetPurity_RecoJetNPV_alljets,*JetPurity_RecoJetNPV_alljets_matchedjet,*JetPurity_RecoJetNPV_alljets_central,*JetPurity_RecoJetNPV_alljets_central_matchedjet,*JetPurity_RecoJetNPV_alljets_forward,*JetPurity_RecoJetNPV_alljets_forward_matchedjet;

    TH1F *JetPurity_RecoJetNPV_alljets_0to1p3,*JetPurity_RecoJetNPV_alljets_1p3to2p5,*JetPurity_RecoJetNPV_alljets_2p5to3,*JetPurity_RecoJetNPV_alljets_3to5;
    TH1F *JetPurity_RecoJetNPV_alljets_0to1p3_matchedjet,*JetPurity_RecoJetNPV_alljets_1p3to2p5_matchedjet,*JetPurity_RecoJetNPV_alljets_2p5to3_matchedjet,*JetPurity_RecoJetNPV_alljets_3to5_matchedjet;


    //General
    TH1F *JetRecoPT, *JetGenPT, *MET,*JetRecoETA, *JetGenETA;
    TH1F *JetRecoPT_0to1p3, *JetGenPT_0to1p3, *JetRecoETA_0to1p3, *JetGenETA_0to1p3;
    TH1F *JetRecoPT_1p3to2p5, *JetGenPT_1p3to2p5, *JetRecoETA_1p3to2p5, *JetGenETA_1p3to2p5;
    TH1F *JetRecoPT_2p5to3, *JetGenPT_2p5to3, *JetRecoETA_2p5to3, *JetGenETA_2p5to3;
    TH1F *JetRecoPT_3to5, *JetGenPT_3to5, *JetRecoETA_3to5, *JetGenETA_3to5;
};

}
