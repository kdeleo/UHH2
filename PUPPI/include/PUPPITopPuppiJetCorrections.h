#pragma once
#include "UHH2/core/include/AnalysisModule.h"
#include "UHH2/core/include/Selection.h"
#include "UHH2/core/include/Utils.h"

#include "UHH2/common/include/ObjectIdUtils.h"
#include "UHH2/common/include/NSelections.h"
#include "UHH2/common/include/JetIds.h"
#include "UHH2/common/include/JetCorrections.h"
#include "UHH2/common/include/CleaningModules.h"
#include "UHH2/common/include/YearRunSwitchers.h"


class PUPPITopPuppiJetCorrections: public uhh2::AnalysisModule {
public:
    PUPPITopPuppiJetCorrections();

    virtual bool process(uhh2::Event & event) override;
    void init(uhh2::Context & ctx);

private:

    std::unique_ptr<YearSwitcher> topjet_corrector_MC;//, topjet_subjet_corrector_MC;
    std::shared_ptr<RunSwitcher> topjec_switcher_2017UL;//, topjec_subjet_switcher_2017UL;

    std::string jec_tag_2017UL, jec_ver_2017UL;
    std::string jec_jet_coll;//, jec_subjet_coll;
    std::string top_puppi;

};

