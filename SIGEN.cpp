#include "sigen/common/binary_cube.h"
#include "sigen/interface.h"
#include "SIGEN.h"



//void reconstruction_func(
//    V3DPluginCallback2 &callback,

//    const V3DPluginArgList & input,
//    const V3DPluginArgList & output,
//    bool via_gui) {

//void reconstruction_func(
//    Image4DSimple &p4DImage,
//    bool via_gui) {

// Try to pass the data directly
void reconstruction_func(unsigned char * pData,
    V3DLONG sz0,
    V3DLONG sz1,
    V3DLONG sz2,
    V3DLONG sz3,
    const int datatype,
    bool via_gui){

  input_PARA &PARA;
  PARA.channel = 0;
  //PARA.inimg_file =

  unsigned char *data1d = NULL;
  V3DLONG N, M, P, sc, c;
  V3DLONG in_sz[4];

  /*
  if (via_gui) {
    v3dhandle curwin = callback.currentImageWindow();
    if (!curwin) {
      QMessageBox::information(0, "", "You don't have any image open in the main window.");
      return;
    }
    Image4DSimple *p4DImage = callback.getImage(curwin);
    if (!p4DImage) {
      QMessageBox::information(0, "", "The image pointer is invalid. Ensure your data is valid and try again!");
      return;
    }
    data1d = p4DImage->getRawData();
    N = p4DImage->getXDim();
    M = p4DImage->getYDim();
    P = p4DImage->getZDim();
    sc = p4DImage->getCDim();
    bool ok1;
    if (sc == 1) {
      c = 1;
      ok1 = true;
    } else {
      c = QInputDialog::getInteger(parent, "Channel", "Enter channel NO:", 1, 1, sc, 1, &ok1);
    }
    if (!ok1) {
      return;
    }
    in_sz[0] = N;
    in_sz[1] = M;
    in_sz[2] = P;
    in_sz[3] = sc;
    PARA.inimg_file = p4DImage->getFileName();
  } else {
    int datatype = 0;
    if (!simple_loadimage_wrapper(callback, PARA.inimg_file.toStdString().c_str(), data1d, in_sz, datatype)) {
      fprintf(stderr, "Error happens in reading the subject file [%s]. Exit. \n", PARA.inimg_file.toStdString().c_str());
      return;
    }


    if (PARA.channel < 1 || PARA.channel > in_sz[3]) {
      fprintf(stderr, "Invalid channel number. \n");
      return;
    }
    N = in_sz[0];
    M = in_sz[1];
    P = in_sz[2];
    sc = in_sz[3];
    c = PARA.channel;
  }
  */


  // The image

  if (!p4DImage) {
    QMessageBox::information(0, "", "The image pointer is invalid. Ensure your data is valid and try again!");
    return;
  }
  data1d = p4DImage->getRawData();
  N = p4DImage->getXDim();
  M = p4DImage->getYDim();
  P = p4DImage->getZDim();
  sc = p4DImage->getCDim();
z




  //main neuron reconstruction code
  //// THIS IS WHERE THE DEVELOPERS SHOULD ADD THEIR OWN NEURON TRACING CODE

  // show configure GUI window
  sigen::interface::Options options;
  if(via_gui)
  {
      bool retval = getConfig(parent, &options);
      if (!retval) {
        return;
      }
  }else
  {
      options.scale_xy = 1.0;
      options.scale_z = 1.0;

      options.enable_interpolation = true;
      options.volume_threshold = 0;
      options.distance_threshold = 0;

      options.enable_smoothing = true;
      options.smoothing_level = 0;

      options.enable_clipping = true;
      options.clipping_level = 0;

  }
  // check config
  // v3d_msg((retval ? QString("OK") : QString("Cancel")), via_gui);
  // v3d_msg(QString("VT = %1\nDT = %2\nSM = %3\nCL = %4").arg(options.volume_threshold).arg(options.distance_threshold).arg(options.smoothing_level).arg(options.clipping_level), via_gui);
  // return;

  sigen::BinaryCube cube = convertToBinaryCube(data1d, /* unit_byte = */ 1, N, M, P, sc, c - 1);
  std::vector<int> out_n, out_type, out_pn;
  std::vector<double> out_x, out_y, out_z, out_r;
  sigen::interface::Extract(
      cube, out_n, out_type,
      out_x, out_y, out_z,
      out_r, out_pn, options);

  // construct NeuronTree
  NeuronTree nt;
  nt.name = "SIGEN";
  nt.comment = "SIGEN";
  for (int i = 0; i < (int)out_n.size(); ++i) {
    NeuronSWC pt;
    pt.n = out_n[i];
    pt.type = out_type[i];
    pt.x = out_x[i];
    pt.y = out_y[i];
    pt.z = out_z[i];
    pt.r = out_r[i];
    pt.pn = out_pn[i];
    nt.listNeuron.push_back(pt);
  }

  // QString swc_name = PARA.inimg_file + "_SIGEN.swc";
  QString swc_name = "output_SIGEN.swc";
  writeSWC_file(swc_name.toStdString().c_str(), nt);
  if (!via_gui) {
    if (data1d) {
      delete[] data1d;
      data1d = NULL;
    }
  }
  v3d_msg(QString("Now you can drag and drop the generated swc fle [%1] into Vaa3D.").arg(swc_name.toStdString().c_str()), via_gui);
}
