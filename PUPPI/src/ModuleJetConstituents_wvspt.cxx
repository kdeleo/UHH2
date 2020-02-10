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




class HistsPtWeight_eta2: public uhh2::Hists {
public:
    HistsPtWeight_eta2(uhh2::Context & ctx, const std::string & dirname);
    virtual void fill(const uhh2::Event & ev) override; // need this to override abstract base method
    virtual void fill(float puppiPt, float puppiWeight);
    virtual ~HistsPtWeight_eta2();
private:
    TH2F * hPtWeight_eta2;
};

HistsPtWeight_eta2::HistsPtWeight_eta2(Context & ctx, const string & dirname):
Hists(ctx, dirname)
{   
   hPtWeight_eta2 = book<TH2F>("w vs pT, 1.3<#eta<2", ";p_{T};w", 1000, 0.0, 1000.0, 110, 0.0, 1.1);
   }
   
  void HistsPtWeight_eta2::fill(float puppiPt, float puppiWeight) {
  hPtWeight_eta2->Fill(puppiPt, puppiWeight);
  }
  
  void HistsPtWeight_eta2::fill(const Event & event){
    (void)event; // unused var
    }
  
    HistsPtWeight_eta2::~HistsPtWeight_eta2(){}






class HistsPtWeight_eta3: public uhh2::Hists {
public:
    HistsPtWeight_eta3(uhh2::Context & ctx, const std::string & dirname);
    virtual void fill(const uhh2::Event & ev) override; // need this to override abstract base method
    virtual void fill(float puppiPt, float puppiWeight);
    virtual ~HistsPtWeight_eta3();
private:
    TH2F * hPtWeight_eta3;
};

HistsPtWeight_eta3::HistsPtWeight_eta3(Context & ctx, const string & dirname):
Hists(ctx, dirname)
{   
   hPtWeight_eta3 = book<TH2F>("w vs pT, 2<#eta<2.5", ";p_{T};w", 1000, 0.0, 1000.0, 110, 0.0, 1.1);
   }
   
  void HistsPtWeight_eta3::fill(float puppiPt, float puppiWeight) {
  hPtWeight_eta3->Fill(puppiPt, puppiWeight);
  }
  
  void HistsPtWeight_eta3::fill(const Event & event){
    (void)event; // unused var
    }
  
    HistsPtWeight_eta3::~HistsPtWeight_eta3(){}





class HistsPtWeight_eta4: public uhh2::Hists {
public:
    HistsPtWeight_eta4(uhh2::Context & ctx, const std::string & dirname);
    virtual void fill(const uhh2::Event & ev) override; // need this to override abstract base method
    virtual void fill(float puppiPt, float puppiWeight);
    virtual ~HistsPtWeight_eta4();
private:
    TH2F * hPtWeight_eta4;
};

HistsPtWeight_eta4::HistsPtWeight_eta4(Context & ctx, const string & dirname):
Hists(ctx, dirname)
{   
   hPtWeight_eta4 = book<TH2F>("w vs pT, 2.5<#eta<3", ";p_{T};w", 1000, 0.0, 1000.0, 110, 0.0, 1.1);
   }
   
  void HistsPtWeight_eta4::fill(float puppiPt, float puppiWeight) {
  hPtWeight_eta4->Fill(puppiPt, puppiWeight);
  }
  
  void HistsPtWeight_eta4::fill(const Event & event){
    (void)event; // unused var
    }
  
    HistsPtWeight_eta4::~HistsPtWeight_eta4(){}




class HistsPtWeight_eta5: public uhh2::Hists {
public:
    HistsPtWeight_eta5(uhh2::Context & ctx, const std::string & dirname);
    virtual void fill(const uhh2::Event & ev) override; // need this to override abstract base method
    virtual void fill(float puppiPt, float puppiWeight);
    virtual ~HistsPtWeight_eta5();
private:
    TH2F * hPtWeight_eta5;
};

HistsPtWeight_eta5::HistsPtWeight_eta5(Context & ctx, const string & dirname):
Hists(ctx, dirname)
{   
   hPtWeight_eta5 = book<TH2F>("w vs pT, 3<#eta<10", ";p_{T};w", 1000, 0.0, 1000.0, 110, 0.0, 1.1);
   }
   
  void HistsPtWeight_eta5::fill(float puppiPt, float puppiWeight) {
  hPtWeight_eta5->Fill(puppiPt, puppiWeight);
  }
  
  void HistsPtWeight_eta5::fill(const Event & event){
    (void)event; // unused var
    }
  
    HistsPtWeight_eta5::~HistsPtWeight_eta5(){}







