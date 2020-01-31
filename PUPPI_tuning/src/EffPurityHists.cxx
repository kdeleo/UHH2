#include "UHH2/PUPPI_tuning/include/EffPurityHists.h"
#include "UHH2/core/include/Event.h"
#include "UHH2/core/include/GenParticle.h"
#include "UHH2/core/include/LorentzVector.h"


#include <iostream>

using namespace std;
using namespace uhh2;
using namespace uhh2examples;

/*
This Hist Class should cover
jet eff and purity histos.
For eff and purity we need to clean gen and reco jets. typically we use:
Eff: gen jets pT > 30 GeV, reco pT > 20 GeV
Purity: gen jets pT > 20 GeV, reco pT > 30 GeV
to avoid effects from JEC
The matching parameter need for eff and purity is choosen to be 0.4, to be compatible with the PU Jet ID definition.
Afterwards we divided the matche hist by the "all jets" hist in a script. 
*/

EffPurityHists::EffPurityHists(Context & ctx, const string & dirname, const std::string & collection_): Hists(ctx, dirname), collection(collection_){
  berror=(ctx.get("debug") == "true");

  // get pT cut for eff and purity from config, normaly at 30 GeV
  pTcut =atof(ctx.get("pTCut").c_str());

  // use the standard collection or the one assigned, important for quark gluon comparison
  if(!collection.empty()){
    h_myjets = ctx.get_handle<std::vector<Jet> >(collection);
    h_mygenjets = ctx.get_handle<std::vector<GenJet> >("gen"+collection);
  }

  ////////////////////////////////////////////////////////////////       Jet Efficiency       /////////////////////////////////////////////////////////
  JetEfficiency_GenJetEta_alljets = book<TH1F>("JetEfficiency_GenJetEta_alljets", "Jet Efficiency GenJetEta", 100,-5,5); 
  JetEfficiency_GenJetEta_alljets_matchedjet = book<TH1F>("JetEfficiency_GenJetEta_alljets_matchedjet", "Jet Efficiency GenJetEta",100,-5,5); 

  JetEfficiency_GenJetEta = book<TH1F>("JetEfficiency_GenJetEta", "Jet Efficiency GenJetEta", 100,-5,5); 
  JetEfficiency_GenJetEta_matchedjet = book<TH1F>("JetEfficiency_GenJetEta_matchedjet", "Jet Efficiency GenJetEta",100,-5,5); 

  JetEfficiency_GenJetEta_genJetPt_alljets = book<TH2F>("JetEfficiency_GenJetEta_genJetPt_alljets", "Jet Efficiency GenJetEta", 100,-5,5,200,0,2000); 
  JetEfficiency_GenJetEta_genJetPt_alljets_matchedjet = book<TH2F>("JetEfficiency_GenJetEta_genJetPt_alljets_matchedjet", "Jet Efficiency GenJetEta",100,-5,5,200,0,2000); 

  JetEfficiency_GenJetEta_genJetPt = book<TH2F>("JetEfficiency_GenJetEta_genJetPt", "Jet Efficiency GenJetEta", 100,-5,5,2000,0,2000); 
  JetEfficiency_GenJetEta_genJetPt_matchedjet = book<TH2F>("JetEfficiency_GenJetEta_genJetPt_matchedjet", "Jet Efficiency GenJetEta",100,-5,5,2000,0,2000); 


  JetEfficiency_GenJetPt = book<TH1F>("JetEfficiency_GenJetPt", "Jet Efficiency GenJetPt", 2000,0,2000); 
  JetEfficiency_GenJetPt_matchedjet = book<TH1F>("JetEfficiency_GenJetPt_matchedjet", "Jet Efficiency GenJetPt", 2000,0,2000); 

  JetEfficiency_GenJetPt_central = book<TH1F>("JetEfficiency_GenJetPt_central", "Jet Efficiency GenJetPt_central", 2000,0,2000); 
  JetEfficiency_GenJetPt_central_matchedjet = book<TH1F>("JetEfficiency_GenJetPt_central_matchedjet", "Jet Efficiency GenJetPt central", 2000,0,2000); 

  JetEfficiency_GenJetPt_forward = book<TH1F>("JetEfficiency_GenJetPt_forward", "Jet Efficiency GenJetPt forward", 2000,0,2000); 
  JetEfficiency_GenJetPt_forward_matchedjet = book<TH1F>("JetEfficiency_GenJetPt_forward_matchedjet", "Jet Efficiency GenJetPt forward", 2000,0,2000); 

  JetEfficiency_GenJetPt_0to1p3 = book<TH1F>("JetEfficiency_GenJetPt_0to1p3", "Jet Efficiency Pt (0to1p3)",2000,0,2000); 
  JetEfficiency_GenJetPt_0to1p3_matchedjet = book<TH1F>("JetEfficiency_GenJetPt_0to1p3_matchedjet", "Jet Efficiency Pt (0to1p3)",2000,0,2000 ); 

  JetEfficiency_GenJetPt_1p3to2p5 = book<TH1F>("JetEfficiency_GenJetPt_1p3to2p5", "Jet Efficiency Pt (1p3to2p5)",2000,0,2000); 
  JetEfficiency_GenJetPt_1p3to2p5_matchedjet = book<TH1F>("JetEfficiency_GenJetPt_1p3to2p5_matchedjet", "Jet Efficiency Pt (1p3to2p5)",2000,0,2000 ); 

  JetEfficiency_GenJetPt_2p5to3 = book<TH1F>("JetEfficiency_GenJetPt_2p5to3", "Jet Efficiency Pt (2p5to3)",2000,0,2000); 
  JetEfficiency_GenJetPt_2p5to3_matchedjet = book<TH1F>("JetEfficiency_GenJetPt_2p5to3_matchedjet", "Jet Efficiency Pt (2p5to3)",2000,0,2000 ); 

  JetEfficiency_GenJetPt_3to5 = book<TH1F>("JetEfficiency_GenJetPt_3to5", "Jet Efficiency Pt (3to5)",2000,0,2000); 
  JetEfficiency_GenJetPt_3to5_matchedjet = book<TH1F>("JetEfficiency_GenJetPt_3to5_matchedjet", "Jet Efficiency Pt (3to5)",2000,0,2000 ); 

  JetEfficiency_GenJetPt_alljets = book<TH1F>("JetEfficiency_GenJetPt_alljets", "Jet Efficiency GenJetPt", 2000,0,2000); 
  JetEfficiency_GenJetPt_alljets_matchedjet = book<TH1F>("JetEfficiency_GenJetPt_alljets_matchedjet", "Jet Efficiency GenJetPt", 2000,0,2000); 
			
  JetEfficiency_GenJetPt_alljets_central = book<TH1F>("JetEfficiency_GenJetPt_alljets_central", "Jet Efficiency GenJetPt_central", 2000,0,2000); 
  JetEfficiency_GenJetPt_alljets_central_matchedjet = book<TH1F>("JetEfficiency_GenJetPt_alljets_central_matchedjet", "Jet Efficiency GenJetPt central", 2000,0,2000); 
			
  JetEfficiency_GenJetPt_alljets_forward = book<TH1F>("JetEfficiency_GenJetPt_alljets_forward", "Jet Efficiency GenJetPt forward", 2000,0,2000); 
  JetEfficiency_GenJetPt_alljets_forward_matchedjet = book<TH1F>("JetEfficiency_GenJetPt_alljets_forward_matchedjet", "Jet Efficiency GenJetPt forward", 2000,0,2000); 
			
  JetEfficiency_GenJetPt_alljets_0to1p3 = book<TH1F>("JetEfficiency_GenJetPt_alljets_0to1p3", "Jet Efficiency Pt (0to1p3)",2000,0,2000); 
  JetEfficiency_GenJetPt_alljets_0to1p3_matchedjet = book<TH1F>("JetEfficiency_GenJetPt_alljets_0to1p3_matchedjet", "Jet Efficiency Pt (0to1p3)",2000,0,2000 ); 
			
  JetEfficiency_GenJetPt_alljets_1p3to2p5 = book<TH1F>("JetEfficiency_GenJetPt_alljets_1p3to2p5", "Jet Efficiency Pt (1p3to2p5)",2000,0,2000); 
  JetEfficiency_GenJetPt_alljets_1p3to2p5_matchedjet = book<TH1F>("JetEfficiency_GenJetPt_alljets_1p3to2p5_matchedjet", "Jet Efficiency Pt (1p3to2p5)",2000,0,2000 ); 
			
  JetEfficiency_GenJetPt_alljets_2p5to3 = book<TH1F>("JetEfficiency_GenJetPt_alljets_2p5to3", "Jet Efficiency Pt (2p5to3)",2000,0,2000); 
  JetEfficiency_GenJetPt_alljets_2p5to3_matchedjet = book<TH1F>("JetEfficiency_GenJetPt_alljets_2p5to3_matchedjet", "Jet Efficiency Pt (2p5to3)",2000,0,2000 ); 
			
  JetEfficiency_GenJetPt_alljets_3to5 = book<TH1F>("JetEfficiency_GenJetPt_alljets_3to5", "Jet Efficiency Pt (3to5)",2000,0,2000); 
  JetEfficiency_GenJetPt_alljets_3to5_matchedjet = book<TH1F>("JetEfficiency_GenJetPt_alljets_3to5_matchedjet", "Jet Efficiency Pt (3to5)",2000,0,2000 ); 



  JetEfficiency_GenJetNPV = book<TH1F>("JetEfficiency_GenJetNPV", "Jet Efficiency NPV",80,0,80); 
  JetEfficiency_GenJetNPV_matchedjet = book<TH1F>("JetEfficiency_GenJetNPV_matchedjet", "Jet Efficiency NPV",80,0,80 ); 

  JetEfficiency_GenJetNPV_alljets = book<TH1F>("JetEfficiency_GenJetNPV_alljets", "Jet Efficiency NPV",80,0,80); 
  JetEfficiency_GenJetNPV_alljets_matchedjet = book<TH1F>("JetEfficiency_GenJetNPV_alljets_matchedjet", "Jet Efficiency NPV",80,0,80 ); 

  JetEfficiency_GenJetNPV_alljets_central = book<TH1F>("JetEfficiency_GenJetNPV_alljets_central", "Jet Efficiency NPV",80,0,80); 
  JetEfficiency_GenJetNPV_alljets_central_matchedjet = book<TH1F>("JetEfficiency_GenJetNPV_alljets_central_matchedjet", "Jet Efficiency NPV",80,0,80 ); 

  JetEfficiency_GenJetNPV_alljets_forward = book<TH1F>("JetEfficiency_GenJetNPV_alljets_forward", "Jet Efficiency NPV",80,0,80); 
  JetEfficiency_GenJetNPV_alljets_forward_matchedjet = book<TH1F>("JetEfficiency_GenJetNPV_alljets_forward_matchedjet", "Jet Efficiency NPV",80,0,80 ); 

  JetEfficiency_GenJetNPV_alljets_0to1p3 = book<TH1F>("JetEfficiency_GenJetNPV_alljets_0to1p3", "Jet Efficiency NPV (0to1p3)",80,0,80); 
  JetEfficiency_GenJetNPV_alljets_0to1p3_matchedjet = book<TH1F>("JetEfficiency_GenJetNPV_alljets_0to1p3_matchedjet", "Jet Efficiency NPV (0to1p3)",80,0,80 ); 

  JetEfficiency_GenJetNPV_alljets_1p3to1p6 = book<TH1F>("JetEfficiency_GenJetNPV_alljets_1p3to1p6", "Jet Efficiency NPV (1p3to1p6)",80,0,80); 
  JetEfficiency_GenJetNPV_alljets_1p3to1p6_matchedjet = book<TH1F>("JetEfficiency_GenJetNPV_alljets_1p3to1p6_matchedjet", "Jet Efficiency NPV (1p3to1p6)",80,0,80 ); 

  JetEfficiency_GenJetNPV_alljets_1p6to2 = book<TH1F>("JetEfficiency_GenJetNPV_alljets_1p6to2", "Jet Efficiency NPV (1p6to2)",80,0,80); 
  JetEfficiency_GenJetNPV_alljets_1p6to2_matchedjet = book<TH1F>("JetEfficiency_GenJetNPV_alljets_1p6to2_matchedjet", "Jet Efficiency NPV (1p6to2)",80,0,80 ); 

  JetEfficiency_GenJetNPV_alljets_2to2p5 = book<TH1F>("JetEfficiency_GenJetNPV_alljets_2to2p5", "Jet Efficiency NPV (2to2p5)",80,0,80); 
  JetEfficiency_GenJetNPV_alljets_2to2p5_matchedjet = book<TH1F>("JetEfficiency_GenJetNPV_alljets_2to2p5_matchedjet", "Jet Efficiency NPV (2to2p5)",80,0,80 ); 

  JetEfficiency_GenJetNPV_alljets_2p5to3 = book<TH1F>("JetEfficiency_GenJetNPV_alljets_2p5to3", "Jet Efficiency NPV (2p5to3)",80,0,80); 
  JetEfficiency_GenJetNPV_alljets_2p5to3_matchedjet = book<TH1F>("JetEfficiency_GenJetNPV_alljets_2p5to3_matchedjet", "Jet Efficiency NPV (2p5to3)",80,0,80 ); 

  JetEfficiency_GenJetNPV_alljets_3to5 = book<TH1F>("JetEfficiency_GenJetNPV_alljets_3to5", "Jet Efficiency NPV (3to5)",80,0,80); 
  JetEfficiency_GenJetNPV_alljets_3to5_matchedjet = book<TH1F>("JetEfficiency_GenJetNPV_alljets_3to5_matchedjet", "Jet Efficiency NPV (3to5)",80,0,80 ); 


  JetEfficiency_GenJetNPV_central = book<TH1F>("JetEfficiency_GenJetNPV_central", "Jet Efficiency NPV (central)",80,0,80); 
  JetEfficiency_GenJetNPV_central_matchedjet = book<TH1F>("JetEfficiency_GenJetNPV_central_matchedjet", "Jet Efficiency NPV (central)",80,0,80 ); 

  JetEfficiency_GenJetNPV_forward = book<TH1F>("JetEfficiency_GenJetNPV_forward", "Jet Efficiency NPV (forward)",80,0,80); 
  JetEfficiency_GenJetNPV_forward_matchedjet = book<TH1F>("JetEfficiency_GenJetNPV_forward_matchedjet", "Jet Efficiency NPV (forward)",80,0,80 ); 


  JetEfficiency_GenJetNPV_0to1p3 = book<TH1F>("JetEfficiency_GenJetNPV_0to1p3", "Jet Efficiency NPV (0to1p3)",80,0,80); 
  JetEfficiency_GenJetNPV_0to1p3_matchedjet = book<TH1F>("JetEfficiency_GenJetNPV_0to1p3_matchedjet", "Jet Efficiency NPV (0to1p3)",80,0,80 ); 

  JetEfficiency_GenJetNPV_1p3to2p5 = book<TH1F>("JetEfficiency_GenJetNPV_1p3to2p5", "Jet Efficiency NPV (1p3to2p5)",80,0,80); 
  JetEfficiency_GenJetNPV_1p3to2p5_matchedjet = book<TH1F>("JetEfficiency_GenJetNPV_1p3to2p5_matchedjet", "Jet Efficiency NPV (1p3to2p5)",80,0,80 ); 

  JetEfficiency_GenJetNPV_2p5to3 = book<TH1F>("JetEfficiency_GenJetNPV_2p5to3", "Jet Efficiency NPV (2p5to3)",80,0,80); 
  JetEfficiency_GenJetNPV_2p5to3_matchedjet = book<TH1F>("JetEfficiency_GenJetNPV_2p5to3_matchedjet", "Jet Efficiency NPV (2p5to3)",80,0,80 ); 

  JetEfficiency_GenJetNPV_3to5 = book<TH1F>("JetEfficiency_GenJetNPV_3to5", "Jet Efficiency NPV (3to5)",80,0,80); 
  JetEfficiency_GenJetNPV_3to5_matchedjet = book<TH1F>("JetEfficiency_GenJetNPV_3to5_matchedjet", "Jet Efficiency NPV (3to5)",80,0,80 ); 


  // ////////////////////////////////////////////////////////////////       Jet Purity       /////////////////////////////////////////////////////////
  JetPurity_RecoJetEta = book<TH1F>("JetPurity_RecoJetEta", "Jet Purity RecoJetEta", 100,-5,5); 
  JetPurity_RecoJetEta_matchedjet = book<TH1F>("JetPurity_RecoJetEta_matchedjet", "Jet Purity RecoJetEta",100,-5,5); 
  JetPurity_RecoJetPt = book<TH1F>("JetPurity_RecoJetPt", "Jet Purity RecoJetPt", 2000,0,2000); 
  JetPurity_RecoJetPt_matchedjet = book<TH1F>("JetPurity_RecoJetPt_matchedjet", "Jet Purity RecoJetPt", 2000,0,2000); 

  JetPurity_RecoJetPt_central = book<TH1F>("JetPurity_RecoJetPt_central", "Jet Purity RecoJetPt central", 2000,0,2000); 
  JetPurity_RecoJetPt_central_matchedjet = book<TH1F>("JetPurity_RecoJetPt_central_matchedjet", "Jet Purity RecoJetPt central", 2000,0,2000); 

  JetPurity_RecoJetPt_forward = book<TH1F>("JetPurity_RecoJetPt_forward", "Jet Purity RecoJetPt forward", 2000,0,2000); 
  JetPurity_RecoJetPt_forward_matchedjet = book<TH1F>("JetPurity_RecoJetPt_forward_matchedjet", "Jet Purity RecoJetPt forward", 2000,0,2000); 

  JetPurity_RecoJetEta_alljets = book<TH1F>("JetPurity_RecoJetEta_alljets", "Jet Purity RecoJetEta", 100,-5,5); 
  JetPurity_RecoJetEta_alljets_matchedjet = book<TH1F>("JetPurity_RecoJetEta_alljets_matchedjet", "Jet Purity RecoJetEta",100,-5,5); 
  JetPurity_RecoJetPt_alljets = book<TH1F>("JetPurity_RecoJetPt_alljets", "Jet Purity RecoJetPt", 2000,0,2000); 
  JetPurity_RecoJetPt_alljets_matchedjet = book<TH1F>("JetPurity_RecoJetPt_alljets_matchedjet", "Jet Purity RecoJetPt", 2000,0,2000); 
		     
  JetPurity_RecoJetPt_alljets_central = book<TH1F>("JetPurity_RecoJetPt_alljets_central", "Jet Purity RecoJetPt central", 2000,0,2000); 
  JetPurity_RecoJetPt_alljets_central_matchedjet = book<TH1F>("JetPurity_RecoJetPt_alljets_central_matchedjet", "Jet Purity RecoJetPt central", 2000,0,2000); 
		     
  JetPurity_RecoJetPt_alljets_forward = book<TH1F>("JetPurity_RecoJetPt_alljets_forward", "Jet Purity RecoJetPt forward", 2000,0,2000); 
  JetPurity_RecoJetPt_alljets_forward_matchedjet = book<TH1F>("JetPurity_RecoJetPt_alljets_forward_matchedjet", "Jet Purity RecoJetPt forward", 2000,0,2000); 


  JetPurity_RecoJetNPV = book<TH1F>("JetPurity_RecoJetNPV", "Jet Purity NPV",80,0,80); 
  JetPurity_RecoJetNPV_matchedjet = book<TH1F>("JetPurity_RecoJetNPV_matchedjet", "Jet Purity NPV",80,0,80 ); 


  JetPurity_RecoJetNPV_central = book<TH1F>("JetPurity_RecoJetNPV_central", "Jet Purity NPV (central)",80,0,80); 
  JetPurity_RecoJetNPV_central_matchedjet = book<TH1F>("JetPurity_RecoJetNPV_central_matchedjet", "Jet Purity NPV (central)",80,0,80 ); 

  JetPurity_RecoJetNPV_forward = book<TH1F>("JetPurity_RecoJetNPV_forward", "Jet Purity NPV (forward)",80,0,80); 
  JetPurity_RecoJetNPV_forward_matchedjet = book<TH1F>("JetPurity_RecoJetNPV_forward_matchedjet", "Jet Purity NPV (forward)",80,0,80 ); 


  JetPurity_RecoJetNPV_alljets = book<TH1F>("JetPurity_RecoJetNPV_alljets", "Jet Purity NPV",80,0,80); 
  JetPurity_RecoJetNPV_alljets_matchedjet = book<TH1F>("JetPurity_RecoJetNPV_alljets_matchedjet", "Jet Purity NPV",80,0,80 ); 


  JetPurity_RecoJetNPV_alljets_central = book<TH1F>("JetPurity_RecoJetNPV_alljets_central", "Jet Purity NPV (central)",80,0,80); 
  JetPurity_RecoJetNPV_alljets_central_matchedjet = book<TH1F>("JetPurity_RecoJetNPV_alljets_central_matchedjet", "Jet Purity NPV (central)",80,0,80 ); 

  JetPurity_RecoJetNPV_alljets_forward = book<TH1F>("JetPurity_RecoJetNPV_alljets_forward", "Jet Purity NPV (forward)",80,0,80); 
  JetPurity_RecoJetNPV_alljets_forward_matchedjet = book<TH1F>("JetPurity_RecoJetNPV_alljets_forward_matchedjet", "Jet Purity NPV (forward)",80,0,80 ); 

  JetPurity_RecoJetNPV_alljets_0to1p3 = book<TH1F>("JetPurity_RecoJetNPV_alljets_0to1p3", "Jet Purity NPV (0to1p3)",80,0,80); 
  JetPurity_RecoJetNPV_alljets_0to1p3_matchedjet = book<TH1F>("JetPurity_RecoJetNPV_alljets_0to1p3_matchedjet", "Jet Purity NPV (0to1p3)",80,0,80 ); 

  JetPurity_RecoJetNPV_alljets_1p3to1p6 = book<TH1F>("JetPurity_RecoJetNPV_alljets_1p3to1p6", "Jet Purity NPV (1p3to1p6)",80,0,80); 
  JetPurity_RecoJetNPV_alljets_1p3to1p6_matchedjet = book<TH1F>("JetPurity_RecoJetNPV_alljets_1p3to1p6_matchedjet", "Jet Purity NPV (1p3to1p6)",80,0,80 ); 

  JetPurity_RecoJetNPV_alljets_1p6to2 = book<TH1F>("JetPurity_RecoJetNPV_alljets_1p6to2", "Jet Purity NPV (1p6to2)",80,0,80); 
  JetPurity_RecoJetNPV_alljets_1p6to2_matchedjet = book<TH1F>("JetPurity_RecoJetNPV_alljets_1p6to2_matchedjet", "Jet Purity NPV (1p6to2)",80,0,80 ); 

  JetPurity_RecoJetNPV_alljets_2to2p5 = book<TH1F>("JetPurity_RecoJetNPV_alljets_2to2p5", "Jet Purity NPV (2to2p5)",80,0,80); 
  JetPurity_RecoJetNPV_alljets_2to2p5_matchedjet = book<TH1F>("JetPurity_RecoJetNPV_alljets_2to2p5_matchedjet", "Jet Purity NPV (2to2p5)",80,0,80 ); 

  JetPurity_RecoJetNPV_alljets_2p5to3 = book<TH1F>("JetPurity_RecoJetNPV_alljets_2p5to3", "Jet Purity NPV (2p5to3)",80,0,80); 
  JetPurity_RecoJetNPV_alljets_2p5to3_matchedjet = book<TH1F>("JetPurity_RecoJetNPV_alljets_2p5to3_matchedjet", "Jet Purity NPV (2p5to3)",80,0,80 ); 

  JetPurity_RecoJetNPV_alljets_3to5 = book<TH1F>("JetPurity_RecoJetNPV_alljets_3to5", "Jet Purity NPV (3to5)",80,0,80); 
  JetPurity_RecoJetNPV_alljets_3to5_matchedjet = book<TH1F>("JetPurity_RecoJetNPV_alljets_3to5_matchedjet", "Jet Purity NPV (3to5)",80,0,80 ); 


  // ////////////////////////////////////////////////////////////////       diff QCD and DY       /////////////////////////////////////////////////////////
  deltar_nearest_gen_reco = book<TH1F>("deltar_nearest_gen_reco","DeltaR between the nearest gen to reco jet",100,0,10);

  matched_neutralEmEnergyFraction = book<TH1F>("matched_neutralEmEnergyFraction","matched_neutralEmEnergyFraction",100,0,1);
  matched_neutralHadronEnergyFraction= book<TH1F>("matched_neutralHadronEnergyFraction","matched_neutralHadronEnergyFraction",100,0,1);
  matched_chargedEmEnergyFraction= book<TH1F>("matched_chargedEmEnergyFraction","matched_chargedEmEnergyFraction",100,0,1);
  matched_chargedHadronEnergyFraction= book<TH1F>("matched_chargedHadronEnergyFraction","matched_chargedHadronEnergyFraction",100,0,1);

  unmatched_neutralEmEnergyFraction = book<TH1F>("unmatched_neutralEmEnergyFraction","unmatched_neutralEmEnergyFraction",100,0,1);
  unmatched_neutralHadronEnergyFraction= book<TH1F>("unmatched_neutralHadronEnergyFraction","unmatched_neutralHadronEnergyFraction",100,0,1);
  unmatched_chargedEmEnergyFraction= book<TH1F>("unmatched_chargedEmEnergyFraction","unmatched_chargedEmEnergyFraction",100,0,1);
  unmatched_chargedHadronEnergyFraction= book<TH1F>("unmatched_chargedHadronEnergyFraction","unmatched_chargedHadronEnergyFraction",100,0,1);


  ////////////////////////////////////          General Hist  ///////////////////////////////////////////
  pT_genjets = book<TH1F>("pT_genjets","pT of Genjets",2000,0,2000); //before cleaning
  pT_recojets = book<TH1F>("pT_recojets","pT of Recojets",2000,0,2000); //before cleaning

  eta_genjets = book<TH1F>("eta_genjets","eta of Genjets",20,-10,10); //before cleaning
  eta_recojets = book<TH1F>("eta_recojets","eta of Recojets",20,-10,10); //before cleaning

  pT_genjets_firstjet = book<TH1F>("pT_genjets_firstjet","pT of Genjets",2000,0,2000); //before cleaning
  pT_recojets_firstjet = book<TH1F>("pT_recojets_firstjet","pT of Recojets",2000,0,2000); //before cleaning

  eta_genjets_firstjet = book<TH1F>("eta_genjets_firstjet","eta of Genjets",20,-10,10); //before cleaning
  eta_recojets_firstjet = book<TH1F>("eta_recojets_firstjet","Eta of Recojets",20,-10,10); //before cleaning

  pT_eff_recojets = book<TH1F>("pT_eff_recojets","pT of Recojets during eff calculation",2000,0,2000); // pT of the matched reco jets to genrator jets, should be below the pT of generator jets in eff
  pT_purity_genjets = book<TH1F>("pT_purity_genjets","pT of Genjets during purity calculation",2000,0,2000); // pT of matched gen jets to reco jets, should be below the pT of reco jets in purity

  // CHS AK4jets for testing the difference between CHS and PUPPI
  h_CHSjets = ctx.get_handle<std::vector<Jet>>("slimmedJets");

}


