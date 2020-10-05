#include "UHH2/common/include/JetCorrections.h"
#include "UHH2/common/include/JetCorrectionSets.h"
#include "UHH2/common/include/MCWeight.h"
#include "UHH2/common/include/EventVariables.h"
#include "UHH2/common/include/CleaningModules.h"
#include "UHH2/common/include/EventVariables.h"
#include "UHH2/common/include/LumiSelection.h"
#include "UHH2/common/include/Utils.h"
#include "UHH2/common/include/TriggerSelection.h"

#include "UHH2/PUPPI/include/PUPPITopPuppiJetCorrections.h"

using namespace uhh2;
using namespace std;

PUPPITopPuppiJetCorrections::PUPPITopPuppiJetCorrections(){

    jec_tag_2017UL = "Summer19UL17";
    jec_ver_2017UL = "5";

    //jec_jet_coll = "AK8PFPuppi";
    jec_jet_coll = "dummy";
    //jec_subjet_coll = "AK4PFPuppi";

    top_puppi = "toppuppijets";
}

void PUPPITopPuppiJetCorrections::init(Context & ctx){

    std::string userTopJetColl = string2lowercase(ctx.get("TopJetCollection"));
    std::string algo = "";
    std::string pus = "PFchs";

    if(userTopJetColl.find("ak8") != std::string::npos){
    algo = "AK8";
    }
    else if (userTopJetColl.find("ak8") == std::string::npos) {
    std::cout << "TopJetCorrections.cxx: Cannot determine tjet cone + radius (neither AK4 nor AK8) - going to assume it is AK8 for JECs" << '\n';
    algo = "AK8";
    }

    if (userTopJetColl.find("puppi") != std::string::npos) {
    pus = "PFPuppi";
    }
    else if (userTopJetColl.find("chs") == std::string::npos) {
    std::cout << "Cannot determine pileup subtraction (neither CHS nor PUPPI) - going to assume it is CHS for JECs" << std::endl;
    }

    jec_jet_coll = algo + pus;

    topjet_corrector_MC.reset(new YearSwitcher(ctx));
    //topjet_subjet_corrector_MC.reset(new YearSwitcher(ctx));

    //topjet_corrector_MC->setup2017UL(std::make_shared<GenericTopJetCorrector>(ctx, JERFiles::JECFilesMC(jec_tag_2017UL, jec_ver_2017UL, jec_jet_coll),top_puppi));
    //topjet_subjet_corrector_MC->setup2017UL(std::make_shared<GenericSubJetCorrector>(ctx, JERFiles::JECFilesMC(jec_tag_2017UL, jec_ver_2017UL, jec_subjet_coll),top_puppi));
    topjet_corrector_MC->setup2017UL(std::make_shared<TopJetCorrector>(ctx, JERFiles::JECFilesMC(jec_tag_2017UL, jec_ver_2017UL, jec_jet_coll)));
    //topjet_subjet_corrector_MC->setup2017UL(std::make_shared<SubJetCorrector>(ctx, JERFiles::JECFilesMC(jec_tag_2017UL, jec_ver_2017UL, jec_subjet_coll)));
  
    std::cout << "UL17 JECs: " << jec_tag_2017UL << " V" << jec_ver_2017UL << " for " << jec_jet_coll << endl;

}

bool PUPPITopPuppiJetCorrections::process(uhh2::Event & event){
    topjet_corrector_MC->process(event);
    //topjet_subjet_corrector_MC->process(event);

  return true;
}
