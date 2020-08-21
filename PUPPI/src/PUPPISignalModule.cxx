#include <iostream>
#include <memory>

#include "UHH2/core/include/AnalysisModule.h"
#include "UHH2/core/include/Event.h"
#include "UHH2/common/include/CommonModules.h"
#include "UHH2/common/include/CleaningModules.h"
#include "UHH2/common/include/ElectronHists.h"
#include "UHH2/common/include/NSelections.h"
#include "UHH2/PUPPI/include/PUPPIHists.h"
#include "UHH2/common/include/PrintingModules.h"
#include <UHH2/common/include/JetCorrections.h>


using namespace std;
using namespace uhh2;

namespace uhh2examples {

/** \brief Basic analysis example of an AnalysisModule (formerly 'cycle') in UHH2
 * 
 * This is the central class which calls other AnalysisModules, Hists or Selection classes.
 * This AnalysisModule, in turn, is called (via AnalysisModuleRunner) by SFrame.
 */
class PUPPISignalModule: public AnalysisModule {
public:
    
    explicit PUPPISignalModule(Context & ctx);
    virtual bool process(Event & event) override;
    void init_TopJEC(uhh2::Context& ctx);

private:
    
    std::unique_ptr<CommonModules> common;
    std::unique_ptr<YearSwitcher> topjet_corrector_MC;
    std::shared_ptr<RunSwitcher> topjec_switcher_17UL;

    std::unique_ptr<AnalysisModule> printer;
    
    //std::unique_ptr<JetCleaner> jetcleaner;
    //std::unique_ptr<GenJetCleaner> genjetcleaner;
 
    //std::unique_ptr<Selection> njet_sel;
    
    std::unique_ptr<Hists> h_nocuts, h_njet;

};


void PUPPISignalModule::init_TopJEC(uhh2::Context& ctx){ 

    std::string jec_tag_2017UL, jec_ver_2017UL;
    std::string jec_jet_coll; 
    std::string top_puppi;
  
    jec_tag_2017UL = "Summer19UL17";
    jec_ver_2017UL = "5";

    jec_jet_coll = "AK8PFPuppi";
    //jec_jet_coll = "AK8PFchs";

    top_puppi = "toppuppijets";
    
    topjet_corrector_MC.reset(new YearSwitcher(ctx));

    topjet_corrector_MC->setup2017UL(std::make_shared<GenericTopJetCorrector>(ctx, JERFiles::JECFilesMC(jec_tag_2017UL, jec_ver_2017UL, jec_jet_coll),top_puppi));
    //topjet_corrector_MC->setup2017UL(std::make_shared<TopJetCorrector>(ctx, JERFiles::JECFilesMC(jec_tag_2017UL, jec_ver_2017UL, jec_jet_coll)));


}

PUPPISignalModule::PUPPISignalModule(Context & ctx){
    
    cout << "Hello World from PUPPISignalModule!" << endl;
    
    for(auto & kv : ctx.get_all()){
        cout << " " << kv.first << " = " << kv.second << endl;
    }


    init_TopJEC(ctx);


    // 1. setup other modules. CommonModules and the JetCleaner:
    common.reset(new CommonModules());
    common->disable_mcpileupreweight(); 
    common->disable_mclumiweight();
    common->disable_lumisel();
    common->disable_metfilters(); 
    common->disable_pvfilter();
    common->disable_jersmear();
    //common->disable_jec();
    common->disable_jetpfidfilter();
    common->init(ctx);


    //jetcleaner.reset(new JetCleaner(ctx, 20.0, 5)); 
    //genjetcleaner.reset(new GenJetCleaner(ctx, 20.0, 5)); 
    
    //njet_sel.reset(new NJetSelection(4)); // see common/include/NSelections.h

    h_nocuts.reset(new PUPPIHists(ctx, "NoCuts"));
    h_njet.reset(new PUPPIHists(ctx, "Njet"));


    // printer
    printer.reset(new GenParticlesPrinter(ctx));

}


bool PUPPISignalModule::process(Event & event) {

//if(event.event != 7517385) return false;
     
//    cout << "PUPPISignalModule: Starting to process event (runid, eventid) = (" << event.run << ", " << event.event << "); weight = " << event.weight << endl;

//    printer->process(event);

    topjet_corrector_MC->process(event);  
 
    common->process(event);
//    jetcleaner->process(event);
//    genjetcleaner->process(event);
 

    h_nocuts->fill(event);
    
//    bool njet_selection = njet_sel->passes(event);
//    if(njet_selection){
//        h_njet->fill(event);
//    }
//    // 3. decide whether or not to keep the current event in the output:
//    return njet_selection;



return true;
}

// as we want to run the ExampleCycleNew directly with AnalysisModuleRunner,
// make sure the PUPPISignalModule is found by class name. This is ensured by this macro:
UHH2_REGISTER_ANALYSIS_MODULE(PUPPISignalModule)

}
