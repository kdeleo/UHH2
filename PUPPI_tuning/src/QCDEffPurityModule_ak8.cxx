#include <iostream>
#include <memory>

#include "UHH2/core/include/AnalysisModule.h"
#include "UHH2/core/include/Event.h"
#include "UHH2/common/include/CommonModules.h"
#include "UHH2/common/include/CleaningModules.h"
#include "UHH2/common/include/ElectronHists.h"
#include "UHH2/common/include/NSelections.h"
#include <UHH2/common/include/EventHists.h>
#include <UHH2/common/include/MuonHists.h>
#include <UHH2/common/include/ElectronHists.h>
#include <UHH2/common/include/JetHists.h>
#include "UHH2/common/include/NSelections.h"
#include <UHH2/common/include/LumiSelection.h>
#include "UHH2/common/include/PrintingModules.h"
#include <UHH2/common/include/MCWeight.h>
#include <UHH2/common/include/MuonIds.h>
#include "UHH2/common/include/EventVariables.h"
#include "UHH2/common/include/PrimaryLepton.h"


//#include "UHH2/PUPPI_tuning/include/PuppiSelections.h"
#include "UHH2/PUPPI_tuning/include/PuppiJetHists_ak8.h"
//#include "UHH2/PUPPI_tuning/include/DYHists.h"
//#include "UHH2/PUPPI_tuning/include/VBFHists.h"
//#include "UHH2/PUPPI_tuning/include/EffPurityHists.h"


using namespace std;
using namespace uhh2;

namespace uhh2examples {


class QCDEffPurityModule_ak8: public AnalysisModule {
public:
    
    explicit QCDEffPurityModule_ak8(Context & ctx);
    virtual bool process(Event & event) override;

private:
    
  bool berror; 
  bool isMC;

  ///////////////////////////    Corrections    /////////////////////////////////  
   std::unique_ptr<CommonModules> common; // implemented year dependent in common modules
    // std::unique_ptr<JetCorrector> jet_corrector;

  ///////////////////////////    Selections    /////////////////////////////////  
  //std::unique_ptr<JetCleaner> jet_pt_10_sel;
  //std::unique_ptr<JetCleaner> jet_pt_15_sel;
  //std::unique_ptr<JetCleaner> jet_pt_20_sel;

  ///////////////////////////    Hists    /////////////////////////////////  
  //default histogramms
  //std::unique_ptr<uhh2::Hists> input_h_jet;
  //std::unique_ptr<uhh2::Hists>input_h_event;
  //std::unique_ptr<PuppiJetHists> h_jet_input; 


  //std::unique_ptr<EffPurityHists> h_jet_pt_8;
  //std::unique_ptr<EffPurityHists> h_jet_pt_8_wJEC;

  //std::unique_ptr<EffPurityHists> h_jet_pt_8_2jetEvents;
  //
  //std::unique_ptr<PuppiJetHists> h_puppi_jet_pt_8;
  std::unique_ptr<PuppiJetHists_ak8> h_puppi_jet_pt_8_wJEC;
  //std::unique_ptr<PuppiJetHists> h_puppi_jet_pt_10;
  //std::unique_ptr<PuppiJetHists> h_puppi_jet_pt_15;
  //std::unique_ptr<PuppiJetHists> h_puppi_jet_pt_20;


  /////////////////////////////////  Quark Gluon Comparison  /////////////////////////////////////////
  //std::unique_ptr<uhh2::AnalysisModule> QuarkGluonHandelMaker;
  //std::unique_ptr<EffPurityHists> h_quark_pt_8;
  //std::unique_ptr<EffPurityHists> h_quark_pt_8_wJEC;
  //std::unique_ptr<PuppiJetHists> h_puppi_quark_pt_8;
  //std::unique_ptr<PuppiJetHists> h_puppi_quark_pt_8_wJEC;
  //std::unique_ptr<EffPurityHists> h_gluon_pt_8;
  //std::unique_ptr<EffPurityHists> h_gluon_pt_8_wJEC;
  //std::unique_ptr<PuppiJetHists> h_puppi_gluon_pt_8;
  //std::unique_ptr<PuppiJetHists> h_puppi_gluon_pt_8_wJEC;
  


