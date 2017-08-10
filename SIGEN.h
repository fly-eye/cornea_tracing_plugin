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
void reconstruction_func(unsigned char * pData,
  V3DLONG sz0,
  V3DLONG sz1,
  V3DLONG sz2,
  V3DLONG sz3,
  const int datatype,
  bool via_gui);
