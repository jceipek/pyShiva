#include "test.h"
#include <vg/openvg.h>

// FIXME unsafe
#define min(x,y) ((x<y)?(x):(y))

VGPath backdrop;
VGPaint backdrop_p;
VGImage kirpicz;
VGImage walls;
VGPath wall;
VGPaint wall_p;

VGPaint shadow_p;
VGPath shadows;

VGPaint bevel_h_p;
VGPath bevel_h;

VGPaint bevel_l_p;
VGPath bevel_l;


void display(float phase)
{
  VGfloat cc[] = {0,0,0,1};
  
  vgSetfv(VG_CLEAR_COLOR, 4, cc);
  vgClear(0,0,testWidth(),testHeight());

  /* Detecting the aspect ratio */
  float ratio=min(testWidth()/13,testHeight()/8);
  
  /* Poor man's flip */
  vgSeti(VG_MATRIX_MODE, VG_MATRIX_FILL_PAINT_TO_USER);
  vgLoadIdentity();
  vgScale(1.0/ratio,1.0/ratio);
  
  vgSeti(VG_MATRIX_MODE, VG_MATRIX_PATH_USER_TO_SURFACE);
  vgLoadIdentity();
  vgScale(ratio,-ratio);
  vgTranslate(0,-8); /* TODO center */
  
  
  vgSetPaint(backdrop_p, VG_FILL_PATH);
  vgDrawPath(backdrop,VG_FILL_PATH);
  vgSetPaint(wall_p, VG_FILL_PATH);
  vgDrawPath(wall,VG_FILL_PATH);
  vgSetPaint(shadow_p, VG_FILL_PATH);
  vgDrawPath(shadows, VG_FILL_PATH);
  vgSetPaint(bevel_h_p, VG_FILL_PATH);
  vgDrawPath(bevel_h, VG_FILL_PATH);
  vgSetPaint(bevel_l_p, VG_FILL_PATH);
  vgDrawPath(bevel_l, VG_FILL_PATH);
}

