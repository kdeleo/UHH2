#pragma once

#include "UHH2/core/include/fwd.h"
#include "UHH2/core/include/Selection.h"
#include "UHH2/core/include/Event.h"
#include "UHH2/core/include/Utils.h"

#include "UHH2/core/include/AnalysisModule.h"
#include "UHH2/common/include/ObjectIdUtils.h"

using namespace uhh2;
using namespace std;

namespace uhh2examples {
    
/* Select events with at least two jets in which the leading two jets have deltaphi > 2.7 and the third jet pt is
 * below 20% of the average of the leading two jets, where the minimum deltaphi and
 * maximum third jet pt fraction can be changed in the constructor.
 * The jets are assumed to be sorted in pt.
 */

class ZmassCut: public uhh2::Selection {
public:
    ZmassCut(double threshold);
    virtual bool passes(const uhh2::Event & event) override;
private:
    double threshold;
    double inv_mass;  
    FlavorParticle lep_pos;
    FlavorParticle lep_neg;
   
};

class DeltaEtaCut: public uhh2::Selection {
public:
    DeltaEtaCut(double threshold);
    virtual bool passes(const uhh2::Event & event) override;
private:
    double threshold;
};

class InvMass2AK4Jets: public uhh2::Selection {
public:
    InvMass2AK4Jets(double threshold);
    virtual bool passes(const uhh2::Event & event) override;
private:
    double threshold;
};

class PUJetID: public uhh2::AnalysisModule {
public:
    explicit PUJetID(TString threshold_);
    virtual bool process(uhh2::Event & event)override;
private:
    TString threshold;
   
};


class PUjetsByDeltaPhi: public uhh2::AnalysisModule {
public:
    explicit PUjetsByDeltaPhi(double threshold_);
    virtual bool process(uhh2::Event & event)override;
private:
    double threshold;
   
};


class QuarkGluonComparisoon: public uhh2::AnalysisModule {
public:
    explicit QuarkGluonComparisoon(Context & ctx);
    virtual bool process(uhh2::Event & event)override;
private:
    uhh2::Event::Handle< std::vector<Jet> > h_quarks;
    uhh2::Event::Handle< std::vector<Jet> > h_gluons;
    uhh2::Event::Handle< std::vector<Jet> > h_quarks_udsc;
    uhh2::Event::Handle< std::vector<GenJet> > h_genquarks;
    uhh2::Event::Handle< std::vector<GenJet> > h_gengluons;
    uhh2::Event::Handle< std::vector<GenJet> > h_genquarks_udsc;
    bool isMC;

   
};

class MultiplicityCleaner: public uhh2::AnalysisModule {
public:
    explicit MultiplicityCleaner(double threshold_);
    virtual bool process(uhh2::Event & event)override;
private:
    double threshold;
   
};

class LVjetsByDeltaPhi: public uhh2::AnalysisModule {
public:
    explicit LVjetsByDeltaPhi(double threshold_);
    virtual bool process(uhh2::Event & event)override;
private:
    double threshold;
   
};

class AK4JetMuonOverlapRemoval: public uhh2::AnalysisModule {
public:
    explicit AK4JetMuonOverlapRemoval(double threshold_);
    virtual bool process(uhh2::Event & event)override;
private:
    double threshold;
   
};

class ZptCut: public uhh2::Selection {
public:
    ZptCut(double threshold, TString minmax);
    virtual bool passes(const uhh2::Event & event) override;
private:
    double threshold;
    double inv_pt;  
    FlavorParticle lep_pos;
    FlavorParticle lep_neg;
    TString minmax;
   
};


 class HtSelection: public uhh2::Selection {
  public:
    explicit HtSelection(double ht_min=0., double ht_max=-1);
    virtual bool passes(const uhh2::Event & event);
 private:
    double ht_min, ht_max;
 };

}



class TrueInteractionSelection:  public uhh2::Selection {

 public:
  explicit TrueInteractionSelection( double minTrueInteraction_, double maxTrueInteraction_);
  virtual bool passes(const uhh2::Event & event);

 private:
  double minTrueInteraction;
  double maxTrueInteraction;

};
