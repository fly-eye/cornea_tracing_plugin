#include "SIGEN.h"
#include <basic_surf_objs.h>
#include <v3d_interface.h>
#include <v3dr_mainwindow.h>
#include <iostream>
//in V3D_PLUGINS/bigneuron_hide_ikeno_SIGEN/src/sigen
#include <interface.h>
#include "common/binary_cube.h"
#include "common/neuron.h"
#include "common/disjoint_set.h"
#include "builder/builder.h"
#include "extractor/extractor.h"
#include "toolbox/toolbox.h"
#include <boost/foreach.hpp>
#include <cassert>
#include <vector>

using namespace std;

static sigen::BinaryCube convertToBinaryCube(
    const unsigned char *p,
    const int unit_byte,
    const int xdim,
    const int ydim,
    const int zdim,
    const int /* channel_dim */,
    const int channel) {
  const int stride_x = unit_byte;
  const int stride_y = unit_byte * xdim;
  const int stride_z = unit_byte * xdim * ydim;
  const int stride_c = unit_byte * xdim * ydim * zdim;
  sigen::BinaryCube cube(xdim, ydim, zdim);
  for (int x = 0; x < xdim; ++x) {
    for (int y = 0; y < ydim; ++y) {
      for (int z = 0; z < zdim; ++z) {
        if (p[stride_x * x + stride_y * y + stride_z * z + stride_c * channel] >= 128) {
          cube[x][y][z] = true;
        } else {
          cube[x][y][z] = false;
        }
      }
    }
  }
  return cube;
}

void rescale (unsigned char * data1d, int totalsize){
	for (int i=0;i<totalsize;i++)
	{
		//if (data1d[i]<0) data1d[i] = 0;
		//if (data1d[i]>255) data1d[i] = 255;

    //thresholds the image at 200: what should be the threshold???
    //if (data1d[i]>240) data1d[i] = 255;
    //if (data1d[i]<=240) data1d[i] = 0;
	}
}
void neuron_tracing(unsigned char * data1d,
      V3DLONG sz0,
      V3DLONG sz1,
      V3DLONG sz2,
      V3DLONG sz3,
      const int datatype,
      input_PARA &PARA,
      bool via_gui){

  /* neuron_tracing: this function runs the neuron tracing algorithm
    - it generates a neuron tree object which is stored .swc file
    - if I want to automatically generate the name of the output file, I need to
      pass the name of the input file
  */

  V3DLONG N, M, P, sc, c;
  V3DLONG in_sz[4];

  in_sz[0] = sz0;
  in_sz[1] = sz1;
  in_sz[2] = sz2;
  in_sz[3] = sz3;

  N = in_sz[0];
  M = in_sz[1];
  P = in_sz[2];
  sc = in_sz[3];
  c = 1; //first channel

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


  //main neuron reconstruction code

  // show configure GUI window
  sigen::interface::Options options;
  if(via_gui)
  {
      //bool retval = getConfig(parent, &options);
      //if (!retval) {
      //  return;
      //}
      cout<<"Via Gui";
      return;
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
  cout<<"extract end"<<endl;

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



  // QString swc_name = "output_SIGEN.swc";
  QString inimgfile = PARA.inimg_file;
  QString swc_name = inimgfile.section(".",0,0) + "_SIGEN.swc";
  writeSWC_file(swc_name.toStdString().c_str(), nt);
  if (!via_gui) {
    if (data1d) {
      //delete[] data1d;
      //data1d = NULL;
    }
  }
  v3d_msg(QString("Now you can drag and drop the generated swc file [%1] into Vaa3D.").arg(swc_name.toStdString().c_str()), via_gui);
  return;
}
