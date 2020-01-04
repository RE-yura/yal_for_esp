//robot configulation
#ifndef _YAL_CONFIG
#define _YAL_CONFIG
//使うコントローラを選択
//#define ARDUINO_KAL
#define ESP32
//#define TITECH
//#define EIGEN_KAL
//#define X2_KAL

//Sampling Time
#ifndef Ts
#define Ts 0.001  //sampling time[s]
#endif

namespace Yal{

template<class T>
class RobotData{
public:
  //姿勢
  T phi;
  T dphi;
  T d2phi;
  T theta;
  T dtheta;
  T d2theta;
  //位置
  T x;
  T dx;
  T d2x;
  //関節用
  T q;
  T dq;
  T d2q;
  
  RobotData();  
  
};

template<class T>
RobotData<T>::RobotData(){
  phi = 0.0;
  dphi = 0.0;
  d2phi = 0.0;
  theta = 0.0;
  dtheta = 0.0;
  d2theta = 0.0;
  x = 0.0;
  dx = 0.0;
  d2x = 0.0;
  q = 0.0;
  dq = 0.0;
  d2q = 0.0;
}

}

#endif
