#include "UHH2/PUPPI_tuning/include/PuppiSelections.h"
#include "UHH2/core/include/Event.h"
#include "UHH2/common/include/CleaningModules.h"
#include "UHH2/common/include/Utils.h"



#include <stdexcept>

using namespace uhh2examples;
using namespace uhh2;
using namespace std;



/*
Selects Leptons around the Z boson mass peak.
Important for Z+Jets (DY+Jets) sample.
 */
ZmassCut::ZmassCut(double threshold_): threshold(threshold_){}
    
bool ZmassCut::passes(const Event & event ){

    if(event.muons){
      for(auto m:*event.muons){
	if(m.charge()==1){
	  lep_pos=m;
	  break;
	}
      }
      for(auto m:*event.muons){
	if(m.charge()==-1){
	  lep_neg=m;
	  break;
	}
      }
    }else if(event.electrons){
      for(auto m:*event.electrons){
	if(m.charge()==1){
	  lep_pos=m;
	  break;
	}
      }
      for(auto m:*event.electrons){
	if(m.charge()==-1){
	  lep_neg=m;
	  break;
	}
      }
    }


    inv_mass=(lep_pos.v4() + lep_neg.v4()).M();
    return abs(inv_mass-91)<threshold;

}

/*
Selects jets with a certain PU Jet ID WP. Applied on top of CHS.
Values represent 2016 training:
https://twiki.cern.ch/twiki/bin/viewauth/CMS/PileupJetID#Information_for_13_TeV_data_anal
https://indico.cern.ch/event/559594/contributions/2257924/attachments/1317046/1973307/PUID_JMAR_2016_07_26_v1.pdf
 */
PUJetID::PUJetID(TString threshold_): threshold(threshold_){}
    
