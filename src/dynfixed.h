#ifndef FIXED_H
#define FIXED_H
#include "stdint.h"

/* global set in main */
/*extern  double SCALE;*/
typedef struct fixed_point
{
  int64_t val;
  int expLog ;
  long double orig;
} fixed_point_t;

fixed_point_t  domult(fixed_point_t l, fixed_point_t r);
fixed_point_t  dosub(fixed_point_t l, fixed_point_t r);
fixed_point_t  doadd(fixed_point_t l, fixed_point_t r);
fixed_point_t  dofloor(fixed_point_t l);

fixed_point_t getAbs(fixed_point_t f);

fixed_point_t  convert_in(float input);

fixed_point_t fxabs(fixed_point_t f);

long double convert_out(fixed_point_t output);
float convert_out_float(fixed_point_t output);
void setPrecision(int precision);
int getPrecision();
void printfp(fixed_point_t fp, const char * msg);



int dfp_intialize(const char * name);
int dfp_finalize();

extern int allowDynamic;
#endif
