#include "UHH2/PUPPI/include/PUPPIHists.h"
#include "UHH2/core/include/Event.h"

#include "TH1F.h"
#include "TH2F.h"
#include <iostream>

using namespace std;
using namespace uhh2;
using namespace uhh2examples;

PUPPIHists::PUPPIHists(Context & ctx, const string & dirname): Hists(ctx, dirname){
  // book all histograms here
  // jets
  book<TH1F>("N_jets", "N_{jets}", 20, 0, 20);  
  book<TH1F>("N_PU", "N_{PU}", 100, 0, 100);  
  book<TH1F>("eta_jet1", "#eta^{jet 1}", 40, -2.5, 2.5);
  book<TH1F>("pt_jet1", "p_{T}^{jet 1}", 100, 10, 500);
  book<TH1F>("eta_jet2", "#eta^{jet 2}", 40, -2.5, 2.5);
  book<TH1F>("eta_jet3", "#eta^{jet 3}", 40, -2.5, 2.5);
  book<TH1F>("eta_jet4", "#eta^{jet 4}", 40, -2.5, 2.5);

  book<TH1F>("EMcharged_jet1", "EMcharged_jet1", 100,0.0,1.0);
  book<TH1F>("EMneutral_jet1", "EMneutral_jet1", 100,0.0,1.0);
  book<TH1F>("HADcharged_jet1", "HADcharged_jet1", 100,0.0,1.0);
  book<TH1F>("HADneutral_jet1", "HADneutral_jet1", 100,0.0,1.0);

  book<TH2D>("EMcharged_vs_eta_jet1","EMcharged vs #eta; #eta; EMcharged",100,-6,6,100,0.0,1.0);   
  book<TH2D>("EMneutral_vs_eta_jet1","EMneutral vs #eta; #eta; EMneutral",100,-6,6,100,0.0,1.0);   
  book<TH2D>("HADcharged_vs_eta_jet1","HADcharged vs #eta; #eta; HADcharged",100,-6,6,100,0.0,1.0);   
  book<TH2D>("HADneutral_vs_eta_jet1","HADneutral vs #eta; #eta; HADneutral",100,-6,6,100,0.0,1.0);   
  book<TH2D>("EMcharged_vs_PU_jet1","EMcharged vs PU; PU; EMcharged",100,0,100,100,0.0,1.0);   
  book<TH2D>("EMneutral_vs_PU_jet1","EMneutral vs PU; PU; EMneutral",100,0,100,100,0.0,1.0);   
  book<TH2D>("HADcharged_vs_PU_jet1","HADcharged vs PU; PU; HADcharged",100,0,100,100,0.0,1.0);   
  book<TH2D>("HADneutral_vs_PU_jet1","HADneutral vs PU; PU; HADneutral",100,0,100,100,0.0,1.0);   


  // leptons
  book<TH1F>("N_mu", "N^{#mu}", 10, 0, 10);
  book<TH1F>("pt_mu", "p_{T}^{#mu} [GeV/c]", 40, 0, 200);
  book<TH1F>("eta_mu", "#eta^{#mu}", 40, -2.1, 2.1);
  book<TH1F>("reliso_mu", "#mu rel. Iso", 40, 0, 0.5);

  // primary vertices
  book<TH1F>("N_pv", "N^{PV}", 50, 0, 50);

  // gen particles
  book<TH1F>("mH_gen", "mH_{gen}", 300, 0, 300);
  book<TH1F>("mG_gen", "mG_{gen}", 6000, 0, 6000);

  // SD mass response
  book<TH1F>("H_mSD_mGEN", "mSD_{reco}/mSD_{gen}", 200, -5, 5);
  book<TH1F>("W_mSD_mGEN", "W mSD_{reco}/mSD_{gen}", 200, -5, 5);
  book<TH1F>("H_mSD_mGEN_PU0-10", "mSD_{reco}/mSD_{gen} PU0-10", 200, -5, 5);
  book<TH1F>("W_mSD_mGEN_PU0-10", "W mSD_{reco}/mSD_{gen} PU0-10", 200, -5, 5);
  book<TH1F>("H_mSD_mGEN_PU10-20", "mSD_{reco}/mSD_{gen} PU10-20", 200, -5, 5);
  book<TH1F>("W_mSD_mGEN_PU10-20", "W mSD_{reco}/mSD_{gen} PU10-20", 200, -5, 5);
  book<TH1F>("H_mSD_mGEN_PU20-30", "mSD_{reco}/mSD_{gen} PU20-30", 200, -5, 5);
  book<TH1F>("W_mSD_mGEN_PU20-30", "W mSD_{reco}/mSD_{gen} PU20-30", 200, -5, 5);
  book<TH1F>("H_mSD_mGEN_PU30-40", "mSD_{reco}/mSD_{gen} PU30-40", 200, -5, 5);
  book<TH1F>("W_mSD_mGEN_PU30-40", "W mSD_{reco}/mSD_{gen} PU30-40", 200, -5, 5);
  book<TH1F>("H_mSD_mGEN_PU40-50", "mSD_{reco}/mSD_{gen} PU40-50", 200, -5, 5);
  book<TH1F>("W_mSD_mGEN_PU40-50", "W mSD_{reco}/mSD_{gen} PU40-50", 200, -5, 5);
  book<TH1F>("H_mSD_mGEN_PU50-60", "mSD_{reco}/mSD_{gen} PU50-60", 200, -5, 5);
  book<TH1F>("W_mSD_mGEN_PU50-60", "W mSD_{reco}/mSD_{gen} PU50-60", 200, -5, 5);
  book<TH1F>("H_mSD_mGEN_PU60-70", "mSD_{reco}/mSD_{gen} PU60-70", 200, -5, 5);
  book<TH1F>("W_mSD_mGEN_PU60-70", "W mSD_{reco}/mSD_{gen} PU60-70", 200, -5, 5);


  // SD mass response pt 400-600
  book<TH1F>("H_mSD_mGEN_pt400-600", "mSD_{reco}/mSD_{gen} pt 400-600", 200, -5, 5);
  book<TH1F>("W_mSD_mGEN_pt400-600", "W mSD_{reco}/mSD_{gen} pt 400-600", 200, -5, 5);
  book<TH1F>("H_mSD_mGEN_PU0-10_pt400-600", "mSD_{reco}/mSD_{gen} PU0-10 pt 400-600", 200, -5, 5);
  book<TH1F>("W_mSD_mGEN_PU0-10_pt400-600", "W mSD_{reco}/mSD_{gen} PU0-10 pt 400-600", 200, -5, 5);
  book<TH1F>("H_mSD_mGEN_PU10-20_pt400-600", "mSD_{reco}/mSD_{gen} PU10-20 pt 400-600", 200, -5, 5);
  book<TH1F>("W_mSD_mGEN_PU10-20_pt400-600", "W mSD_{reco}/mSD_{gen} PU10-20 pt 400-600", 200, -5, 5);
  book<TH1F>("H_mSD_mGEN_PU20-30_pt400-600", "mSD_{reco}/mSD_{gen} PU20-30 pt 400-600", 200, -5, 5);
  book<TH1F>("W_mSD_mGEN_PU20-30_pt400-600", "W mSD_{reco}/mSD_{gen} PU20-30 pt 400-600", 200, -5, 5);
  book<TH1F>("H_mSD_mGEN_PU30-40_pt400-600", "mSD_{reco}/mSD_{gen} PU30-40 pt 400-600", 200, -5, 5);
  book<TH1F>("W_mSD_mGEN_PU30-40_pt400-600", "W mSD_{reco}/mSD_{gen} PU30-40 pt 400-600", 200, -5, 5);
  book<TH1F>("H_mSD_mGEN_PU40-50_pt400-600", "mSD_{reco}/mSD_{gen} PU40-50 pt 400-600", 200, -5, 5);
  book<TH1F>("W_mSD_mGEN_PU40-50_pt400-600", "W mSD_{reco}/mSD_{gen} PU40-50 pt 400-600", 200, -5, 5);
  book<TH1F>("H_mSD_mGEN_PU50-60_pt400-600", "mSD_{reco}/mSD_{gen} PU50-60 pt 400-600", 200, -5, 5);
  book<TH1F>("W_mSD_mGEN_PU50-60_pt400-600", "W mSD_{reco}/mSD_{gen} PU50-60 pt 400-600", 200, -5, 5);
  book<TH1F>("H_mSD_mGEN_PU60-70_pt400-600", "mSD_{reco}/mSD_{gen} PU60-70 pt 400-600", 200, -5, 5);
  book<TH1F>("W_mSD_mGEN_PU60-70_pt400-600", "W mSD_{reco}/mSD_{gen} PU60-70 pt 400-600", 200, -5, 5);


  // SD mass W boson
  book<TH1F>("H_mSD", "H mSD_{reco}", 200, 0, 200);
  book<TH1F>("W_mSD", "W mSD_{reco}", 200, 0, 200);
  book<TH1F>("W_mSD_PU0-10", "W mSD_{reco} PU0-10", 200, 0, 200);
  book<TH1F>("W_mSD_PU10-20", "W mSD_{reco} PU10-20", 200, 0, 200);
  book<TH1F>("W_mSD_PU20-30", "W mSD_{reco} PU20-30", 200, 0, 200);
  book<TH1F>("W_mSD_PU30-40", "W mSD_{reco} PU30-40", 200, 0, 200);
  book<TH1F>("W_mSD_PU40-50", "W mSD_{reco} PU40-50", 200, 0, 200);
  book<TH1F>("W_mSD_PU50-60", "W mSD_{reco} PU50-60", 200, 0, 200);
  book<TH1F>("W_mSD_PU60-70", "W mSD_{reco} PU60-70", 200, 0, 200);


  // W boson pt spectrum
  book<TH1F>("H_pt", "H pt", 700, 0, 7000);
  book<TH1F>("W_pt", "W pt", 700, 0, 7000);
  book<TH1F>("W_pt_PU0-10", "W pt PU0-10", 700, 0, 7000);
  book<TH1F>("W_pt_PU10-20", "W pt PU10-20", 700, 0, 7000);
  book<TH1F>("W_pt_PU20-30", "W pt PU20-30", 700, 0, 7000);
  book<TH1F>("W_pt_PU30-40", "W pt PU30-40", 700, 0, 7000);
  book<TH1F>("W_pt_PU40-50", "W pt PU40-50", 700, 0, 7000);
  book<TH1F>("W_pt_PU50-60", "W pt PU50-60", 700, 0, 7000);
  book<TH1F>("W_pt_PU60-70", "W pt PU60-70", 700, 0, 7000);

  // SD mass W boson pt 400-600 GeV
  book<TH1F>("H_mSD_pt400-600", "H mSD_{reco}", 200, 0, 200);
  book<TH1F>("W_mSD_pt400-600", "W mSD_{reco}", 200, 0, 200);
  book<TH1F>("W_mSD_PU0-10_pt400-600", "W mSD_{reco} PU0-10", 200, 0, 200);
  book<TH1F>("W_mSD_PU10-20_pt400-600", "W mSD_{reco} PU10-20", 200, 0, 200);
  book<TH1F>("W_mSD_PU20-30_pt400-600", "W mSD_{reco} PU20-30", 200, 0, 200);
  book<TH1F>("W_mSD_PU30-40_pt400-600", "W mSD_{reco} PU30-40", 200, 0, 200);
  book<TH1F>("W_mSD_PU40-50_pt400-600", "W mSD_{reco} PU40-50", 200, 0, 200);
  book<TH1F>("W_mSD_PU50-60_pt400-600", "W mSD_{reco} PU50-60", 200, 0, 200);
  book<TH1F>("W_mSD_PU60-70_pt400-600", "W mSD_{reco} PU60-70", 200, 0, 200);

}