bool PUJetID::process(Event & event ){
  vector<Jet> result;
  for(const auto jet : *event.jets ){
    double jet_pt = jet.pt();
    double jet_eta = abs(jet.eta());
    if(threshold.Contains("tight")){
      if(jet_pt <= 10 && jet_pt > 0){
	if(jet_eta <= 2.5 && jet_eta > 0){
	  if(0.26 < jet.pileupID()) result.push_back(jet);
	}else if(jet_eta <= 2.75 && jet_eta > 2.5){ 
	  if(-0.34 < jet.pileupID()) result.push_back(jet);
	}else if(jet_eta <= 3 && jet_eta > 2.75){ 
	  if(-0.24 < jet.pileupID()) result.push_back(jet);
	}else if(jet_eta <= 5 && jet_eta > 3){
	  if(-0.26 < jet.pileupID()) result.push_back(jet);
	}else{
	  result.push_back(jet);
	}
      }else if(jet_pt <= 20 && jet_pt > 10){ 
	if(jet_eta <= 2.5 && jet_eta > 0){
	  if(0.26< jet.pileupID()) result.push_back(jet);
	}else if(jet_eta <= 2.75 && jet_eta > 2.5){ 
	  if(-0.34 < jet.pileupID()) result.push_back(jet);
	}else if(jet_eta <= 3 && jet_eta > 2.75){ 
	  if(-0.24 < jet.pileupID()) result.push_back(jet);
	}else if(jet_eta <= 5 && jet_eta > 3){
	  if(-0.26 < jet.pileupID()) result.push_back(jet);
	}else{
	  result.push_back(jet);
	}
      }else if(jet_pt <= 30 && jet_pt > 20){ 
	if(jet_eta <= 2.5 && jet_eta > 0){
	  if(0.26< jet.pileupID()) result.push_back(jet);
	}else if(jet_eta <= 2.75 && jet_eta > 2.5){ 
	  if(-0.34 < jet.pileupID()) result.push_back(jet);
	}else if(jet_eta <= 3 && jet_eta > 2.75){ 
	  if(-0.24 < jet.pileupID()) result.push_back(jet);
	}else if(jet_eta <= 5 && jet_eta > 3){
	  if(-0.26 < jet.pileupID()) result.push_back(jet);
	}else{
	  result.push_back(jet);
	}
      }else if(jet_pt <= 50 && jet_pt > 30){ 
	if(jet_eta <= 2.5 && jet_eta > 0){
	  if(0.62< jet.pileupID()) result.push_back(jet);
	}else if(jet_eta <= 2.75 && jet_eta > 2.5){ 
	  if(-0.21 < jet.pileupID()) result.push_back(jet);
	}else if(jet_eta <= 3 && jet_eta > 2.75){ 
	  if(-0.07 < jet.pileupID()) result.push_back(jet);
	}else if(jet_eta <= 5 && jet_eta > 3){
	  if(-0.03 < jet.pileupID()) result.push_back(jet);
	}else{
	  result.push_back(jet);
	}
      }else{
	result.push_back(jet);
      }
    }else if(threshold.Contains("medium")){
      if(jet_pt <= 10 && jet_pt > 0){
	if(jet_eta <= 2.5 && jet_eta > 0){
	  if(-0.49 < jet.pileupID()) result.push_back(jet);
	}else if(jet_eta <= 2.75 && jet_eta > 2.5){ 
	  if(-0.53 < jet.pileupID()) result.push_back(jet);
	}else if(jet_eta <= 3 && jet_eta > 2.75){ 
	  if(-0.44 < jet.pileupID()) result.push_back(jet);
	}else if(jet_eta <= 5 && jet_eta > 3){
	  if(-0.42 < jet.pileupID()) result.push_back(jet);
	}else{
	  result.push_back(jet);
	}
      }else if(jet_pt <= 20 && jet_pt > 10){ 
	if(jet_eta <= 2.5 && jet_eta > 0){
	  if(-0.49 < jet.pileupID()) result.push_back(jet);
	}else if(jet_eta <= 2.75 && jet_eta > 2.5){ 
	  if(-0.53 < jet.pileupID()) result.push_back(jet);
	}else if(jet_eta <= 3 && jet_eta > 2.75){ 
	  if(-0.44 < jet.pileupID()) result.push_back(jet);
	}else if(jet_eta <= 5 && jet_eta > 3){
	  if(-0.42 < jet.pileupID()) result.push_back(jet);
	}else{
	  result.push_back(jet);
	}
      }else if(jet_pt <= 30 && jet_pt > 20){ 
	if(jet_eta <= 2.5 && jet_eta > 0){
	  if(-0.49 < jet.pileupID()) result.push_back(jet);
	}else if(jet_eta <= 2.75 && jet_eta > 2.5){ 
	  if(-0.53 < jet.pileupID()) result.push_back(jet);
	}else if(jet_eta <= 3 && jet_eta > 2.75){ 
	  if(-0.44 < jet.pileupID()) result.push_back(jet);
	}else if(jet_eta <= 5 && jet_eta > 3){
	  if(-0.42 < jet.pileupID()) result.push_back(jet);
	}else{
	  result.push_back(jet);
	}
      }else if(jet_pt <= 50 && jet_pt > 30){ 
	if(jet_eta <= 2.5 && jet_eta > 0){
	  if(-0.06 < jet.pileupID()) result.push_back(jet);
	}else if(jet_eta <= 2.75 && jet_eta > 2.5){ 
	  if(-0.42 < jet.pileupID()) result.push_back(jet);
	}else if(jet_eta <= 3 && jet_eta > 2.75){ 
	  if(-0.3 < jet.pileupID()) result.push_back(jet);
	}else if(jet_eta <= 5 && jet_eta > 3){
	  if(-0.23 < jet.pileupID()) result.push_back(jet);
	}else{
	  result.push_back(jet);
	}
      }else{
	result.push_back(jet);
      }
    }else if(threshold.Contains("loose")){
      if(jet_pt <= 10 && jet_pt > 0){
	if(jet_eta <= 2.5 && jet_eta > 0){
	  if(-0.96 < jet.pileupID()) result.push_back(jet);
	}else if(jet_eta <= 2.75 && jet_eta > 2.5){ 
	  if(-0.64 < jet.pileupID()) result.push_back(jet);
	}else if(jet_eta <= 3 && jet_eta > 2.75){ 
	  if(-0.56 < jet.pileupID()) result.push_back(jet);
	}else if(jet_eta <= 5 && jet_eta > 3){
	  if(-0.54 < jet.pileupID()) result.push_back(jet);
	}else{
	  result.push_back(jet);
	}
      }else if(jet_pt <= 20 && jet_pt > 10){ 
	if(jet_eta <= 2.5 && jet_eta > 0){
	  if(-0.96 < jet.pileupID()) result.push_back(jet);
	}else if(jet_eta <= 2.75 && jet_eta > 2.5){ 
	  if(-0.64 < jet.pileupID()) result.push_back(jet);
	}else if(jet_eta <= 3 && jet_eta > 2.75){ 
	  if(-0.56 < jet.pileupID()) result.push_back(jet);
	}else if(jet_eta <= 5 && jet_eta > 3){
	  if(-0.54 < jet.pileupID()) result.push_back(jet);
	}else{
	  result.push_back(jet);
	}
      }else if(jet_pt <= 30 && jet_pt > 20){ 
	if(jet_eta <= 2.5 && jet_eta > 0){
	  if(-0.96 < jet.pileupID()) result.push_back(jet);
	}else if(jet_eta <= 2.75 && jet_eta > 2.5){ 
	  if(-0.64 < jet.pileupID()) result.push_back(jet);
	}else if(jet_eta <= 3 && jet_eta > 2.75){ 
	  if(-0.56 < jet.pileupID()) result.push_back(jet);
	}else if(jet_eta <= 5 && jet_eta > 3){
	  if(-0.54 < jet.pileupID()) result.push_back(jet);
	}else{
	  result.push_back(jet);
	}
      }else if(jet_pt <= 50 && jet_pt > 30){ 
	if(jet_eta <= 2.5 && jet_eta > 0){
	  if(-0.92 < jet.pileupID()) result.push_back(jet);
	}else if(jet_eta <= 2.75 && jet_eta > 2.5){ 
	  if(-0.56 < jet.pileupID()) result.push_back(jet);
	}else if(jet_eta <= 3 && jet_eta > 2.75){ 
	  if(-0.44 < jet.pileupID()) result.push_back(jet);
	}else if(jet_eta <= 5 && jet_eta > 3){
	  if(-0.39 < jet.pileupID()) result.push_back(jet);
	}else{
	  result.push_back(jet);
	}
      }else{
	result.push_back(jet);
      }
    }else{
	result.push_back(jet);
    }
  }

  event.jets->clear();
  event.jets->reserve(result.size());
  for(auto jet:result) event.jets->push_back(jet);
  return true;
}