void createPaths()
{
  /* Very silly, I know */
  VGubyte segsbk[]= { 
    VG_MOVE_TO_ABS, VG_LINE_TO_REL, VG_LINE_TO_REL, VG_LINE_TO_REL, VG_CLOSE_PATH };
  VGfloat databk[]= {
    0,0, 13,0, 0,8, -13,0  };
  if (!backdrop)
  {
    backdrop=testCreatePath();
  }
  else vgClearPath(backdrop, VG_PATH_CAPABILITY_ALL);
  vgAppendPathData(backdrop,sizeof(segsbk),segsbk,databk);
  
  VGubyte segwls[]= {
    VG_MOVE_TO_ABS, VG_LINE_TO_REL, VG_LINE_TO_REL, VG_LINE_TO_REL, VG_LINE_TO_REL, VG_LINE_TO_REL, VG_CLOSE_PATH,
    VG_MOVE_TO_ABS, VG_LINE_TO_REL, VG_LINE_TO_REL, VG_LINE_TO_REL, VG_LINE_TO_REL, VG_LINE_TO_REL, VG_LINE_TO_REL, VG_LINE_TO_REL, VG_CLOSE_PATH };
    
  VGfloat datawls[] = {
    8,2, 5,0, 0,3, -2,0, 0,-1, -3,0,
    0,2, 2,0, 0,4, 9,0, 0,1, 2,0, 0,1, -13,0 };
  
  if (!wall)
    wall=testCreatePath();
  else
    vgClearPath(wall, VG_PATH_CAPABILITY_ALL);
  vgAppendPathData(wall,sizeof(segwls),segwls,datawls);
  
  {
    VGubyte segs[]={
      VG_MOVE_TO_ABS,VG_LINE_TO_REL,VG_LINE_TO_REL,VG_LINE_TO_REL,VG_CLOSE_PATH,
      VG_MOVE_TO_ABS,VG_LINE_TO_REL,VG_LINE_TO_REL,VG_LINE_TO_REL,VG_CLOSE_PATH,
      VG_MOVE_TO_ABS,VG_LINE_TO_REL,VG_LINE_TO_REL,VG_LINE_TO_REL,VG_CLOSE_PATH,
      VG_MOVE_TO_ABS,VG_LINE_TO_REL,VG_LINE_TO_REL,VG_LINE_TO_REL,VG_CLOSE_PATH
    };
    VGfloat data[]={
      2,2,0,4,0.7,0,0,-3.3,
      11,6,0,1,0.7,0,0,-0.3,
      8,4,3,0,0,0.7,-2.3,0,
      11,5,2,0,0,0.7,-1.3,0
    };
    
    if (!shadows)
      shadows=testCreatePath();
    else
      vgClearPath(shadows, VG_PATH_CAPABILITY_ALL);
    vgAppendPathData(shadows,sizeof(segs),segs,data);
  }
  
  
  {
    VGubyte segs[]={
      VG_MOVE_TO_ABS,VG_LINE_TO_REL,VG_LINE_TO_REL,VG_LINE_TO_REL,VG_CLOSE_PATH,
      VG_MOVE_TO_ABS,VG_LINE_TO_REL,VG_LINE_TO_REL,VG_LINE_TO_REL,VG_CLOSE_PATH,
      VG_MOVE_TO_ABS,VG_LINE_TO_REL,VG_LINE_TO_REL,VG_LINE_TO_REL,VG_CLOSE_PATH,
      VG_MOVE_TO_ABS,VG_LINE_TO_REL,VG_LINE_TO_REL,VG_LINE_TO_REL,VG_CLOSE_PATH,
      VG_MOVE_TO_ABS,VG_LINE_TO_REL,VG_LINE_TO_REL,VG_LINE_TO_REL,VG_LINE_TO_REL,VG_LINE_TO_REL,VG_CLOSE_PATH
    };
    VGfloat data[]={
      0,2,2,0,-0.3,0.3,-1.7,0,
      2,6,-0.3,0.3,9,0,0.3,-0.3,
      11,7,-0.3,0.3,2.3,0,0,-0.3,
      11,4,0.3,-0.3,0,1,-0.3,0.3,
      8,2,5,0,0,0.3,-4.7,0,0,1.4,-0.3,0.3
    };
    
    if (!bevel_h)
      bevel_h=testCreatePath();
    else
      vgClearPath(bevel_h, VG_PATH_CAPABILITY_ALL);
    vgAppendPathData(bevel_h,sizeof(segs),segs,data);
  }
  
  {
    VGubyte segs[]={
      VG_MOVE_TO_ABS,VG_LINE_TO_REL,VG_LINE_TO_REL,VG_LINE_TO_REL,VG_CLOSE_PATH,
      VG_MOVE_TO_ABS,VG_LINE_TO_REL,VG_LINE_TO_REL,VG_LINE_TO_REL,VG_CLOSE_PATH,
      VG_MOVE_TO_ABS,VG_LINE_TO_REL,VG_LINE_TO_REL,VG_LINE_TO_REL,VG_CLOSE_PATH,
      VG_MOVE_TO_ABS,VG_LINE_TO_REL,VG_LINE_TO_REL,VG_LINE_TO_REL,VG_CLOSE_PATH
    };
    VGfloat data[]={
      2,2,0,4,-0.3,0.3,0,-4,
      11,6,0,1,-0.3,0.3,0,-1,
      11,5,2,0,0,-0.3,-1.7,0,
      8,4,3,0,0.3,-0.3,-3,0
    };
    
    if (!bevel_l)
      bevel_l=testCreatePath();
    else
      vgClearPath(bevel_l, VG_PATH_CAPABILITY_ALL);
    vgAppendPathData(bevel_l,sizeof(segs),segs,data);
  }
  
}

