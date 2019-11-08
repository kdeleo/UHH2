#include <iostream>
#include <memory>

#include "UHH2/core/include/AnalysisModule.h"
#include "UHH2/core/include/Event.h"
#include "UHH2/common/include/CommonModules.h"
#include "UHH2/common/include/CleaningModules.h"
#include <UHH2/common/include/MuonIds.h>
#include <UHH2/common/include/ElectronIds.h>
#include <UHH2/common/include/EventHists.h>
#include <UHH2/common/include/MuonHists.h>
#include <UHH2/common/include/ElectronHists.h>
#include <UHH2/common/include/JetHists.h>
#include "UHH2/common/include/NSelections.h"
#include <UHH2/common/include/MCWeight.h> 

#include "UHH2/common/include/PrintingModules.h"
#include "UHH2/common/include/EventVariables.h"
#include "UHH2/common/include/PrimaryLepton.h"


#include "UHH2/PUPPI_tuning/include/PuppiSelections.h"
#include "UHH2/PUPPI_tuning/include/PuppiJetHists.h"
#include "UHH2/PUPPI_tuning/include/DYHists.h"
#include "UHH2/PUPPI_tuning/include/EffPurityHists.h"


using namespace std;
using namespace uhh2;

namespace uhh2examples {

  class DYEffPurityModule: public AnalysisModule {
  public:
    
    explicit DYEffPurityModule(Context & ctx);
    virtual bool process(Event & event) override;

  private:
    bool berror; 
    TString filename;

    // split the sample in PU & LV enriched region:
    // normal == no splitting, puenriched: true == PU enriched region, false == LV enriched region
    bool puenriched;
    bool normal;

    //PUJetID
    TString pujetid_wp;

    //channel
    enum lepton { muon, elec };
    lepton channel_;
    std::string channel;

    std::unique_ptr<AnalysisModule> printer;
    ///////////////////////////    cleaner    /////////////////////////////////  

    std::unique_ptr<MuonCleaner> muo_cleaner; //pT>20, eta<2.4, Tight ID
    std::unique_ptr<ElectronCleaner> ele_cleaner; //pT>20, eta<2.4, Tight ID
    std::unique_ptr<JetCleaner> jet_cleaner; //pT>20, eta<5, No ID
    std::unique_ptr<GenJetCleaner> genjet_cleaner; //same as jet cleaner but on genjet collection

    std::unique_ptr<uhh2::AnalysisModule> jetmuonOverlapRemoval; //removes jets and generator jets that overlap with the muon from the Z Boson

    ///////////////////////////    Corrections    /////////////////////////////////  

    std::unique_ptr<CommonModules> common; // implemented year dependent in common modules

    ///////////////////////////    Selections    /////////////////////////////////  

    std::unique_ptr<uhh2::AndSelection> lep2_sel; //selectes depending on the channel two leptons and veto the other 
    std::unique_ptr<uhh2::Selection> Zmass_sel; //events with inv lepton mass around 20 GeV of the Z boson mass

    std::unique_ptr<uhh2::AnalysisModule> PUJetID_sel;  // PUJetID on top of CHS depending on WP in config
    std::unique_ptr<uhh2::AnalysisModule> PUjetsByDeltaPhi_sel; //criterion for PU enriched region
    std::unique_ptr<uhh2::AnalysisModule> LVjetsByDeltaPhi_sel; //criterion for LV enriched region
    ///////////////////////////    Hists    /////////////////////////////////  
    
    //default histogramms
    std::unique_ptr<uhh2::Hists> input_h_jet;
    std::unique_ptr<uhh2::Hists>input_h_event;
    std::unique_ptr<uhh2::Hists>input_h_muon;
    std::unique_ptr<uhh2::Hists>input_h_electron;

    std::unique_ptr<PuppiJetHists> h_jet_input; 
    std::unique_ptr<DYHists> h_DY_input;

    //output hist after selection
    std::unique_ptr<uhh2::Hists> output_h_jet;
    std::unique_ptr<uhh2::Hists>output_h_event;
    std::unique_ptr<uhh2::Hists>output_h_muon;
    std::unique_ptr<uhh2::Hists>output_h_electron;