/*
Devides the jets into PU enriched and LV enriched based on DeltaPhi.
Typically PU enriched is DelTaPhi< 1.5 and LV enriched is DeltaPhi > 2.5.
Does it for both generator and reco jets.
 */
PUjetsByDeltaPhi::PUjetsByDeltaPhi(double threshold_): threshold(threshold_){}
    
bool PUjetsByDeltaPhi::process(Event & event ){
  if(event.muons->size()!=2) return false;
  
  LorentzVector z = event.muons->at(0).v4()+event.muons->at(1).v4();

  vector<Jet> result;
  for(const auto jet : *event.jets ){
    double deltaphi = deltaPhi(jet, z);
    if(deltaphi < threshold) result.push_back(jet);
  }

  event.jets->clear();
  event.jets->reserve(result.size());
  for(auto jet:result) event.jets->push_back(jet);

  //and for genjets
  vector<GenJet> result_gen;
  for(const auto jet : *event.genjets ){
    double deltaphi = deltaPhi(jet, z);
    if(deltaphi < threshold) result_gen.push_back(jet);
  }

  event.genjets->clear();
  event.genjets->reserve(result.size());
  for(auto jet:result_gen) event.genjets->push_back(jet);

  return true;
}

/*
Devides the jets into PU enriched and LV enriched based on DeltaPhi.
Typically PU enriched is DelTaPhi< 1.5 and LV enriched is DeltaPhi > 2.5.
Does it for both generator and reco jets.
 */