void reshapeFunc(int x,int y)
{
  createPaths();
  createTextures();
}

VGPath getRectangle(float x, float y, float w, float h)
{
  VGPath ret=testCreatePath();
  VGubyte segs[]= { VG_MOVE_TO_ABS, VG_LINE_TO_REL, VG_LINE_TO_REL, VG_LINE_TO_REL, VG_CLOSE_PATH };
  VGfloat data[]= { x,y, w,0, 0,h, -w,0 };
  vgAppendPathData(ret,sizeof(segs),segs,data);
  return ret;
}

void drawKirpicz()
{
  VGfloat dark[] = { 0.1411765, 0.2666667, 0.3294118, 1.0 };
  VGfloat border[] = { 0.1686275, 0.5019608, 0.5647059, 0.4745098 };
  VGfloat light[] = { 0.1254902, 0.6470588, 0.4784314, 0.2039216 };
  VGPaint back=vgCreatePaint();
  VGPaint stroke=vgCreatePaint();
  vgSetParameterfv(back, VG_PAINT_COLOR, 4, dark);
  
  VGPath path=getRectangle(0,0,64,64);
  vgSetPaint(back, VG_FILL_PATH);
  vgDrawPath(path, VG_FILL_PATH);
  vgDestroyPath(path);
  
  vgSetParameterfv(back, VG_PAINT_COLOR, 4, light);
  vgSetParameterfv(stroke, VG_PAINT_COLOR, 4, border); 
  vgSetPaint(stroke, VG_STROKE_PATH);
  path=getRectangle(2.459,43.874,36.831,17.326);
  vgSetf(VG_STROKE_LINE_WIDTH, 2.825);
  vgDrawPath(path, VG_FILL_PATH); vgDrawPath(path, VG_STROKE_PATH); vgDestroyPath(path);
  
  path=getRectangle(43.598,24.212,16.250,35.305);
  vgSetf(VG_STROKE_LINE_WIDTH, 2.679);
  vgDrawPath(path, VG_FILL_PATH); vgDrawPath(path, VG_STROKE_PATH); vgDestroyPath(path);
  
  path=getRectangle(3.641,4.286,16.250,35.305);
  vgSetf(VG_STROKE_LINE_WIDTH, 2.679);
  vgDrawPath(path, VG_FILL_PATH); vgDrawPath(path, VG_STROKE_PATH); vgDestroyPath(path);
  
  path=getRectangle(23.859,2.912,37.539,17.694);
  vgSetf(VG_STROKE_LINE_WIDTH, 2.882);
  vgDrawPath(path, VG_FILL_PATH); vgDrawPath(path, VG_STROKE_PATH); vgDestroyPath(path);
  
  path=getRectangle(23.572,23.753,16.932,16.932);
  vgSetf(VG_STROKE_LINE_WIDTH, 1.938);
  vgDrawPath(path, VG_FILL_PATH); vgDrawPath(path, VG_STROKE_PATH); vgDestroyPath(path);
}

