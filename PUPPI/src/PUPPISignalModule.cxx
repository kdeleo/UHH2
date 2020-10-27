#include <iostream>
#include <memory>

#include "UHH2/core/include/AnalysisModule.h"
#include "UHH2/core/include/Event.h"
#include "UHH2/common/include/CommonModules.h"
#include "UHH2/common/include/CleaningModules.h"
#include "UHH2/common/include/ElectronHists.h"
#include "UHH2/common/include/NSelections.h"
#include "UHH2/common/include/PrintingModules.h"
#include <UHH2/common/include/JetCorrections.h>
#include <UHH2/common/include/TopJetIds.h>

#include "UHH2/PUPPI/include/PUPPIHists.h"
#include "UHH2/PUPPI/include/TopPuppiJetCorrections.h"

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

private:
    
    std::unique_ptr<CommonModules> common;
    std::unique_ptr<TopPuppiJetCorrections> toppuppi_JEC;

    std::unique_ptr<AnalysisModule> printer;
    
    std::unique_ptr<TopJetCleaner> topjet_puppi_cleaner, topjet_cleaner;
 
    //std::unique_ptr<Selection> njet_sel;
    
    std::unique_ptr<Hists> h_nocuts, h_njet;

};


PUPPISignalModule::PUPPISignalModule(Context & ctx){
    
    cout << "Hello World from PUPPISignalModule!" << endl;
    
    for(auto & kv : ctx.get_all()){
        cout << " " << kv.first << " = " << kv.second << endl;
    }



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
    common->switch_jetPtSorter(true);
    common->init(ctx);

    toppuppi_JEC.reset(new TopPuppiJetCorrections());
    toppuppi_JEC->init(ctx);

    topjet_puppi_cleaner.reset(new TopJetCleaner(ctx, TopJetId(PtEtaCut(200.0, 2.0)), "toppuppijets"));
    topjet_cleaner.reset(new TopJetCleaner(ctx, TopJetId(PtEtaCut(200.0, 2.0)), "topjets"));
    
    //njet_sel.reset(new NJetSelection(4)); // see common/include/NSelections.h

    h_nocuts.reset(new PUPPIHists(ctx, "NoCuts"));
    h_njet.reset(new PUPPIHists(ctx, "Njet"));


    // printer
    printer.reset(new GenParticlesPrinter(ctx));

}


bool PUPPISignalModule::process(Event & event) {

//if(event.event != 7517385) return false;
     
    cout << "PUPPISignalModule: Starting to process event (runid, eventid) = (" << event.run << ", " << event.event << "); weight = " << event.weight << endl;

    cout << "Before common modules"<< endl;

    for (const Jet & jet: *event.jets) { 
    cout << "-- jet pT = " << jet.pt() << endl;
    }
    for (const TopJet & toppuppijet: *event.toppuppijets) { 
    cout << "-- Top jet pT = " << toppuppijet.pt() << endl;
    }

    //printer->process(event);
 

    bool common_module = common->process(event);
    if(!common_module) return false;

    cout << "After common modules"<< endl;

    for (const Jet & jet: *event.jets) { 
    cout << "-- jet pT = " << jet.pt() << endl;
    }
    for (const TopJet & toppuppijet: *event.toppuppijets) { 
    cout << "-- Top jet pT = " << toppuppijet.pt() << endl;
    }

    toppuppi_JEC->process(event);

    cout << "After topjet correction"<< endl;

    for (const Jet & jet: *event.jets) { 
    cout << "-- jet pT = " << jet.pt() << endl;
    }
    for (const TopJet & toppuppijet: *event.toppuppijets) { 
    cout << "-- Top jet pT = " << toppuppijet.pt() << endl;
    }


    topjet_puppi_cleaner->process(event);
    //topjet_cleaner->process(event);
 
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
