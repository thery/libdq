


#include "dq.h"

#include <stdio.h>
#include <math.h>
#include <string.h>


void dq_cr_rotation( dq_t out, double zita, double s[3], double c[3] )
{
   double ss, cs;

   /* Store sin and cos values to speed up calculations. */
   ss = sin( zita/2. );
   cs = cos( zita/2. );

   out[0] = cs;
   out[1] = ss*s[0];
   out[2] = ss*s[1];
   out[3] = ss*s[2];
   /* We do cross product with the line point and line vector to get the plucker coordinates. */
   out[4] = ss*(  s[1]*c[2] - s[2]*c[1] );
   out[5] = ss*( -s[0]*c[2] + s[2]*c[0] );
   out[6] = ss*(  s[0]*c[1] - s[1]*c[0] );
   out[7] = 0.;
}


void dq_cr_translation( dq_t out, double t[3] )
{
   out[0] = 1.;
   out[1] = 0.;
   out[2] = 0.;
   out[3] = 0.;
   out[4] = t[0] / 2.;
   out[5] = t[1] / 2.;
   out[6] = t[2] / 2.;
   out[7] = 0.;
}


void dq_cr_point( dq_t out, double pos[3] )
{
   out[0] = 1.;
   out[1] = 0.;
   out[2] = 0.;
   out[3] = 0.;
   out[4] = pos[0];
   out[5] = pos[1];
   out[6] = pos[2];
   out[7] = 0.;
}


void dq_cr_copy( dq_t out, dq_t in )
{
   int i;
   for (i=0; i<8; i++)
      out[i] = in[i];
}


void dq_op_add( dq_t out, dq_t in_p, dq_t in_q )
{
   int i;
   for (i=0; i<8; i++)
      out[i] = in_p[i] + in_q[i];
}


void dq_op_mul( dq_t pq, dq_t p, dq_t q )
{
   /* Real quaternion. */
   pq[0] = p[0]*q[0] - p[1]*q[1] - p[2]*q[2] - p[3]*q[3];
   pq[1] = p[0]*q[1] + p[1]*q[0] + p[2]*q[3] - p[3]*q[2];
   pq[2] = p[0]*q[2] + p[2]*q[0] + p[1]*q[3] - p[3]*q[1];
   pq[3] = p[0]*q[3] + p[3]*q[0] + p[1]*q[2] - p[2]*q[1];

   /* Dual unit quaternion. */
   pq[4] = p[0]*q[4] + p[4]*q[0] + p[1]*q[7] - p[7]*q[1] +
           p[6]*q[2] - p[2]*q[6] + p[5]*q[3] - p[3]*q[5];
   pq[5] = p[0]*q[5] + p[5]*q[0] - p[1]*q[6] + p[6]*q[1] -
           p[7]*q[2] + p[2]*q[7] + p[5]*q[3] - p[3]*q[4];
   pq[6] = p[0]*q[6] + p[6]*q[0] + p[1]*q[5] - p[5]*q[1] +
           p[4]*q[2] - p[2]*q[4] + p[7]*q[3] - p[3]*q[7];
   pq[7] = p[0]*q[7] + p[7]*q[0] - p[1]*q[4] - p[4]*q[1] -
           p[2]*q[5] - p[5]*q[2] - p[3]*q[6] - p[6]*q[3];
}


void dq_op_f2g( dq_t ABA, dq_t A, dq_t B )
{
   dq_t tmp, Astar;

   dq_op_mul( tmp, A, B );
   dq_cr_copy( Astar, A );
   dq_op_mul( ABA, tmp, Astar );
}


void dq_op_f4g( dq_t ABA, dq_t A, dq_t B )
{
   dq_t tmp, Astar;

   dq_op_mul( tmp, A, B );
   Astar[0] =  A[0];
   Astar[1] = -A[1];
   Astar[2] = -A[2];
   Astar[3] = -A[3];
   Astar[4] =  A[4];
   Astar[5] =  A[5];
   Astar[6] =  A[6];
   Astar[7] = -A[7];
   dq_op_mul( ABA, tmp, Astar );
}


int dq_cmp( dq_t p, dq_t q )
{
   int i, ret;
   
   ret = 0;
   for (i=0; i<8; i++)
      if (fabs(p[i]-q[i]) > 1e-10)
         ret++;

   return ret;
}


void dq_print( dq_t dq )
{
   printf( "%.2f + %.2fi + %.2fj + %.2fk + %.2fie + %.2fje + %.2fke + %.2fe\n",
         dq[0], dq[1], dq[2], dq[3], dq[4], dq[5], dq[6], dq[7] );
}


void dq_print_vert( dq_t dq )
{
   printf( "   %.2f   %.2f\n", dq[1], dq[4] );
   printf( "   %.2f   %.2f\n", dq[2], dq[5] );
   printf( "   %.2f + %.2f\n", dq[3], dq[6] );
   printf( "   %.2f   %.2f\n", dq[0], dq[7] );
}



