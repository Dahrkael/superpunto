#include "helper.h"

#ifndef _WIN32
double pow(double x, int n){ 
  double res = x;
  for(int i=0;i<n-1;i++)res*=x; 
  return res;
}
#endif
double dist(const vector<double> &a, const vector<double> &b){
  double res = 0.0;
  fori(0,a.size()) res+= pow(a[i]-b[i],2);
  return sqrt(res);
}
double square_dist(const vector<double> &a, const vector<double> &b){
  double res = 0.0;
  fori(0,a.size()) res+= pow(a[i]-b[i],2);
  return abs(res);
}
double square_mod(const vector<double> &a){
  double res = 0.0;
  fori(0,a.size()) res+= pow(a[i],2);
  return abs(res);
}
void rand_vec(int l, double a, double b, vector<double> &res){
  if(res.size()<l) res.resize(l,0);
  fori(0,l)res[i] = RANDESP*(a-b)+b;
}

double vmul(const vector<double> &a, const vector<double> &b){
  double res = 0.0;
  fori(0,a.size()){
    res += a[i]*b[i];
  }
  return res;
}


//From -0.5*bound to 0.5*bound
void get_inbounds(vector<double> &a){
  fori(0,a.size()) a[i]-=int( ( (a[i]<0)?-0.5:0.5 ) + a[i]);
}

