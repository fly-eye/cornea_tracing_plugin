#include <QString>
#include "v3d_basicdatatype.h"
#include <v3d_interface.h>

// missing definition of input_PARA
struct input_PARA {
  QString inimg_file;
  V3DLONG channel;
};

//void reconstruction_func(
//  V3DPluginCallback2 &callback,
//    QWidget *parent,
//    input_PARA &PARA,
//    bool via_gui);

//void reconstruction_func(Image4DSimple &p4DImage, bool via_gui);
// reconstruction_func(unsigned char * pData,
//  V3DLONG sz0,
//  V3DLONG sz1,
//  V3DLONG sz2,
//  V3DLONG sz3,
//  const int datatype,
//  bool via_gui);
void rescale (unsigned char * data1d, int totalsize);
void reconstruction_func(unsigned char * pData,
    bool via_gui);
//void reconstruction_func_full(unsigned char * pData,
//void reconstruction_func_full(Image4DSimple *p4DImage,
//    V3DLONG sz0,
//    V3DLONG sz1,
//    V3DLONG sz2,
//    V3DLONG sz3,
//    const int datatype,
//        bool via_gui);

//void reconstruction_func_full(V3DPluginCallback2 &callback, int datatype, QString outimg_file, bool via_gui);
void reconstruction_func_direct(unsigned char * pData,
    V3DLONG sz0,
    V3DLONG sz1,
    V3DLONG sz2,
    V3DLONG sz3,
    const int datatype,
    bool via_gui);
