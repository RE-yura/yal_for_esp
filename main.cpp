#include <stdio.h>
#include "yal.h"

int main(){

  // ベクトルの定義
  Yal::Vector<3> vec = {3.2, 4.2, 5.3};
  Yal::Vector<3> F = {10.0, 10.0, 30.0};

  // 行列の定義
  Yal::Matrix<3,4> mat = { 100.0,    0.0, -100.0,    0.0, 
                             0.0,  100.0,    0.0, -100.0, 
                           100.0,  100.0,  100.0,  100.0 };
  printf("mat = \n");
  mat.print();

  // 逆行列の計算
  Yal::Matrix<4,3> mat_inv = mat.inv();   
  printf("mat_inv = \n");
  mat_inv.print();

  // 列数の取得
  printf("rowSize = %ld\n", sizeof(mat.row(0))/sizeof(mat.row(0)[0]));
  printf("rowSize = %d\n", mat.rows());

  // 行数の取得
  printf("columnSize = %ld\n", sizeof(mat.column(0))/sizeof(mat.column(0)[0]));
  printf("columnSize = %d\n", mat.columns());

  // 単位行列の定義
  Yal::Matrix<3,3> E(1);
  // E.unitize();
  printf("E = \n");
  E.print();

  Yal::Matrix<1,1> Q = {4.0};
  // Q = Q * F;
  printf("Q = ");
  Q.print();
  
  // 行列とベクトルの掛け算
  Yal::Vector<4> I = mat_inv * F;
  printf("I = \n");
  I.print();

  double I_max = 0.18;
  double I_min = 0.0;

  for(int i=0; i<4; i++){
    if(I[i] > I_max){
      I[i] = I_max;
    } 
    else if(I[i] < I_min) {
      I[i] = I_min;
    }
  }
  printf("I = \n");
  I.print();


  // -------- Datalistテスト ---------------------
  // double a = 3.0;
  // Yal::Datalist<double> list;
  // list.append(a);
  // printf("%f\n",list[0]);

  // Yal::Datalist2D<double> list2D(3);
  // list2D.append(1, 13.0);
  // printf("%f\n",list2D[1][0]);

  // -------- vstackテスト ---------------------
  Yal::Matrix<2,2> mat_u = { 100.0,    0.0, 
                             0.0, -100.0 };
  
  Yal::Matrix<3,2> mat_l = { 140.0,    0.0, 
                            40.0, -120.0 ,
                            30.0,  320.0 };
  
  Yal::Matrix<5,2> mat_c = Yal::vstack(mat_u, mat_l);
  mat_c.print();

  // -------- LPFテスト ---------------------
  Yal::Vector<6> x = {3.2, 4.2, 5.3, 3.2, 4.2, 5.3};
  Yal::Vector<6> x_filter;


  double y = 3.0;
  double y_fil;

  Yal::LPF<Yal::Vector<6>> lpf;
  lpf.set_fc(100.0);

  printf("------ LPFテスト -------\n");
  x.print();
  lpf.update(x, x_filter);
  x_filter.print();
  printf("-----------------------\n");
  
  // -------- cutテスト ---------------------
  Yal::Vector<3> x_cut = x.cut<2,4>();
  x_cut.print();


  // --------- アドミタンステスト --------------------
  Yal::Matrix<3,3> Md = { 1.0, 0.0, 0.0,
                          0.0, 1.0, 0.0,
                          0.0, 0.0, 1.0 };
  Yal::Matrix<3,3> Cd = { 100.0, 0.0, 0.0,
                          0.0, 100.0, 0.0,
                          0.0, 0.0, 100.0 };
  Yal::Matrix<3,3> Kd = { 1000.0,    0.0,    0.0,
                             0.0, 1000.0,    0.0,
                             0.0,    0.0, 1000.0 };
  Yal::impedance_control<Yal::Vector<3>, Yal::Matrix<3,3>> adm(Md,Cd,Kd);

  Yal::Vector<3> f_human = {1.0, 0.0, 0.0};

  Yal::Vector<3> dx;
  Yal::Vector<3> pos;

  for(int j=0; j<10; j++){
    dx = adm.admitance_out(f_human);
    // dx.print();
    pos += dx;
    // pos.print();
  }

  // --------- カルマンフィルタテスト --------------------

  
  return 0;
}

