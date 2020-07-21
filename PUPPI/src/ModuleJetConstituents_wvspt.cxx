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

class HistsPtWeight_CH: public uhh2::Hists {
public:
    HistsPtWeight_CH(uhh2::Context & ctx, const std::string & dirname);
    virtual void fill(const uhh2::Event & ev) override; // need this to override abstract base method
    virtual void fill(float puppiPt, float puppiWeight);
    virtual ~HistsPtWeight_CH();
private:
    TH2F * hPtWeight_CH;
};

HistsPtWeight_CH::HistsPtWeight_CH(Context & ctx, const string & dirname):
Hists(ctx, dirname)
{   
   hPtWeight_CH = book<TH2F>("w vs pT, CH", ";p_{T};w", 1000, 0.0, 1000.0, 110, 0.0, 1.1);
   }
   
  void HistsPtWeight_CH::fill(float puppiPt, float puppiWeight) {
  hPtWeight_CH->Fill(puppiPt, puppiWeight);
  }
  
  void HistsPtWeight_CH::fill(const Event & event){
    (void)event; // unused var
    }
  
    HistsPtWeight_CH::~HistsPtWeight_CH(){}




class HistsPtWeight_NH: public uhh2::Hists {
public:
    HistsPtWeight_NH(uhh2::Context & ctx, const std::string & dirname);
    virtual void fill(const uhh2::Event & ev) override; // need this to override abstract base method
    virtual void fill(float puppiPt, float puppiWeight);
    virtual ~HistsPtWeight_NH();
private:
    TH2F * hPtWeight_NH;
};

HistsPtWeight_NH::HistsPtWeight_NH(Context & ctx, const string & dirname):
Hists(ctx, dirname)
{   
   hPtWeight_NH = book<TH2F>("w vs pT, NH", ";p_{T};w", 1000, 0.0, 1000.0, 110, 0.0, 1.1);
   }
   
  void HistsPtWeight_NH::fill(float puppiPt, float puppiWeight) {
  hPtWeight_NH->Fill(puppiPt, puppiWeight);
  }
  
  void HistsPtWeight_NH::fill(const Event & event){
    (void)event; // unused var
    }
  
    HistsPtWeight_NH::~HistsPtWeight_NH(){}



class ModuleJetConstituents_wvspt: public AnalysisModule {
public:

    explicit ModuleJetConstituents_wvspt(Context & ctx);
    virtual bool process(Event & event) override;
private:
    unique_ptr<HistsPtWeight_CH> topjetHistsPtWeight_CH;
    unique_ptr<HistsPtWeight_NH> topjetHistsPtWeight_NH;
};


ModuleJetConstituents_wvspt::ModuleJetConstituents_wvspt(Context & ctx)
{
    cout << "Hello World from ModuleJetConstituents_wvspt!" << endl;

    topjetHistsPtWeight_CH.reset(new HistsPtWeight_CH(ctx, "topjetHistsPtWeight_CH"));
    topjetHistsPtWeight_NH.reset(new HistsPtWeight_NH(ctx, "topjetHistsPtWeight_NH"));

}

bool ModuleJetConstituents_wvspt::process(Event & event) {

//    cout << "*** EVENT: " << event.event << endl;



float puppiPt;
float puppiWeight;
    for(unsigned int i=1; i<event.puppiparticles->size(); i++){

          if(fabs(event.puppiparticles->at(i).charge())==1 && event.puppiparticles->at(i).fromPV()==0 && fabs(event.puppiparticles->at(i).dz())<=0.2){
              puppiPt = event.puppiparticles->at(i).pt();    
              puppiWeight = event.puppiparticles->at(i).mypuppiweight();

              topjetHistsPtWeight_CH->fill(puppiPt, puppiWeight);  
          }


          if(fabs(event.puppiparticles->at(i).charge())==0 && event.puppiparticles->at(i).fromPV()==0 && fabs(event.puppiparticles->at(i).dz())<=0.2){
              puppiPt = event.puppiparticles->at(i).pt();    
              puppiWeight = event.puppiparticles->at(i).mypuppiweight();

              topjetHistsPtWeight_NH->fill(puppiPt, puppiWeight);  
          }


    }


    return true;
}

UHH2_REGISTER_ANALYSIS_MODULE(ModuleJetConstituents_wvspt)

}
