#include "../include/TopPuppiJetCorrections.h"
#include "UHH2/common/include/JetCorrections.h"
#include "UHH2/common/include/JetCorrectionSets.h"
#include "UHH2/common/include/MCWeight.h"
#include "UHH2/common/include/EventVariables.h"
#include "UHH2/common/include/CleaningModules.h"
#include "UHH2/common/include/EventVariables.h"
#include "UHH2/common/include/LumiSelection.h"
#include "UHH2/common/include/Utils.h"
#include "UHH2/common/include/TriggerSelection.h"

using namespace uhh2;
using namespace std;

void TopPuppiJetCorrections::fail_if_init() const{
  if(init_done){
    throw invalid_argument("TopPuppiJetCorrections::init already called!");
  }
}


TopPuppiJetCorrections::TopPuppiJetCorrections(){

  tpuppijec_tag_2017UL = "Summer19UL17";
  tpuppijec_ver_2017UL = "5";

  tpuppijec_tpuppijet_coll = "dummy";
  tpuppijec_subjet_coll = "dummy";
}


void TopPuppiJetCorrections::init(Context & ctx){
  if(init_done){
    throw invalid_argument("TopPuppiJetCorrections::init called twice!");
  }
  init_done = true;

  is_mc = ctx.get("dataset_type") == "MC";
  year = extract_year(ctx);

  // setup correction tjet type for JECs
  std::string userTopJetColl = string2lowercase(ctx.get("TopPuppiJetCollection"));

  std::string algo = "";
  // algo size
  if (userTopJetColl.find("ak4") != std::string::npos) {
    algo = "AK4";
  }
  else if(userTopJetColl.find("ak8") != std::string::npos){
    algo = "AK8";
  }
  else if (userTopJetColl.find("ak8") == std::string::npos) {
    std::cout << "TopPuppiJetCorrections.cxx: Cannot determine tjet cone + radius (neither AK4 nor AK8) - going to assume it is AK8 for JECs" << '\n';
    algo = "AK8";
  }

  std::string pus = "PFchs";
  // Pileup subtraction
  if (userTopJetColl.find("puppi") != std::string::npos) {
    pus = "PFPuppi";
  } else if (userTopJetColl.find("chs") == std::string::npos) {
    std::cout << "Cannot determine pileup subtraction (neither CHS nor PUPPI) - going to assume it is CHS for JECs" << std::endl;
  }
  tpuppijec_tpuppijet_coll = algo + pus;
  tpuppijec_subjet_coll = "AK4PFPuppi";
  std::cout << "Top Jet collection = " << tpuppijec_tpuppijet_coll << "	" << "Subjet collection = " << tpuppijec_subjet_coll << std::endl;
  cout << "2017UL JECs: " << tpuppijec_tag_2017UL << " V" << tpuppijec_ver_2017UL << " for " << tpuppijec_tpuppijet_coll << endl;

  if(is_mc){
    cout << "Setup corrections MC for PUPPI jets 2017UL" << endl;
    tpuppijet_corrector_MC.reset(new YearSwitcher(ctx));
    tpuppijet_corrector_MC->setup2017UL(std::make_shared<GenericTopJetCorrector>(ctx, JERFiles::JECFilesMC(tpuppijec_tag_2017UL, tpuppijec_ver_2017UL, tpuppijec_tpuppijet_coll),"toppuppijets"));

    tpuppijet_subjet_corrector_MC.reset(new YearSwitcher(ctx));
    tpuppijet_subjet_corrector_MC->setup2017UL(std::make_shared<GenericSubJetCorrector>(ctx, JERFiles::JECFilesMC(tpuppijec_tag_2017UL, tpuppijec_ver_2017UL, tpuppijec_subjet_coll),"toppuppijets"));

    tpuppijet_JLC_MC.reset(new YearSwitcher(ctx));
    tpuppijet_JLC_MC->setup2017UL(std::make_shared<JetLeptonCleaner_by_KEYmatching>(ctx, JERFiles::JECFilesMC(tpuppijec_tag_2017UL, tpuppijec_ver_2017UL, tpuppijec_tpuppijet_coll),"toppuppijets"));
     


    //std::string version = "";
    //if (year == Year::is2017UL) {
    //  version = "Summer19UL17_JRV2";
    //} else {
    //  throw runtime_error("Cannot find suitable jet resolution file & scale factors for this year for JetResolutionSmearer");
    //}
    //std::string JER_sf  = "JRDatabase/textFiles/"+version+"_MC/"+version+"_MC_SF_"+algo+pus+".txt";
    //std::string resFilename = "JRDatabase/textFiles/"+version+"_MC/"+version+"_MC_PtResolution_"+algo+pus+".txt";

    //tpuppijet_resolution_smearer.reset(new GenericJetResolutionSmearer(ctx, "toppuppijets", "gentopjets", JER_sf, resFilename));
  }
}

bool TopPuppiJetCorrections::process(uhh2::Event & event){
  if(!init_done){
    throw runtime_error("TopPuppiJetCorrections::init not called (has to be called in AnalysisModule constructor)");
  }
  if (is_mc) {
    cout << "Correct MC for PUPPI jets" << endl;
    tpuppijet_corrector_MC->process(event);
    tpuppijet_subjet_corrector_MC->process(event);
    tpuppijet_JLC_MC->process(event);
  } 

  //if(is_mc) tpuppijet_resolution_smearer->process(event);
  return true;
}

