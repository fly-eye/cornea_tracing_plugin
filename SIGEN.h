#include <QString>
#include "v3d_basicdatatype.h"
#include <v3d_interface.h>

// missing definition of input_PARA
struct input_PARA {
  QString inimg_file;
  V3DLONG channel;
};

void rescale (unsigned char * data1d, int totalsize);

void neuron_tracing(unsigned char * pData,
    V3DLONG sz0,
    V3DLONG sz1,
    V3DLONG sz2,
    V3DLONG sz3,
    const int datatype,
    input_PARA &PARA,
    bool via_gui);
