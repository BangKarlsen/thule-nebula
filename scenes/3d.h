#ifndef THREED_H 
#define THREED_H
// Cube
/* #define NUM_VERT 8 */
/* #define NUM_FACES 6 */

/* struct Vertices */
/* { */
/*     int x[NUM_VERT]; */
/*     int y[NUM_VERT]; */
/*     int z[NUM_VERT]; */
/* }; */

/* struct Faces */
/* { */
/*     int vi[4]; */
/* }; */


void init_3d();

void tick_3d(char* screen);

void deinit_3d();

static short sin256[] =
{  0,   -6,  -12,  -18,  -24,  -31,  -37,  -43,  -49,  -55,  -61,  -68, 
 -74,  -79,  -85,  -91,  -97, -103, -109, -114, -120, -125, -131, -136,
-141, -146, -151, -156, -161, -166, -171, -175, -180, -184, -188, -193,
-197, -201, -204, -208, -212, -215, -218, -221, -224, -227, -230, -233,
-235, -237, -240, -242, -244, -245, -247, -248, -250, -251, -252, -253, 
-253, -254, -254, -254, -255, -254, -254, -254, -253, -253, -252, -251, 
-250, -248, -247, -245, -244, -242, -240, -237, -235, -233, -230, -227, 
-224, -221, -218, -215, -212, -208, -204, -201, -197, -193, -188, -184, 
-180, -175, -171, -166, -161, -156, -151, -146, -141, -136, -131, -125, 
-120, -114, -109, -103,  -97,  -91,  -85,  -79,  -74,  -68,  -61,  -55, 
 -49,  -43,  -37,  -31,  -24,  -18,  -12,   -6,    0,    6,   12,   18, 
  24,   31,   37,   43,   49,   55,   61,   68,   74,   79,   85,   91, 
  97,  103,  109,  114,  120,  125,  131,  136,  141,  146,  151,  156,
 161,  166,  171,  175,  180,  184,  188,  193,  197,  201,  204,  208,
 212,  215,  218,  221,  224,  227,  230,  233,  235,  237,  240,  242, 
 244,  245,  247,  248,  250,  251,  252,  253,  253,  254,  254,  254, 
 255,  254,  254,  254,  253,  253,  252,  251,  250,  248,  247,  245,
 244,  242,  240,  237,  235,  233,  230,  227,  224,  221,  218,  215, 
 212,  208,  204,  201,  197,  193,  188,  184,  180,  175,  171,  166, 
 161,  156,  151,  146,  141,  136,  131,  125,  120,  114,  109,  103, 
  97,   91,   85,   79,   74,   68,   61,   55,   49,   43,   37,   31,
  24,   18,   12,    6};

static short cos256[] = {
-255, -254, -254, -254, -253, -253, -252, -251, -250, -248, -247, -245, 
-244, -242, -240, -237, -235, -233, -230, -227, -224, -221, -218, -215, 
-212, -208, -204, -201, -197, -193, -188, -184, -180, -175, -171, -166, 
-161, -156, -151, -146, -141, -136, -131, -125, -120, -114, -109, -103, 
 -97,  -91,  -85,  -79,  -74,  -68,  -61,  -55,  -49,  -43,  -37,  -31,
 -24,  -18,  -12,   -6,    0,    6,   12,   18,   24,   31,   37,   43,
  49,   55,   61,   68,   74,   79,   85,   91,   97,  103,  109,  114, 
 120,  125,  131,  136,  141,  146,  151,  156,  161,  166,  171,  175,
 180,  184,  188,  193,  197,  201,  204,  208,  212,  215,  218,  221,
 224,  227,  230,  233,  235,  237,  240,  242,  244,  245,  247,  248,
 250,  251,  252,  253,  253,  254,  254,  254,  255,  254,  254,  254,
 253,  253,  252,  251,  250,  248,  247,  245,  244,  242,  240,  237,
 235,  233,  230,  227,  224,  221,  218,  215,  212,  208,  204,  201,
 197,  193,  188,  184,  180,  175,  171,  166,  161,  156,  151,  146,
 141,  136,  131,  125,  120,  114,  109,  103,   97,   91,   85,   79,
  74,   68,   61,   55,   49,   43,   37,   31,   24,   18,   12,    6,
   0,   -6,  -12,  -18,  -24,  -31,  -37,  -43,  -49,  -55,  -61,  -68,
 -74,  -79,  -85,  -91,  -97, -103, -109, -114, -120, -125, -131, -136, 
-141, -146, -151, -156, -161, -166, -171, -175, -180, -184, -188, -193, 
-197, -201, -204, -208, -212, -215, -218, -221, -224, -227, -230, -233, 
-235, -237, -240, -242, -244, -245, -247, -248, -250, -251, -252, -253, 
-253, -254, -254, -254};

#endif
