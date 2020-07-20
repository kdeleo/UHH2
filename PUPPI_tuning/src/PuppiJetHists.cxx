#include "UHH2/PUPPI_tuning/include/PuppiJetHists.h"
#include "UHH2/core/include/Event.h"
#include "UHH2/core/include/GenParticle.h"
#include "UHH2/core/include/LorentzVector.h"


#include <iostream>

using namespace std;
using namespace uhh2;
using namespace uhh2examples;

/*
This class is used to determine the Jet pT scale and response in different eta bins.
We also store some different PU bins.
 */

PuppiJetHists::PuppiJetHists(Context & ctx, const string & dirname, const string & collection_): Hists(ctx, dirname), collection(collection_){
  berror=(ctx.get("debug") == "true");
  pTcut =atof(ctx.get("pTCut").c_str());

  if(!collection.empty()){
    h_myjets = ctx.get_handle<std::vector<Jet> >(collection);
    h_mygenjets = ctx.get_handle<std::vector<GenJet> >("gen"+collection);
  }

  ////////////////////////////////////////////////////////////////       Jet Number Bins Eta       /////////////////////////////////////////////////////////
  JetNumber_Eta0to1p3   = book<TH1F>("JetNumber_Eta0to1p3", "Number of jets (0<eta<1p3)", 21, -.5, 20.5);
  JetNumber_Eta1p3to1p6 = book<TH1F>("JetNumber_Eta1p3to1p6", "Number of jets (1p3<eta<1p6)", 21, -.5, 20.5);
  JetNumber_Eta1p6to2   = book<TH1F>("JetNumber_Eta1p6to2", "Number of jets (1p6<eta<2)", 21, -.5, 20.5);
  JetNumber_Eta2to2p5   = book<TH1F>("JetNumber_Eta2to2p5", "Number of jets (2<eta<2p5)", 21, -.5, 20.5);
  JetNumber_Eta2p5to3   = book<TH1F>("JetNumber_Eta2p5to3", "Number of jets (2p5<eta<3)", 21, -.5, 20.5);
  JetNumber_Eta3to10    = book<TH1F>("JetNumber_Eta3to10", "Number of jets (3<eta<10)", 21, -.5, 20.5);

  ////////////////////////////////////////////////////////////////       Jet Pt Bins Eta       /////////////////////////////////////////////////////////
  JetPt_Eta0to1p3   = book<TH1F>("JetPt_Eta0to1p3", "pt jets (0<eta<1p3)", 150, 0.0, 1500.0);
  JetPt_Eta1p3to1p6 = book<TH1F>("JetPt_Eta1p3to1p6", "pt jets (1p3<eta<1p6)", 150, 0.0, 1500.0);
  JetPt_Eta1p6to2   = book<TH1F>("JetPt_Eta1p6to2", "pt jets (1p6<eta<2)", 150, 0.0, 1500.0);
  JetPt_Eta2to2p5   = book<TH1F>("JetPt_Eta2to2p5", "pt jets (2<eta<2p5)", 150, 0.0, 1500.0);
  JetPt_Eta2p5to3   = book<TH1F>("JetPt_Eta2p5to3", "pt jets (2p5<eta<3)", 150, 0.0, 1500.0);
  JetPt_Eta3to10    = book<TH1F>("JetPt_Eta3to10", "pt jets (3<eta<10)", 150, 0.0, 1500.0);


  ////////////////////////////////////////////////////////////////       Jet Eta Bins Eta       /////////////////////////////////////////////////////////
  JetEta   = book<TH1F>("JetEta", "eta jets", 100,-5,5);


  ////////////////////////////////////////////////////////////////       Jet Mass Bins Eta       /////////////////////////////////////////////////////////
  JetMass_Eta0to1p3   = book<TH1F>("JetMass_Eta0to1p3", "mass jets (0<eta<1p3)", 150, 0.0, 300.0);
  JetMass_Eta1p3to1p6 = book<TH1F>("JetMass_Eta1p3to1p6", "mass jets (1p3<eta<1p6)", 150, 0.0, 300.0);
  JetMass_Eta1p6to2   = book<TH1F>("JetMass_Eta1p6to2", "mass jets (1p6<eta<2)", 150, 0.0, 300.0);
  JetMass_Eta2to2p5   = book<TH1F>("JetMass_Eta2to2p5", "mass jets (2<eta<2p5)", 150, 0.0, 300.0);
  JetMass_Eta2p5to3   = book<TH1F>("JetMass_Eta2p5to3", "mass jets (2p5<eta<3)", 150, 0.0, 300.0);
  JetMass_Eta3to10    = book<TH1F>("JetMass_Eta3to10", "mass jets (3<eta<10)", 150, 0.0, 300.0);


  ////////////////////////////////////////////////////////////////       Jet PT Scale       /////////////////////////////////////////////////////////
  JetPtScale_Eta0to1p3 = book<TH2F>("JetPtScale_Eta0to1p3", "Jet p_{T} scale (0<eta<1p3)", 100, 0, 100,200,-2,2); 
  JetPtScale_Eta1p3to1p6 = book<TH2F>("JetPtScale_Eta1p3to1p6", "Jet p_{T} scale (1p3<eta<1p6)", 100, 0, 100,200,-2,2); 
  JetPtScale_Eta1p6to2 = book<TH2F>("JetPtScale_Eta1p6to2", "Jet p_{T} scale (1p6<eta<2)", 100, 0, 100,200,-2,2); 
  JetPtScale_Eta2to2p5 = book<TH2F>("JetPtScale_Eta2to2p5", "Jet p_{T} scale (2<eta<2p5)", 100, 0, 100,200,-2,2); 
  JetPtScale_Eta2p5to3 = book<TH2F>("JetPtScale_Eta2p5to3", "Jet p_{T} scale (2p5<eta<3)", 100, 0, 100,200,-2,2); 
  JetPtScale_Eta3to10 = book<TH2F>("JetPtScale_Eta3to10", "Jet p_{T} scale (3<eta<10)", 100, 0, 100,200,-2,2); 

  ////////////////////////////////////////////////////////////////       Jet PT Response       /////////////////////////////////////////////////////////
  JetPtResponse_Eta0to1p3 = book<TH2F>("JetPtResponse_Eta0to1p3", "Jet p_{T} scale (0<eta<1p3)", 2000, 0, 2000,200,-4,4); 
  JetPtResponse_Eta1p3to1p6 = book<TH2F>("JetPtResponse_Eta1p3to1p6", "Jet p_{T} scale (1p3<eta<1p6)", 2000, 0, 2000,200,-4,4); 
  JetPtResponse_Eta1p6to2 = book<TH2F>("JetPtResponse_Eta1p6to2", "Jet p_{T} scale (1p6<eta<2)", 2000, 0, 2000,200,-4,4); 
  JetPtResponse_Eta2to2p5 = book<TH2F>("JetPtResponse_Eta2to2p5", "Jet p_{T} scale (2<eta<2p5)", 2000, 0, 2000,200,-4,4); 
  JetPtResponse_Eta2p5to3 = book<TH2F>("JetPtResponse_Eta2p5to3", "Jet p_{T} scale (2p5<eta<3)", 2000, 0, 2000,200,-4,4); 
  JetPtResponse_Eta3to10 = book<TH2F>("JetPtResponse_Eta3to10", "Jet p_{T} scale (3<eta<10)", 2000, 0, 2000,200,-4,4); 
  JetPtResponse_Eta3to3 = book<TH1F>("JetPtResponse_Eta3to3", "Jet p_{T} response (3.314<=eta<3.489)", 100,0,4); 


  ////////////////////////////////////////////////////////////////       Jet PT Response PU 0 - 10       /////////////////////////////////////////////////////////
  JetPtResponse_Eta0to1p3_PU0to10 = book<TH2F>("JetPtResponse_Eta0to1p3_PU0to10", "Jet p_{T} scale (0<eta<1p3) PU0-10", 2000, 0, 2000,200,-4,4); 
  JetPtResponse_Eta1p3to1p6_PU0to10 = book<TH2F>("JetPtResponse_Eta1p3to1p6_PU0to10", "Jet p_{T} scale (1p3<eta<1p6) PU0-10", 2000, 0, 2000,200,-4,4); 
  JetPtResponse_Eta1p6to2_PU0to10 = book<TH2F>("JetPtResponse_Eta1p6to2_PU0to10", "Jet p_{T} scale (1p6<eta<2) PU0-10", 2000, 0, 2000,200,-4,4);
  JetPtResponse_Eta2to2p5_PU0to10 = book<TH2F>("JetPtResponse_Eta2to2p5_PU0to10", "Jet p_{T} scale (2<eta<2p5) PU0-10", 2000, 0, 2000,200,-4,4); 
  JetPtResponse_Eta2p5to3_PU0to10 = book<TH2F>("JetPtResponse_Eta2p5to3_PU0to10", "Jet p_{T} scale (2p5<eta<3) PU0-10", 2000, 0, 2000,200,-4,4); 
  JetPtResponse_Eta3to10_PU0to10 = book<TH2F>("JetPtResponse_Eta3to10_PU0to10", "Jet p_{T} scale (3<eta<10) PU0-10", 2000, 0, 2000,200,-4,4); 

  ////////////////////////////////////////////////////////////////       Jet PT Response PU 10 - 20       /////////////////////////////////////////////////////////
  JetPtResponse_Eta0to1p3_PU10to20 = book<TH2F>("JetPtResponse_Eta0to1p3_PU10to20", "Jet p_{T} scale (0<eta<1p3) PU10-20", 2000, 0, 2000,200,-4,4); 
  JetPtResponse_Eta1p3to1p6_PU10to20 = book<TH2F>("JetPtResponse_Eta1p3to1p6_PU10to20", "Jet p_{T} scale (1p3<eta<1p6) PU10-20", 2000, 0, 2000,200,-4,4); 
  JetPtResponse_Eta1p6to2_PU10to20 = book<TH2F>("JetPtResponse_Eta1p6to2_PU10to20", "Jet p_{T} scale (1p6<eta<2) PU10-20", 2000, 0, 2000,200,-4,4);
  JetPtResponse_Eta2to2p5_PU10to20 = book<TH2F>("JetPtResponse_Eta2to2p5_PU10to20", "Jet p_{T} scale (2<eta<2p5) PU10-20", 2000, 0, 2000,200,-4,4); 
  JetPtResponse_Eta2p5to3_PU10to20 = book<TH2F>("JetPtResponse_Eta2p5to3_PU10to20", "Jet p_{T} scale (2p5<eta<3) PU10-20", 2000, 0, 2000,200,-4,4); 
  JetPtResponse_Eta3to10_PU10to20 = book<TH2F>("JetPtResponse_Eta3to10_PU10to20", "Jet p_{T} scale (3<eta<10) PU10-20", 2000, 0, 2000,200,-4,4); 

  ////////////////////////////////////////////////////////////////       Jet PT Response PU 30 - 40       /////////////////////////////////////////////////////////
  JetPtResponse_Eta0to1p3_PU30to40 = book<TH2F>("JetPtResponse_Eta0to1p3_PU30to40", "Jet p_{T} scale (0<eta<1p3) PU30-40", 2000, 0, 2000,200,-4,4); 
  JetPtResponse_Eta1p3to1p6_PU30to40 = book<TH2F>("JetPtResponse_Eta1p3to1p6_PU30to40", "Jet p_{T} scale (1p3<eta<1p6) PU30-40", 2000, 0, 2000,200,-4,4); 
  JetPtResponse_Eta1p6to2_PU30to40 = book<TH2F>("JetPtResponse_Eta1p6to2_PU30to40", "Jet p_{T} scale (1p6<eta<2) PU30-40", 2000, 0, 2000,200,-4,4);
  JetPtResponse_Eta2to2p5_PU30to40 = book<TH2F>("JetPtResponse_Eta2to2p5_PU30to40", "Jet p_{T} scale (2<eta<2p5) PU30-40", 2000, 0, 2000,200,-4,4); 
  JetPtResponse_Eta2p5to3_PU30to40 = book<TH2F>("JetPtResponse_Eta2p5to3_PU30to40", "Jet p_{T} scale (2p5<eta<3) PU30-40", 2000, 0, 2000,200,-4,4); 
  JetPtResponse_Eta3to10_PU30to40 = book<TH2F>("JetPtResponse_Eta3to10_PU30to40", "Jet p_{T} scale (3<eta<10) PU30-40", 2000, 0, 2000,200,-4,4); 

  ////////////////////////////////////////////////////////////////       Jet PT Response PU 50 - 70       /////////////////////////////////////////////////////////
  JetPtResponse_Eta0to1p3_PU50to70 = book<TH2F>("JetPtResponse_Eta0to1p3_PU50to70", "Jet p_{T} scale (0<eta<1p3) PU>50", 2000, 0, 2000,200,-4,4); 
  JetPtResponse_Eta1p3to1p6_PU50to70 = book<TH2F>("JetPtResponse_Eta1p3to1p6_PU50to70", "Jet p_{T} scale (1p3<eta<1p6) PU>50", 2000, 0, 2000,200,-4,4); 
  JetPtResponse_Eta1p6to2_PU50to70 = book<TH2F>("JetPtResponse_Eta1p6to2_PU50to70", "Jet p_{T} scale (1p6<eta<2) PU>50", 2000, 0, 2000,200,-4,4);
  JetPtResponse_Eta2to2p5_PU50to70 = book<TH2F>("JetPtResponse_Eta2to2p5_PU50to70", "Jet p_{T} scale (2<eta<2p5) PU>50", 2000, 0, 2000,200,-4,4); 
  JetPtResponse_Eta2p5to3_PU50to70 = book<TH2F>("JetPtResponse_Eta2p5to3_PU50to70", "Jet p_{T} scale (2p5<eta<3) PU>50", 2000, 0, 2000,200,-4,4); 
  JetPtResponse_Eta3to10_PU50to70 = book<TH2F>("JetPtResponse_Eta3to10_PU50to70", "Jet p_{T} scale (3<eta<10) PU>50", 2000, 0, 2000,200,-4,4); 

  ////////////////////////////////////////////////////////////////       Jet PT Response PU 40 - 70       /////////////////////////////////////////////////////////
/*  JetPtResponse_Eta0to1p3_PU40to70 = book<TH2F>("JetPtResponse_Eta0to1p3_PU40to70", "Jet p_{T} scale (0<eta<1p3) PU>40", 2000, 0, 2000,200,-4,4); 
  JetPtResponse_Eta1p3to1p6_PU40to70 = book<TH2F>("JetPtResponse_Eta1p3to1p6_PU40to70", "Jet p_{T} scale (1p3<eta<1p6) PU>40", 2000, 0, 2000,200,-4,4); 
  JetPtResponse_Eta1p6to2_PU40to70 = book<TH2F>("JetPtResponse_Eta1p6to2_PU40to70", "Jet p_{T} scale (1p6<eta<2) PU>40", 2000, 0, 2000,200,-4,4); 
  JetPtResponse_Eta2to2p5_PU40to70 = book<TH2F>("JetPtResponse_Eta2to2p5_PU40to70", "Jet p_{T} scale (2<eta<2p5) PU>40", 2000, 0, 2000,200,-4,4); 
  JetPtResponse_Eta2p5to3_PU40to70 = book<TH2F>("JetPtResponse_Eta2p5to3_PU40to70", "Jet p_{T} scale (2p5<eta<3) PU>40", 2000, 0, 2000,200,-4,4); 
  JetPtResponse_Eta3to10_PU40to70 = book<TH2F>("JetPtResponse_Eta3to10_PU40to70", "Jet p_{T} scale (3<eta<10) PU>40", 2000, 0, 2000,200,-4,4); 
*/
  ////////////////////////////////////////////////////////////////       Jet PT Response PU 40 - 50       /////////////////////////////////////////////////////////
  JetPtResponse_Eta0to1p3_PU40to50 = book<TH2F>("JetPtResponse_Eta0to1p3_PU40to50", "Jet p_{T} scale (0<eta<1p3) PU40-50", 2000, 0, 2000,200,-4,4); 
  JetPtResponse_Eta1p3to1p6_PU40to50 = book<TH2F>("JetPtResponse_Eta1p3to1p6_PU40to50", "Jet p_{T} scale (1p3<eta<1p6) PU40-50", 2000, 0, 2000,200,-4,4); 
  JetPtResponse_Eta1p6to2_PU40to50 = book<TH2F>("JetPtResponse_Eta1p6to2_PU40to50", "Jet p_{T} scale (1p6<eta<2) PU40-50", 2000, 0, 2000,200,-4,4); 
  JetPtResponse_Eta2to2p5_PU40to50 = book<TH2F>("JetPtResponse_Eta2to2p5_PU40to50", "Jet p_{T} scale (2<eta<2p5) PU40-50", 2000, 0, 2000,200,-4,4); 
  JetPtResponse_Eta2p5to3_PU40to50 = book<TH2F>("JetPtResponse_Eta2p5to3_PU40to50", "Jet p_{T} scale (2p5<eta<3) PU40-50", 2000, 0, 2000,200,-4,4); 
  JetPtResponse_Eta3to10_PU40to50= book<TH2F>("JetPtResponse_Eta3to10_PU40to50", "Jet p_{T} scale (3<eta<10) PU40-50", 2000, 0, 2000,200,-4,4); 

  ///// pT binnend jet response as function of the neutral multiplicity
  JetPtResponse_Eta3to5_pt17to20_vsNeutralMultiplicity = book<TH2F>("JetPtResponse_Eta3to5_pt17to20_vsNeutralMultiplicity", "Jet p_{T} response (3.0<=eta<5, 17<pt<20 GeV)", 30,0,30,100,0,4); 
  JetPtResponse_Eta0to2p5_pt17to20_vsNeutralMultiplicity = book<TH2F>("JetPtResponse_Eta0to2p5_pt17to20_vsNeutralMultiplicity", "Jet p_{T} response (0<=eta<2.5, 17<pt<20 GeV)", 30,0,30,100,0,4); 

  JetPtResponse_Eta3to5_pt15to17_vsNeutralMultiplicity = book<TH2F>("JetPtResponse_Eta3to5_pt15to17_vsNeutralMultiplicity", "Jet p_{T} response (3.0<=eta<5, 15<pt<17 GeV)", 30,0,30,100,0,4); 
  JetPtResponse_Eta0to2p5_pt15to17_vsNeutralMultiplicity = book<TH2F>("JetPtResponse_Eta0to2p5_pt15to17_vsNeutralMultiplicity", "Jet p_{T} response (0<=eta<2.5, 15<pt<17 GeV)", 30,0,30,100,0,4); 

  JetPtResponse_Eta3to5_pt20to23_vsNeutralMultiplicity = book<TH2F>("JetPtResponse_Eta3to5_pt20to23_vsNeutralMultiplicity", "Jet p_{T} response (3.0<=eta<5, 20<pt<23 GeV)", 30,0,30,100,0,4); 
  JetPtResponse_Eta0to2p5_pt20to23_vsNeutralMultiplicity = book<TH2F>("JetPtResponse_Eta0to2p5_pt20to23_vsNeutralMultiplicity", "Jet p_{T} response (0<=eta<2.5, 20<pt<23 GeV)", 30,0,30,100,0,4); 

  JetPtResponse_Eta3to5_pt27to30_vsNeutralMultiplicity = book<TH2F>("JetPtResponse_Eta3to5_pt27to30_vsNeutralMultiplicity", "Jet p_{T} response (3.0<=eta<5, 27<pt<230 GeV)", 30,0,30,100,0,4); 
  JetPtResponse_Eta0to2p5_pt27to30_vsNeutralMultiplicity = book<TH2F>("JetPtResponse_Eta0to2p5_pt27to30_vsNeutralMultiplicity", "Jet p_{T} response (0<=eta<2.5, 27<pt<30 GeV)", 30,0,30,100,0,4); 

  //for two different PU bins 10-20 and 50-60
  JetPtResponse_Eta3to5_pt17to20_vsNeutralMultiplicity_mu_10to20 = book<TH2F>("JetPtResponse_Eta3to5_pt17to20_vsNeutralMultiplicity_mu_10to20", "Jet p_{T} response (3.0<=eta<5, 17<pt<20 GeV)", 30,0,30,100,0,4); 
  JetPtResponse_Eta0to2p5_pt17to20_vsNeutralMultiplicity_mu_10to20 = book<TH2F>("JetPtResponse_Eta0to2p5_pt17to20_vsNeutralMultiplicity_mu_10to20", "Jet p_{T} response (0<=eta<2.5, 17<pt<20 GeV)", 30,0,30,100,0,4); 

  JetPtResponse_Eta3to5_pt15to17_vsNeutralMultiplicity_mu_10to20 = book<TH2F>("JetPtResponse_Eta3to5_pt15to17_vsNeutralMultiplicity_mu_10to20", "Jet p_{T} response (3.0<=eta<5, 15<pt<17 GeV)", 30,0,30,100,0,4); 
  JetPtResponse_Eta0to2p5_pt15to17_vsNeutralMultiplicity_mu_10to20 = book<TH2F>("JetPtResponse_Eta0to2p5_pt15to17_vsNeutralMultiplicity_mu_10to20", "Jet p_{T} response (0<=eta<2.5, 15<pt<17 GeV)", 30,0,30,100,0,4); 

  JetPtResponse_Eta3to5_pt20to23_vsNeutralMultiplicity_mu_10to20 = book<TH2F>("JetPtResponse_Eta3to5_pt20to23_vsNeutralMultiplicity_mu_10to20", "Jet p_{T} response (3.0<=eta<5, 20<pt<23 GeV)", 30,0,30,100,0,4); 
  JetPtResponse_Eta0to2p5_pt20to23_vsNeutralMultiplicity_mu_10to20 = book<TH2F>("JetPtResponse_Eta0to2p5_pt20to23_vsNeutralMultiplicity_mu_10to20", "Jet p_{T} response (0<=eta<2.5, 20<pt<23 GeV)", 30,0,30,100,0,4); 

  JetPtResponse_Eta3to5_pt27to30_vsNeutralMultiplicity_mu_10to20 = book<TH2F>("JetPtResponse_Eta3to5_pt27to30_vsNeutralMultiplicity_mu_10to20", "Jet p_{T} response (3.0<=eta<5, 27<pt<230 GeV)", 30,0,30,100,0,4); 
  JetPtResponse_Eta0to2p5_pt27to30_vsNeutralMultiplicity_mu_10to20 = book<TH2F>("JetPtResponse_Eta0to2p5_pt27to30_vsNeutralMultiplicity_mu_10to20", "Jet p_{T} response (0<=eta<2.5, 27<pt<30 GeV)", 30,0,30,100,0,4); 


  JetPtResponse_Eta3to5_pt17to20_vsNeutralMultiplicity_mu_50to60 = book<TH2F>("JetPtResponse_Eta3to5_pt17to20_vsNeutralMultiplicity_mu_50to60", "Jet p_{T} response (3.0<=eta<5, 17<pt<20 GeV)", 30,0,30,100,0,4); 
  JetPtResponse_Eta0to2p5_pt17to20_vsNeutralMultiplicity_mu_50to60 = book<TH2F>("JetPtResponse_Eta0to2p5_pt17to20_vsNeutralMultiplicity_mu_50to60", "Jet p_{T} response (0<=eta<2.5, 17<pt<20 GeV)", 30,0,30,100,0,4); 

  JetPtResponse_Eta3to5_pt15to17_vsNeutralMultiplicity_mu_50to60 = book<TH2F>("JetPtResponse_Eta3to5_pt15to17_vsNeutralMultiplicity_mu_50to60", "Jet p_{T} response (3.0<=eta<5, 15<pt<17 GeV)", 30,0,30,100,0,4); 
  JetPtResponse_Eta0to2p5_pt15to17_vsNeutralMultiplicity_mu_50to60 = book<TH2F>("JetPtResponse_Eta0to2p5_pt15to17_vsNeutralMultiplicity_mu_50to60", "Jet p_{T} response (0<=eta<2.5, 15<pt<17 GeV)", 30,0,30,100,0,4); 

  JetPtResponse_Eta3to5_pt20to23_vsNeutralMultiplicity_mu_50to60 = book<TH2F>("JetPtResponse_Eta3to5_pt20to23_vsNeutralMultiplicity_mu_50to60", "Jet p_{T} response (3.0<=eta<5, 20<pt<23 GeV)", 30,0,30,100,0,4); 
  JetPtResponse_Eta0to2p5_pt20to23_vsNeutralMultiplicity_mu_50to60 = book<TH2F>("JetPtResponse_Eta0to2p5_pt20to23_vsNeutralMultiplicity_mu_50to60", "Jet p_{T} response (0<=eta<2.5, 20<pt<23 GeV)", 30,0,30,100,0,4); 

  JetPtResponse_Eta3to5_pt27to30_vsNeutralMultiplicity_mu_50to60 = book<TH2F>("JetPtResponse_Eta3to5_pt27to30_vsNeutralMultiplicity_mu_50to60", "Jet p_{T} response (3.0<=eta<5, 27<pt<230 GeV)", 30,0,30,100,0,4); 
  JetPtResponse_Eta0to2p5_pt27to30_vsNeutralMultiplicity_mu_50to60 = book<TH2F>("JetPtResponse_Eta0to2p5_pt27to30_vsNeutralMultiplicity_mu_50to60", "Jet p_{T} response (0<=eta<2.5, 27<pt<30 GeV)", 30,0,30,100,0,4); 

  // pT binnend jet response as function of mu
  JetPtResponse_Eta3to5_pt17to20_vsmu = book<TH2F>("JetPtResponse_Eta3to5_pt17to20_vsmu", "Jet p_{T} response (3.0<=eta<5, 17<pt<20 GeV)",100,0,4, 70,0,70); 
  JetPtResponse_Eta0to2p5_pt17to20_vsmu = book<TH2F>("JetPtResponse_Eta0to2p5_pt17to20_vsmu", "Jet p_{T} response (0<=eta<2.5, 17<pt<20 GeV)",100,0,4, 70,0,70); 

  JetPtResponse_Eta3to5_pt15to17_vsmu = book<TH2F>("JetPtResponse_Eta3to5_pt15to17_vsmu", "Jet p_{T} response (3.0<=eta<5, 15<pt<17 GeV)",100,0,4, 70,0,70); 
  JetPtResponse_Eta0to2p5_pt15to17_vsmu = book<TH2F>("JetPtResponse_Eta0to2p5_pt15to17_vsmu", "Jet p_{T} response (0<=eta<2.5, 15<pt<17 GeV)",100,0,4, 70,0,70); 

  JetPtResponse_Eta3to5_pt20to23_vsmu = book<TH2F>("JetPtResponse_Eta3to5_pt20to23_vsmu", "Jet p_{T} response (3.0<=eta<5, 20<pt<23 GeV)",100,0,4, 70,0,70); 
  JetPtResponse_Eta0to2p5_pt20to23_vsmu = book<TH2F>("JetPtResponse_Eta0to2p5_pt20to23_vsmu", "Jet p_{T} response (0<=eta<2.5, 20<pt<23 GeV)",100,0,4, 70,0,70); 

  JetPtResponse_Eta3to5_pt27to30_vsmu = book<TH2F>("JetPtResponse_Eta3to5_pt27to30_vsmu", "Jet p_{T} response (3.0<=eta<5, 27<pt<30 GeV)",100,0,4, 70,0,70); 
  JetPtResponse_Eta0to2p5_pt27to30_vsmu = book<TH2F>("JetPtResponse_Eta0to2p5_pt27to30_vsmu", "Jet p_{T} response (0<=eta<2.5, 27<pt<30 GeV)",100,0,4, 70,0,70); 

  JetPtResponse_Eta2p5to3p0_pt17to20_vsmu = book<TH2F>("JetPtResponse_Eta2p5to3p0_pt17to20_vsmu", "Jet p_{T} response (0<=eta<2.5, 17<pt<20 GeV)",100,0,4, 70,0,70); 
  JetPtResponse_Eta2p5to3p0_pt15to17_vsmu = book<TH2F>("JetPtResponse_Eta2p5to3p0_pt15to17_vsmu", "Jet p_{T} response (0<=eta<2.5, 15<pt<17 GeV)",100,0,4, 70,0,70); 
  JetPtResponse_Eta2p5to3p0_pt20to23_vsmu = book<TH2F>("JetPtResponse_Eta2p5to3p0_pt20to23_vsmu", "Jet p_{T} response (0<=eta<2.5, 20<pt<23 GeV)",100,0,4, 70,0,70); 
  JetPtResponse_Eta2p5to3p0_pt27to30_vsmu = book<TH2F>("JetPtResponse_Eta2p5to3p0_pt27to30_vsmu", "Jet p_{T} response (0<=eta<2.5, 27<pt<30 GeV)",100,0,4, 70,0,70); 

  // Neutral Multiplicity as function of mu (used to compare to Eirinis results)
  NeutralMultiplicity_vs_mu_Eta3to5_pt20to23  = book<TH2F>("NeutralMultiplicity_vs_mu_Eta3to5_pt20to23", "Neutral Multiplicity vs Mu (3.0<=eta<5, 17<pt<20 GeV)", 30,0,30,70,0,70); 
  NeutralMultiplicity_vs_mu_unmatched_Eta3to5_pt20to23  = book<TH2F>("NeutralMultiplicity_vs_mu_unmatched_Eta3to5_pt20to23", "Neutral Multiplicity vs Mu unmatched (3.0<=eta<5, 17<pt<20 GeV)", 30,0,30,70,0,70); 
  NeutralMultiplicity_vs_mu_matched_Eta3to5_pt20to23  = book<TH2F>("NeutralMultiplicity_vs_mu_matched_Eta3to5_pt20to23", "Neutral Multiplicity vs Mu matched (3.0<=eta<5, 17<pt<20 GeV)", 30,0,30,70,0,70); 

  NeutralMultiplicity_vs_mu_Eta0to2p5_pt20to23  = book<TH2F>("NeutralMultiplicity_vs_mu_Eta0to2p5_pt20to23", "Neutral Multiplicity vs Mu (3.0<=eta<5, 17<pt<20 GeV)", 30,0,30,70,0,70); 
  NeutralMultiplicity_vs_mu_unmatched_Eta0to2p5_pt20to23  = book<TH2F>("NeutralMultiplicity_vs_mu_unmatched_Eta0to2p5_pt20to23", "Neutral Multiplicity vs Mu unmatched (3.0<=eta<5, 17<pt<20 GeV)", 30,0,30,70,0,70); 
  NeutralMultiplicity_vs_mu_matched_Eta0to2p5_pt20to23  = book<TH2F>("NeutralMultiplicity_vs_mu_matched_Eta0to2p5_pt20to23", "Neutral Multiplicity vs Mu matched (3.0<=eta<5, 17<pt<20 GeV)", 30,0,30,70,0,70); 

  NeutralMultiplicity_vs_mu_Eta3to5_pt27to30  = book<TH2F>("NeutralMultiplicity_vs_mu_Eta3to5_pt27to30", "Neutral Multiplicity vs Mu (3.0<=eta<5, 17<pt<20 GeV)", 30,0,30,70,0,70); 
  NeutralMultiplicity_vs_mu_unmatched_Eta3to5_pt27to30  = book<TH2F>("NeutralMultiplicity_vs_mu_unmatched_Eta3to5_pt27to30", "Neutral Multiplicity vs Mu unmatched (3.0<=eta<5, 17<pt<20 GeV)", 30,0,30,70,0,70); 
  NeutralMultiplicity_vs_mu_matched_Eta3to5_pt27to30  = book<TH2F>("NeutralMultiplicity_vs_mu_matched_Eta3to5_pt27to30", "Neutral Multiplicity vs Mu matched (3.0<=eta<5, 17<pt<20 GeV)", 30,0,30,70,0,70); 

  NeutralMultiplicity_vs_mu_Eta0to2p5_pt27to30  = book<TH2F>("NeutralMultiplicity_vs_mu_Eta0to2p5_pt27to30", "Neutral Multiplicity vs Mu (3.0<=eta<5, 17<pt<20 GeV)", 30,0,30,70,0,70); 
  NeutralMultiplicity_vs_mu_unmatched_Eta0to2p5_pt27to30  = book<TH2F>("NeutralMultiplicity_vs_mu_unmatched_Eta0to2p5_pt27to30", "Neutral Multiplicity vs Mu unmatched (3.0<=eta<5, 17<pt<20 GeV)", 30,0,30,70,0,70); 
  NeutralMultiplicity_vs_mu_matched_Eta0to2p5_pt27to30  = book<TH2F>("NeutralMultiplicity_vs_mu_matched_Eta0to2p5_pt27to30", "Neutral Multiplicity vs Mu matched (3.0<=eta<5, 17<pt<20 GeV)", 30,0,30,70,0,70); 


  NeutralMultiplicity_vs_mu_Eta3to5_pt17to20  = book<TH2F>("NeutralMultiplicity_vs_mu_Eta3to5_pt17to20", "Neutral Multiplicity vs Mu (3.0<=eta<5, 17<pt<20 GeV)", 30,0,30,70,0,70); 
  NeutralMultiplicity_vs_mu_unmatched_Eta3to5_pt17to20  = book<TH2F>("NeutralMultiplicity_vs_mu_unmatched_Eta3to5_pt17to20", "Neutral Multiplicity vs Mu unmatched (3.0<=eta<5, 17<pt<20 GeV)", 30,0,30,70,0,70); 
  NeutralMultiplicity_vs_mu_matched_Eta3to5_pt17to20  = book<TH2F>("NeutralMultiplicity_vs_mu_matched_Eta3to5_pt17to20", "Neutral Multiplicity vs Mu matched (3.0<=eta<5, 17<pt<20 GeV)", 30,0,30,70,0,70); 

  NeutralMultiplicity_vs_mu_Eta0to2p5_pt17to20  = book<TH2F>("NeutralMultiplicity_vs_mu_Eta0to2p5_pt17to20", "Neutral Multiplicity vs Mu (3.0<=eta<5, 17<pt<20 GeV)", 30,0,30,70,0,70); 
  NeutralMultiplicity_vs_mu_unmatched_Eta0to2p5_pt17to20  = book<TH2F>("NeutralMultiplicity_vs_mu_unmatched_Eta0to2p5_pt17to20", "Neutral Multiplicity vs Mu unmatched (3.0<=eta<5, 17<pt<20 GeV)", 30,0,30,70,0,70); 
  NeutralMultiplicity_vs_mu_matched_Eta0to2p5_pt17to20  = book<TH2F>("NeutralMultiplicity_vs_mu_matched_Eta0to2p5_pt17to20", "Neutral Multiplicity vs Mu matched (3.0<=eta<5, 17<pt<20 GeV)", 30,0,30,70,0,70); 

  NeutralMultiplicity_vs_mu_Eta3to5_pt15to17  = book<TH2F>("NeutralMultiplicity_vs_mu_Eta3to5_pt15to17", "Neutral Multiplicity vs Mu (3.0<=eta<5, 17<pt<20 GeV)", 30,0,30,70,0,70); 
  NeutralMultiplicity_vs_mu_unmatched_Eta3to5_pt15to17  = book<TH2F>("NeutralMultiplicity_vs_mu_unmatched_Eta3to5_pt15to17", "Neutral Multiplicity vs Mu unmatched (3.0<=eta<5, 17<pt<20 GeV)", 30,0,30,70,0,70); 
  NeutralMultiplicity_vs_mu_matched_Eta3to5_pt15to17  = book<TH2F>("NeutralMultiplicity_vs_mu_matched_Eta3to5_pt15to17", "Neutral Multiplicity vs Mu matched (3.0<=eta<5, 17<pt<20 GeV)", 30,0,30,70,0,70); 

  NeutralMultiplicity_vs_mu_Eta0to2p5_pt15to17  = book<TH2F>("NeutralMultiplicity_vs_mu_Eta0to2p5_pt15to17", "Neutral Multiplicity vs Mu (3.0<=eta<5, 17<pt<20 GeV)", 30,0,30,70,0,70); 
  NeutralMultiplicity_vs_mu_unmatched_Eta0to2p5_pt15to17  = book<TH2F>("NeutralMultiplicity_vs_mu_unmatched_Eta0to2p5_pt15to17", "Neutral Multiplicity vs Mu unmatched (3.0<=eta<5, 17<pt<20 GeV)", 30,0,30,70,0,70); 
  NeutralMultiplicity_vs_mu_matched_Eta0to2p5_pt15to17  = book<TH2F>("NeutralMultiplicity_vs_mu_matched_Eta0to2p5_pt15to17", "Neutral Multiplicity vs Mu matched (3.0<=eta<5, 17<pt<20 GeV)", 30,0,30,70,0,70); 

  /// same histograms but with PUPPIMultiplicity (non integer)
  NeutralPuppiMultiplicity_vs_mu_Eta3to5_pt20to23  = book<TH2F>("NeutralPuppiMultiplicity_vs_mu_Eta3to5_pt20to23", "Neutral PUPPI  Multiplicity vs Mu (3.0<=eta<5, 17<pt<20 GeV)", 300,0,30,70,0,70); 
  NeutralPuppiMultiplicity_vs_mu_unmatched_Eta3to5_pt20to23  = book<TH2F>("NeutralPuppiMultiplicity_vs_mu_unmatched_Eta3to5_pt20to23", "Neutral PUPPI Multiplicity vs Mu unmatched (3.0<=eta<5, 17<pt<20 GeV)", 300,0,30,70,0,70); 
  NeutralPuppiMultiplicity_vs_mu_matched_Eta3to5_pt20to23  = book<TH2F>("NeutralPuppiMultiplicity_vs_mu_matched_Eta3to5_pt20to23", "Neutral PUPPI Multiplicity vs Mu matched (3.0<=eta<5, 17<pt<20 GeV)", 300,0,30,70,0,70); 

  NeutralPuppiMultiplicity_vs_mu_Eta0to2p5_pt20to23  = book<TH2F>("NeutralPuppiMultiplicity_vs_mu_Eta0to2p5_pt20to23", "Neutral PUPPI Multiplicity vs Mu (3.0<=eta<5, 17<pt<20 GeV)", 300,0,30,70,0,70); 
  NeutralPuppiMultiplicity_vs_mu_unmatched_Eta0to2p5_pt20to23  = book<TH2F>("NeutralPuppiMultiplicity_vs_mu_unmatched_Eta0to2p5_pt20to23", "Neutral PUPPI Multiplicity vs Mu unmatched (3.0<=eta<5, 17<pt<20 GeV)", 300,0,30,70,0,70); 
  NeutralPuppiMultiplicity_vs_mu_matched_Eta0to2p5_pt20to23  = book<TH2F>("NeutralPuppiMultiplicity_vs_mu_matched_Eta0to2p5_pt20to23", "Neutral PUPPI Multiplicity vs Mu matched (3.0<=eta<5, 17<pt<20 GeV)", 300,0,30,70,0,70); 

  NeutralPuppiMultiplicity_vs_mu_Eta3to5_pt27to30  = book<TH2F>("NeutralPuppiMultiplicity_vs_mu_Eta3to5_pt27to30", "Neutral PUPPI Multiplicity vs Mu (3.0<=eta<5, 17<pt<20 GeV)", 300,0,30,70,0,70); 
  NeutralPuppiMultiplicity_vs_mu_unmatched_Eta3to5_pt27to30  = book<TH2F>("NeutralPuppiMultiplicity_vs_mu_unmatched_Eta3to5_pt27to30", "Neutral PUPPI Multiplicity vs Mu unmatched (3.0<=eta<5, 17<pt<20 GeV)", 300,0,30,70,0,70); 
  NeutralPuppiMultiplicity_vs_mu_matched_Eta3to5_pt27to30  = book<TH2F>("NeutralPuppiMultiplicity_vs_mu_matched_Eta3to5_pt27to30", "Neutral PUPPI Multiplicity vs Mu matched (3.0<=eta<5, 17<pt<20 GeV)", 300,0,30,70,0,70); 

  NeutralPuppiMultiplicity_vs_mu_Eta0to2p5_pt27to30  = book<TH2F>("NeutralPuppiMultiplicity_vs_mu_Eta0to2p5_pt27to30", "Neutral PUPPI Multiplicity vs Mu (3.0<=eta<5, 17<pt<20 GeV)", 300,0,30,70,0,70); 
  NeutralPuppiMultiplicity_vs_mu_unmatched_Eta0to2p5_pt27to30  = book<TH2F>("NeutralPuppiMultiplicity_vs_mu_unmatched_Eta0to2p5_pt27to30", "Neutral PUPPI Multiplicity vs Mu unmatched (3.0<=eta<5, 17<pt<20 GeV)", 300,0,30,70,0,70); 
  NeutralPuppiMultiplicity_vs_mu_matched_Eta0to2p5_pt27to30  = book<TH2F>("NeutralPuppiMultiplicity_vs_mu_matched_Eta0to2p5_pt27to30", "Neutral PUPPI Multiplicity vs Mu matched (3.0<=eta<5, 17<pt<20 GeV)", 300,0,30,70,0,70); 


  NeutralPuppiMultiplicity_vs_mu_Eta3to5_pt17to20  = book<TH2F>("NeutralPuppiMultiplicity_vs_mu_Eta3to5_pt17to20", "Neutral PUPPI Multiplicity vs Mu (3.0<=eta<5, 17<pt<20 GeV)", 300,0,30,70,0,70); 
  NeutralPuppiMultiplicity_vs_mu_unmatched_Eta3to5_pt17to20  = book<TH2F>("NeutralPuppiMultiplicity_vs_mu_unmatched_Eta3to5_pt17to20", "Neutral PUPPI Multiplicity vs Mu unmatched (3.0<=eta<5, 17<pt<20 GeV)", 300,0,30,70,0,70); 
  NeutralPuppiMultiplicity_vs_mu_matched_Eta3to5_pt17to20  = book<TH2F>("NeutralPuppiMultiplicity_vs_mu_matched_Eta3to5_pt17to20", "Neutral PUPPI Multiplicity vs Mu matched (3.0<=eta<5, 17<pt<20 GeV)", 300,0,30,70,0,70); 

  NeutralPuppiMultiplicity_vs_mu_Eta0to2p5_pt17to20  = book<TH2F>("NeutralPuppiMultiplicity_vs_mu_Eta0to2p5_pt17to20", "Neutral PUPPI Multiplicity vs Mu (3.0<=eta<5, 17<pt<20 GeV)", 300,0,30,70,0,70); 
  NeutralPuppiMultiplicity_vs_mu_unmatched_Eta0to2p5_pt17to20  = book<TH2F>("NeutralPuppiMultiplicity_vs_mu_unmatched_Eta0to2p5_pt17to20", "Neutral PUPPI Multiplicity vs Mu unmatched (3.0<=eta<5, 17<pt<20 GeV)", 300,0,30,70,0,70); 
  NeutralPuppiMultiplicity_vs_mu_matched_Eta0to2p5_pt17to20  = book<TH2F>("NeutralPuppiMultiplicity_vs_mu_matched_Eta0to2p5_pt17to20", "Neutral PUPPI Multiplicity vs Mu matched (3.0<=eta<5, 17<pt<20 GeV)", 300,0,30,70,0,70); 

  NeutralPuppiMultiplicity_vs_mu_Eta3to5_pt15to17  = book<TH2F>("NeutralPuppiMultiplicity_vs_mu_Eta3to5_pt15to17", "Neutral PUPPI Multiplicity vs Mu (3.0<=eta<5, 17<pt<20 GeV)", 300,0,30,70,0,70); 
  NeutralPuppiMultiplicity_vs_mu_unmatched_Eta3to5_pt15to17  = book<TH2F>("NeutralPuppiMultiplicity_vs_mu_unmatched_Eta3to5_pt15to17", "Neutral PUPPI Multiplicity vs Mu unmatched (3.0<=eta<5, 17<pt<20 GeV)", 300,0,30,70,0,70); 
  NeutralPuppiMultiplicity_vs_mu_matched_Eta3to5_pt15to17  = book<TH2F>("NeutralPuppiMultiplicity_vs_mu_matched_Eta3to5_pt15to17", "Neutral PUPPI Multiplicity vs Mu matched (3.0<=eta<5, 17<pt<20 GeV)", 300,0,30,70,0,70); 

  NeutralPuppiMultiplicity_vs_mu_Eta0to2p5_pt15to17  = book<TH2F>("NeutralPuppiMultiplicity_vs_mu_Eta0to2p5_pt15to17", "Neutral PUPPI Multiplicity vs Mu (3.0<=eta<5, 17<pt<20 GeV)", 300,0,30,70,0,70); 
  NeutralPuppiMultiplicity_vs_mu_unmatched_Eta0to2p5_pt15to17  = book<TH2F>("NeutralPuppiMultiplicity_vs_mu_unmatched_Eta0to2p5_pt15to17", "Neutral PUPPI Multiplicity vs Mu unmatched (3.0<=eta<5, 17<pt<20 GeV)", 300,0,30,70,0,70); 
  NeutralPuppiMultiplicity_vs_mu_matched_Eta0to2p5_pt15to17  = book<TH2F>("NeutralPuppiMultiplicity_vs_mu_matched_Eta0to2p5_pt15to17", "Neutral PUPPI Multiplicity vs Mu matched (3.0<=eta<5, 17<pt<20 GeV)", 300,0,30,70,0,70); 

  /////////////////////////////////////////////////////////////     General Hists             //////////////////////////////////////////////////////////
  JetRecoPT= book <TH1F>("JetRecoPT", "Jet Reco PT",500,0,500);
  JetGenPT= book <TH1F>("JetGenPT", "Jet Gen PT",500,0,500);
  JetRecoETA= book <TH1F>("JetRecoETA", "Jet Reco ETA",100,-5,5);
  JetGenETA= book <TH1F>("JetGenETA", "Jet Gen ETA",100,-5,5);

  JetRecoPT_0to1p3= book <TH1F>("JetRecoPT_0to1p3", "Jet Reco PT",500,0,500);
  JetGenPT_0to1p3= book <TH1F>("JetGenPT_0to1p3", "Jet Gen PT",500,0,500);
  JetRecoETA_0to1p3= book <TH1F>("JetRecoETA_0to1p3", "Jet Reco ETA",100,-5,5);
  JetGenETA_0to1p3= book <TH1F>("JetGenETA_0to1p3", "Jet Gen ETA",100,-5,5);

  JetRecoPT_1p3to2p5= book <TH1F>("JetRecoPT_1p3to2p5", "Jet Reco PT",500,0,500);
  JetGenPT_1p3to2p5= book <TH1F>("JetGenPT_1p3to2p5", "Jet Gen PT",500,0,500);
  JetRecoETA_1p3to2p5= book <TH1F>("JetRecoETA_1p3to2p5", "Jet Reco ETA",100,-5,5);
  JetGenETA_1p3to2p5= book <TH1F>("JetGenETA_1p3to2p5", "Jet Gen ETA",100,-5,5);

  JetRecoPT_2p5to3= book <TH1F>("JetRecoPT_2p5to3", "Jet Reco PT",500,0,500);
  JetGenPT_2p5to3= book <TH1F>("JetGenPT_2p5to3", "Jet Gen PT",500,0,500);
  JetRecoETA_2p5to3= book <TH1F>("JetRecoETA_2p5to3", "Jet Reco ETA",100,-5,5);
  JetGenETA_2p5to3= book <TH1F>("JetGenETA_2p5to3", "Jet Gen ETA",100,-5,5);

  JetRecoPT_3to5= book <TH1F>("JetRecoPT_3to5", "Jet Reco PT",500,0,500);
  JetGenPT_3to5= book <TH1F>("JetGenPT_3to5", "Jet Gen PT",500,0,500);
  JetRecoETA_3to5= book <TH1F>("JetRecoETA_3to5", "Jet Reco ETA",100,-5,5);
  JetGenETA_3to5= book <TH1F>("JetGenETA_3to5", "Jet Gen ETA",100,-5,5);

}