LVjetsByDeltaPhi::LVjetsByDeltaPhi(double threshold_): threshold(threshold_){}
    
bool LVjetsByDeltaPhi::process(Event & event ){
  if(event.muons->size()!=2) return false;
  
  LorentzVector z = event.muons->at(0).v4()+event.muons->at(1).v4();

  vector<Jet> result;
  for(const auto jet : *event.jets ){
    double deltaphi = deltaPhi(jet, z);
    if(deltaphi > threshold) result.push_back(jet);
  }

  event.jets->clear();
  event.jets->reserve(result.size());
  for(auto jet:result) event.jets->push_back(jet);

  //and for genjets
  vector<GenJet> result_gen;
  for(const auto jet : *event.genjets ){
    double deltaphi = deltaPhi(jet, z);
    if(deltaphi > threshold) result_gen.push_back(jet);
  }

  event.genjets->clear();
  event.genjets->reserve(result.size());
  for(auto jet:result_gen) event.genjets->push_back(jet);

  return true;
}

/*
Seperate the jets in quark and gluon jets, to see differences in eff and purity.
Quarks are split in light and all.
Splitted by closest genparticles PDGId.
Done for both, generator and reco jets, with the same method.
 */
QuarkGluonComparisoon::QuarkGluonComparisoon(Context & ctx) {
    h_quarks= ctx.declare_event_output< std::vector<Jet> > ("quarks");
    h_gluons= ctx.declare_event_output< std::vector<Jet> > ("gluons");
    h_quarks_udsc= ctx.declare_event_output< std::vector<Jet> > ("quarks_udsc");

    h_genquarks= ctx.declare_event_output< std::vector<GenJet> > ("genquarks");
    h_gengluons= ctx.declare_event_output< std::vector<GenJet> > ("gengluons");
    h_genquarks_udsc= ctx.declare_event_output< std::vector<GenJet> > ("genquarks_udsc");

    isMC = (ctx.get("dataset_type") == "MC");
}
    
bool QuarkGluonComparisoon::process(Event & event ){
  std::unique_ptr< std::vector<Jet> > gluon_jets(new std::vector<Jet> (*event.jets));
  std::unique_ptr< std::vector<Jet> > quark_jets(new std::vector<Jet> (*event.jets));
  std::unique_ptr< std::vector<Jet> > quark_udsc_jets(new std::vector<Jet> (*event.jets));

  quark_udsc_jets->clear();
  gluon_jets->clear();
  quark_jets->clear();
  gluon_jets->reserve(event.jets->size());
  quark_jets->reserve(event.jets->size());
  quark_udsc_jets->reserve(event.jets->size());
  
  for(auto jet:*event.jets){
    double deltar_min=1000;
    GenParticle closest_genp;

    if(isMC){

      // find clostest genp
      for(auto genp:*event.genparticles){
	double deltar = deltaR(jet,genp);
	if(deltar < deltar_min){
	  deltar_min = deltar;
	  closest_genp = genp;
	  }
      }

      // check closest genp pgdID
      if(abs(closest_genp.pdgId())==21) {
	gluon_jets->push_back(jet);
      }
      else if(abs(closest_genp.pdgId()) <5) {
	quark_jets->push_back(jet);     
	quark_udsc_jets->push_back(jet);
      }else{
	quark_jets->push_back(jet);
      }
    }
  }

  // set quarks and gluons handel
  event.set(h_quarks,*quark_jets);
  event.set(h_quarks_udsc,*quark_udsc_jets );
  event.set(h_gluons,*gluon_jets);


  ///////////    gen jets
  std::unique_ptr< std::vector<GenJet> > gengluon_jets(new std::vector<GenJet> (*event.genjets));
  std::unique_ptr< std::vector<GenJet> > genquark_jets(new std::vector<GenJet> (*event.genjets));
  std::unique_ptr< std::vector<GenJet> > genquark_udsc_jets(new std::vector<GenJet> (*event.genjets));

  genquark_udsc_jets->clear();
  gengluon_jets->clear();
  genquark_jets->clear();
  gengluon_jets->reserve(event.genjets->size());
  genquark_jets->reserve(event.genjets->size());
  genquark_udsc_jets->reserve(event.genjets->size());
  
  for(auto jet:*event.genjets){
    double deltar_min=1000;
    GenParticle closest_genp;

    if(isMC){

      // find clostest genp
      for(auto genp:*event.genparticles){
	double deltar = deltaR(jet,genp);
	if(deltar < deltar_min){
	  deltar_min = deltar;
	  closest_genp = genp;
	  }
      }

      // check closest genp pgdID
      if(abs(closest_genp.pdgId())==21) {
	gengluon_jets->push_back(jet);
      }
      else if(abs(closest_genp.pdgId()) <5) {
	genquark_jets->push_back(jet);     
	genquark_udsc_jets->push_back(jet);
      }else{
	genquark_jets->push_back(jet);
      }
    }
  }

  // set genquarks and gengluons handel
  event.set(h_genquarks,*genquark_jets);
  event.set(h_genquarks_udsc,*genquark_udsc_jets );
  event.set(h_gengluons,*gengluon_jets);
  
  return true;
}

