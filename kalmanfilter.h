//kalman filter for SISO system
//@todo:MIMOの宣言の部分をコンパクトにしたい
//@todo:EKFにも対応させる
#ifndef _YAL_KALMAN_FILTER
#define _YAL_KALMAN_FILTER
#include "config.h"
#include "utilize.h"
#include "la.h"
// #ifdef X2_KAL
// #include<eigen3/Eigen/Dense>
// #endif
// #ifdef EIGEN_KAL
// #include "eigen3.3.7/Eigen/Core"
// #endif

namespace Yal{

// ========== 状態空間 ===========================================
template <class Ta,class Tb,class Tc,class Td>
class StateSpace{
  public:
  Ta A;
  Tb B;
  Tc C;
  Td D;

  StateSpace();
  StateSpace(Ta A_in,Tb B_in,Tc C_in,Td D_in);
  void set_state(Ta A_in,Tb B_in,Tc C_in,Td D_in);
};

template <class Ta,class Tb,class Tc,class Td>
StateSpace<Ta,Tb,Tc,Td>::StateSpace(){
}

template <class Ta,class Tb,class Tc,class Td>
StateSpace<Ta,Tb,Tc,Td>::StateSpace(Ta A_in,Tb B_in,Tc C_in,Td D_in){
  this->A = A_in;
  this->B = B_in;
  this->C = C_in;
  this->D = D_in;
}

template <class Ta,class Tb,class Tc,class Td>
void StateSpace<Ta,Tb,Tc,Td>::set_state(Ta A_in,Tb B_in,Tc C_in,Td D_in){
  this->A = A_in;
  this->B = B_in;
  this->C = C_in;
  this->D = D_in;
}

// ========== Kalmanfilter for SISO =============================
template <class T>
class KalmanFilter{
 
public:
  T x_;
  T x_est;
  //T y;
  T g;
  T P_bfr;
  T P_aft;
  T Q;//vの分散
  T R;//wの分散
  StateSpace<T,T,T,T> sys;
  KalmanFilter();
  KalmanFilter(T xx,T QQ, T RR);
  void update(T u,T y);
};

template <class T>
KalmanFilter<T>::KalmanFilter(){
  x_est = 0.0;
  Q = 1.0;
  R = 1.0;
}

template <class T>
KalmanFilter<T>::KalmanFilter(T xx,T QQ, T RR){
  x_est = xx;
  Q = QQ;
  R = RR;
}

template <class T>
void KalmanFilter<T>::update(T u,T y){
  x_ = x_est + Ts * u;
  P_bfr = P_aft + Q;
  g = P_bfr / (P_bfr + R);
  x_est = x_ + g * (y - x_);
  P_aft = (1 - g*1) * P_bfr;
}

// =========== Kalmanfilter for MIMO ============================
template <class Ta, class Tb, class Tc, class Td, class Tx, class Tz>
class KalmanFilter_MIMO{
 
public:
  Tx x_;
  Tx x_est;
  //Tz z;
  Ta P_bfr;   // 事前分布
  Ta P_aft;   // 事後分布
  StateSpace<Ta,Tb,Tc,Td> sys;  // 状態空間
  Yal::Matrix<P_bfr.columns(), sys.C.columns()> G;   // カルマンゲイン
  Yal:: Q;  // vの分散
  Yal::Matrix<sys.C.columns(), sys.C.columns()> R;  // wの分散
  Ta I(1);  // 単位行列
  KalmanFilter_MIMO();
  KalmanFilter_MIMO(Tx x, Eigen::MatrixXd Q_in, Eigen::MatrixXd R_in);
  KalmanFilter_MIMO(StateSpace<Ta,Tb,Tc,Td> sys_in,Tx x, Eigen::MatrixXd Q_in, Eigen::MatrixXd R_in);
  void x_init(Tx x);
  void update(Tx u,Tz z,Tx& x_ret);
};

template <class Ta,class Tb,class Tc,class Td,class Tx, class Tz>
KalmanFilter_MIMO<Ta,Tb,Tc,Td,Tx,Tz>::KalmanFilter_MIMO(){
  I.unitize();
}

template <class Ta,class Tb,class Tc,class Td,class Tx, class Tz>
KalmanFilter_MIMO<Ta,Tb,Tc,Td,Tx,Tz>::KalmanFilter_MIMO(Tx x, Eigen::MatrixXd Q_in, Eigen::MatrixXd R_in){
  this->x_est = x;
  this->Q = Q_in;
  this->R = R_in;
  I.unitize();
}

template <class Ta,class Tb,class Tc,class Td,class Tx, class Tz>
KalmanFilter_MIMO<Ta,Tb,Tc,Td,Tx,Tz>::KalmanFilter_MIMO(StateSpace<Ta,Tb,Tc,Td> sys_in,Tx x,Eigen::MatrixXd Q_in, Eigen::MatrixXd R_in):{
  this->x_est = x;
  this->sys = sys_in;
  Q = Q_in;
  R = R_in;
  I.unitize(); sys(sys_in)
}

template <class Ta,class Tb,class Tc,class Td,class Tx, class Tz>
void KalmanFilter_MIMO<Ta,Tb,Tc,Td,Tx,Tz>::x_init(Tx x){
  x_ = x;
  x_est = x;
}

template <class Ta,class Tb,class Tc,class Td,class Tx, class Tz>
void KalmanFilter_MIMO<Ta,Tb,Tc,Td,Tx,Tz>::update(Tx u, Tz z, Tx& x_ret){
  // 予測ステップ
  x_ = sys.A * x_est + sys.B * u;   // 状態方程式に基づいた予測
  P_bfr = sys.A * P_aft * ~sys.A + sys.B * Q * ~sys.B; // 誤差分散の更新

  // 観測ステップ
  G = P_bfr * ~sys.C * (sys.C * P_bfr * ~sys.C + R).inv();  // カルマンゲイン
  x_est = x_ + G * (z - sys.C * x_);  // 観測と予測の誤差分を補正し，推定値を算出
  x_ret = x_est;
  // P_aft = (Eigen::MatrixXd::IdentiTz(sys.A.rows(),sys.A.cols()) - G * sys.C) * P_bfr;
  P_aft = (I - G * sys.C) * P_bfr;  // 誤差分散を更新
}

}

#endif
