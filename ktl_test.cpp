#include <stdio.h>
#include <ktl.h>
#include "yal.h"

int main(){
  // ベクトルの定義
  Ktl::Vector<3> vec = {3.2, 4.2, 5.3};
  Ktl::Vector<3> F = {10.0, 10.0, 30.0};

  // 行列の定義
  Ktl::Matrix<3,4> mat = { 100.0,    0.0, -100.0,    0.0, 
                             0.0,  100.0,    0.0, -100.0, 
                           100.0,  100.0,  100.0,  100.0 };
  printf("mat = \n");
  mat.print();

  // 逆行列の計算
  Ktl::Matrix<4,3> mat_inv = mat.inv();   
  printf("mat_inv = \n");
  mat_inv.print();

  // 列数の取得
  printf("rowSize = %ld\n", sizeof(mat.row(0))/sizeof(mat.row(0)[0]));
  // printf("rowSize = %d\n", mat.rows());

  // 行数の取得
  printf("columnSize = %ld\n", sizeof(mat.column(0))/sizeof(mat.column(0)[0]));
  // printf("columnSize = %d\n", mat.columns());

  // 単位行列の定義
  Ktl::Matrix<3,3> E;
  E.unitize();
  printf("E = \n");
  E.print();

  Ktl::Matrix<1,1> Q = {4.0};
  // Q = F * Q;
  printf("Q = ");
  Q.print();
  
  // 行列とベクトルの掛け算
  Ktl::Vector<4> I = mat_inv * F;
  printf("I = \n");
  I.print();

  // --------- アドミタンステスト --------------------
  Ktl::Matrix<3,3> Md = { 1.0, 0.0, 0.0,
                          0.0, 1.0, 0.0,
                          0.0, 0.0, 1.0 };
  Ktl::Matrix<3,3> Cd = { 100.0, 0.0, 0.0,
                          0.0, 100.0, 0.0,
                          0.0, 0.0, 100.0 };
  Ktl::Matrix<3,3> Kd = { 1000.0,    0.0,    0.0,
                             0.0, 1000.0,    0.0,
                             0.0,    0.0, 1000.0 };
  Yal::impedance_control<Ktl::Vector<3>, Ktl::Matrix<3,3>> adm(Md,Cd,Kd);

  Ktl::Vector<3> f_human = {1.0, 0.0, 0.0};
 
  Ktl::Vector<3> dx;
  Ktl::Vector<3> pos;

  for(int j=0; j<400; j++){
    dx = adm.admitance_out(f_human);
    // dx.print();
    pos += dx;
    pos.print();
  }

  
  return 0;
}