void drawWall()
{
  VGfloat darkblue[]= { 0,(VGfloat)0x54/0xff,(VGfloat)0x54/0xff, 1.0 };
  VGPaint back=vgCreatePaint();
  vgSetParameterfv(back, VG_PAINT_COLOR, 4, darkblue);
  vgSetPaint(back,VG_FILL_PATH);
  VGPath path=getRectangle(0,0,64,64);
  vgDrawPath(path,VG_FILL_PATH); vgDestroyPath(path);
  VGfloat lightblue[]= { 0,(VGfloat)0x7d/0xff,(VGfloat)0x7d/0xff, 1.0 };
  vgSetParameterfv(back, VG_PAINT_COLOR, 4, lightblue);
  path=testCreatePath();
  {
    VGubyte segs[]={ VG_MOVE_TO_ABS, VG_CUBIC_TO_REL,VG_CUBIC_TO_REL,VG_CUBIC_TO_REL,VG_CUBIC_TO_REL,VG_CUBIC_TO_REL,VG_CUBIC_TO_REL,VG_CUBIC_TO_REL, VG_LINE_TO_REL};
    VGfloat data[]={ 31.086705,0.39403254, 
      1.600315,1.49696116,3.195776,2.99930386,4.786384,4.50700266,
      1.567573,1.4858647,3.130419,2.9769061,4.688486,4.473124,
      0.553137,0.354059,1.144372,0.6462808,1.759647,0.8697978,
      1.818693,0.660657,3.84842,0.701327,5.691179,0.114039,
      1.129409,-0.6488438,2.13278,-1.5245098,2.935578,-2.562009,
      0.640071,-0.8271804,1.149426,-1.7493078,1.684809,-2.650447,
      0.965953,-1.6258381,2.024296,-3.1946566,3.167989,-4.69594866
      -0.271891,-0.00513};
    vgAppendPathData(path,sizeof(segs),segs,data);
    vgDrawPath(path,VG_FILL_PATH);
    vgDestroyPath(path);
  }
  
  path=testCreatePath();
  {
    VGubyte segs[]={ VG_MOVE_TO_ABS, VG_CUBIC_TO_REL,VG_CUBIC_TO_REL,VG_CUBIC_TO_REL,VG_CUBIC_TO_REL,VG_CUBIC_TO_REL,VG_CUBIC_TO_REL,VG_CUBIC_TO_REL,VG_CUBIC_TO_REL};
    VGfloat data[]={ 39.512033,44.165844,
      1.812381,-1.043265,3.531365,-2.254949,5.128637,-3.615115,
      1.368986,-1.165761,2.689111,-2.466139,4.348647,-3.133357,
      1.611732,-0.647999,3.471616,-0.627138,5.068963,0.05689,
      1.597347,0.684004,2.913625,2.023181,3.587443,3.649865,
      0.134789,2.825195,-0.993187,5.68796,-3.007776,7.633671,
      -1.80969,1.747822,-4.303106,2.73909,-6.796145,2.701829,
      -1.560858,-0.916669,-3.039159,-1.979257,-4.410257,-3.170081,
      -1.431377,-1.243179,-2.745844,-2.626101,-3.919512,-4.123652};
    vgAppendPathData(path,sizeof(segs),segs,data);
    vgDrawPath(path,VG_FILL_PATH);
    vgDestroyPath(path);
  }
  
  path=testCreatePath();
  {
    VGubyte segs[]={ VG_MOVE_TO_ABS, VG_CUBIC_TO_REL,VG_CUBIC_TO_REL,VG_CUBIC_TO_REL,VG_CUBIC_TO_REL,VG_CUBIC_TO_REL,VG_CUBIC_TO_REL,VG_CUBIC_TO_REL,VG_CUBIC_TO_REL,VG_CUBIC_TO_REL,VG_CUBIC_TO_REL};
    VGfloat data[]={ 18.591284,61.418338,
      4.174982,0.790841,8.656934,-0.281459,12.050392,-2.88301,
      2.572252,-1.971979,4.510947,-4.788308,5.454921,-7.924305,
      -2.1447,-1.6836,-4.830668,-2.638633,-7.537583,-2.680071,
      -3.305242,-0.05074,-6.610735,1.281312,-8.988248,3.621726,
      -0.316929,1.235953,-0.968316,2.380982,-1.863329,3.275381,
      -0.623825,0.623397,-1.373135,1.140083,-1.829807,1.899148,
      -0.28532,0.474248,-0.441283,1.028376,-0.44601,1.58463,
      -0.005,0.556254,0.14183,1.113072,0.418976,1.592267,
      0.277223,0.47922,0.68433,0.879485,1.164188,1.144593,
      0.479883,0.265135,1.031184,0.394396,1.5765,0.369641};
    vgAppendPathData(path,sizeof(segs),segs,data);
    vgDrawPath(path,VG_FILL_PATH);
    vgDestroyPath(path);
  }
  
  path=testCreatePath();
  {
    VGubyte segs[]={ VG_MOVE_TO_ABS, VG_CUBIC_TO_REL,VG_CUBIC_TO_REL,VG_CUBIC_TO_REL,VG_CUBIC_TO_REL,VG_CUBIC_TO_REL,VG_CUBIC_TO_REL,VG_CUBIC_TO_REL,VG_CUBIC_TO_REL,VG_CUBIC_TO_REL,VG_CUBIC_TO_REL,VG_CUBIC_TO_REL,VG_CUBIC_TO_REL,VG_LINE_TO_ABS};
    VGfloat data[]={ 8.35596,56.735407,
      -2.49502,-1.942713,-4.06574,-5.065332,-4.1568,-8.263833,
      -0.0671,-2.355252,0.64878,-4.726573,2.00325,-6.636202,
      1.35448,-1.909628,3.33947,-3.346059,5.555059,-4.019915,
      0.725873,-0.180924,1.499073,-0.160423,2.214685,0.05869,
      0.715638,0.219108,1.371828,0.636466,1.879549,1.195206,
      0.507721,0.55874,0.86564,1.257479,1.025174,2.001424,
      0.159433,0.743944,0.120203,1.531147,-0.112408,2.254795,
      -0.435649,1.35481,-1.488183,2.388695,-2.39841,3.471374,
      -0.455088,0.541366,-0.885154,1.112509,-1.190215,1.753998,
      -0.30506,0.641489,-0.480688,1.361139,-0.405372,2.069847,
      0.05432,0.5121,0.240407,1.009079,0.534553,1.428025,
      0.294096,0.418921,0.695571,0.758834,1.153601,0.976687,
      8.35594,56.735309};
    vgAppendPathData(path,sizeof(segs),segs,data);
    vgDrawPath(path,VG_FILL_PATH);
    vgDestroyPath(path);
  }
  
  path=testCreatePath();
  {
    VGubyte segs[]={ VG_MOVE_TO_ABS, VG_CUBIC_TO_ABS,VG_CUBIC_TO_ABS,VG_CUBIC_TO_ABS,VG_CUBIC_TO_ABS,VG_CUBIC_TO_ABS,VG_LINE_TO_ABS};
    VGfloat data[]={ 0.09186,37.284233,
      1.30828,37.464645,2.57135,37.307293,3.70863,36.83087,
      5.29046,36.169751,6.59904,34.908121,7.45641,33.400729,
      8.3138,31.893364,8.73499,30.151513,8.8049,28.410688,
      8.94472,24.929012,7.72679,21.546998,6.35652,18.354443,
      5.39105,16.105003,4.34038,13.893542,3.20767,11.726621,
      0.18252,10.248009};
    vgAppendPathData(path,sizeof(segs),segs,data);
    vgDrawPath(path,VG_FILL_PATH);
    vgDestroyPath(path);
  }
  
  path=testCreatePath();
  {
    VGubyte segs[]={ VG_MOVE_TO_ABS, VG_CUBIC_TO_ABS,VG_CUBIC_TO_REL,VG_CUBIC_TO_REL,VG_CUBIC_TO_REL,VG_CUBIC_TO_REL,VG_CUBIC_TO_REL,VG_CUBIC_TO_ABS,VG_CUBIC_TO_ABS};
    VGfloat data[]={ 6.38387,7.0532826,
      7.58784,6.5178628,8.75372,5.893467,9.8693,5.1866299,
      0.95257,-0.6035357,1.885252,-1.275341,2.955364,-1.6179194,
      1.046323,-0.3349673,2.19409,-0.3322509,3.238904,0.00769,
      1.044789,0.3399132,1.981772,1.0154601,2.644072,1.9063229,
      -0.161193,1.5131573,-0.908164,2.9534064,-2.044111,3.9416509,
      -1.25766,1.0941337,-2.979134,1.6141267,-4.618728,1.3950947,
      11.00177,10.363569,9.99767,9.8152079,9.04756,9.1825859,
      8.1017,8.5527828,7.20938,7.8394877,6.38387,7.0533102};
    vgAppendPathData(path,sizeof(segs),segs,data);
    vgDrawPath(path,VG_FILL_PATH);
    vgDestroyPath(path);
  }
  
  path=testCreatePath();
  {
    VGubyte segs[]={ VG_MOVE_TO_ABS, VG_CUBIC_TO_REL,VG_CUBIC_TO_REL,VG_CUBIC_TO_REL,VG_CUBIC_TO_REL,VG_CUBIC_TO_REL,VG_CUBIC_TO_REL,VG_CUBIC_TO_REL,VG_CUBIC_TO_REL,VG_LINE_TO_REL,VG_LINE_TO_REL};
    VGfloat data[]={ 63.700046,37.588199,
      -1.057463,-0.409028,-2.094079,-0.874102,-3.10419,-1.392737,
      -2.203997,-1.1316,-4.278587,-2.516752,-6.220325,-4.069067,
      -0.908215,-0.726083,-1.798198,-1.500113,-2.46593,-2.459707,
      -0.667732,-0.959568,-1.101068,-2.131274,-0.99706,-3.302853,
      0.108385,-1.219679,0.78268,-2.321271,1.60718,-3.212211,
      0.8245,-0.890965,1.805113,-1.614024,2.704879,-2.425854,
      1.730174,-1.561079,3.162732,-3.457689,4.238603,-5.541862,
      0.346679,-0.6716,0.656894,-1.362779,0.92876,-2.069258,
      0.900168,-3.4551779,
      2.439802,0.9589529};
    vgAppendPathData(path,sizeof(segs),segs,data);
    vgDrawPath(path,VG_FILL_PATH);
    vgDestroyPath(path);
  }

  path=testCreatePath();
  {
    VGubyte segs[]={ VG_MOVE_TO_ABS, VG_CUBIC_TO_REL,VG_CUBIC_TO_REL,VG_CUBIC_TO_REL,VG_CUBIC_TO_REL,VG_CUBIC_TO_REL,VG_CUBIC_TO_REL};
    VGfloat data[]={ 32.787698,63.743796,
      4.142134,-1.689901,8.22034,-3.542718,12.2225,-5.552898,
      1.310375,-0.658188,2.631648,-1.340094,4.062104,-1.641667,
      0.715241,-0.150786,1.454319,-0.203614,2.178185,-0.105429,
      0.723865,0.09818,1.432671,0.351199,2.031672,0.776666,
      0.830778,0.590102,1.41078,1.475973,1.904928,2.37676,
      0.7361,1.341786,1.327314,2.76609,1.759625,4.238974};
    vgAppendPathData(path,sizeof(segs),segs,data);
    vgDrawPath(path,VG_FILL_PATH);
    vgDestroyPath(path);
  }
  
  path=testCreatePath();
  {
    VGubyte segs[]={ VG_MOVE_TO_ABS, VG_CUBIC_TO_REL,VG_CUBIC_TO_REL,VG_CUBIC_TO_REL,VG_CUBIC_TO_REL,VG_CUBIC_TO_REL,VG_CUBIC_TO_REL,VG_CUBIC_TO_REL,VG_CUBIC_TO_REL,VG_CUBIC_TO_REL,VG_CUBIC_TO_REL};
    VGfloat data[]={ 29.757351,42.293579,
      -2.873113,-2.547581,-5.75797,-5.081401,-8.654469,-7.601356,
      -2.198566,-1.912756,-4.458314,-3.885811,-5.79217,-6.499406,
      -1.052962,-2.06321,-1.464495,-4.460311,-1.161446,-6.765284,
      0.303073,-2.304972,1.31932,-4.50713,2.868133,-6.215077,
      1.548789,-1.707946,3.623001,-2.913787,5.853956,-3.403181,
      2.23093,-0.489394,4.608292,-0.260086,6.709513,0.647152,
      3.164141,1.166479,6.138144,2.866558,8.764438,5.010133,
      4.118853,3.361794,7.365702,7.823181,9.326879,12.815734,
      -1.866397,3.018883,-4.340575,5.645702,-7.224049,7.669728,
      -3.185767,2.236217,-6.86892,3.731973,-10.690785,4.341557};
    vgAppendPathData(path,sizeof(segs),segs,data);
    vgDrawPath(path,VG_FILL_PATH);
    vgDestroyPath(path);
  }
}

