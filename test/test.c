#include "dynfixed.h"
#include "stdio.h"
#include "stdint.h"
#include <stdlib.h>

int main(int argc, char** argv)
{
  if( argc == 4 )
  {
    float a = atof(argv[2]);
    float b = atof(argv[3]);
    float resf;
    setPrecision(atoi(argv[1]));
    //allowDynamic = 0;
    printf("vals %f %f\n", a, b);
    fixed_point_t fa = convert_in(a);
    fixed_point_t fb = convert_in(b);
    fixed_point_t res;
    printfp(fa, "first");
    printfp(fb, "second");
    res = doadd(fa, fb);
    resf = convert_out(res);
    printf("add ours %f orig %Lf\n", resf, res.orig);

    res = dosub(fa, fb);
    resf = convert_out(res);
    printf("sub ours %f orig %Lf\n", (resf), res.orig);

    res = domult(fa, fb);
    resf = convert_out(res);
    printf("mult ours %f orig %Lf\n", (resf), res.orig);

    res = dofloor(fa);
    resf = convert_out(res);
    printf("floor ours %f orig %Lf\n", (resf), res.orig);

    return 0;
  }

  float l=3.0;//2.987654;
  float r = 2.01;
  int PRECISION = 30;
  setPrecision(PRECISION);
  fixed_point_t lc = convert_in(l);
  fixed_point_t rc= convert_in(r);
  allowDynamic = 1;
  int i;
  fixed_point_t res = doadd(lc,rc);
  long double check = l+r;
  for(i = 0; i < 2; i++)
  {
    check = check*check;
    res = domult(res,res);
  }
  res = dosub(res,convert_in(10.0));
  check -=10.0;
 // printf("Check against %f\n", check);
 // printfp(res,"restest");
 // printfp(lc,"L ");
  //printfp(rc,"R ");
//  long double lconv = convert_out(lc);
 // long double rconv = convert_out(rc);
//  printf("lconv is %Lf rconv is %Lf\n", lconv,rconv);
  long double result = convert_out(res);
  printf("Final val is %Lf. Should be %Lf\n", result,check);


  printf("\n Checking very small numbers times whole numbers\n");
  fixed_point_t small = convert_in(1e-12);
  fixed_point_t one = convert_in(1);
  fixed_point_t res2 = domult(small, one);

  printfp( small, "small");
  printfp( one, "one");
  printfp( res2, "res2");

return 0;
}