class ModuleJetConstituents_wvspt: public AnalysisModule {
public:

    explicit ModuleJetConstituents_wvspt(Context & ctx);
    virtual bool process(Event & event) override;
private:
    unique_ptr<HistsPtWeight_eta1> topjetHistsPtWeight_eta1;
    unique_ptr<HistsPtWeight_eta2> topjetHistsPtWeight_eta2;
    unique_ptr<HistsPtWeight_eta3> topjetHistsPtWeight_eta3;
    unique_ptr<HistsPtWeight_eta4> topjetHistsPtWeight_eta4;
    unique_ptr<HistsPtWeight_eta5> topjetHistsPtWeight_eta5;
};


ModuleJetConstituents_wvspt::ModuleJetConstituents_wvspt(Context & ctx)
{
    cout << "Hello World from ModuleJetConstituents_wvspt!" << endl;

    topjetHistsPtWeight_eta1.reset(new HistsPtWeight_eta1(ctx, "topjetHistsPtWeight_eta1"));
    topjetHistsPtWeight_eta2.reset(new HistsPtWeight_eta2(ctx, "topjetHistsPtWeight_eta2"));
    topjetHistsPtWeight_eta3.reset(new HistsPtWeight_eta3(ctx, "topjetHistsPtWeight_eta3"));
    topjetHistsPtWeight_eta4.reset(new HistsPtWeight_eta4(ctx, "topjetHistsPtWeight_eta4"));
    topjetHistsPtWeight_eta5.reset(new HistsPtWeight_eta5(ctx, "topjetHistsPtWeight_eta5"));

}

bool ModuleJetConstituents_wvspt::process(Event & event) {

//    cout << "*** EVENT: " << event.event << endl;





//Match jets
float puppiPt;
float puppiWeight;
      for(unsigned int i=1; i<event.puppiparticles->size(); i++){

          if(fabs(event.puppiparticles->at(i).charge())==0 && fabs(event.puppiparticles->at(i).eta())>=0.0 && fabs(event.puppiparticles->at(i).eta())<1.3){
              puppiPt = event.puppiparticles->at(i).pt();    
              puppiWeight = event.puppiparticles->at(i).mypuppiweight();

              topjetHistsPtWeight_eta1->fill(puppiPt, puppiWeight);  
          }


          if(fabs(event.puppiparticles->at(i).charge())==0 && fabs(event.puppiparticles->at(i).eta())>=1.3 && fabs(event.puppiparticles->at(i).eta())<2.0){
              puppiPt = event.puppiparticles->at(i).pt();    
              puppiWeight = event.puppiparticles->at(i).mypuppiweight();

              topjetHistsPtWeight_eta2->fill(puppiPt, puppiWeight);  
          }


          if(fabs(event.puppiparticles->at(i).charge())==0 && fabs(event.puppiparticles->at(i).eta())>=2.0 && fabs(event.puppiparticles->at(i).eta())<2.5){
              puppiPt = event.puppiparticles->at(i).pt();    
              puppiWeight = event.puppiparticles->at(i).mypuppiweight();

              topjetHistsPtWeight_eta3->fill(puppiPt, puppiWeight);  
          }


          if(fabs(event.puppiparticles->at(i).charge())==0 && fabs(event.puppiparticles->at(i).eta())>=2.5 && fabs(event.puppiparticles->at(i).eta())<3.0){
              puppiPt = event.puppiparticles->at(i).pt();    
              puppiWeight = event.puppiparticles->at(i).mypuppiweight();

              topjetHistsPtWeight_eta4->fill(puppiPt, puppiWeight);  
          }


          if(fabs(event.puppiparticles->at(i).charge())==0 && fabs(event.puppiparticles->at(i).eta())>=3.0 && fabs(event.puppiparticles->at(i).eta())<=10.0){
              puppiPt = event.puppiparticles->at(i).pt();    
              puppiWeight = event.puppiparticles->at(i).mypuppiweight();

              topjetHistsPtWeight_eta5->fill(puppiPt, puppiWeight);  
          }











      }


    return true;
}

UHH2_REGISTER_ANALYSIS_MODULE(ModuleJetConstituents_wvspt)

}