void createTextures()
{
  VGfloat shadow_col[] = { 0.0,0.0,0.0,0.7 };
  if (!shadow_p)
    shadow_p = vgCreatePaint();
  vgSetParameterfv(shadow_p, VG_PAINT_COLOR, 4, shadow_col);
  
  VGfloat bevel_high[] = { 1.0,1.0,1.0,0.35 };
  if (!bevel_h_p)
    bevel_h_p = vgCreatePaint();
  vgSetParameterfv(bevel_h_p, VG_PAINT_COLOR, 4, bevel_high);
  
  VGfloat bevel_low[] = { 0.0,0.0,0.0,0.35 };
  if (!bevel_l_p)
    bevel_l_p = vgCreatePaint();
  vgSetParameterfv(bevel_l_p, VG_PAINT_COLOR, 4, bevel_low);
  
  if (!wall_p)
    wall_p = vgCreatePaint();
  vgSetParameteri(wall_p, VG_PAINT_TYPE, VG_PAINT_TYPE_PATTERN);
  vgSetParameteri(wall_p, VG_PAINT_PATTERN_TILING_MODE, VG_TILE_REPEAT);
  
  if (!backdrop_p)
    backdrop_p = vgCreatePaint();
  vgSetParameteri(backdrop_p, VG_PAINT_TYPE, VG_PAINT_TYPE_PATTERN);
  vgSetParameteri(backdrop_p, VG_PAINT_PATTERN_TILING_MODE, VG_TILE_REPEAT);
  
  int ratio=min(testWidth()/13,testHeight()/8);
  if (kirpicz)
    vgDestroyImage(kirpicz);
  kirpicz=vgCreateImage(VG_sRGBA_8888,ratio,ratio,VG_IMAGE_QUALITY_BETTER);
  if (walls)
    vgDestroyImage(walls);
  walls=vgCreateImage(VG_sRGBA_8888,ratio,ratio,VG_IMAGE_QUALITY_BETTER);
  /* Rendering takes place */
  VGfloat cc[] = {0,0,0,1};
  
  vgSetfv(VG_CLEAR_COLOR, 4, cc);
  vgClear(0,0,testWidth(),testHeight());
  vgLoadIdentity();
  vgScale((float)ratio/64.0,(float)ratio/64.0);
  drawKirpicz();
  vgTranslate(64.0,0.0);
  drawWall();
  vgGetPixels(kirpicz,0,0,0,0,ratio,ratio);
  vgGetPixels(walls,0,0,ratio,0,ratio,ratio);
  vgPaintPattern(backdrop_p,kirpicz);
  vgPaintPattern(wall_p,walls);
}

int main(int argc,char argv[])
{
  testInit(argc, argv, 832,512, "ShivaVG: Electro Body scene test");
  testCallback(TEST_CALLBACK_DISPLAY, (CallbackFunc)display);
  testCallback(TEST_CALLBACK_RESHAPE, (CallbackFunc)reshapeFunc);
  testRun();
  return EXIT_SUCCESS;
}
