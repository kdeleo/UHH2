#pragma once

#include "UHH2/core/include/Hists.h"
#include "TH2F.h"

namespace uhh2examples {


class VBFHists: public uhh2::Hists {
public:
    // use the same constructor arguments as Hists for forwarding:
    VBFHists(uhh2::Context & ctx, const std::string & dirname);

    virtual void fill(const uhh2::Event & ev) override;
    virtual ~VBFHists();
  protected:
    bool berror;

    TH1F *DeltaEta,*invMass;

  };

}
