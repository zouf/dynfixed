#include "dynfixed.h"
#include "stdio.h"
#include "stdlib.h"
#include "stdint.h"
#include "math.h"
#include <assert.h>
#include <Python.h>

/*double SCALE;*/
static __int128_t MASK = 0x3FFFF;
int PRECISION = 18;
int allowDynamic = 1;
static const int NUMWHOLEBITS = 20;

int dfp_initialize( const char * name)
{

}
int dfp_finalize()
{

}

void printfp(fixed_point_t fp, const char * msg)
{
  printf("FP print  in %s =  %ld explog is %d as fp %Lf\n",
      msg, fp.val, fp.expLog, convert_out(fp) );
  printf("\t\t---orig is %Le\n",fp.orig);
}

int getPrecision(int precision)
{
  return PRECISION;
}

void setPrecision(int precision)
{
  int i;
  MASK = 1;
  for(i = 0; i < precision; ++i)
  {
    MASK = (MASK << 1) + 1;
  }
  MASK = (MASK << 1) + 1;
  MASK = 0xFFFFFFFFFFFFFFFF;
  PRECISION = precision;
}


void normalize(fixed_point_t *l, fixed_point_t *r)
{
  int llog = l->expLog;
  int rlog = r->expLog;
  /* NORMALIZE */
/*  printfp(*l,"normalize l");
  printfp(*r,"normalize r");*/
  __int128_t rval = r->val;
  __int128_t lval = l->val;
  if ((__int128_t)lval==(__int128_t)0)
  {
/*    printf("in l->val 0");*/
    l->expLog = r->expLog;
    return;
  }
  if ((__int128_t)rval==(__int128_t)0)
  {
/*    printf("in r->val 0");*/
    r->expLog = l->expLog;
    return;
  }
  if (rlog > llog)
  {
    /* if l value is larger than r value */
    int diff = rlog - llog;
    long double diffMag = pow(10,(int)diff);
/*    printf("diffmag1 is %Lf\n", diffMag);*/
    r->val = r->val >> diff;
    r->expLog = l->expLog;
    /*
    l->val = l->val*diffMag;
    l->expLog = r->expLog;*/
  }
  else if (rlog < llog)
  {
    int diff = llog - rlog;
    long double diffMag = pow(10,(int)diff);
/*    printf("diffmag2 is %Lf\n", diffMag);*/
/*    r->val = r->val*diffMag;
    r->expLog = l->expLog;*/
    l->val = l->val >> diff;
    l->expLog = r->expLog;
    }
  else
  {
    /* we're good*/
  }

}

fixed_point_t  domult(fixed_point_t l, fixed_point_t r)
{
  int posneg;
  fixed_point_t fp;
  normalize(&l,&r);
  fp.expLog = l.expLog + r.expLog;
  __int128_t tval;
  tval = (l.val * r.val);
  if( allowDynamic == 1)
  {
    while (abs(tval) > (1<<(PRECISION)))
    {
      /* overflow */
      /*printf("overflow detected val is %d and the l is %d and r is %d\n", tval, l.val,r.val);*/
      fp.expLog -=1;
      tval = tval >> 1;
    }
  }
  posneg = (fp.val > 0) ? 1 : -1;
  fp.val = (tval & MASK) * posneg;
  fp.val = tval;
  fp.orig = l.orig * r.orig;

  /*printfp(l,"multval_l");
  printfp(r,"multval_r");
  printfp(fp,"multval result");*/
/*  float lconv = convert_out(l);
  float rconv = convert_out(r);
  float rres = convert_out(fp);
  if (abs(rres - (rconv*lconv)) > 10)
  {
    printf("Validation error mult l=%f r =%f, rres = %f!\n", lconv,rconv, rres);
    printfp(l,"mult-left param");
    printfp(r,"mult-right param");
    printfp(fp,"mult-result");
    exit(1);
    }*/
  return fp;
}

fixed_point_t  dosub(fixed_point_t l, fixed_point_t r)
{
  fixed_point_t tmp = r;
  tmp.val = tmp.val * -1;
  tmp.orig = tmp.orig * -1;
  return doadd(l,tmp);
}


