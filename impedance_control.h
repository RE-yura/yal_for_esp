//インピーダンス制御用
//@todo:インピーダンス型の実装
//未チェック
#ifndef _YAL_IMPEDANCE_CONTROL
#define _YAL_IMPEDANCE_CONTROL

#include "config.h"
#include "utilize.h"

//#define IMPEDANCE_FORM
#define ADMITANCE_FORM

namespace Yal{

template<class T,class Tmat>
class impedance_control{
public:
  // RobotData<T> zero;  // 基準位置

  //desired impedance
  Tmat Md;    // N/m/s^2=kg
  Tmat Cd;    // N/m/s
  Tmat Kd;    // N/m
  
  //admitance control
  // RobotData<T> ref;
  // RobotData<T> ref_b1;
  // RobotData<T> ref_b2;

  T x;
  T x_b1;
  T x_b2;
  
  T dx;
  T dx_b1;
  T dx_b2;

  T fext;
  T fext_b1;
  T fext_b2;
  
  impedance_control();
  impedance_control(Tmat Md,Tmat Cd,Tmat Kd);
  void set_impedance(Tmat Md,Tmat Cd,Tmat Kd);
  void impedance_out(RobotData<T> state,T fref,T& f); //運動入力力出力  
  // void admitance_out(T fext,T fref,RobotData<T>& ref);//力入力運動出力
  T admitance_out(T fext);//力入力運動出力
};

template<class T,class Tmat>
impedance_control<T,Tmat>::impedance_control(){
}

template<class T,class Tmat>
impedance_control<T,Tmat>::impedance_control(Tmat Md,Tmat Cd,Tmat Kd){
  this->Md = Md;
  this->Cd = Cd;
  this->Kd = Kd;
}

template<class T,class Tmat>
void impedance_control<T,Tmat>::set_impedance(Tmat Md,Tmat Cd,Tmat Kd){
  this->Md = Md;
  this->Cd = Cd;
  this->Kd = Kd;
}

template<class T,class Tmat>
void impedance_control<T,Tmat>::impedance_out(RobotData<T> state,T fref,T& f){
  f = Md * state.d2x + Cd * state.dx + Kd * state.x;//@todo:動力学補償入れる
}

// 入力: 力,  出力: 速度
// fext: 外力ベクトル


template<class T,class Tmat>
// T impedance_control<T,Tmat>::admitance_out(T fext, T fref, RobotData<T>& ref){ 
T impedance_control<T,Tmat>::admitance_out(T fext){ 
  this->fext = fext;

  dx = (4*Md + 2*Ts*Cd + Kd*pow(Ts,2)).inv() * 
  ( 2*Ts*(fext - fext_b2) + (8*Md - 2*Kd*pow(Ts,2)) * dx_b1 - (4*Md - 2*Ts*Cd + Kd*pow(Ts,2)) * dx_b2 );

  // x = (4*Md + 2*Ts*Cd + Kd*pow(Ts,2)).inv() * 
  // ( pow(Ts,2) * ( fext + 2*fext_b1 + fext_b2 ) * (8*Md - 2*Kd*pow(Ts,2)) * dx_b1 - (4*Md - 2*Ts*Cd + Kd*pow(Ts,2)) * dx_b2 );
  
  this->fext_b2 = this->fext_b1;
  this->fext_b1 = fext;

  dx_b2 = dx_b1;
  dx_b1 = dx;

  // this->ref_b2 = this->ref_b1;
  // this->ref_b1 = this->ref;

  // ref.dx = this->ref.dx * 1000.0;   // mm/sに直す

  return dx;
}

}


#endif
