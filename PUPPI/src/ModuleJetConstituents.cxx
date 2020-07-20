#include <iostream>
#include <memory>

#include "UHH2/core/include/AnalysisModule.h"
#include "UHH2/core/include/Event.h"
#include "UHH2/core/include/Hists.h"
#include "UHH2/core/include/Utils.h"
#include "../include/PUPPIHists.h"

#include "TH1F.h"
#include "TH2F.h"
#include "TStyle.h"
#include "TLine.h"

using namespace std;
using namespace uhh2;


namespace uhh2examples {

class HistsPtWeight_eta1: public uhh2::Hists {
public:
    HistsPtWeight_eta1(uhh2::Context & ctx, const std::string & dirname);
    virtual void fill(const uhh2::Event & ev) override; // need this to override abstract base method
    virtual void fill(float puppiPt, float puppiWeight);
    virtual ~HistsPtWeight_eta1();
private:
    TH2F * hPtWeight_eta1;
};

HistsPtWeight_eta1::HistsPtWeight_eta1(Context & ctx, const string & dirname):
Hists(ctx, dirname)
{   
   hPtWeight_eta1 = book<TH2F>("w vs pT, 0<#eta<1.3", ";p_{T};w", 1000, 0.0, 1000.0, 110, 0.0, 1.1);
   }
   
  void HistsPtWeight_eta1::fill(float puppiPt, float puppiWeight) {
  hPtWeight_eta1->Fill(puppiPt, puppiWeight);
  }
  
  void HistsPtWeight_eta1::fill(const Event & event){
    (void)event; // unused var
    }
  
    HistsPtWeight_eta1::~HistsPtWeight_eta1(){}




class ModuleJetConstituents: public AnalysisModule {
public:

    explicit ModuleJetConstituents(Context & ctx);
    virtual bool process(Event & event) override;
private:
    unique_ptr<HistsPtWeight_eta1> topjetHistsPtWeight_eta1;
};


ModuleJetConstituents::ModuleJetConstituents(Context & ctx)
{
    cout << "Hello World from ModuleJetConstituents!" << endl;

    topjetHistsPtWeight_eta1.reset(new HistsPtWeight_eta1(ctx, "topjetHistsPtWeight_eta1"));

}

bool ModuleJetConstituents::process(Event & event) {

//    cout << "*** EVENT: " << event.event << endl;


float DeltaRchs;
uint jetInd = 0;
for (const TopJet & topjet: *event.topjets) {
if(topjet.pt() >= 100.0 && topjet.pt() <= 200.0){
     for(const auto indTEST : topjet.pfcand_indexs()){
     if (event.pfparticles->at(indTEST).puppiWeight()!=0) continue;
//     cout << "CHS delta R  " << deltaR(topjet,event.pfparticles->at(indTEST)) << "     " << "for jet#" << jetInd << "     " << "for part#" << indTEST << endl;
     DeltaRchs=deltaR(topjet,event.pfparticles->at(indTEST));
     topjetHistsDeltaRchs->fill(DeltaRchs);
     }
jetInd++;
}
}


      }


    return true;
}

UHH2_REGISTER_ANALYSIS_MODULE(ModuleJetConstituents)

}
