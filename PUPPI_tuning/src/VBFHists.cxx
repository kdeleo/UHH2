#include "UHH2/PUPPI_tuning/include/VBFHists.h"
#include "UHH2/core/include/Event.h"
#include "UHH2/core/include/GenParticle.h"
#include "UHH2/core/include/LorentzVector.h"
#include "UHH2/common/include/Utils.h"

#include <iostream>

using namespace std;
using namespace uhh2;
using namespace uhh2examples;

VBFHists::VBFHists(Context & ctx, const string & dirname): Hists(ctx, dirname){
  berror=(ctx.get("debug") == "true");
  
  ////////////////////////////////////  General  /////////////////////////////////////////////
  DeltaEta = book<TH1F>("DeltaEta","DeltaEta between the leading AK4 jets",50,0,5);
  invMass = book<TH1F>("invMass","inv MAss btween the leading AK4 jets",3000,0,3000);

}


void VBFHists::fill(const Event & event){
  if(berror) std::cout<<"in VBFHists  "<<std::endl;

  assert(event.jets);
  sort_by_pt<Jet>(*event.jets);
  double weight = event.weight;

  if(event.jets->size()<2) throw "VBF Selection not applied!";

  //// DeltaEta
  double eta1 = event.jets->at(0).eta();
  double eta2 = event.jets->at(1).eta();
  DeltaEta->Fill(abs(eta1-eta2),weight);


  //// invMass
  LorentzVector v41 = event.jets->at(0).v4();
  LorentzVector v42 = event.jets->at(1).v4();
  double invmass = 0;
  if( (v41+v42).isTimelike() )
    invmass=(v41+v42).M();
  else{
    invmass=sqrt(-(v41+v42).mass2());
  }
  invMass->Fill(invmass,weight);

}



VBFHists::~VBFHists(){}
