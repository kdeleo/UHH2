#include "UHH2/PUPPI_tuning/include/DYHists.h"
#include "UHH2/core/include/Event.h"
#include "UHH2/core/include/GenParticle.h"
#include "UHH2/core/include/LorentzVector.h"


#include <iostream>

using namespace std;
using namespace uhh2;
using namespace uhh2examples;

/* THIS HIST CLASS ONLY WORKS FOR MUONS! IN CASE OF ELECTRON CHANNEL PLEASE ADAPT*/

/*
This Hist Class should cover relevant reconstructed Z boson quantities from the two muons,
the met response based on the resolution
*/

DYHists::DYHists(Context & ctx, const string & dirname): Hists(ctx, dirname){
  berror=(ctx.get("debug") == "true");

  // get pT cut for eff and purity from config, normaly at 30 GeV
  pTcut =atof(ctx.get("pTCut").c_str());

  ////////////////////////////////////          General Hist  ///////////////////////////////////////////
  MET_h = book<TH1F>("MET_h", "uncorrected MET",1500,0,1500);

  number_genjets = book<TH1F>("number_genjets","Number of Genjets",20,0,20); //before cleaning
  number_recojets= book<TH1F>("number_recojets","Number of RecoJets",20,0,20); //before cleaning

  muon_invariant_mass = book<TH1F>("muon_invariant_mass","Muon invariant mass",200,0,200); //inv mass of the muons should be around 20 GeV within the Z boson mass

  generater_HT = book<TH1F>("generator_HT","Generator HT",3500,0,3500);

  pujetid = book<TH1F>("pujetid","PU jet ID discriminator",200,-1,1);
  pujetid_forward = book<TH1F>("pujetid_forward","PU jet ID forward  discriminator",200,-1,1);

  ////////////////////////////////////       Response Hist  ///////////////////////////////////////////
  response_ZP=book<TH2F>("response_ZP", "reponse as function of ZpT",200,0,200,100,0,2);
  upar_ZPT=book<TH2F>("upar_ZPT", "upar as function of ZpT",500,0,500,500,0,500);
  response_NPV=book<TH2F>("response_NPV", "reponse as function of NVP",100,0,100,100,0,2);
  rms_ZP=book<TH2F>("rms_ZP", "rms_ZP",200,0,200,400,-100,100);
  rms_NPV=book<TH2F>("rms_NPV", "rms_NPV",100,0,100,1200,-200,200);
  rms_uper_ZP=book<TH2F>("rms_uper_ZP", "rms_uper",200,0,200,200,0,100);
  rms_uper_NPV=book<TH2F>("rms_uper_NPV", "rms_uper",100,0,100,600,-100,100);



}


void DYHists::fill(const Event & event){

  assert(event.met);
  assert(event.pvs);
  assert(event.jets);
  
  double weight = event.weight;
  if(berror) std::cout<<"weight  "<<weight<<std::endl;
  

  //define the two muons from the Z Boson
  assert(event.muons); // if this fails, it probably means muons are not read in
  std::vector<Muon> *muons = event.muons;
  
  Muon muon_pos;
  Muon muon_neg;
  //find muons of the Z decay
  for(auto m:*muons){
    if(m.charge()==1){
      muon_pos=m;
      break;
    }
  }
  for(auto m:*muons){
    if(m.charge()==-1){
      muon_neg=m;
      break;
    }
  }

  // define the jets that should not overlap with the muons
  std::unique_ptr< std::vector<Jet> > jets(new std::vector<Jet> (*event.jets));
  jets->clear();
  jets->reserve(event.jets->size());

  for(auto j:*event.jets) {
    double dRL1 = deltaR(j,muon_pos);
    double dRL2 = deltaR(j,muon_neg);
    if(dRL1>0.4 && dRL2>0.4)    jets->push_back(j);
    
  }

  //read in genjets
  std::vector<GenJet>* genparticles = event.genjets;

  //decide if it should be as function of reco vertices or true Interaction
  //  double npvs = event.pvs->size();
  double npvs = event.genInfo->pileup_TrueNumInteractions();

  if(berror)std::cout<<"Number of PV  "<<npvs<<std::endl;
  if(berror)std::cout<<"Number of Jets  "<<jets->size()<<std::endl;
  if(berror)std::cout<<"Number of GenJets  "<<genparticles->size()<<std::endl;
 
  if(berror) std::cout<<"DYHists::Response hists"<<std::endl;

  /////////////////////////////////////    General  Hists  /////////////////////////////////////////////
  number_genjets->Fill(event.genjets->size(),weight);
  number_recojets->Fill(event.jets->size(),weight);
  MET_h->Fill(event.met->uncorr_pt(),weight);
   
  /////////////////////////////////////    Response Hists  /////////////////////////////////////////////
  //Pt and mass of the reconstructed Z
  double Z_pt = (muon_pos.v4() + muon_neg.v4()).pt();
  double Z_mass = (muon_pos.v4() + muon_neg.v4()).M();
  muon_invariant_mass->Fill(Z_mass,weight);

  //Pt of the recoiled jets is defined as the sum of all remaining jets that are not overlapping with the muons
  LorentzVector jets_v4;
  for(auto j:*jets)    jets_v4 +=j.v4(); 
   
  //calculate the parallel and perpendicular component of the recoil

  //unit vector
  double ZPx = (muon_pos.v4() + muon_neg.v4()).Px()/Z_pt;
  double ZPy = (muon_pos.v4() + muon_neg.v4()).Py()/Z_pt;
 
  if(berror) std::cout<<"DYHists:: unit vectors"<< "  ZPx "<<ZPx<<"  ZPy    "<< ZPy <<"  ZP  "<<Z_pt<<std::endl;
  if(berror)std::cout<<"upar2 " << ZPx * jets_v4.Px() + ZPy * jets_v4.Py()<<std::endl;
  double upar = ZPx * jets_v4.Px() + ZPy * jets_v4.Py();
  double uper= sqrt(jets_v4.pt()*jets_v4.pt() -upar * upar );
  if(berror) std::cout<<"DYHists::fill response"<<" -upar  " <<-upar<<" -upar/Z_pt  " <<-upar/Z_pt<<"  uper/Z_pt   "<<uper/Z_pt<<std::endl;

  response_ZP->Fill(Z_pt,-upar/Z_pt,weight);
  upar_ZPT->Fill(Z_pt,upar,weight);
  response_NPV->Fill(npvs,-upar/Z_pt,weight);
  rms_ZP->Fill(Z_pt,-upar-Z_pt,weight);
  rms_NPV->Fill(npvs,-upar-Z_pt,weight);
  rms_uper_ZP->Fill(Z_pt,uper,weight);
  rms_uper_NPV->Fill(npvs,uper,weight);

 
}

DYHists::~DYHists(){}