void PuppiJetHists::fill(const Event & event){
  if(berror) std::cout<<"in PuppiJetHists  "<<std::endl;
  assert(event.met);
  assert(event.pvs);
  assert(event.jets);

  double weight = event.weight;
  if(berror) std::cout<<"weight  "<<weight<<std::endl;

  if(berror)std::cout<<"Number of PV  "<<event.pvs->size()<<std::endl;
  if(berror)std::cout<<"Number of Jets  "<<event.jets->size()<<std::endl;
  if(berror)std::cout<<"Number of GenJets  "<<event.genjets->size()<<std::endl;

  vector<Jet> jets = collection.empty() ?  *event.jets : event.get(h_myjets);
  vector<GenJet> genparticles = collection.empty() ?  *event.genjets : event.get(h_mygenjets);

  double npvs = event.pvs->size();
  double trueinteraction = event.genInfo->pileup_TrueNumInteractions();



  ////////////////////////////////////////   Jet pt scale    /////////////////////////////////////////
  double genp_pt=0;

  //for each jet in the event calculate eta and do matching to GenParticles
  for(unsigned int i=0;i<jets.size();i++){

    double jet_eta = abs(jets.at(i).eta());
    double jet_pt = jets.at(i).pt();
    double jet_mass = jets.at(i).v4().M();
    double jet_number = jets.size();

    JetEta ->Fill(jets.at(i).eta(), weight); 

    if(0<= jet_eta && jet_eta <=1.3){
    JetPt_Eta0to1p3 ->Fill(jet_pt, weight); 
    JetMass_Eta0to1p3 ->Fill(jet_mass, weight); 
    JetNumber_Eta0to1p3 ->Fill(jet_number, weight);

    }else if(1.3<= jet_eta && jet_eta <=1.6){
    JetPt_Eta1p3to1p6 ->Fill(jet_pt, weight); 
    JetMass_Eta1p3to1p6 ->Fill(jet_mass, weight); 
    JetNumber_Eta1p3to1p6 ->Fill(jet_number, weight);

    }else if(1.6<= jet_eta && jet_eta <=2.0){
    JetPt_Eta1p6to2 ->Fill(jet_pt, weight); 
    JetMass_Eta1p6to2 ->Fill(jet_mass, weight); 
    JetNumber_Eta1p6to2 ->Fill(jet_number, weight);

    }else if(2.0<= jet_eta && jet_eta <=2.5){
    JetPt_Eta2to2p5 ->Fill(jet_pt, weight); 
    JetMass_Eta2to2p5 ->Fill(jet_mass, weight); 
    JetNumber_Eta2to2p5 ->Fill(jet_number, weight);

    }else if(2.5<= jet_eta && jet_eta <=3.0){
    JetPt_Eta2p5to3 ->Fill(jet_pt, weight); 
    JetMass_Eta2p5to3 ->Fill(jet_mass, weight); 
    JetNumber_Eta2p5to3 ->Fill(jet_number, weight);

    }else if(3.0<= jet_eta){
    JetPt_Eta3to10 ->Fill(jet_pt, weight); 
    JetMass_Eta3to10 ->Fill(jet_mass, weight); 
    JetNumber_Eta3to10 ->Fill(jet_number, weight);
    }

    genp_pt=-1;
    //do matching to calculate jet pt scale
    for(unsigned int j=0;j<genparticles.size();j++){
      double deltaR_gen_jet = deltaR(genparticles.at(j),jets.at(i));
      if(berror)   std::cout<<"Delta R Genparticle Jet "<<j<<"  "<<deltaR_gen_jet<<std::endl;
      if(deltaR_gen_jet<0.4){
	genp_pt = genparticles.at(j).pt();
      }
    }//for-loop over all genparticles
    
    if(berror)  std::cout<<"Pt Genparticle  "<< genp_pt <<std::endl;

    /// Fill Jet PT scale and response histograms
    double jet_pt_scale = (jet_pt - genp_pt)/genp_pt;
    double jet_pt_response = (jet_pt)/genp_pt;
    double neutral_multiplicity = jets.at(i).neutralMultiplicity();
    double puppi_neutral_multiplicity = jets.at(i).neutralPuppiMultiplicity();
    
    if(0<= jet_eta && jet_eta <=1.3){
      JetPtScale_Eta0to1p3->Fill(npvs,jet_pt_scale, weight);
      JetPtResponse_Eta0to1p3->Fill(genp_pt,jet_pt_response, weight);
    }else if(1.3<= jet_eta && jet_eta <=1.6){
      JetPtScale_Eta1p3to1p6->Fill(npvs,jet_pt_scale, weight);
      JetPtResponse_Eta1p3to1p6->Fill(genp_pt,jet_pt_response, weight);
    }else if(1.6<= jet_eta && jet_eta <=2.0){
      JetPtScale_Eta1p6to2->Fill(npvs,jet_pt_scale, weight);
      JetPtResponse_Eta1p6to2->Fill(genp_pt,jet_pt_response, weight);
    }else if(2.0<= jet_eta && jet_eta <=2.5){
      JetPtScale_Eta2to2p5->Fill(npvs,jet_pt_scale, weight);
      JetPtResponse_Eta2to2p5->Fill(genp_pt,jet_pt_response, weight);
    }else if(2.5<= jet_eta && jet_eta <=3.0){
      JetPtScale_Eta2p5to3->Fill(npvs,jet_pt_scale, weight);
      JetPtResponse_Eta2p5to3->Fill(genp_pt,jet_pt_response, weight);
    }else if(3.0<= jet_eta){
      JetPtScale_Eta3to10->Fill(npvs,jet_pt_scale, weight);
      JetPtResponse_Eta3to10->Fill(genp_pt,jet_pt_response, weight);
    }
    if(3.314 <= jet_eta && jet_eta < 3.489){
      if(15<= genp_pt && genp_pt < 17)  JetPtResponse_Eta3to3->Fill(jet_pt_response, weight);
    }



    //// Fill Jet PT response histograms for PU 0 - 10
    if(trueinteraction>=0 && trueinteraction<=10){
       if(0<= jet_eta && jet_eta <=1.3){
         JetPtResponse_Eta0to1p3_PU0to10->Fill(genp_pt,jet_pt_response, weight);
       }else if(1.3<= jet_eta && jet_eta <=1.6){
         JetPtResponse_Eta1p3to1p6_PU0to10->Fill(genp_pt,jet_pt_response, weight);
       }else if(1.6<= jet_eta && jet_eta <=2.0){
         JetPtResponse_Eta1p6to2_PU0to10->Fill(genp_pt,jet_pt_response, weight);
       }else if(2.0<= jet_eta && jet_eta <=2.5){
         JetPtResponse_Eta2to2p5_PU0to10->Fill(genp_pt,jet_pt_response, weight);
       }else if(2.5<= jet_eta && jet_eta <=3.0){
         JetPtResponse_Eta2p5to3_PU0to10->Fill(genp_pt,jet_pt_response, weight);
       }else if(3.0<= jet_eta){
         JetPtResponse_Eta3to10_PU0to10->Fill(genp_pt,jet_pt_response, weight);
       }
    }

    //// Fill Jet PT response histograms for PU 10 - 20
    if(trueinteraction>=10 && trueinteraction<=20){
       if(0<= jet_eta && jet_eta <=1.3){
         JetPtResponse_Eta0to1p3_PU10to20->Fill(genp_pt,jet_pt_response, weight);
       }else if(1.3<= jet_eta && jet_eta <=1.6){
         JetPtResponse_Eta1p3to1p6_PU10to20->Fill(genp_pt,jet_pt_response, weight);
       }else if(1.6<= jet_eta && jet_eta <=2.0){
         JetPtResponse_Eta1p6to2_PU10to20->Fill(genp_pt,jet_pt_response, weight);
       }else if(2.0<= jet_eta && jet_eta <=2.5){
         JetPtResponse_Eta2to2p5_PU10to20->Fill(genp_pt,jet_pt_response, weight);
       }else if(2.5<= jet_eta && jet_eta <=3.0){
         JetPtResponse_Eta2p5to3_PU10to20->Fill(genp_pt,jet_pt_response, weight);
       }else if(3.0<= jet_eta){
         JetPtResponse_Eta3to10_PU10to20->Fill(genp_pt,jet_pt_response, weight);
       }
    }

    //// Fill Jet PT response histograms for PU 30 - 40
    if(trueinteraction>=30 && trueinteraction<=40){
       if(0<= jet_eta && jet_eta <=1.3){
         JetPtResponse_Eta0to1p3_PU30to40->Fill(genp_pt,jet_pt_response, weight);
       }else if(1.3<= jet_eta && jet_eta <=1.6){
         JetPtResponse_Eta1p3to1p6_PU30to40->Fill(genp_pt,jet_pt_response, weight);
       }else if(1.6<= jet_eta && jet_eta <=2.0){
         JetPtResponse_Eta1p6to2_PU30to40->Fill(genp_pt,jet_pt_response, weight);
       }else if(2.0<= jet_eta && jet_eta <=2.5){
         JetPtResponse_Eta2to2p5_PU30to40->Fill(genp_pt,jet_pt_response, weight);
       }else if(2.5<= jet_eta && jet_eta <=3.0){
         JetPtResponse_Eta2p5to3_PU30to40->Fill(genp_pt,jet_pt_response, weight);
       }else if(3.0<= jet_eta){
         JetPtResponse_Eta3to10_PU30to40->Fill(genp_pt,jet_pt_response, weight);
       }
    }


    //// Fill Jet PT response histograms for PU 50 - 70
    if(trueinteraction>=50){
       if(0<= jet_eta && jet_eta <=1.3){
         JetPtResponse_Eta0to1p3_PU50to70->Fill(genp_pt,jet_pt_response, weight);
       }else if(1.3<= jet_eta && jet_eta <=1.6){
         JetPtResponse_Eta1p3to1p6_PU50to70->Fill(genp_pt,jet_pt_response, weight);
       }else if(1.6<= jet_eta && jet_eta <=2.0){
         JetPtResponse_Eta1p6to2_PU50to70->Fill(genp_pt,jet_pt_response, weight);
       }else if(2.0<= jet_eta && jet_eta <=2.5){
         JetPtResponse_Eta2to2p5_PU50to70->Fill(genp_pt,jet_pt_response, weight);
       }else if(2.5<= jet_eta && jet_eta <=3.0){
         JetPtResponse_Eta2p5to3_PU50to70->Fill(genp_pt,jet_pt_response, weight);
       }else if(3.0<= jet_eta){
         JetPtResponse_Eta3to10_PU50to70->Fill(genp_pt,jet_pt_response, weight);
       }
    }

/*
    //// Fill Jet PT response histograms for PU 40 - 70
    if(trueinteraction>=40){
       if(0<= jet_eta && jet_eta <=1.3){
         JetPtResponse_Eta0to1p3_PU40to70->Fill(genp_pt,jet_pt_response, weight);
       }else if(1.3<= jet_eta && jet_eta <=1.6){
         JetPtResponse_Eta1p3to1p6_PU40to70->Fill(genp_pt,jet_pt_response, weight);
       }else if(1.6<= jet_eta && jet_eta <=2.0){
         JetPtResponse_Eta1p6to2_PU40to70->Fill(genp_pt,jet_pt_response, weight);
       }else if(2.0<= jet_eta && jet_eta <=2.5){
         JetPtResponse_Eta2to2p5_PU40to70->Fill(genp_pt,jet_pt_response, weight);
       }else if(2.5<= jet_eta && jet_eta <=3.0){
         JetPtResponse_Eta2p5to3_PU40to70->Fill(genp_pt,jet_pt_response, weight);
       }else if(3.0<= jet_eta){
         JetPtResponse_Eta3to10_PU40to70->Fill(genp_pt,jet_pt_response, weight);
       }
    }
*/
    //// Fill Jet PT response histograms for PU 40 - 50
    if(trueinteraction>=40 && trueinteraction<=50){
       if(0<= jet_eta && jet_eta <=1.3){
         JetPtResponse_Eta0to1p3_PU40to50->Fill(genp_pt,jet_pt_response, weight);
       }else if(1.3<= jet_eta && jet_eta <=1.6){
         JetPtResponse_Eta1p3to1p6_PU40to50->Fill(genp_pt,jet_pt_response, weight);
       }else if(1.6<= jet_eta && jet_eta <=2.0){
         JetPtResponse_Eta1p6to2_PU40to50->Fill(genp_pt,jet_pt_response, weight);
       }else if(2.0<= jet_eta && jet_eta <=2.5){
         JetPtResponse_Eta2to2p5_PU40to50->Fill(genp_pt,jet_pt_response, weight);
       }else if(2.5<= jet_eta && jet_eta <=3.0){
         JetPtResponse_Eta2p5to3_PU40to50->Fill(genp_pt,jet_pt_response, weight);
       }else if(3.0<= jet_eta){
         JetPtResponse_Eta3to10_PU40to50->Fill(genp_pt,jet_pt_response, weight);
       }
    }

    if(3 <= jet_eta && jet_eta <= 5){
      if(17<= genp_pt && genp_pt <=20){
	JetPtResponse_Eta3to5_pt17to20_vsNeutralMultiplicity->Fill(neutral_multiplicity,jet_pt_response,weight);
	JetPtResponse_Eta3to5_pt17to20_vsmu->Fill(jet_pt_response,trueinteraction,weight);
	NeutralMultiplicity_vs_mu_Eta3to5_pt17to20->Fill(neutral_multiplicity,trueinteraction,weight);
	if(10 <= trueinteraction && trueinteraction<20)	JetPtResponse_Eta3to5_pt17to20_vsNeutralMultiplicity_mu_10to20->Fill(neutral_multiplicity,jet_pt_response,weight);
	if(50 <= trueinteraction && trueinteraction<60)	JetPtResponse_Eta3to5_pt17to20_vsNeutralMultiplicity_mu_50to60->Fill(neutral_multiplicity,jet_pt_response,weight);
	if(genp_pt == -1)     NeutralMultiplicity_vs_mu_unmatched_Eta3to5_pt17to20->Fill(neutral_multiplicity,trueinteraction,weight);
	else     NeutralMultiplicity_vs_mu_matched_Eta3to5_pt17to20->Fill(neutral_multiplicity,trueinteraction,weight);
	NeutralPuppiMultiplicity_vs_mu_Eta3to5_pt17to20->Fill(puppi_neutral_multiplicity,trueinteraction,weight);
	if(genp_pt == -1)     NeutralPuppiMultiplicity_vs_mu_unmatched_Eta3to5_pt17to20->Fill(puppi_neutral_multiplicity,trueinteraction,weight);
	else     NeutralPuppiMultiplicity_vs_mu_matched_Eta3to5_pt17to20->Fill(puppi_neutral_multiplicity,trueinteraction,weight);
      }
      if(20<= genp_pt && genp_pt <=23){
	JetPtResponse_Eta3to5_pt20to23_vsNeutralMultiplicity->Fill(neutral_multiplicity,jet_pt_response,weight);
	JetPtResponse_Eta3to5_pt20to23_vsmu->Fill(jet_pt_response,trueinteraction,weight);
	NeutralMultiplicity_vs_mu_Eta3to5_pt20to23->Fill(neutral_multiplicity,trueinteraction,weight);
	if(10 <= trueinteraction && trueinteraction<20)	JetPtResponse_Eta3to5_pt20to23_vsNeutralMultiplicity_mu_10to20->Fill(neutral_multiplicity,jet_pt_response,weight);
	if(50 <= trueinteraction && trueinteraction<60)	JetPtResponse_Eta3to5_pt20to23_vsNeutralMultiplicity_mu_50to60->Fill(neutral_multiplicity,jet_pt_response,weight);
	if(genp_pt == -1)     NeutralMultiplicity_vs_mu_unmatched_Eta3to5_pt20to23->Fill(neutral_multiplicity,trueinteraction,weight);
	else     NeutralMultiplicity_vs_mu_matched_Eta3to5_pt20to23->Fill(neutral_multiplicity,trueinteraction,weight);
	NeutralPuppiMultiplicity_vs_mu_Eta3to5_pt20to23->Fill(puppi_neutral_multiplicity,trueinteraction,weight);
	if(genp_pt == -1)     NeutralPuppiMultiplicity_vs_mu_unmatched_Eta3to5_pt20to23->Fill(puppi_neutral_multiplicity,trueinteraction,weight);
	else     NeutralPuppiMultiplicity_vs_mu_matched_Eta3to5_pt20to23->Fill(puppi_neutral_multiplicity,trueinteraction,weight);
      }

      if(15<= genp_pt && genp_pt <=17)	{
	JetPtResponse_Eta3to5_pt15to17_vsNeutralMultiplicity->Fill(neutral_multiplicity,jet_pt_response,weight);
	JetPtResponse_Eta3to5_pt15to17_vsmu->Fill(jet_pt_response,trueinteraction,weight);
	NeutralMultiplicity_vs_mu_Eta3to5_pt15to17->Fill(neutral_multiplicity,trueinteraction,weight);
	if(10 <= trueinteraction && trueinteraction<20)	JetPtResponse_Eta3to5_pt15to17_vsNeutralMultiplicity_mu_10to20->Fill(neutral_multiplicity,jet_pt_response,weight);
	if(50 <= trueinteraction && trueinteraction<60)	JetPtResponse_Eta3to5_pt15to17_vsNeutralMultiplicity_mu_50to60->Fill(neutral_multiplicity,jet_pt_response,weight);
	if(genp_pt == -1)     NeutralMultiplicity_vs_mu_unmatched_Eta3to5_pt15to17->Fill(neutral_multiplicity,trueinteraction,weight);
	else     NeutralMultiplicity_vs_mu_matched_Eta3to5_pt15to17->Fill(neutral_multiplicity,trueinteraction,weight);
	NeutralPuppiMultiplicity_vs_mu_Eta3to5_pt15to17->Fill(puppi_neutral_multiplicity,trueinteraction,weight);
	if(genp_pt == -1)     NeutralPuppiMultiplicity_vs_mu_unmatched_Eta3to5_pt15to17->Fill(puppi_neutral_multiplicity,trueinteraction,weight);
	else     NeutralPuppiMultiplicity_vs_mu_matched_Eta3to5_pt15to17->Fill(puppi_neutral_multiplicity,trueinteraction,weight);
      }

      if(27<= genp_pt && genp_pt <=30){
	JetPtResponse_Eta3to5_pt27to30_vsNeutralMultiplicity->Fill(neutral_multiplicity,jet_pt_response,weight);
	JetPtResponse_Eta3to5_pt27to30_vsmu->Fill(jet_pt_response,trueinteraction,weight);
	NeutralMultiplicity_vs_mu_Eta3to5_pt27to30->Fill(neutral_multiplicity,trueinteraction,weight);
	if(10 <= trueinteraction && trueinteraction<20)	JetPtResponse_Eta3to5_pt27to30_vsNeutralMultiplicity_mu_10to20->Fill(neutral_multiplicity,jet_pt_response,weight);
	if(50 <= trueinteraction && trueinteraction<60)	JetPtResponse_Eta3to5_pt27to30_vsNeutralMultiplicity_mu_50to60->Fill(neutral_multiplicity,jet_pt_response,weight);
	if(genp_pt == -1)     NeutralMultiplicity_vs_mu_unmatched_Eta3to5_pt27to30->Fill(neutral_multiplicity,trueinteraction,weight);
	else     NeutralMultiplicity_vs_mu_matched_Eta3to5_pt27to30->Fill(neutral_multiplicity,trueinteraction,weight);
	NeutralPuppiMultiplicity_vs_mu_Eta3to5_pt27to30->Fill(puppi_neutral_multiplicity,trueinteraction,weight);
	if(genp_pt == -1)     NeutralPuppiMultiplicity_vs_mu_unmatched_Eta3to5_pt27to30->Fill(puppi_neutral_multiplicity,trueinteraction,weight);
	else     NeutralPuppiMultiplicity_vs_mu_matched_Eta3to5_pt27to30->Fill(puppi_neutral_multiplicity,trueinteraction,weight);
      }

    }

    if(0 <= jet_eta && jet_eta <= 2.5){
      if(17<= genp_pt && genp_pt <=20)	{
	JetPtResponse_Eta0to2p5_pt17to20_vsNeutralMultiplicity->Fill(neutral_multiplicity,jet_pt_response,weight);
	JetPtResponse_Eta0to2p5_pt17to20_vsmu->Fill(jet_pt_response,trueinteraction,weight);
	NeutralMultiplicity_vs_mu_Eta0to2p5_pt17to20->Fill(neutral_multiplicity,trueinteraction,weight);
	if(genp_pt == -1)     NeutralMultiplicity_vs_mu_unmatched_Eta0to2p5_pt17to20->Fill(neutral_multiplicity,trueinteraction,weight);
	else     NeutralMultiplicity_vs_mu_matched_Eta0to2p5_pt17to20->Fill(neutral_multiplicity,trueinteraction,weight);
	NeutralPuppiMultiplicity_vs_mu_Eta0to2p5_pt17to20->Fill(puppi_neutral_multiplicity,trueinteraction,weight);
	if(genp_pt == -1)     NeutralPuppiMultiplicity_vs_mu_unmatched_Eta0to2p5_pt17to20->Fill(puppi_neutral_multiplicity,trueinteraction,weight);
	else     NeutralPuppiMultiplicity_vs_mu_matched_Eta0to2p5_pt17to20->Fill(puppi_neutral_multiplicity,trueinteraction,weight);
      }

      if(20<= genp_pt && genp_pt <=23)	{
	JetPtResponse_Eta0to2p5_pt20to23_vsNeutralMultiplicity->Fill(neutral_multiplicity,jet_pt_response,weight);
	JetPtResponse_Eta0to2p5_pt20to23_vsmu->Fill(jet_pt_response,trueinteraction,weight);
	NeutralMultiplicity_vs_mu_Eta0to2p5_pt20to23->Fill(neutral_multiplicity,trueinteraction,weight);
	if(genp_pt == -1)     NeutralMultiplicity_vs_mu_unmatched_Eta0to2p5_pt20to23->Fill(neutral_multiplicity,trueinteraction,weight);
	else     NeutralMultiplicity_vs_mu_matched_Eta0to2p5_pt20to23->Fill(neutral_multiplicity,trueinteraction,weight);
	NeutralPuppiMultiplicity_vs_mu_Eta0to2p5_pt20to23->Fill(puppi_neutral_multiplicity,trueinteraction,weight);
	if(genp_pt == -1)     NeutralPuppiMultiplicity_vs_mu_unmatched_Eta0to2p5_pt20to23->Fill(puppi_neutral_multiplicity,trueinteraction,weight);
	else     NeutralPuppiMultiplicity_vs_mu_matched_Eta0to2p5_pt20to23->Fill(puppi_neutral_multiplicity,trueinteraction,weight);
      }

      if(15<= genp_pt && genp_pt <=17)	{
	JetPtResponse_Eta0to2p5_pt15to17_vsNeutralMultiplicity->Fill(neutral_multiplicity,jet_pt_response,weight);
	JetPtResponse_Eta0to2p5_pt15to17_vsmu->Fill(jet_pt_response,trueinteraction,weight);
	NeutralMultiplicity_vs_mu_Eta0to2p5_pt15to17->Fill(neutral_multiplicity,trueinteraction,weight);
	if(genp_pt == -1)     NeutralMultiplicity_vs_mu_unmatched_Eta0to2p5_pt15to17->Fill(neutral_multiplicity,trueinteraction,weight);
	else     NeutralMultiplicity_vs_mu_matched_Eta0to2p5_pt15to17->Fill(neutral_multiplicity,trueinteraction,weight);
	NeutralPuppiMultiplicity_vs_mu_Eta0to2p5_pt15to17->Fill(puppi_neutral_multiplicity,trueinteraction,weight);
	if(genp_pt == -1)     NeutralPuppiMultiplicity_vs_mu_unmatched_Eta0to2p5_pt15to17->Fill(puppi_neutral_multiplicity,trueinteraction,weight);
	else     NeutralPuppiMultiplicity_vs_mu_matched_Eta0to2p5_pt15to17->Fill(puppi_neutral_multiplicity,trueinteraction,weight);
      }

      if(27<= genp_pt && genp_pt <=30)	{
	JetPtResponse_Eta0to2p5_pt27to30_vsNeutralMultiplicity->Fill(neutral_multiplicity,jet_pt_response,weight);
	JetPtResponse_Eta0to2p5_pt27to30_vsmu->Fill(jet_pt_response,trueinteraction,weight);
	NeutralMultiplicity_vs_mu_Eta0to2p5_pt27to30->Fill(neutral_multiplicity,trueinteraction,weight);
	if(genp_pt == -1)     NeutralMultiplicity_vs_mu_unmatched_Eta0to2p5_pt27to30->Fill(neutral_multiplicity,trueinteraction,weight);
	else     NeutralMultiplicity_vs_mu_matched_Eta0to2p5_pt27to30->Fill(neutral_multiplicity,trueinteraction,weight);
	NeutralPuppiMultiplicity_vs_mu_Eta0to2p5_pt27to30->Fill(puppi_neutral_multiplicity,trueinteraction,weight);
	if(genp_pt == -1)     NeutralPuppiMultiplicity_vs_mu_unmatched_Eta0to2p5_pt27to30->Fill(puppi_neutral_multiplicity,trueinteraction,weight);
	else     NeutralPuppiMultiplicity_vs_mu_matched_Eta0to2p5_pt27to30->Fill(puppi_neutral_multiplicity,trueinteraction,weight);
      }

    }

    if(2.5 < jet_eta && jet_eta <= 3){
      if(17<= genp_pt && genp_pt <=20)	{
	JetPtResponse_Eta2p5to3p0_pt17to20_vsmu->Fill(jet_pt_response,trueinteraction,weight);
      }

      if(20<= genp_pt && genp_pt <=23)	{
	JetPtResponse_Eta2p5to3p0_pt20to23_vsmu->Fill(jet_pt_response,trueinteraction,weight);
      }

      if(15<= genp_pt && genp_pt <=17)	{
	JetPtResponse_Eta2p5to3p0_pt15to17_vsmu->Fill(jet_pt_response,trueinteraction,weight);
      }

      if(27<= genp_pt && genp_pt <=30)	{
	JetPtResponse_Eta2p5to3p0_pt27to30_vsmu->Fill(jet_pt_response,trueinteraction,weight);
      }

    }



  }//for-loop over all jets
 
}

PuppiJetHists::~PuppiJetHists(){}
