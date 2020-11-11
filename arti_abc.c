/* Simple C program for .wav soundscape generation. (C) P.B.L. Meijer 1996 */
//fonte: https://www.seeingwithsound.com/im2sound.htm
#define FNAME "arti_abc.wav"
#define N     64   /* Resolution, i.e., # rows and columns   */
#define FL   500   /* Lowest  frequency (Hz) in visual sound */
#define FH  5000   /* Highest frequency (Hz)                 */
#define FS 20000   /* Sample  frequency (Hz)                 */
#define T   1.05   /* Image to sound conversion time (s)     */
#define D      1   /* Linear|Exponential=0|1 distribution    */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define C_ALLOC(number, type) ((type *) calloc((number),sizeof(type)) )
#define TwoPi 6.283185307179586476925287

static char *P[] = {  /* 64 x 64 pixels, 16 grey levels a,...,p */
"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa",
"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa",
"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa",
"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa",
"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa",
"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa",
"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa",
"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa",
"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa",
"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa",
"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa",
"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa",
"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa",
"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa",
"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa",
"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa",
"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa",
"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa",
"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa",
"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa",
"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa",
"aaaaaaaaaaaaadaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaabdffecaaaebaaa",
"aaaaaaaaaaaacneaaaaaaaaaglmmmmmmmmkjhcaaaaaaaaaadjmoooonkhhncaaa",
"aaaaaaaaaaaahpiaaaaaaaaaabipppiffhjopohbaaaaaacipoidbabfkppocaaa",
"aaaaaaaaaaabmpncaaaaaaaaaacopocaaaaenppfaaaaabkpoeaaaaaabippdaaa",
"aaaaaaaaaaagppphaaaaaaaaaacopocaaaaahpplaaaabjppfaaaaaaaabkpeaaa",
"aaaaaaaaaabkiopmbaaaaaaaaacopocaaaaacppoaaaafppjaaaaaaaaaaepeaaa",
"aaaaaaaaaaeodippfaaaaaaaaacopocaaaaacppoaaabkppfaaaaaaaaaabjdaaa",
"aaaaaaaaaaijbdppkaaaaaaaaacopocaaaaahppjaaadppnbaaaaaaaaaaacbaaa",
"aaaaaaaaacofabjpoeaaaaaaaacopocaaacgopmdaaagpplaaaaaaaaaaaaaaaaa",
"aaaaaaaaahmbaafppiaaaaaaaacoppjjjkmpmhcaaaaippkaaaaaaaaaaaaaaaaa",
"aaaaaaaabmhaaabmpocaaaaaaacoppiiijmpojdaaaaippkaaaaaaaaaaaaaaaaa",
"aaaaaaaafocaaaaipphaaaaaaacopocaaaadmpogaaaippkaaaaaaaaaaaaaaaaa",
"aaaaaaabkmggggghppmbaaaaaacopocaaaaadoppdaahppmaaaaaaaaaaaaaaaaa",
"aaaaaaaeolllllllnppfaaaaaacopocaaaaaajppiaafppocaaaaaaaaaaaaaaaa",
"aaaaaaailbaaaaaagppkaaaaaacopocaaaaaaippkaacmppgaaaaaaaaaaaaaaaa",
"aaaaaabohaaaaaaacnpoeaaaaacopocaaaaaajppjaaaipplbaaaaaaaaaabbaaa",
"aaaaaahncaaaaaaaaippiaaaaacopocaaaaabnppgaaaclpphaaaaaaaaabifaaa",
"aaaaacnkaaaaaaaaadopodaaaacopocaaaabipplbaaaadnpphbaaaaaaelkcaaa",
"aaaacjplcaaaaaaabdnppmdaacipppjgghinpojcaaaaaackpplheeehkoibaaaa",
"aaabjllljfaaaaaahkllllkefjllllllllljidaaaaaaaaabfjnppppmieaaaaaa",
"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaacdeedbaaaaaaaa",
"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa",
"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa",
"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa",
"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa",
"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa",
"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa",
"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa",
"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa",
"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa",
"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa",
"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa",
"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa",
"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa",
"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa",
"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa",
"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa",
"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa",
"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa",
"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa",
"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa",
"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa",
"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"};

FILE *fp; unsigned long ir=0L, ia=9301L, ic=49297L, im=233280L;
void wi(unsigned int i) { int b1,b0; b0=i%256; b1=(i-b0)/256; putc(b0,fp); putc(b1,fp); }
void wl(long l) { unsigned int i1,i0; i0=l%65536L; i1=(l-i0)/65536L; wi(i0); wi(i1); }
double rnd(void){ ir = (ir*ia+ic) % im; return ir / (1.0*im); }

int main(int argc, char *argv[])
{
   int    i, j, b, d = D;
   long   k = 0L, ns = (long) (FS * T), m = ns / N;
   double **A, t, dt = 1.0 / FS, *w, *phi0, s, y, yp, z, tau1, tau2, scale = 0.5/sqrt(N);
   w    = C_ALLOC(N, double);
   phi0 = C_ALLOC(N, double);
   A    = C_ALLOC(N, double *);
   for (i=0; i<N; i++) A[i] = C_ALLOC(N, double);  /* N x N pixel matrix */

   /* Set hard-coded image */
   for (i=0; i<N; i++) {
      for (j=0; j<N; j++) {
         if (P[i][j]>'a') A[N-i-1][j] = pow(10.0,(P[i][j]-'a'-15)/10.0); /* 2dB steps */
         else A[N-i-1][j] = 0.0;
      }
   }

   /* Set lin|exp (0|1) frequency distribution and random initial phase */
   if (d) for (i=0; i<N; i++) w[i] = TwoPi * FL * pow(1.0* FH/FL,1.0*i/(N-1));
   else   for (i=0; i<N; i++) w[i] = TwoPi * FL + TwoPi * (FH-FL)   *i/(N-1) ;
   for (i=0; i<N; i++) phi0[i] = TwoPi * rnd();

   /* Write 8-bit .wav visual sound file, using rectangular time window */
   fp = fopen(FNAME,"wb");
   fprintf(fp,"RIFF"); wl(ns+36L); fprintf(fp,"WAVEfmt "); wl(16L);
   wi(1); wi(1); wl(0L+FS); wl(0L+FS); wi(1); wi(8); fprintf(fp,"data"); wl(ns);
   tau1 = 0.5 / w[N-1]; tau2 = 0.25 * tau1*tau1; y = z = 0.0; 
   while (k < ns) {  /* Not optimized for speed (or anything else) */
      s = 0.0; t = k * dt; j = k / m; if (j>N-1) j=N-1;
      for (i=0; i<N; i++) s += A[i][j] * sin(w[i] * t + phi0[i]);
      if (k < ns/(5*N)) s = (2.0*rnd()-1.0) / scale;  /* "click" */
      yp = y; y = tau1/dt + tau2/(dt*dt);
      y  = (s + y * yp + tau2/dt * z) / (1.0 + y); z = (y - yp) / dt;
      b  = 128 + (int) (scale * 128 * y); /* y = 2nd order filtered s */
      if (b > 255) b = 255;
      if (b <   0) b =   0;
      putc(b,fp);
      k++;
   }
   if ((ceil(ns/2))!=(ns/2)) putc(0,fp);
   fclose(fp); return(0);
}
