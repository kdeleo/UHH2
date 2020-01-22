#pragma once

#include "UHH2/core/include/Hists.h"
#include "TH2F.h"
#include "UHH2/core/include/Event.h"

using namespace uhh2;
using namespace std;

namespace uhh2examples {


class DYHists: public uhh2::Hists {
public:
    // use the same constructor arguments as Hists for forwarding:
    DYHists(uhh2::Context & ctx, const std::string & dirname);

    virtual void fill(const uhh2::Event & ev) override;
    virtual ~DYHists();
  protected:
    bool berror;
    double pTcut;
    Event::Handle<MET> h_mymet;
    //General
    TH1F *MET_h, *pujetid, *pujetid_forward;
    TH1F *number_genjets, *number_recojets;

    TH1F *generater_HT, *muon_invariant_mass;
    //Response
    TH2F *response_ZP, *response_NPV,*rms_ZP,*rms_NPV,*rms_uper_ZP,*rms_uper_NPV, *upar_ZPT;


};

}