    std::unique_ptr<PuppiJetHists> h_jet_output; 
    std::unique_ptr<DYHists> h_DY_output;
    std::unique_ptr<EffPurityHists> h_EffPurity_output;

    /////////////////////////////////  Quark Gluon Comparison  /////////////////////////////////////////
    std::unique_ptr<uhh2::AnalysisModule> QuarkGluonHandelMaker;
    std::unique_ptr<PuppiJetHists> h_quark_jet_output; 
    std::unique_ptr<EffPurityHists> h_quark_EffPurity_output;

    std::unique_ptr<PuppiJetHists> h_gluon_jet_output; 
    std::unique_ptr<EffPurityHists> h_gluon_EffPurity_output;

  
  };


  DYEffPurityModule::DYEffPurityModule(Context & ctx){
    // read configuration from config
    berror=(ctx.get("debug") == "true");
    filename =  ctx.get("dataset_version");
    puenriched = (ctx.get("puenriched") == "true");
    normal = (ctx.get("puenriched") == "none");
    pujetid_wp = ctx.get("pujetid_wp");

    // printer
    printer.reset(new GenParticlesPrinter(ctx));

    //channel
    channel = ctx.get("channel", "");
    if     (channel == "muon") channel_ = muon;
    else if(channel == "elec") channel_ = elec;
    else throw std::runtime_error("DYEffPurityModule -- undefined argument for 'channel' key in xml file (must be 'muon' or 'elec'): "+channel);

    ///////////////////////////    Cleaner    /////////////////////////////////  
    muo_cleaner.reset(new MuonCleaner    (AndId<Muon>    (PtEtaCut  (20., 2.4), MuonID(Muon::Tight))));
    ele_cleaner.reset(new ElectronCleaner    (AndId<Electron>    (ElectronID_Fall17_tight, PtEtaCut(30.0, 2.4))));
    jet_cleaner.reset(new JetCleaner(ctx,PtEtaCut( 20., 5)));
    genjet_cleaner.reset(new GenJetCleaner(ctx, 20., 5));
    jetmuonOverlapRemoval.reset(new AK4JetMuonOverlapRemoval(0.4));

    ///////////////////////////    Corrections    /////////////////////////////////  
    common.reset(new CommonModules());
    common->disable_mcpileupreweight(); //no PU reweighting needed since no comparisoon to data
    common->disable_lumisel(); //no lumi selection needed since no comparison to data
    common->disable_metfilters(); //no met filters apply to see every event, main effect on data
    common->disable_pvfilter(); //no pvfilter applied
    common->disable_jetpfidfilter(); //no jet ID applied to measure eff and purity without the effect of JetID
    common->switch_jetPtSorter();
    common->init(ctx);

    ///////////////////////////    Selections    /////////////////////////////////  
    lep2_sel.reset(new uhh2::AndSelection(ctx));

    if(channel_ == muon){

      lep2_sel->add<NMuonSelection>    ("muoN == 2", 2);
      lep2_sel->add<NElectronSelection>("eleN == 0", 0, 0);
    }
    else if(channel_ == elec){

      lep2_sel->add<NMuonSelection>    ("muoN == 0", 0, 0);
      lep2_sel->add<NElectronSelection>("eleN == 2", 2);
    }

    Zmass_sel.reset(new ZmassCut(20));

    // PuJetID and PU/LV enriched region
    PUJetID_sel.reset(new PUJetID(pujetid_wp));
    PUjetsByDeltaPhi_sel.reset(new PUjetsByDeltaPhi(1.5));
    LVjetsByDeltaPhi_sel.reset(new LVjetsByDeltaPhi(1.5));

    ///////////////////////////    Hists    /////////////////////////////////  
    input_h_jet.reset(new JetHists(ctx, "input_Jets"));
    input_h_event.reset(new EventHists (ctx, "input_Event"));
    input_h_muon.reset(new MuonHists (ctx, "input_Muon"));
    input_h_electron.reset(new ElectronHists (ctx, "input_Electron"));

    h_jet_input.reset(new PuppiJetHists(ctx, "jet_input"));
    h_DY_input.reset(new DYHists(ctx, "DY_input"));


    //output hist after selection
    output_h_jet.reset(new JetHists(ctx, "output_Jets"));
    output_h_event.reset(new EventHists (ctx, "output_Event"));
    output_h_muon.reset(new MuonHists (ctx, "output_Muon"));
    output_h_electron.reset(new ElectronHists (ctx, "output_Electron"));
   
    h_jet_output.reset(new PuppiJetHists(ctx, "jet_output"));
    h_DY_output.reset(new DYHists(ctx, "DY_output"));
    h_EffPurity_output.reset(new EffPurityHists(ctx, "EffPurity_output"));


    /////////////////////////////////  Quark Gluon Comparison  /////////////////////////////////////////
    QuarkGluonHandelMaker.reset(new QuarkGluonComparisoon(ctx));
    h_quark_jet_output.reset(new PuppiJetHists(ctx, "quark_jet_output","quarks"));
    h_quark_EffPurity_output.reset(new EffPurityHists(ctx, "quark_EffPurity_output","quarks"));

    h_gluon_jet_output.reset(new PuppiJetHists(ctx, "gluon_jet_output","gluons"));
    h_gluon_EffPurity_output.reset(new EffPurityHists(ctx, "gluon_EffPurity_output","gluons"));

  }