  TString filename;
};


QCDEffPurityModule_ak8::QCDEffPurityModule_ak8(Context & ctx){
    berror=(ctx.get("debug") == "true");

    isMC = (ctx.get("dataset_type") == "MC");
    filename =  ctx.get("dataset_version");

    ///////////////////////////    Corrections    /////////////////////////////////  
    common.reset(new CommonModules());
    common->disable_mcpileupreweight(); //no PU reweighting needed since no comparisoon to data
    common->disable_lumisel(); //no lumi selection needed since no comparison to data
    common->disable_metfilters(); //no met filters apply to see every event, main effect on data
    common->disable_pvfilter(); //no pvfilter applied
    common->disable_jetpfidfilter(); //no jet ID applied to measure eff and purity without the effect of JetID
    common->switch_jetPtSorter();
    common->disable_mclumiweight();
    common->disable_jersmear();
    common->init(ctx);

    // isMC = (ctx.get("dataset_type") == "MC");
    // filename =  ctx.get("dataset_version");
    // std::vector<std::string> JEC_AK4, JEC_AK8;
    // if(isMC){
    //   JEC_AK4 = JERFiles::Summer16_07Aug2017_V11_L123_AK4PFPuppi_MC;
    //   JEC_AK8 = JERFiles::Summer16_07Aug2017_V11_L123_AK8PFPuppi_MC;
      
    //   jet_corrector.reset(new JetCorrector(ctx, JEC_AK4));
    // }

    ///////////////////////////    Selections    /////////////////////////////////  
    /*
      Selections for different pt cuts on the jets
     */
    //jet_pt_10_sel.reset(new JetCleaner(ctx,PtEtaCut( 10., 11)));
    //jet_pt_15_sel.reset(new JetCleaner(ctx,PtEtaCut( 15., 11)));
    //jet_pt_20_sel.reset(new JetCleaner(ctx,PtEtaCut( 20., 11)));


    ///////////////////////////    Hists    /////////////////////////////////  
    //input_h_jet.reset(new JetHists     (ctx, "input_Jets"));
    //input_h_event.reset(new EventHists (ctx, "input_Event"));

    //h_jet_input.reset(new PuppiJetHists(ctx, "jet_input"));

    //h_jet_pt_8.reset(new EffPurityHists(ctx, "jet_pt_8"));
    //h_jet_pt_8_wJEC.reset(new EffPurityHists(ctx, "jet_pt_8_wJEC"));
    //h_puppi_jet_pt_8.reset(new PuppiJetHists(ctx, "puppi_jet_pt_8"));
    h_puppi_jet_pt_8_wJEC.reset(new PuppiJetHists_ak8(ctx, "puppi_jet_pt_8_wJEC"));
    //h_puppi_jet_pt_10.reset(new PuppiJetHists(ctx, "puppi_jet_pt_10"));
    //h_puppi_jet_pt_15.reset(new PuppiJetHists(ctx, "puppi_jet_pt_15"));
    //h_puppi_jet_pt_20.reset(new PuppiJetHists(ctx, "puppi_jet_pt_20"));

    /////////////////////////////////  Quark Gluon Comparison  /////////////////////////////////////////
    //QuarkGluonHandelMaker.reset(new QuarkGluonComparisoon(ctx));
    //h_quark_pt_8.reset(new EffPurityHists(ctx, "quark_pt_8","quarks"));
    //h_quark_pt_8_wJEC.reset(new EffPurityHists(ctx, "quark_pt_8_wJEC","quarks"));
    //h_puppi_quark_pt_8.reset(new PuppiJetHists(ctx, "puppi_quark_pt_8","quarks"));
    //h_puppi_quark_pt_8_wJEC.reset(new PuppiJetHists(ctx, "puppi_quark_pt_8_wJEC","quarks"));


    //h_gluon_pt_8.reset(new EffPurityHists(ctx, "gluon_pt_8","gluons"));
    //h_gluon_pt_8_wJEC.reset(new EffPurityHists(ctx, "gluon_pt_8_wJEC","gluons"));
    //h_puppi_gluon_pt_8.reset(new PuppiJetHists(ctx, "puppi_gluon_pt_8","gluons"));
    //h_puppi_gluon_pt_8_wJEC.reset(new PuppiJetHists(ctx, "puppi_gluon_pt_8_wJEC","gluons"));


}


bool QCDEffPurityModule_ak8::process(Event & event) {
    if(berror)std::cout<<" ====================    New Event   ===================="<<std::endl;

    ///////////////////    quark gluon comparison  /////////////
    //QuarkGluonHandelMaker->process(event);


    //    JEC anwenden
    if(isMC){
      common->process(event);
    }
    // if(isMC){
    //   jet_corrector->process(event);
    // }



    //input_h_jet   ->fill(event);
    //input_h_event   ->fill(event);
    //h_jet_pt_8_wJEC->fill(event);
    //h_quark_pt_8_wJEC->fill(event);
    //h_gluon_pt_8_wJEC->fill(event);
    h_puppi_jet_pt_8_wJEC->fill(event);
    //h_puppi_quark_pt_8_wJEC->fill(event);
    //h_puppi_gluon_pt_8_wJEC->fill(event);

    ///////////////////////////   Jets remove JEC correction  ///////////////////////////////// 
    /*
      remove the JEC to look at uncorrected jets.
     */
    //for (auto & jet:*event.jets){
    //  jet.set_v4(jet.v4() * jet.JEC_factor_raw() );
    //  jet.set_JEC_factor_raw(1);
    //}

    /*
      uncleaned jets are the original jets. In the following we only consider jets with pT> 10/20 GeV. But this collection always have all jets.
     */
 /*   std::unique_ptr< std::vector<Jet> > uncleaned_jets (new std::vector<Jet> (*event.jets));

    h_jet_pt_8->fill(event);
    h_puppi_jet_pt_8->fill(event);

    h_quark_pt_8->fill(event);
    h_puppi_quark_pt_8->fill(event);

    h_gluon_pt_8->fill(event);
    h_puppi_gluon_pt_8->fill(event);


    ///////////// raw pT>10
    jet_pt_10_sel->process(event);
    h_puppi_jet_pt_10->fill(event);

    event.jets->clear();
    event.jets->reserve(uncleaned_jets->size());
    for(const auto & j : *uncleaned_jets) event.jets->push_back(j); 
    sort_by_pt<Jet>(*event.jets);

    ///////////// raw pT>15
    jet_pt_15_sel->process(event);
    h_puppi_jet_pt_15->fill(event);

    event.jets->clear();
    event.jets->reserve(uncleaned_jets->size());
    for(const auto & j : *uncleaned_jets) event.jets->push_back(j); 
    sort_by_pt<Jet>(*event.jets);

    ///////////// raw pT>20
    jet_pt_20_sel->process(event);
    h_puppi_jet_pt_20->fill(event);

    event.jets->clear();
    event.jets->reserve(uncleaned_jets->size());
    for(const auto & j : *uncleaned_jets) event.jets->push_back(j); 
    sort_by_pt<Jet>(*event.jets);
*/
    return true;


}

// as we want to run the ExampleCycleNew directly with AnalysisModuleRunner,
// make sure the QCDEffPurityModule is found by class name. This is ensured by this macro:
UHH2_REGISTER_ANALYSIS_MODULE(QCDEffPurityModule_ak8)

}
