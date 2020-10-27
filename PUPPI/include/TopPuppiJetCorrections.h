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

class TopPuppiJetCorrections: public uhh2::AnalysisModule {
public:
    TopPuppiJetCorrections();

    virtual bool process(uhh2::Event & event) override;
    void init(uhh2::Context & ctx);

private:
    void fail_if_init() const;

    std::unique_ptr<YearSwitcher> tpuppijet_corrector_MC, tpuppijet_subjet_corrector_MC, tpuppijet_JLC_MC;
    //std::shared_ptr<RunSwitcher> tpuppijec_switcher_2017UL;
    //std::unique_ptr<GenericJetResolutionSmearer> tpuppijet_resolution_smearer;

    bool is_mc;
    bool init_done = false;

    Year year;

    // Parameters for JEC & JLC sets
    std::string tpuppijec_tag_2017UL, tpuppijec_ver_2017UL;
    std::string tpuppijec_tpuppijet_coll, tpuppijec_subjet_coll;
};

