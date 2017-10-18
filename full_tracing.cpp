/** full_tracing.cpp contains the definition of the full_tracing function.
*/


#include "full_tracing.h"
#include <iostream>
using namespace std;

/** bool full_tracing(V3DPluginCallback2 &callback, const V3DPluginArgList & input, V3DPluginArgList & output)
A function which runs the full pipeline of tracing.

Uses:

adaptive_thresholding
hist_eq_range_uint8
rescale
neuron_tracing

Inputs:
- input image filename
- sampling interval (default h = 5)
- number of sampling points (default d = 3)
- channel (default c = 1)

Outputs:
- output image filename

*/

bool full_tracing(V3DPluginCallback2 &callback, const V3DPluginArgList & input, V3DPluginArgList & output){
  unsigned char *pData1;
  //void *pData1=NULL;
  V3DLONG sz_data[4];
  V3DLONG *sz_data_address[4] ;
  sz_data_address[4] = &sz_data[4];

  V3DLONG sz0,sz1,sz2,sz3;
  //V3DLONG *sz0_address, *sz1_address, *sz2_address,*sz3_address;
  //sz0_address = &sz0;
  //sz1_address = &sz1;
  //sz2_address = &sz2;
  //sz3_address = &sz3;

  // adaptive_thresholding
  Image4DSimple image;
  Image4DSimple *p4DImage;
  p4DImage = &image;

  adaptive_thresholding(callback, input, output, (unsigned char *)pData1,&image);

  //V3DLONG sz0 = *sz_data[0];
  //V3DLONG sz1 = *sz_data[1];
  //V3DLONG sz2 = *sz_data[2];
  //V3DLONG sz3 = *sz_data[3];

  cout<<"after adaptive thresholding"<<endl;

  sz0 = image.getXDim();
  sz1 = image.getYDim();
  sz2 = image.getZDim();
  sz3 = image.getCDim();
  int datatype = image.getDatatype();

  cout<<sz0<<endl;
  cout<<sz1<<endl;
  cout<<sz2<<endl;
  cout<<sz3<<endl;

  unsigned char *pData = image.getRawData();




  unsigned int sum = 0;
  for (V3DLONG i=0; i<sz0*sz1*sz2;i++)
    sum = sum + unsigned(((unsigned char *)pData)[i]);
  cout<<"sum"<<unsigned(sum)<<endl;

  ////-------------------------------------------------------------------------------------------
  // Histogram equalization
  //-------------------------------------------------------------------------------------------
  printf("\nhistogram equalization ...\n");
  int lowerbound = 30, higherbound = 255;
  if (!hist_eq_range_uint8((unsigned char *)pData, sz0*sz1*sz2*sz3, lowerbound, higherbound))
  {
      v3d_msg("Error happens in proj_general_hist_equalization();\n");
      return(false);
  }
  cout<<"after histogram equalization"<<endl;
  sum = 0;
  for (V3DLONG i=0; i<sz0*sz1*sz2;i++)
    sum = sum + unsigned(((unsigned char *)pData)[i]);
  cout<<"sum"<<unsigned(sum)<<endl;
  //------------------------------------------------------------------------------------------
  // Rescaling/Thresholding
  //------------------------------------------------------------------------------------------
  int max = 0;
  for (V3DLONG i=0; i<sz0*sz1*sz2;i++)
    if (unsigned(((unsigned char *)pData)[i])>max)
      max = unsigned(((unsigned char *)pData)[i]);
  cout<<"max"<<max<<endl;

  int min = 255;
  for (V3DLONG i=0; i<sz0*sz1*sz2;i++)
    if (unsigned(((unsigned char *)pData)[i])<min)
      min = unsigned(((unsigned char *)pData)[i]);
  cout<<"min"<<min<<endl;


  rescale((unsigned char *) pData, sz0*sz1*sz2*sz3);

  // neuron tracing algorithm
  //-------------------------------------------------------------------------------------------
  // SIGEN Tracing
  //-------------------------------------------------------------------------------------------
  input_PARA PARA;
  // read the image filename
  vector<char *> *pinfiles = (input.size() >= 1) ? (vector<char *> *)input[0].p : 0;
  vector<char *> *pparas = (input.size() >= 2) ? (vector<char *> *)input[1].p : 0;
  vector<char *> infiles = (pinfiles != 0) ? *pinfiles : vector<char *>();
  vector<char *> paras = (pparas != 0) ? *pparas : vector<char *>();
  if (infiles.empty()) {
  	fprintf(stderr, "Need input image. \n");
  	return false;
  } else {
  	PARA.inimg_file = infiles[0];
  }

  // set the channel to process
  int k = 2;
  PARA.channel = ((int)paras.size() >= k + 1) ? atoi(paras[k]) : 1;
  k++;
  // channel out of range
  if (PARA.channel > sz3 || PARA.channel < 1){
    cout<<"Channel out of range: setting to 1."<<endl;
    PARA.channel = 1;
  }
  cout<<"channel = "<<PARA.channel<<endl;
  sz3 = 1;
  cout<<output.size()<<endl;

  vector<char *> *poutfiles = (output.size() >= 1) ? (vector<char *> *)output[0].p : 0;
  vector<char *> outfiles = (poutfiles != 0) ? *poutfiles : vector<char *>();

  // set the swc filename
  //char * outimg_file = ((vector<char*> *)(output.at(0).p))->at(0);
  //cout<<"outputfilename"<<outfiles[0]<<endl;
  cout<<output.size()<<endl;

  if (output.size()<1){
    PARA.swc_file = PARA.inimg_file.section(".",0,0) + "_SIGEN.swc";
  }else{
    //PARA.swc_file = outfiles[0];
    PARA.swc_file = outfiles[0];
  }
  cout<<PARA.swc_file.toStdString().c_str()<<endl;
  //QString inimgfile = PARA.inimg_file;
  //QString swc_name = inimgfile.section(".",0,0) + "_SIGEN.swc";
  //cout<<"Output_filename: "<<PARA.swc_file<<endl;
  cout<<"neuron tracing"<<endl;

  // tracing
  neuron_tracing((unsigned char *)pData, sz0, sz1, sz2, sz3, datatype,PARA,false);

 return(true);
}
