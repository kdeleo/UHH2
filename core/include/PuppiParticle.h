#pragma once

#include "Particle.h"

class PuppiParticle : public Particle{
public:

  PuppiParticle(){
    
    m_nAlgos=0;
    m_rawAlphas=0;
    m_alphas=0;
    m_alphasMed=0;
    m_alphasRms=0;
    m_myweight = 0;
    m_weightwoWeightCut=0;
    m_pTunweighted=0;

    m_vx=0;
    m_vy=0;
    m_vz=0;
  }

  

  double nAlgos() const{return m_nAlgos;}
  double rawAlphas() const{return m_rawAlphas;}
  double alphas() const{return m_alphas;}
  double alphasMed() const{return m_alphasMed;}
  double alphasRms() const{return m_alphasRms;}
  double mypuppiweight() const{return m_myweight;}
  double weightwoWeightCut() const{return m_weightwoWeightCut;}
  double pTunweighted() const{return m_pTunweighted;}



  double vx()const{return m_vx;}
  double vy()const{return  m_vy;}
  double vz()const{return  m_vz;}

  double fromPV()const{return  m_fromPV;}
  double puppiweight()const{return  m_puppiweight;}

  double dz()const{return  m_dz;}
  double dxy()const{return  m_dxy;}
  double fdzcut()const{return  m_fdzcut;}

  void set_nAlgos(double x){m_nAlgos=x;}
  void set_rawAlphas(double x){m_rawAlphas=x;}
  void set_alphas(double x){m_alphas=x;}
  void set_alphasMed(double x){m_alphasMed=x;}
  void set_alphasRms(double x){m_alphasRms=x;}
  void set_mypuppiweight(double x){m_myweight=x;}
  void set_weightwoWeightCut(double x){m_weightwoWeightCut=x;}
  void set_pTunweighted(double x){m_pTunweighted=x;}


  void set_vx(double x){ m_vx=x;}
  void set_vy(double x){ m_vy=x;}
  void set_vz(double x){ m_vz=x;}

  void set_fromPV(double x){ m_fromPV=x;}
  void set_puppiweight(double x){ m_puppiweight=x;}

  void set_dz(double x){ m_dz=x;}
  void set_dxy(double x){ m_dxy=x;}
  void set_fdzcut(double x){ m_fdzcut=x;}

private:

  double m_nAlgos;
  double m_rawAlphas;
  double m_alphas;
  double m_alphasMed;
  double m_alphasRms;
  double m_myweight;
  double m_weightwoWeightCut;
  double m_pTunweighted;

  double  m_vx;
  double  m_vy;
  double  m_vz;

  int  m_fromPV;
  double m_puppiweight;

  double m_dz;
  double m_dxy;
  double m_fdzcut;
};