/*
Removes the reconstructed jets that are overlapping with the muons from the Z boson decay.
This jets decrease the purity since generator jets do not have an overlap with the muons.
 */
AK4JetMuonOverlapRemoval::AK4JetMuonOverlapRemoval(double threshold_): threshold(threshold_){}
    
bool AK4JetMuonOverlapRemoval::process(Event & event ){
  assert(event.muons);
  assert(event.jets);

  vector<Muon> muons = *event.muons;
  if(muons.size()!=2) return false;

  vector<Jet> result;
  for(const auto jet : *event.jets ){
    for(const auto muon:muons){
      double del = deltaR(jet,muon);
      if(del>threshold) result.push_back(jet); 
    }
  }

  event.jets->clear();
  for(auto jet:result) event.jets->push_back(jet);
  return true;
}

/*
Cuts on the reconstructed Z pT.
 */
ZptCut::ZptCut(double threshold_,TString minmax_): threshold(threshold_),minmax(minmax_){}
    
bool ZptCut::passes(const Event & event){
  if(event.muons){
      for(auto m:*event.muons){
	if(m.charge()==1){
	  lep_pos=m;
	  break;
	}
      }
      for(auto m:*event.muons){
	if(m.charge()==-1){
	  lep_neg=m;
	  break;
	}
      }
    }else if(event.electrons){
      for(auto m:*event.electrons){
	if(m.charge()==1){
	  lep_pos=m;
	  break;
	}
      }
      for(auto m:*event.electrons){
	if(m.charge()==-1){
	  lep_neg=m;
	  break;
	}
      }
    }

    inv_pt=(lep_pos.v4() + lep_neg.v4()).pt();
    if(minmax=="max") return inv_pt<threshold;
    if(minmax=="min") return inv_pt>threshold;
    return inv_pt<threshold;
}



/*
Select only events with a certain number of True Interaction. Especially helpfull if you want to compare different years, since all three years have different PU profiles.
 */
TrueInteractionSelection::TrueInteractionSelection(double minTrueInteraction_, double maxTrueInteraction_):minTrueInteraction(minTrueInteraction_),maxTrueInteraction(maxTrueInteraction_){}

bool TrueInteractionSelection::passes(const Event & event){

  double trueInteraction = event.genInfo->pileup_TrueNumInteractions();
  if(trueInteraction>=minTrueInteraction && trueInteraction<=maxTrueInteraction) return true;
  else return false;

}