void EffPurityHists::fill(const Event & event){
  if(berror) std::cout<<"in EffPurityHists  "<<std::endl;
  assert(event.met);
  assert(event.pvs);
  assert(event.jets);
  
  
  double weight = event.weight;
  if(berror) std::cout<<"weight  "<<weight<<std::endl;
 

  if(berror)std::cout<<"Number of PV  "<<event.pvs->size()<<std::endl;
  if(berror)std::cout<<"Number of Jets  "<<event.jets->size()<<std::endl;
  if(berror)std::cout<<"Number of GenJets  "<<event.genjets->size()<<std::endl;

  //setup different jet collections
  vector<Jet> jets = collection.empty() ?  *event.jets : event.get(h_myjets);
  vector<GenJet> genparticles = collection.empty() ?  *event.genjets : event.get(h_mygenjets);
  std::vector<Jet>  chs_jets = event.is_valid(h_CHSjets) ? event.get(h_CHSjets) : *event.jets;

  //setup both vertices collections
  //decide if it should be as function of reco vertices or true Interaction
  //  double npvs = event.pvs->size();
  double npvs = event.genInfo->pileup_TrueNumInteractions();

  ///// general hist of first jet
  if(genparticles.size()){
    pT_genjets_firstjet->Fill(genparticles.at(0).pt(),weight);
    eta_genjets_firstjet->Fill(genparticles.at(0).eta(),weight);
  }
  if(jets.size()){
    pT_recojets_firstjet->Fill(jets.at(0).pt(),weight);
    eta_recojets_firstjet->Fill(jets.at(0).eta(),weight);
  }

  if(berror) std::cout<<"EffPurityHists::JetEfficiency"<<std::endl;
  ////////////////////////////////////////   Jet Efficiency   /////////////////////////////////////////
  double genp_pt=0;
  double genp_eta_matched=0;
  double genp_pt_matched=0;

  //cleaning of the GenJetcollection to 25 GeV
  std::unique_ptr< std::vector<GenJet> > genjets_pt25 (new std::vector<GenJet> (genparticles));
  genjets_pt25->clear();
  genjets_pt25->reserve(genparticles.size());

  // cleaning genjets based pT requirement
  /*
    This means we only want genjets above a certain pT threshold for the eff.
    This pT threshold should be higher than the pT cut on reco jets to be independent of effects from JEC.
  */
  for(auto j:genparticles){
    genp_pt = j.pt();
    eta_genjets->Fill(j.eta(),weight);
    pT_genjets->Fill(genp_pt,weight);
    if(berror)std::cout<<" genp_pt  "<<genp_pt<<"  pTcut  "<<pTcut<<std::endl;
    if(genp_pt>pTcut)genjets_pt25->push_back(j);
  }

  //fill eff hists
  /*
    Fill all histograms with all jets above a certain pT threhold, later on fill histograms only when matched to reco jet
  */
  for(unsigned int j=0;j<genjets_pt25->size();j++){
    double genp_eta = genjets_pt25->at(j).eta();
    genp_pt = genjets_pt25->at(j).pt();
    
    // this is to find a reason why some jets are lost in QCD, we match them to CHS where they are available and look at different varaibles
    bool matched = false;

    // jet eff hists for all genparticles above pT threshold
    JetEfficiency_GenJetEta_genJetPt_alljets->Fill(genp_eta,genp_pt,weight);
    JetEfficiency_GenJetEta_alljets->Fill(genp_eta,weight);
    JetEfficiency_GenJetPt_alljets->Fill(genp_pt,weight);
    JetEfficiency_GenJetNPV_alljets->Fill(npvs,weight);
    if(abs(genp_eta)<2.5){
      JetEfficiency_GenJetNPV_alljets_central->Fill(npvs,weight);
      JetEfficiency_GenJetPt_alljets_central->Fill(genp_pt,weight);
    }else{
      JetEfficiency_GenJetNPV_alljets_forward->Fill(npvs,weight);
      JetEfficiency_GenJetPt_alljets_forward->Fill(genp_pt,weight);
    }
    if(abs(genjets_pt25->at(j).eta())<1.3) JetEfficiency_GenJetPt_alljets_0to1p3->Fill(genp_pt,weight);
    else if(abs(genjets_pt25->at(j).eta())<2.5) JetEfficiency_GenJetPt_alljets_1p3to2p5->Fill(genp_pt,weight);
    else if(abs(genjets_pt25->at(j).eta())<3)JetEfficiency_GenJetPt_alljets_2p5to3->Fill(genp_pt,weight);
    else JetEfficiency_GenJetPt_alljets_3to5->Fill(genp_pt,weight);

    if(abs(genjets_pt25->at(j).eta())<1.3) JetEfficiency_GenJetNPV_alljets_0to1p3->Fill(npvs,weight);
    else if(abs(genjets_pt25->at(j).eta())<1.6) JetEfficiency_GenJetNPV_alljets_1p3to1p6->Fill(npvs,weight);
    else if(abs(genjets_pt25->at(j).eta())<2.0) JetEfficiency_GenJetNPV_alljets_1p6to2->Fill(npvs,weight);
    else if(abs(genjets_pt25->at(j).eta())<2.5) JetEfficiency_GenJetNPV_alljets_2to2p5->Fill(npvs,weight);
    else if(abs(genjets_pt25->at(j).eta())<3)JetEfficiency_GenJetNPV_alljets_2p5to3->Fill(npvs,weight);
    else JetEfficiency_GenJetNPV_alljets_3to5->Fill(npvs,weight);


    /*
      Fill histograms only when matched to reco jet.
      Eff is the following histograms divided by the above histograms.
    */
    double deltar_min = 99;
    for(unsigned int i=0;i<jets.size();i++){
      double deltaR_gen_jet = deltaR(genjets_pt25->at(j),jets.at(i));
      pT_eff_recojets->Fill(jets.at(i).pt(),weight);
      if(deltaR_gen_jet<deltar_min)deltar_min = deltaR_gen_jet;
      
      if(deltaR_gen_jet<0.4){
	matched_neutralEmEnergyFraction->Fill(jets.at(i).neutralEmEnergyFraction(),weight);
	matched_neutralHadronEnergyFraction->Fill(jets.at(i).neutralHadronEnergyFraction(),weight);
	matched_chargedEmEnergyFraction->Fill(jets.at(i).chargedEmEnergyFraction(),weight);
	matched_chargedHadronEnergyFraction->Fill(jets.at(i).chargedHadronEnergyFraction(),weight);

	matched = true;
	genp_eta_matched = genjets_pt25->at(j).eta();
	genp_pt_matched = genjets_pt25->at(j).pt();

	JetEfficiency_GenJetEta_genJetPt_alljets_matchedjet->Fill(genp_eta_matched,genp_pt_matched,weight);
	JetEfficiency_GenJetEta_alljets_matchedjet->Fill(genp_eta_matched,weight);
	JetEfficiency_GenJetPt_alljets_matchedjet->Fill(genp_pt_matched,weight);
	JetEfficiency_GenJetNPV_alljets_matchedjet->Fill(npvs,weight);
	if(abs(genp_eta_matched)<2.5){
	  JetEfficiency_GenJetNPV_alljets_central_matchedjet->Fill(npvs,weight);
	  JetEfficiency_GenJetPt_alljets_central_matchedjet->Fill(genp_pt_matched,weight);
	}else{
	JetEfficiency_GenJetNPV_alljets_forward_matchedjet->Fill(npvs,weight);
	  JetEfficiency_GenJetPt_alljets_forward_matchedjet->Fill(genp_pt_matched,weight);
	}
	if(abs(genjets_pt25->at(j).eta())<1.3) JetEfficiency_GenJetPt_alljets_0to1p3_matchedjet->Fill(genp_pt_matched,weight);
	else if(abs(genjets_pt25->at(j).eta())<2.5) JetEfficiency_GenJetPt_alljets_1p3to2p5_matchedjet->Fill(genp_pt_matched,weight);
	else if(abs(genjets_pt25->at(j).eta())<3)JetEfficiency_GenJetPt_alljets_2p5to3_matchedjet->Fill(genp_pt_matched,weight);
	else JetEfficiency_GenJetPt_alljets_3to5_matchedjet->Fill(genp_pt_matched,weight);

	if(abs(genjets_pt25->at(j).eta())<1.3) JetEfficiency_GenJetNPV_alljets_0to1p3_matchedjet->Fill(npvs,weight);
	else if(abs(genjets_pt25->at(j).eta())<1.6) JetEfficiency_GenJetNPV_alljets_1p3to1p6_matchedjet->Fill(npvs,weight);
	else if(abs(genjets_pt25->at(j).eta())<2.0) JetEfficiency_GenJetNPV_alljets_1p6to2_matchedjet->Fill(npvs,weight);
	else if(abs(genjets_pt25->at(j).eta())<2.5) JetEfficiency_GenJetNPV_alljets_2to2p5_matchedjet->Fill(npvs,weight);
	else if(abs(genjets_pt25->at(j).eta())<3)JetEfficiency_GenJetNPV_alljets_2p5to3_matchedjet->Fill(npvs,weight);
	else JetEfficiency_GenJetNPV_alljets_3to5_matchedjet->Fill(npvs,weight);

	break;
      }
    }
    if(!matched){
      /* 
	 if we can not find a reco jet that is matching to the generator jet we match it with a CHS jet and look at some quantities.
       */
      if(berror)std::cout<<"===============================================================   not matched genjets  "<<j<<std::endl;
      for(auto jet:chs_jets){
	double deltaR_gen_jet = deltaR(genjets_pt25->at(j),jet);
	if(deltaR_gen_jet<0.4){
	  unmatched_neutralEmEnergyFraction->Fill(jet.neutralEmEnergyFraction(),weight);
	  unmatched_neutralHadronEnergyFraction->Fill(jet.neutralHadronEnergyFraction(),weight);
	  unmatched_chargedEmEnergyFraction->Fill(jet.chargedEmEnergyFraction(),weight);
	  unmatched_chargedHadronEnergyFraction->Fill(jet.chargedHadronEnergyFraction(),weight);
	  
	}
      }
    }
    deltar_nearest_gen_reco->Fill(deltar_min,weight);
  }//for-loop over all genjets_pt25

  // fill hists for first jet only
  if(genjets_pt25->size()){
    
    double genp_eta = genjets_pt25->at(0).eta();
    double genp_pt =  genjets_pt25->at(0).pt();
    
    JetEfficiency_GenJetEta_genJetPt->Fill(genp_eta,genp_pt,weight);
    JetEfficiency_GenJetEta->Fill(genp_eta,weight);
    JetEfficiency_GenJetPt->Fill(genp_pt,weight);
    if(abs(genp_eta)<2.5){
      JetEfficiency_GenJetPt_central->Fill(genp_pt,weight);
    }else{
      JetEfficiency_GenJetPt_forward->Fill(genp_pt,weight);
    }
    if(abs(genjets_pt25->at(0).eta())<1.3) JetEfficiency_GenJetPt_0to1p3->Fill(genp_pt,weight);
    else if(abs(genjets_pt25->at(0).eta())<2.5) JetEfficiency_GenJetPt_1p3to2p5->Fill(genp_pt,weight);
    else if(abs(genjets_pt25->at(0).eta())<3)JetEfficiency_GenJetPt_2p5to3->Fill(genp_pt,weight);
    else JetEfficiency_GenJetPt_3to5->Fill(genp_pt,weight);

    JetEfficiency_GenJetNPV->Fill(npvs,weight);
    if(abs(genjets_pt25->at(0).eta())<1.3) JetEfficiency_GenJetNPV_0to1p3->Fill(npvs,weight);
    else if(abs(genjets_pt25->at(0).eta())<2.5) JetEfficiency_GenJetNPV_1p3to2p5->Fill(npvs,weight);
    else if(abs(genjets_pt25->at(0).eta())<3)JetEfficiency_GenJetNPV_2p5to3->Fill(npvs,weight);
    else JetEfficiency_GenJetNPV_3to5->Fill(npvs,weight);
    
    if(abs(genjets_pt25->at(0).eta())<2.5)JetEfficiency_GenJetNPV_central->Fill(npvs,weight);
    else JetEfficiency_GenJetNPV_forward->Fill(npvs,weight); 
    
    for(unsigned int j=0;j<jets.size();j++){
      double deltaR_gen_jet1 = deltaR(genjets_pt25->at(0),jets.at(j));
      if(deltaR_gen_jet1<0.4){
	double genp_eta_matched = genjets_pt25->at(0).eta();
	double genp_pt_matched= genjets_pt25->at(0).pt();

	JetEfficiency_GenJetEta_genJetPt_matchedjet->Fill(genp_eta_matched,genp_pt_matched,weight);
	JetEfficiency_GenJetEta_matchedjet->Fill(genp_eta_matched,weight);
	JetEfficiency_GenJetPt_matchedjet->Fill(genp_pt_matched,weight);
	if(abs(genp_eta_matched)<2.5){
	  JetEfficiency_GenJetPt_central_matchedjet->Fill(genp_pt_matched,weight);
	}else{
	  JetEfficiency_GenJetPt_forward_matchedjet->Fill(genp_pt_matched,weight);
	}
	if(abs(genjets_pt25->at(0).eta())<1.3) JetEfficiency_GenJetPt_0to1p3_matchedjet->Fill(genp_pt_matched,weight);
	else if(abs(genjets_pt25->at(0).eta())<2.5) JetEfficiency_GenJetPt_1p3to2p5_matchedjet->Fill(genp_pt_matched,weight);
	else if(abs(genjets_pt25->at(0).eta())<3)JetEfficiency_GenJetPt_2p5to3_matchedjet->Fill(genp_pt_matched,weight);
	else JetEfficiency_GenJetPt_3to5_matchedjet->Fill(genp_pt_matched,weight);

	JetEfficiency_GenJetNPV_matchedjet->Fill(npvs, weight);
	if(abs(genjets_pt25->at(0).eta())<1.3) JetEfficiency_GenJetNPV_0to1p3_matchedjet->Fill(npvs,weight);
	else if(abs(genjets_pt25->at(0).eta())<2.5) JetEfficiency_GenJetNPV_1p3to2p5_matchedjet->Fill(npvs,weight);
	else if(abs(genjets_pt25->at(0).eta())<3)JetEfficiency_GenJetNPV_2p5to3_matchedjet->Fill(npvs,weight);
	else JetEfficiency_GenJetNPV_3to5_matchedjet->Fill(npvs,weight);
	
	if(abs(genjets_pt25->at(0).eta())<2.5)JetEfficiency_GenJetNPV_central_matchedjet->Fill(npvs,weight);
	else JetEfficiency_GenJetNPV_forward_matchedjet->Fill(npvs,weight); 
	break;
      }
    }
  }
    

  if(berror) std::cout<<"EffPurityHists::JetPurity"<<std::endl;
  ////////////////////////////////////////   Jet Purity   /////////////////////////////////////////
  double reco_eta_matched=0;
  double reco_pt_matched=0;
  
  //cleaning of the Jetcollection to 25 GeV
  /*
    This means we only want jets above a certain pT threshold for the purity.
    This pT threshold should be higher than the pT cut on gen jets to be independent of effects from JEC.
  */
  std::unique_ptr< std::vector<Jet> > jets_pt25 (new std::vector<Jet> (jets));
  jets_pt25->clear();
  jets_pt25->reserve(jets.size());
  for(auto j:jets){
    double reco_pt = j.pt();
    pT_recojets->Fill(j.pt(),weight);
    eta_recojets->Fill(j.eta(),weight);
    
    if(reco_pt>pTcut)jets_pt25->push_back(j);
  }

  //fill purity hists
  /*
    Fill all histograms with all jets above a certain pT threhold, later on fill histograms only when matched to generator jet
  */
  for(unsigned int j=0;j<jets_pt25->size();j++){
    double reco_eta = jets_pt25->at(j).eta();
    double reco_pt = jets_pt25->at(j).pt();

    JetPurity_RecoJetEta_alljets->Fill(reco_eta,weight);
    JetPurity_RecoJetPt_alljets->Fill(reco_pt,weight);
    JetPurity_RecoJetNPV_alljets->Fill(npvs,weight);

    if(abs(reco_eta)<2.5){
      JetPurity_RecoJetNPV_alljets_central->Fill(npvs,weight);
      JetPurity_RecoJetPt_alljets_central->Fill(reco_pt,weight);
    }else{
      JetPurity_RecoJetNPV_alljets_forward->Fill(npvs,weight);
      JetPurity_RecoJetPt_alljets_forward->Fill(reco_pt,weight);
    }

    if(abs(jets_pt25->at(j).eta())<1.3) JetPurity_RecoJetNPV_alljets_0to1p3->Fill(npvs,weight);
    else if(abs(jets_pt25->at(j).eta())<1.6) JetPurity_RecoJetNPV_alljets_1p3to1p6->Fill(npvs,weight);
    else if(abs(jets_pt25->at(j).eta())<2.0) JetPurity_RecoJetNPV_alljets_1p6to2->Fill(npvs,weight);
    else if(abs(jets_pt25->at(j).eta())<2.5) JetPurity_RecoJetNPV_alljets_2to2p5->Fill(npvs,weight);
    else if(abs(jets_pt25->at(j).eta())<3)JetPurity_RecoJetNPV_alljets_2p5to3->Fill(npvs,weight);
    else JetPurity_RecoJetNPV_alljets_3to5->Fill(npvs,weight);
    
    //do matching 
    /*
	Fill histograms only when matched to generator jet.
	Purity is the following histograms divided by the above histograms.
    */
    if(berror) std::cout<<"do matching for purity"<<std::endl;
    for(unsigned int k=0;k<genparticles.size();k++){
      genp_pt = genparticles.at(k).pt();
	
      double deltaR_gen_jet = deltaR(genparticles.at(k),jets_pt25->at(j));
	if(deltaR_gen_jet<0.4){
	  reco_eta_matched = jets_pt25->at(j).eta();
	  reco_pt_matched = jets_pt25->at(j).pt();


	  JetPurity_RecoJetEta_alljets_matchedjet->Fill(reco_eta_matched,weight);
	  JetPurity_RecoJetPt_alljets_matchedjet->Fill(reco_pt_matched,weight);
	  JetPurity_RecoJetNPV_alljets_matchedjet->Fill(npvs,weight);
	  if(abs(reco_eta_matched)<2.5){
	    JetPurity_RecoJetNPV_alljets_central_matchedjet->Fill(npvs,weight);
	    JetPurity_RecoJetPt_alljets_central_matchedjet->Fill(reco_pt_matched,weight);
	  }else{
	    JetPurity_RecoJetNPV_alljets_forward_matchedjet->Fill(npvs,weight);
	    JetPurity_RecoJetPt_alljets_forward_matchedjet->Fill(reco_pt_matched,weight);
	  }
	
	  if(abs(jets_pt25->at(j).eta())<1.3) JetPurity_RecoJetNPV_alljets_0to1p3_matchedjet->Fill(npvs,weight);
	  else if(abs(jets_pt25->at(j).eta())<1.6) JetPurity_RecoJetNPV_alljets_1p3to1p6_matchedjet->Fill(npvs,weight);
	  else if(abs(jets_pt25->at(j).eta())<2.0) JetPurity_RecoJetNPV_alljets_1p6to2_matchedjet->Fill(npvs,weight);
	  else if(abs(jets_pt25->at(j).eta())<2.5) JetPurity_RecoJetNPV_alljets_2to2p5_matchedjet->Fill(npvs,weight);
	  else if(abs(jets_pt25->at(j).eta())<3)JetPurity_RecoJetNPV_alljets_2p5to3_matchedjet->Fill(npvs,weight);
	  else JetPurity_RecoJetNPV_alljets_3to5_matchedjet->Fill(npvs,weight);
	  break;
	}
    }//for-loop over all genparticles
  }//for-loop over all jets_pt25

  //fill hists for first jet only
  if(jets.size()){
    double reco_eta = jets.at(0).eta();
    double reco_pt = jets.at(0).pt();

    JetPurity_RecoJetEta->Fill(reco_eta,weight);
    JetPurity_RecoJetPt->Fill(reco_pt,weight);

    if(abs(reco_eta)<2.5){
      JetPurity_RecoJetPt_central->Fill(reco_pt,weight);
    }else{
      JetPurity_RecoJetPt_forward->Fill(reco_pt,weight);
    }


    JetPurity_RecoJetNPV->Fill(npvs,weight);
    if(abs(jets.at(0).eta())<2.5) JetPurity_RecoJetNPV_central->Fill(npvs, weight);
    else JetPurity_RecoJetNPV_forward->Fill(npvs, weight);
    
    for(unsigned int j=0;j<genparticles.size();j++){
      double deltaR_gen_jet1 = deltaR(genparticles.at(j),jets.at(0));
      if(deltaR_gen_jet1<0.4){
	double reco_eta_matched = jets.at(0).eta();
	double reco_pt_matched = jets.at(0).pt();

	JetPurity_RecoJetEta_matchedjet->Fill(reco_eta_matched,weight);
	JetPurity_RecoJetPt_matchedjet->Fill(reco_pt_matched,weight);
	if(abs(reco_eta_matched)<2.5){
	  JetPurity_RecoJetPt_central_matchedjet->Fill(reco_pt_matched,weight);
	}else{
	  JetPurity_RecoJetPt_forward_matchedjet->Fill(reco_pt_matched,weight);
	}

	pT_purity_genjets->Fill(genp_pt,weight);
	JetPurity_RecoJetNPV_matchedjet->Fill(npvs,weight);
	if(abs(jets.at(0).eta())<2.5) JetPurity_RecoJetNPV_central_matchedjet->Fill(npvs, weight);
	else JetPurity_RecoJetNPV_forward_matchedjet->Fill(npvs, weight);
	break;
      }
    }
  }
}



EffPurityHists::~EffPurityHists(){}