  bool DYEffPurityModule::process(Event & event) {
    if(berror){
      std::cout<<" "<<std::endl;
      std::cout<<" ====================    New Event   ===================="<< "muons  "<<event.muons->size()<< "  electrons  "<<event.electrons->size()<<std::endl;
      std::cout<<" "<<std::endl;
    }

    ///////////////////    quark gluon comparison  /////////////
    QuarkGluonHandelMaker->process(event);


    //    printer->process(event);

    /////////////////////////////     Trigger    /////////////////////////////////////
    uhh2::Event::TriggerIndex ti_HT;
    ti_HT=event.get_trigger_index("HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_v*");
    bool HT_trigger = event.passes_trigger(ti_HT);
    if(!HT_trigger) return false;

    ///////////////////////////////////////////////////      Input Hist    //////////////////////////////////////////
    input_h_jet   ->fill(event);
    input_h_event   ->fill(event);
    input_h_muon   ->fill(event);
    input_h_electron ->fill(event);

    h_jet_input->fill(event);
    h_DY_input->fill(event);

    ///////////////////////////////////////////////////      Cleaner    //////////////////////////////////////////
    if(berror) std::cout<<"cleaner"<<std::endl;
    muo_cleaner->process(event);
    ele_cleaner->process(event);
    jet_cleaner->process(event);
    genjet_cleaner->process(event);

    ///////////////////////////////////////////////////          Selection    //////////////////////////////////////////
    if(berror) std::cout<<"lepton selection"<<std::endl;
    bool pass_lep2 = lep2_sel->passes(event);
    if(!pass_lep2)return false;

    bool pass_ZmassCut = Zmass_sel->passes(event);
    if(!pass_ZmassCut)return false;

    ///////////////////////////////////////////////////          Correction and removal overlap    //////////////////////////////////////////
    //remove reco&gen jets that overlap with leptons
    jetmuonOverlapRemoval->process(event);
    if(berror) std::cout<<"correction and removal"<<std::endl;
    //apply JEC
    common->process(event);

    //apply PUJetID and split into PU enriched and LV enriched
    if(filename.Contains("CHS"))PUJetID_sel->process(event);
    if(!normal){
      if(puenriched)    PUjetsByDeltaPhi_sel->process(event);
      else    LVjetsByDeltaPhi_sel->process(event);
    }

    ///////////////////////////////////////////////////          Output Hist     //////////////////////////////////////////
    if(berror) std::cout<<"output hists"<<std::endl;
    output_h_jet   ->fill(event);
    output_h_event   ->fill(event);
    output_h_muon   ->fill(event);
    output_h_electron ->fill(event);

    h_jet_output->fill(event);
    h_DY_output->fill(event);
    h_EffPurity_output->fill(event);

    h_quark_jet_output->fill(event);
    h_quark_EffPurity_output->fill(event);

    h_gluon_jet_output->fill(event);
    h_gluon_EffPurity_output->fill(event);


    return true;
  }


  UHH2_REGISTER_ANALYSIS_MODULE(DYEffPurityModule)
}