fixed_point_t  doadd(fixed_point_t l, fixed_point_t r)
{
  int posneg;
  fixed_point_t fp;
  __int128_t tval;
  normalize(&l,&r);
/*  printfp(l,"l before");
  printfp(r,"r before");*/

  fp.expLog = l.expLog;
  tval = (l.val + r.val);
  if(allowDynamic == 1)
  {
    while (abs(tval) > (1 << PRECISION))
    {
      /* overflow */
  /*    printf("overflow in add!!!\n");
      printf("lval is %ld rval is %ld\n", l.val,r.val);*/
      fp.expLog -= 1;
      tval = tval >> 1;
    }
  }
  posneg = (fp.val > 0) ? 1 : -1;
  fp.val = (tval & MASK) * posneg;
  fp.val = tval;
  fp.orig = l.orig + r.orig;
  /*
  printfp(l,"addval -lval");
  printfp(r,"addval -rval");
  printfp(fp,"addval");
  */

/*
  float lconv = convert_out(l);
  float rconv = convert_out(r);
  float rres = convert_out(fp);
  if (abs(rres - (rconv+lconv)) > 10)
  {
    printf("Validation error add l=%f r =%f, rres = %f!\n", lconv,rconv, rres);
    printfp(fp,"add");
    exit(1);
  }*/

  //long double res = convert_out(fp);
  //assert( abs(res - fp.orig) < 0.5 && "problem in add");

  return fp;
}

fixed_point_t  dofloor(fixed_point_t l)
{
  fixed_point_t x;
  x.val = l.val >> l.expLog;
  x.expLog = 0;
  x.orig = floor(l.orig);
  return x;
}

fixed_point_t  convert_in(float input)
{
  int i, posneg;
  fixed_point_t fp;
  fp.expLog = 0;/*(int)log10(SCALE);*/

  fp.val = (__int128_t)input;
  if(fabs(input-0)<1e-20)
  {
    fp.val = 0;
    fp.expLog=0;
    return fp;
  }

  __int128_t inCtr = 1 << 1;
#define VAL (input*(float)inCtr)

  if(allowDynamic == 1)
  {

    while( (abs(VAL) < (1 << PRECISION))
        && fp.expLog < (sizeof(inCtr)*8-1) )
    {
      fp.expLog+=1;
      fp.val = VAL;
      inCtr = inCtr << 1;
    }
    printf("the val is %d\n",fp.val);
  } else {
    for(i = 0; i < (PRECISION - NUMWHOLEBITS); ++i)
    {
      fp.expLog+=1;
      fp.val = VAL;
      inCtr = inCtr << 1;
    }
  }
  //printf("fp.expLog %d\n", fp.expLog);
  fp.val += (input >= 0 ? 0.5 : -0.5);
  fp.orig = (double)input;
  //printfp(fp,"conv_in");
  posneg = (fp.val > 0) ? 1 : -1;
  fp.val = (abs(fp.val) & MASK) * posneg;
  //printfp(fp,"conv_in");
  return fp;
}

long double convert_out(fixed_point_t output)
{
/*  printfp(output,"conv_out");*/
/*  printf("expVal is %f\n", expVal);*/
/*  printf("rval is %f\n", rval);*/
/*  rval = rval/(double)SCALE;*/
  //printf("output.val %d as long double %Lf\n", output.val, (long double)output.val);
  printf("exp log is %d\n", output.expLog);
  printf("printed as dec %Le\n", (__int128_t)(1 << output.expLog));
  printf("printed as float %Lf\n", (long double)(1 << output.expLog));
  printf("val is %Ld\n", (output.val));
  long double rval = (long double)(output.val / (long double)(1 << output.expLog));
  //printf("val is %x expLog is %d\n", output.val, output.expLog);
  //printf("Returning %Lf\n", rval);
  return rval;
}

float convert_out_float(fixed_point_t output)
{
/*  printfp(output,"conv_out");*/
/*  printf("expVal is %f\n", expVal);*/
/*  printf("rval is %f\n", rval);*/
/*  rval = rval/(double)SCALE;*/
  //printf("output.val %d as long double %Lf\n", output.val, (long double)output.val);
  float rval = (float)(output.val / (float)(1 << output.expLog));
  //printf("val is %x expLog is %d\n", output.val, output.expLog);
  //printf("Returning %Lf\n", rval);
  return rval;
}

fixed_point_t fxabs(fixed_point_t f)
{
  if (f.val >= 0)
    return f;
  else
  {
    f.val = f.val * -1;
    return f;
  }
}