void PUPPIHists::fill(const Event & event){
  // fill the histograms. Please note the comments in the header file:
  // 'hist' is used here a lot for simplicity, but it will be rather
  // slow when you have many histograms; therefore, better
  // use histogram pointers as members as in 'UHH2/common/include/ElectronHists.h'
  
  // Don't forget to always use the weight when filling.
  double weight = event.weight;
  
/*  std::vector<Jet>* jets = event.jets;
  int Njets = jets->size();
  hist("N_jets")->Fill(Njets, weight);
  if(!event.isRealData)  hist("N_PU")->Fill(event.genInfo->pileup_TrueNumInteractions(), weight);

  if(Njets>=1){
    hist("eta_jet1")->Fill(jets->at(0).eta(), weight);
    hist("pt_jet1")->Fill(jets->at(0).pt(), weight);
    hist("EMcharged_jet1")->Fill(jets->at(0).chargedEmEnergyFraction(), weight);
    hist("EMneutral_jet1")->Fill(jets->at(0).neutralEmEnergyFraction(), weight);
    hist("HADcharged_jet1")->Fill(jets->at(0).chargedHadronEnergyFraction(), weight);
    hist("HADneutral_jet1")->Fill(jets->at(0).neutralHadronEnergyFraction(), weight);
    
    ((TH2D*)hist("EMcharged_vs_eta_jet1"))->Fill(jets->at(0).eta(),jets->at(0).chargedEmEnergyFraction(), weight);
    ((TH2D*)hist("EMneutral_vs_eta_jet1"))->Fill(jets->at(0).eta(),jets->at(0).neutralEmEnergyFraction(), weight);
    ((TH2D*)hist("HADcharged_vs_eta_jet1"))->Fill(jets->at(0).eta(),jets->at(0).chargedHadronEnergyFraction(), weight);
    ((TH2D*)hist("HADneutral_vs_eta_jet1"))->Fill(jets->at(0).eta(),jets->at(0).neutralHadronEnergyFraction(), weight);
    if(!event.isRealData){
      ((TH2D*)hist("EMcharged_vs_PU_jet1"))->Fill(event.genInfo->pileup_TrueNumInteractions(),jets->at(0).chargedEmEnergyFraction(), weight);
      ((TH2D*)hist("EMneutral_vs_PU_jet1"))->Fill(event.genInfo->pileup_TrueNumInteractions(),jets->at(0).neutralEmEnergyFraction(), weight);
      ((TH2D*)hist("HADcharged_vs_PU_jet1"))->Fill(event.genInfo->pileup_TrueNumInteractions(),jets->at(0).chargedHadronEnergyFraction(), weight);
      ((TH2D*)hist("HADneutral_vs_PU_jet1"))->Fill(event.genInfo->pileup_TrueNumInteractions(),jets->at(0).neutralHadronEnergyFraction(), weight);
    }
  }
  if(Njets>=2){
    hist("eta_jet2")->Fill(jets->at(1).eta(), weight);
  }
  if(Njets>=3){
    hist("eta_jet3")->Fill(jets->at(2).eta(), weight);
  }
  if(Njets>=4){
    hist("eta_jet4")->Fill(jets->at(3).eta(), weight);
  }
*/
//  int Nmuons = event.muons->size();
//  hist("N_mu")->Fill(Nmuons, weight);
//  for (const Muon & thismu : *event.muons){
//      hist("pt_mu")->Fill(thismu.pt(), weight);
//      hist("eta_mu")->Fill(thismu.eta(), weight);
//      hist("reliso_mu")->Fill(thismu.relIso(), weight);
//  }
//  
//  int Npvs = event.pvs->size();
//  hist("N_pv")->Fill(Npvs, weight);




  // gen particles
  int N_gp = event.genparticles->size();
  for (const auto & gp : *event.genparticles){
      if(gp.pdgId() == 25){
      hist("mH_gen")->Fill(gp.v4().mass(),weight);
      }
      if(gp.pdgId() == 39){
      hist("mG_gen")->Fill(gp.v4().mass(),weight);
      }
  }

  double trueinteraction = event.genInfo->pileup_TrueNumInteractions(); 


 
// Ak8 PUPPI jets SD mass response
  for (const TopJet & toppuppijet: *event.toppuppijets) {
  for (const GenTopJet & gentopjet: *event.gentopjets) {
       if ( deltaR(gentopjet,toppuppijet)<0.8 && toppuppijet.softdropmass()>=10){
       hist("H_mSD")->Fill(toppuppijet.softdropmass(),weight);
       hist("H_pt")->Fill(toppuppijet.pt(),weight);
       hist("H_mSD_mGEN")->Fill(toppuppijet.softdropmass()/gentopjet.softdropmass(),weight);
         if(trueinteraction>=0 && trueinteraction<=10){
         hist("H_mSD_mGEN_PU0-10")->Fill(toppuppijet.softdropmass()/gentopjet.softdropmass(),weight);
         }
         if(trueinteraction>=10 && trueinteraction<=20){
         hist("H_mSD_mGEN_PU10-20")->Fill(toppuppijet.softdropmass()/gentopjet.softdropmass(),weight);
         }
         if(trueinteraction>=20 && trueinteraction<=30){
         hist("H_mSD_mGEN_PU20-30")->Fill(toppuppijet.softdropmass()/gentopjet.softdropmass(),weight);
         }
         if(trueinteraction>=30 && trueinteraction<=40){
         hist("H_mSD_mGEN_PU30-40")->Fill(toppuppijet.softdropmass()/gentopjet.softdropmass(),weight);
         }
         if(trueinteraction>=40 && trueinteraction<=50){
         hist("H_mSD_mGEN_PU40-50")->Fill(toppuppijet.softdropmass()/gentopjet.softdropmass(),weight);
         }
         if(trueinteraction>=50 && trueinteraction<=60){
         hist("H_mSD_mGEN_PU50-60")->Fill(toppuppijet.softdropmass()/gentopjet.softdropmass(),weight);
         }
         if(trueinteraction>=60 && trueinteraction<=70){
         hist("H_mSD_mGEN_PU60-70")->Fill(toppuppijet.softdropmass()/gentopjet.softdropmass(),weight);
         }
            if (  70 <= gentopjet.softdropmass() && gentopjet.softdropmass() <= 90 ){
            hist("W_mSD")->Fill(toppuppijet.softdropmass(),weight);
            hist("W_pt")->Fill(toppuppijet.pt(),weight);
            hist("W_mSD_mGEN")->Fill(toppuppijet.softdropmass()/gentopjet.softdropmass(),weight);
              if(trueinteraction>=0 && trueinteraction<=10){
              hist("W_mSD_PU0-10")->Fill(toppuppijet.softdropmass(),weight);
              hist("W_pt_PU0-10")->Fill(toppuppijet.pt(),weight);
              hist("W_mSD_mGEN_PU0-10")->Fill(toppuppijet.softdropmass()/gentopjet.softdropmass(),weight);
              }
              if(trueinteraction>=10 && trueinteraction<=20){
              hist("W_mSD_PU10-20")->Fill(toppuppijet.softdropmass(),weight);
              hist("W_pt_PU10-20")->Fill(toppuppijet.pt(),weight);
              hist("W_mSD_mGEN_PU10-20")->Fill(toppuppijet.softdropmass()/gentopjet.softdropmass(),weight);
              }
              if(trueinteraction>=20 && trueinteraction<=30){
              hist("W_mSD_PU20-30")->Fill(toppuppijet.softdropmass(),weight);
              hist("W_pt_PU20-30")->Fill(toppuppijet.pt(),weight);
              hist("W_mSD_mGEN_PU20-30")->Fill(toppuppijet.softdropmass()/gentopjet.softdropmass(),weight);
              }
              if(trueinteraction>=30 && trueinteraction<=40){
              hist("W_mSD_PU30-40")->Fill(toppuppijet.softdropmass(),weight);
              hist("W_pt_PU30-40")->Fill(toppuppijet.pt(),weight);
              hist("W_mSD_mGEN_PU30-40")->Fill(toppuppijet.softdropmass()/gentopjet.softdropmass(),weight);
              }
              if(trueinteraction>=40 && trueinteraction<=50){
              hist("W_mSD_PU40-50")->Fill(toppuppijet.softdropmass(),weight);
              hist("W_pt_PU40-50")->Fill(toppuppijet.pt(),weight);
              hist("W_mSD_mGEN_PU40-50")->Fill(toppuppijet.softdropmass()/gentopjet.softdropmass(),weight);
              }
              if(trueinteraction>=50 && trueinteraction<=60){
              hist("W_mSD_PU50-60")->Fill(toppuppijet.softdropmass(),weight);
              hist("W_pt_PU50-60")->Fill(toppuppijet.pt(),weight);
              hist("W_mSD_mGEN_PU50-60")->Fill(toppuppijet.softdropmass()/gentopjet.softdropmass(),weight);
              }
              if(trueinteraction>=60 && trueinteraction<=70){
              hist("W_mSD_PU60-70")->Fill(toppuppijet.softdropmass(),weight);
              hist("W_pt_PU60-70")->Fill(toppuppijet.pt(),weight);
              hist("W_mSD_mGEN_PU60-70")->Fill(toppuppijet.softdropmass()/gentopjet.softdropmass(),weight);
              }
           }
       }
  }
  }


  // Ak8 PUPPI jets SD mass response pt 400-600
  for (const TopJet & toppuppijet: *event.toppuppijets) {
  for (const GenTopJet & gentopjet: *event.gentopjets) {
       if (deltaR(gentopjet,toppuppijet)<0.8 && toppuppijet.softdropmass()>=10){
       if (toppuppijet.pt()>=400 && toppuppijet.pt()<=600){
       hist("H_mSD_pt400-600")->Fill(toppuppijet.softdropmass(),weight);
       hist("H_mSD_mGEN_pt400-600")->Fill(toppuppijet.softdropmass()/gentopjet.softdropmass(),weight);
         if(trueinteraction>=0 && trueinteraction<=10){
         hist("H_mSD_mGEN_PU0-10_pt400-600")->Fill(toppuppijet.softdropmass()/gentopjet.softdropmass(),weight);
         }
         if(trueinteraction>=10 && trueinteraction<=20){
         hist("H_mSD_mGEN_PU10-20_pt400-600")->Fill(toppuppijet.softdropmass()/gentopjet.softdropmass(),weight);
         }
         if(trueinteraction>=20 && trueinteraction<=30){
         hist("H_mSD_mGEN_PU20-30_pt400-600")->Fill(toppuppijet.softdropmass()/gentopjet.softdropmass(),weight);
         }
         if(trueinteraction>=30 && trueinteraction<=40){
         hist("H_mSD_mGEN_PU30-40_pt400-600")->Fill(toppuppijet.softdropmass()/gentopjet.softdropmass(),weight);
         }
         if(trueinteraction>=40 && trueinteraction<=50){
         hist("H_mSD_mGEN_PU40-50_pt400-600")->Fill(toppuppijet.softdropmass()/gentopjet.softdropmass(),weight);
         }
         if(trueinteraction>=50 && trueinteraction<=60){
         hist("H_mSD_mGEN_PU50-60_pt400-600")->Fill(toppuppijet.softdropmass()/gentopjet.softdropmass(),weight);
         }
         if(trueinteraction>=60 && trueinteraction<=70){
         hist("H_mSD_mGEN_PU60-70_pt400-600")->Fill(toppuppijet.softdropmass()/gentopjet.softdropmass(),weight);
         }
            if (  70 <= gentopjet.softdropmass() && gentopjet.softdropmass() <= 90 ){
            hist("W_mSD_pt400-600")->Fill(toppuppijet.softdropmass(),weight);
            hist("W_mSD_mGEN_pt400-600")->Fill(toppuppijet.softdropmass()/gentopjet.softdropmass(),weight);
              if(trueinteraction>=0 && trueinteraction<=10){
              hist("W_mSD_PU0-10_pt400-600")->Fill(toppuppijet.softdropmass(),weight);
              hist("W_mSD_mGEN_PU0-10_pt400-600")->Fill(toppuppijet.softdropmass()/gentopjet.softdropmass(),weight);
              }
              if(trueinteraction>=10 && trueinteraction<=20){
              hist("W_mSD_PU10-20_pt400-600")->Fill(toppuppijet.softdropmass(),weight);
              hist("W_mSD_mGEN_PU10-20_pt400-600")->Fill(toppuppijet.softdropmass()/gentopjet.softdropmass(),weight);
              }
              if(trueinteraction>=20 && trueinteraction<=30){
              hist("W_mSD_PU20-30_pt400-600")->Fill(toppuppijet.softdropmass(),weight);
              hist("W_mSD_mGEN_PU20-30_pt400-600")->Fill(toppuppijet.softdropmass()/gentopjet.softdropmass(),weight);
              }
              if(trueinteraction>=30 && trueinteraction<=40){
              hist("W_mSD_PU30-40_pt400-600")->Fill(toppuppijet.softdropmass(),weight);
              hist("W_mSD_mGEN_PU30-40_pt400-600")->Fill(toppuppijet.softdropmass()/gentopjet.softdropmass(),weight);
              }
              if(trueinteraction>=40 && trueinteraction<=50){
              hist("W_mSD_PU40-50_pt400-600")->Fill(toppuppijet.softdropmass(),weight);
              hist("W_mSD_mGEN_PU40-50_pt400-600")->Fill(toppuppijet.softdropmass()/gentopjet.softdropmass(),weight);
              }
              if(trueinteraction>=50 && trueinteraction<=60){
              hist("W_mSD_PU50-60_pt400-600")->Fill(toppuppijet.softdropmass(),weight);
              hist("W_mSD_mGEN_PU50-60_pt400-600")->Fill(toppuppijet.softdropmass()/gentopjet.softdropmass(),weight);
              }
              if(trueinteraction>=60 && trueinteraction<=70){
              hist("W_mSD_PU60-70_pt400-600")->Fill(toppuppijet.softdropmass(),weight);
              hist("W_mSD_mGEN_PU60-70_pt400-600")->Fill(toppuppijet.softdropmass()/gentopjet.softdropmass(),weight);
              }
           }
       }
       }
  }
  }



/*

  // Ak8 chs jets SD mass response
  for (const TopJet & topjet: *event.topjets) {
  for (const GenTopJet & gentopjet: *event.gentopjets) {
       if ( deltaR(gentopjet,topjet)<0.8 && topjet.softdropmass()>=10){
       hist("H_mSD")->Fill(topjet.softdropmass(),weight);
       hist("H_pt")->Fill(topjet.pt(),weight);
       hist("H_mSD_mGEN")->Fill(topjet.softdropmass()/gentopjet.softdropmass(),weight);
         if(trueinteraction>=0 && trueinteraction<=10){
         hist("H_mSD_mGEN_PU0-10")->Fill(topjet.softdropmass()/gentopjet.softdropmass(),weight);
         }
         if(trueinteraction>=10 && trueinteraction<=20){
         hist("H_mSD_mGEN_PU10-20")->Fill(topjet.softdropmass()/gentopjet.softdropmass(),weight);
         }
         if(trueinteraction>=20 && trueinteraction<=30){
         hist("H_mSD_mGEN_PU20-30")->Fill(topjet.softdropmass()/gentopjet.softdropmass(),weight);
         }
         if(trueinteraction>=30 && trueinteraction<=40){
         hist("H_mSD_mGEN_PU30-40")->Fill(topjet.softdropmass()/gentopjet.softdropmass(),weight);
         }
         if(trueinteraction>=40 && trueinteraction<=50){
         hist("H_mSD_mGEN_PU40-50")->Fill(topjet.softdropmass()/gentopjet.softdropmass(),weight);
         }
         if(trueinteraction>=50 && trueinteraction<=60){
         hist("H_mSD_mGEN_PU50-60")->Fill(topjet.softdropmass()/gentopjet.softdropmass(),weight);
         }
         if(trueinteraction>=60 && trueinteraction<=70){
         hist("H_mSD_mGEN_PU60-70")->Fill(topjet.softdropmass()/gentopjet.softdropmass(),weight);
         }
            if (  70 <= gentopjet.softdropmass() && gentopjet.softdropmass() <= 90 ){
            hist("W_mSD")->Fill(topjet.softdropmass(),weight);
            hist("W_pt")->Fill(topjet.pt(),weight);
            hist("W_mSD_mGEN")->Fill(topjet.softdropmass()/gentopjet.softdropmass(),weight);
              if(trueinteraction>=0 && trueinteraction<=10){
              hist("W_mSD_PU0-10")->Fill(topjet.softdropmass(),weight);
              hist("W_pt_PU0-10")->Fill(topjet.pt(),weight);
              hist("W_mSD_mGEN_PU0-10")->Fill(topjet.softdropmass()/gentopjet.softdropmass(),weight);
              }
              if(trueinteraction>=10 && trueinteraction<=20){
              hist("W_mSD_PU10-20")->Fill(topjet.softdropmass(),weight);
              hist("W_pt_PU10-20")->Fill(topjet.pt(),weight);
              hist("W_mSD_mGEN_PU10-20")->Fill(topjet.softdropmass()/gentopjet.softdropmass(),weight);
              }
              if(trueinteraction>=20 && trueinteraction<=30){
              hist("W_mSD_PU20-30")->Fill(topjet.softdropmass(),weight);
              hist("W_pt_PU20-30")->Fill(topjet.pt(),weight);
              hist("W_mSD_mGEN_PU20-30")->Fill(topjet.softdropmass()/gentopjet.softdropmass(),weight);
              }
              if(trueinteraction>=30 && trueinteraction<=40){
              hist("W_mSD_PU30-40")->Fill(topjet.softdropmass(),weight);
              hist("W_pt_PU30-40")->Fill(topjet.pt(),weight);
              hist("W_mSD_mGEN_PU30-40")->Fill(topjet.softdropmass()/gentopjet.softdropmass(),weight);
              }
              if(trueinteraction>=40 && trueinteraction<=50){
              hist("W_mSD_PU40-50")->Fill(topjet.softdropmass(),weight);
              hist("W_pt_PU40-50")->Fill(topjet.pt(),weight);
              hist("W_mSD_mGEN_PU40-50")->Fill(topjet.softdropmass()/gentopjet.softdropmass(),weight);
              }
              if(trueinteraction>=50 && trueinteraction<=60){
              hist("W_mSD_PU50-60")->Fill(topjet.softdropmass(),weight);
              hist("W_pt_PU50-60")->Fill(topjet.pt(),weight);
              hist("W_mSD_mGEN_PU50-60")->Fill(topjet.softdropmass()/gentopjet.softdropmass(),weight);
              }
              if(trueinteraction>=60 && trueinteraction<=70){
              hist("W_mSD_PU60-70")->Fill(topjet.softdropmass(),weight);
              hist("W_pt_PU60-70")->Fill(topjet.pt(),weight);
              hist("W_mSD_mGEN_PU60-70")->Fill(topjet.softdropmass()/gentopjet.softdropmass(),weight);
              }
           }
       }
  }
  }


  // Ak8 chs jets SD mass response pt 400-600
  for (const TopJet & topjet: *event.topjets) {
  for (const GenTopJet & gentopjet: *event.gentopjets) {
       if (deltaR(gentopjet,topjet)<0.8 && topjet.softdropmass()>=10){
       if (topjet.pt()>=400 && topjet.pt()<=600){
       hist("H_mSD_pt400-600")->Fill(topjet.softdropmass(),weight);
       hist("H_mSD_mGEN_pt400-600")->Fill(topjet.softdropmass()/gentopjet.softdropmass(),weight);
         if(trueinteraction>=0 && trueinteraction<=10){
         hist("H_mSD_mGEN_PU0-10_pt400-600")->Fill(topjet.softdropmass()/gentopjet.softdropmass(),weight);
         }
         if(trueinteraction>=10 && trueinteraction<=20){
         hist("H_mSD_mGEN_PU10-20_pt400-600")->Fill(topjet.softdropmass()/gentopjet.softdropmass(),weight);
         }
         if(trueinteraction>=20 && trueinteraction<=30){
         hist("H_mSD_mGEN_PU20-30_pt400-600")->Fill(topjet.softdropmass()/gentopjet.softdropmass(),weight);
         }
         if(trueinteraction>=30 && trueinteraction<=40){
         hist("H_mSD_mGEN_PU30-40_pt400-600")->Fill(topjet.softdropmass()/gentopjet.softdropmass(),weight);
         }
         if(trueinteraction>=40 && trueinteraction<=50){
         hist("H_mSD_mGEN_PU40-50_pt400-600")->Fill(topjet.softdropmass()/gentopjet.softdropmass(),weight);
         }
         if(trueinteraction>=50 && trueinteraction<=60){
         hist("H_mSD_mGEN_PU50-60_pt400-600")->Fill(topjet.softdropmass()/gentopjet.softdropmass(),weight);
         }
         if(trueinteraction>=60 && trueinteraction<=70){
         hist("H_mSD_mGEN_PU60-70_pt400-600")->Fill(topjet.softdropmass()/gentopjet.softdropmass(),weight);
         }
            if (  70 <= gentopjet.softdropmass() && gentopjet.softdropmass() <= 90 ){
            hist("W_mSD_pt400-600")->Fill(topjet.softdropmass(),weight);
            hist("W_mSD_mGEN_pt400-600")->Fill(topjet.softdropmass()/gentopjet.softdropmass(),weight);
              if(trueinteraction>=0 && trueinteraction<=10){
              hist("W_mSD_PU0-10_pt400-600")->Fill(topjet.softdropmass(),weight);
              hist("W_mSD_mGEN_PU0-10_pt400-600")->Fill(topjet.softdropmass()/gentopjet.softdropmass(),weight);
              }
              if(trueinteraction>=10 && trueinteraction<=20){
              hist("W_mSD_PU10-20_pt400-600")->Fill(topjet.softdropmass(),weight);
              hist("W_mSD_mGEN_PU10-20_pt400-600")->Fill(topjet.softdropmass()/gentopjet.softdropmass(),weight);
              }
              if(trueinteraction>=20 && trueinteraction<=30){
              hist("W_mSD_PU20-30_pt400-600")->Fill(topjet.softdropmass(),weight);
              hist("W_mSD_mGEN_PU20-30_pt400-600")->Fill(topjet.softdropmass()/gentopjet.softdropmass(),weight);
              }
              if(trueinteraction>=30 && trueinteraction<=40){
              hist("W_mSD_PU30-40_pt400-600")->Fill(topjet.softdropmass(),weight);
              hist("W_mSD_mGEN_PU30-40_pt400-600")->Fill(topjet.softdropmass()/gentopjet.softdropmass(),weight);
              }
              if(trueinteraction>=40 && trueinteraction<=50){
              hist("W_mSD_PU40-50_pt400-600")->Fill(topjet.softdropmass(),weight);
              hist("W_mSD_mGEN_PU40-50_pt400-600")->Fill(topjet.softdropmass()/gentopjet.softdropmass(),weight);
              }
              if(trueinteraction>=50 && trueinteraction<=60){
              hist("W_mSD_PU50-60_pt400-600")->Fill(topjet.softdropmass(),weight);
              hist("W_mSD_mGEN_PU50-60_pt400-600")->Fill(topjet.softdropmass()/gentopjet.softdropmass(),weight);
              }
              if(trueinteraction>=60 && trueinteraction<=70){
              hist("W_mSD_PU60-70_pt400-600")->Fill(topjet.softdropmass(),weight);
              hist("W_mSD_mGEN_PU60-70_pt400-600")->Fill(topjet.softdropmass()/gentopjet.softdropmass(),weight);
              }
           }
       }
       }
  }
  }
*/



}

PUPPIHists::~PUPPIHists(){}
