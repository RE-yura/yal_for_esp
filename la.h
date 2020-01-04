/** @file 
 * Linear Algebra 線形代数
 * @author Kotaro Tadano
 */
#ifndef _YAL_LA
#define _YAL_LA

#include <stdio.h>
#include <initializer_list>
#include <stdarg.h>
#include <utility> // for std:swap
#include <cmath>

namespace Yal{

// ----- ベクトル ----------------------------------------------------------
template<int size, typename T = double> class Vector{
  private:
    double v[size];

  public:
    // 全ての要素を0で初期化
    Vector(){
      for (int i = 0; i < size; i++) v[i] = 0.0;
    }
    // 初期値指定コンストラクタ
    Vector(std::initializer_list<T> V){
      int i = 0;
      for(auto iter = V.begin(); iter != V.end(); iter++){
        v[i] = *iter;
        i += 1;
      }
    }
    
    // 配列のi番目の値を返す
    double &operator[](unsigned int i) { return v[i]; }
    const double operator[](unsigned int i) const { return v[i]; }
    
    // 値代入
    Vector &operator=(std::initializer_list<T> V){
      int i = 0;
      for(auto iter = V.begin(); iter != V.end(); iter++){
        v[i] = *iter;
        i += 1;
      }
      return *this;
    }
    // コピー代入
    Vector &operator=(const Vector &V){
      for (int i = 0; i < size; i++) v[i] = V[i];
      return *this;
    }

    /** 負のベクトル */
    Vector operator-()const{
      Vector Vret = *this;
      for(int i=0; i<size;i++)  Vret[i] *= -1;
      return Vret;
    }
    
    // 全要素を0にする
    void zero(){
      for (int i = 0; i < size; i++) v[i] = 0;            
    }

    /** 標準出力への値表示 */
    void print(int n = 1) const{
      printf("[ ");
      for (int i = 0; i < size; i++) printf("%10.3f ", v[i]);
      putchar(']');
      for (int i = 0; i < n; i++) putchar('\n');
    }

    /** ベクトル加算 */
    Vector& operator+=(const Vector& V){
      for(int i=0; i<size; i++)   v[i] += V[i];
      return *this;
    }
    Vector operator+(const Vector& V) const {
      Vector Vret = *this;
      Vret += V;
      return Vret;
    }

    /** ベクトル減算 */
    Vector& operator-=(const Vector& V){
      for(int i=0; i<size; i++)   v[i] -= V[i];
      return *this;
    }
    Vector operator-(const Vector& V) const{
      Vector Vret = *this;
      Vret -= V;
      return Vret;
    }

    /** スカラー乗算 */
    Vector& operator*=(double c){
      for(int i=0; i<size; i++)  v[i] *= c;
      return *this;
    }
    Vector operator*(double c) const{
      Vector Vret = *this;
      Vret *= c;
      return Vret;
    }
    friend Vector<size> operator*(double c,const Vector<size>& V){ //スカラー * vec
      return V*c;
    }
      
    /** スカラー除算 */
    Vector& operator/=(double c){
      double _c = 1.0/c;
      for(int i=0; i<size; i++)  v[i] *= _c;
      return *this;
    }
    Vector operator/(double c) const{
      Vector Vret = *this;
      Vret /= c;
      return Vret;
    }

    // ベクトルの一部を取得 
    template<int start, int end>
    Vector<end-start+1> cut() { 
      Vector<end-start+1> Vret;
      for(int j=start; j<end+1; j++) Vret[j-start] = v[j];
      return Vret; 
    }

    // 絶対値の2乗 平方根を計算しない
    double abs2()const {
      double value=0;
      for(int i=0;i<size;i++)  value += v[i] * v[i];
      return value;
    }

    // 絶対値を計算する
    double abs()const {
      double value=0;
      for(int i=0;i<size;i++)  value += v[i] * v[i];
      return sqrt(value);
    }
    
    // スカラー倍する
    void extend(double c){ 
      *this *= c;
    }

    // 規格化する
    void normalize(){
      extend( 1.0/abs() );
    }

    /** 内積.
     * @attention ( )で囲むこと ex. (a&b)
     */
    double operator&(const Vector& V) const{
      double value=0;
      for(int i=0;i<size;i++)
        value += v[i] * V[i];
      return value;
    }

};

// ========== 行列 (ｈ行ｗ列) =============================
template <int h, int w> class Matrix{
  private:
    Vector<w> v[h]; // 横ベクトルを縦に並べたもの

  public:
    Matrix(){    // 零行列として初期化
      for (int i = 0; i < h; i++)
        for (int j = 0; j < w; j++)
          v[i][j] = 0.0;
    }

    /** 全ての要素の値を指定して生成
     *  @param v00 最初の要素値
     */
    Matrix(double v00 , ...) {
      va_list args;
      va_start(args ,v00);
      v[0][0] = v00;
      for (int i = 0 ; i < h ; i++) {
        for (int j = 0 ; j < w ; j++){
          if( i == 0 && j == 0 ) continue;
          v[i][j] = (double) va_arg(args , double);
        }
      }
      va_end(args);
    }

    /** 零行列 or 単位行列の生成
     *  @param type zero->零行列 identify->単位行列
     */
    Matrix(int n){  
      switch (n)
      {
      case 0:
        for (int i = 0; i < h; i++)
          for (int j = 0; j < w; j++)
            v[i][j] = 0.0;
        break;
      
      case 1:
        if(h != w){
          printf("正方行列ではありません．\n");
          break;
        }
        for (int i = 0; i < h; i++)
          for (int j = 0; j < w; j++)
            v[i][j] = (i==j) ? 1.0 : 0.0;
        break;
      
      default:
        printf("0か1を入力して下さい．(要素指定ならdoubleを入力)\n");
        break;
      }
    }
    
    Vector<w>& operator[](unsigned int i){ return v[i]; }
    const Vector<w> operator[](unsigned int i)const { return v[i]; }

    /** 標準出力への表示 
    * @param n 改行の数
    */
    void print(int n = 1) const{
      for (int i = 0; i < h; i++){
        for (int j = 0; j < w; j++) printf("%10.3f ", v[i][j]);
        putchar('\n');
      }
      for (int i = 0; i < n - 1; i++) putchar('\n');
    }

    /** 行ベクトルの取得 
     *  @param n 行のインデックス
     *  @return n行目の行ベクトル
     */ 
    const Vector<w> row(unsigned int n)const { return v[n]; }
    /** 行ベクトルへの代入 
     *  @param n 行のインデックス
     *  @param V 代入するベクトル
     */
    void row(unsigned int n,const Vector<w>& V){ 
      v[n] = V;
    }
    /** 列数の取得 
     *  @return 取得した列数
     */
    int rows(){
      return w;
    }

    /** 列ベクトルの取得 
     *  @param n 列のインデックス
     *  @return n列目の列ベクトル
     */ 
    const Vector<h> column(unsigned int n)const { 
      Vector<h> Vret;
      for(int i=0; i<h;i++ ) Vret[i] = v[i][n];
      return Vret; 
    }
    /** 列ベクトルへの代入 
     *  @param n 列のインデックス
     *  @param V 代入するベクトル
     */
    void column(unsigned int n,const Vector<h>& V){ 
      for(int i=0; i<h;i++ ) v[i][n] = V[i];
    }
    /** 行数の取得 
     *  @return 取得した行数
     */
    int columns(){
      return h;
    }

    
    /** 単位行列化する.   */
    void unitize(){
      for(int i=0;i<h;i++)
        for(int j=0;j<w;j++)
	        v[i][j] = (i==j) ? 1.0 : 0.0;
    }

    /** 要素を全て0にする.   */
    void zero(){
      for(int i=0;i<h;i++)
        v[i].zero();
    }

    /** 転置演算子 */
    Matrix<w,h> operator~() const{ 
      Matrix<w,h> Mret;
      for(int i=0; i<w;i++ )
        for(int j=0; j<h;j++ )
          Mret[i][j] = v[j][i];
      return Mret;
    }

    // 四則演算
    Matrix& operator+=(const Matrix& M){
      for(int i=0;i<h;i++)  (*this)[i] += M[i];
      return *this;
    }
    Matrix& operator-=(const Matrix& M){
      for(int i=0;i<h;i++)  (*this)[i] -= M[i];
      return *this;
    }
    Matrix operator+(const Matrix& M) const{
      Matrix Mret= *this; 
      Mret += M;
      return Mret;
    }
    Matrix operator-(const Matrix& M) const{
      Matrix Mret= *this;
      Mret -= M;
      return Mret;
    }
    Matrix& operator*=(const double a){
      for(int i=0;i<h;i++)  (*this)[i] *= a;
      return *this;
    }
    Matrix& operator/=(const double a){
      for(int i=0;i<h;i++)  (*this)[i] /= a;
      return *this;
    }
    Matrix operator*(double c) const{
      Matrix Mret = *this;
      Mret *= c;
      return Mret;
    }
    Matrix operator/(double c) const{
      Matrix Mret = *this;
      Mret /= c;
      return Mret;
    }

    template <int M_w> 
    Matrix<h,M_w> operator*(const Matrix<w,M_w>& M) const{
      Matrix<h,M_w> Mret;
      for(int i=0; i<h; i++){
        for(int j=0; j<M_w; j++){
          Mret[i][j] = 0.0;
          for(int k=0; k<w; k++)
            Mret[i][j] += v[i][k] * M[k][j];
        }
      }
      return Mret;
    }

    Vector<h> operator*(const Vector<w>& V)const{
      Vector<h> Vret;
      for(int i=0; i<h; i++)  Vret[i] += (v[i] & V);
      return Vret;
    }
    friend Matrix<h,w> operator*(double c, const Matrix<h,w>& M){
      Matrix<h,w> Mret;
      for(int i=0; i<h; i++)  Mret[i] = M[i] * c;
      return Mret;
    }
    friend Matrix<h,w> operator/(double c, const Matrix<h,w>& M){
      Matrix<h,w> Mret;
      for(int i=0; i<h; i++)  Mret[i] =  M[i] / c;
      return Mret;
    }

    /** LU分解を行う.
     * @param s ピボットのインデックス
     * @return ピボット交換の回数
     */
    int decompose(int *s){ // LU fact
      int i, j, mx;
      int sgn = 1;

      for (i=0; i<h ; i++) s[i] = i; // index of pivot excheange

      for (int k = 0; k < h - 1; k++) { // check pivot
        for (i = k+1 , mx = k; i < h; i++)
          if( fabs(v[i][k]) > fabs(v[mx][k]) )
            mx = i;

        if( fabs( v[mx][k]) < 1e-6 ){
          fprintf(stderr,"Ktl::Matrix::decompse : Warning : pivot is too small\n");
          return 0;
        }

        if (mx != k){ //exchange pivot index
          std::swap( s[k], s[mx] );
          sgn *= -1;
        }
        for (i = k+1 ; i < h; i++) { // forward elimination
          v[s[i]][k] /= v[s[k]][k]; // mulit
          for (j = k+1; j < w; j++)
            v[s[i]][j] -= v[s[i]][k] * v[s[k]][j];
        }
      }
      return sgn;
    }

    /** 行列式を計算する.
     * @return 行列式の値
     */
    double det() const{
      int s[h];
      Matrix LU = *this;
      double det = LU.decompose(s);
      for(int i=0;i<h;i++)  det *= LU[s[i]][i];
      return det;
    }

    /** 逆行列を計算する.
     * @return 逆行列
     */
    Matrix<w,h> inv()const{

      if( w > h ){ // 横長
        const Matrix<h,w>& A = *this;
        return  ~A * (A*~A).inv();
      }
      else if( w < h ){ // 縦長
        const Matrix<h,w>& A = *this;
        return  (~A*A).inv() * ~A;
      }
      else{ // 正方行列
        if( w==3 ){ // 3x3 LU分解に比べ25%速い
          Matrix<w,h> Mret( v[1][1]*v[2][2] - v[1][2]*v[2][1],
                            v[2][1]*v[0][2] - v[2][2]*v[0][1],
                            v[0][1]*v[1][2] - v[0][2]*v[1][1],
                            v[1][2]*v[2][0] - v[1][0]*v[2][2],
                            v[2][2]*v[0][0] - v[2][0]*v[0][2],
                            v[0][2]*v[1][0] - v[0][0]*v[1][2],
                            v[1][0]*v[2][1] - v[1][1]*v[2][0],
                            v[2][0]*v[0][1] - v[2][1]*v[0][0],
                            v[0][0]*v[1][1] - v[0][1]*v[1][0] );
              
          double det = v[0][0]*Mret[0][0]+v[1][0]*Mret[0][1]+v[2][0]*Mret[0][2];

          if( fabs(det) < 1e-9 ){
            fprintf(stderr,"Ktl::Matrix::inv : Warning : det << 1\n");
          }
          return (1.0/det) * Mret;
        }
        else{
          Matrix<w,h> Mret;
          Mret.unitize();
          int s[h];
          Matrix LU = *this;
          LU.decompose(s);
          for( int k = 0; k < h; k++ ){
            for( int i = 0; i < h; i++ ){    /* 解を求める */
              for( int j = i+1; j < h; j++ )
                Mret[s[j]][k] -= Mret[s[i]][k] * LU[s[j]][i];
            }
            for( int i = h-1; i >= 0; i-- ){
              for( int j = i+1; j<h; j++ )
                Mret[s[i]][k] -= LU[s[i]][j] * Mret[s[j]][k];
                Mret[s[i]][k] /= LU[s[i]][i];
            }
          }
          for( int i = 0; i < h; i++ ){  
            if( i == s[i] ) continue;
            for( int j = 0; j < w; j++ )
              std::swap( Mret[s[s[i]]][j], Mret[s[i]][j] );
            std::swap( s[s[i]], s[i] ); //indexを元に戻していく
          }
          return Mret;
        }
      } 
    }
};

// 行列の結合（行・縦方向）vertical stack
template<int w, int h1, int h2>
Matrix<h1+h2,w> vstack(const Matrix<h1,w>& M1, const Matrix<h2,w>& M2){
    
  Matrix<h1+h2,w> Com_M;
  for(int i=0; i<h1; i++)
    for(int j=0; j<w; j++)
      Com_M[i][j] = M1[i][j];
  for(int i=h1; i<h1+h2; i++)
    for(int j=0; j<w; j++)
      Com_M[i][j] = M2[i-h1][j];
  return Com_M;
}

}

#endif



