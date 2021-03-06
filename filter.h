//filter and differentiator
#ifndef _YAL_FILTER
#define _YAL_FILTER
#include "config.h"
#include "utilize.h"

#define FC_Diff 100//カットオフ

namespace Yal{

//filter base
template <class T>
class filter_base{
public:
  T x;
  T x_bfr;
  void update();
  filter_base();
  filter_base(T x_in);    
};

template <class T>
filter_base<T>::filter_base(){
  // x=0.0;
  // x_bfr=0.0;
  x.zero();
  x_bfr.zero();
}
template <class T>
filter_base<T>::filter_base(T x_in){
  x=x_in;
  x_bfr=x_in;
}

// LPF
template <class T>
class LPF : public filter_base<T>{
public:
  LPF();
  LPF(T x_in, double fc_in);      // x_in: 初期値, fc_in: カットオフ周波数
  void set_fc(double fc);
  void update(T xm, T& ret_xm);   // xm: 今のセンサーの値,  ret_xm: フィルター後の値

private:
  double fc = 100.0;   // カットオフ周波数
};

template<class T>
LPF<T>::LPF() : filter_base<T>(){
}

template<class T>
LPF<T>::LPF(T x_in, double fc_in) : filter_base<T>(x_in){
  fc = fc_in;
}

template <class T>
void LPF<T>::set_fc(double fc){
  this->fc = fc;
}

template <class T>
void LPF<T>::update(T xm, T& ret_xm){
  double Tc = 1.0/(2*M_PI*fc);    // 時定数
  // this->x = Ts/(Ts+2*Tc) * xm + 2*Tc/(Ts+2*Tc) * this->x;
  ret_xm = this->x;
}

//HPF
template <class T>
class HPF : public filter_base<T>{
public:
  double fc = 1.0;//カットオフ周波数
  HPF();
  HPF(T x_in, double fc_in);
  void update(T xm,T& ret_xm);
};

template<class T>
HPF<T>::HPF(T x_in, double fc_in) : filter_base<T>(x_in){
  fc = fc_in;
}

template <class T>
void HPF<T>::update(T xm,T& ret_xm){
  double Tc = 1.0/(2*M_PI*fc);
  this->x = 2.0*Tc/(Ts+2.0*Tc)*xm - Ts/(Ts+2.0*Tc)*this->x;
  ret_xm = this->xm;
}



//differentiator
template <class T>
class Diff : public filter_base<T>{
public:
  double fc = 1.0;//カットオフ周波数
  Diff();
  Diff(T x_in, double fc_in);
  void update(T xm,T& dxm);
};

template<class T>
Diff<T>::Diff() : filter_base<T>(){
  fc = FC_Diff;
}

template<class T>
Diff<T>::Diff(T x_in, double fc_in) : filter_base<T>(x_in){
  fc=fc_in;
}

template <class T>
void Diff<T>::update(T xm/*現在値*/,T& dxm/*微分値*/){//@todo:普通にreturnした方がコンパクトにできそう
  double Tc = 1.0/(2.0*M_PI*fc);
  this->x = ( xm - this->x_bfr + (Tc-Ts/2.0)*this->x )/( Ts / 2.0+Tc );
  dxm = this->x;
  this->x_bfr = xm;
}

}

#endif
