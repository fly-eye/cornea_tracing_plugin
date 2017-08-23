#include "ada_thresholding.h"
//#include </Users/valentina/projects/Vaa3dbuild_new/v3d_external/v3d_main/v3d/v3d_core.h>
#include </Users/valentina/projects/Vaa3dbuild_new/v3d_external/v3d_main/v3d/mainwindow.h>
#include </Users/valentina/projects/Vaa3dbuild_new/v3d_external/v3d_main/v3d/v3d_core.h>
#include </Users/valentina/projects/Vaa3dbuild_new/v3d_external/v3d_main/basic_c_fun/basic_surf_objs.h>
#include </Users/valentina/projects/Vaa3dbuild_new/v3d_external/v3d_main/jba/c++/histeq.h>
#include </Users/valentina/projects/Vaa3dbuild_new/v3d_external/v3d_main/neuron_editing/v_neuronswc.h>
#include <iostream>
#include <vector>
#include </Users/valentina/projects/Vaa3dbuild_new/v3d_external/v3d_main/v3d/mdichild.h>
#include <QMainWindow>
#include </Users/valentina/projects/Vaa3dbuild_new/v3d_external/v3d_main/v3d/xformwidget.h>
#include </Users/valentina/projects/Vaa3dbuild_new/v3d_external/v3d_main/3drenderer/v3dr_mainwindow.h>
#include </Users/valentina/projects/Vaa3dbuild_new/v3d_external/v3d_main/3drenderer/qtr_widget.h>
//#include <QtWidgets>
//#include <QWidget>

//headers needed for running SIGEN
#include "SIGEN.h"

using namespace std;

template <class T>
void BinaryProcess(T *apsInput, T * aspOutput, V3DLONG iImageWidth, V3DLONG iImageHeight, V3DLONG iImageLayer, V3DLONG h, V3DLONG d)
{
	V3DLONG i, j,k,n,count;
	double t, temp;

	V3DLONG mCount = iImageHeight * iImageWidth;
	for (i=0; i<iImageLayer; i++)
	{
		for (j=0; j<iImageHeight; j++)
		{
			for (k=0; k<iImageWidth; k++)
			{
				V3DLONG curpos = i * mCount + j*iImageWidth + k;
				V3DLONG curpos1 = i* mCount + j*iImageWidth;
				V3DLONG curpos2 = j* iImageWidth + k;
				temp = 0;
				count = 0;
				for(n =1 ; n <= d  ;n++)
				{
					if (k>h*n) {temp += apsInput[curpos1 + k-(h*n)]; count++;}
					if (k+(h*n)< iImageWidth) { temp += apsInput[curpos1 + k+(h*n)]; count++;}
                    if (j>h*n) {temp += apsInput[i* mCount + (j-(h*n))*iImageWidth + k]; count++;}//
					if (j+(h*n)<iImageHeight) {temp += apsInput[i* mCount + (j+(h*n))*iImageWidth + k]; count++;}//
					if (i>(h*n)) {temp += apsInput[(i-(h*n))* mCount + curpos2]; count++;}//
					if (i+(h*n)< iImageLayer) {temp += apsInput[(i+(h*n))* mCount + j* iImageWidth + k ]; count++;}
				}
				t =  apsInput[curpos]-temp/(count);
				aspOutput[curpos]= (t > 0)? t : 0;
			}
		}
	}
}

bool thimg(V3DPluginCallback2 &callback, const V3DPluginArgList & input, V3DPluginArgList & output)
{
	cout<<"Welcome to Gaussian filter"<<endl;
	if (input.size()<1 || output.size() != 1) return false;

	V3DLONG h = 5, d = 3;
     if (input.size()>=2)
     {
          vector<char*> paras = (*(vector<char*> *)(input.at(1).p));
          if(paras.size() >= 1) h = atoi(paras.at(0));
          if(paras.size() >= 2) d = atoi(paras.at(1));
	}

	char * inimg_file = ((vector<char*> *)(input.at(0).p))->at(0);
	char * outimg_file = ((vector<char*> *)(output.at(0).p))->at(0);
	cout<<"h = "<<h<<endl;
  cout<<"d = "<<d<<endl;
	cout<<"inimg_file = "<<inimg_file<<endl;
	cout<<"outimg_file = "<<outimg_file<<endl;

    Image4DSimple *subject = callback.loadImage(inimg_file);
    if(!subject || !subject->valid())
    {
         v3d_msg("Fail to load the input image.");
         if (subject) {delete subject; subject=0;}
         return false;
    }

     clock_t start_t = clock(); // record time point
     // =====================================================================>>>>>>>>>
     V3DLONG sz0 = subject->getXDim();
     V3DLONG sz1 = subject->getYDim();
     V3DLONG sz2 = subject->getZDim();
     V3DLONG sz3 = subject->getCDim();
    V3DLONG pagesz_sub = sz0*sz1*sz2;

	//----------------------------------------------------------------------------------------------------------------------------------
	V3DLONG channelsz = sz0*sz1*sz2;
	void *pData=NULL;

	V3DLONG sz_data[4]; sz_data[0]=sz0; sz_data[1]=sz1; sz_data[2]=sz2; sz_data[3]=1;
        switch (subject->getDatatype())
		{
            case V3D_UINT8:
				try
				{
					pData = (void *)(new unsigned char [sz3*channelsz]);
				}
					catch (...)
				{
					v3d_msg("Fail to allocate memory in Distance Transform.",0);
					if (pData) {delete []pData; pData=0;}
					return false;
				}

				{
                    unsigned char * pSubtmp_uint8 = subject->getRawData();

					for (V3DLONG ich=0; ich<sz3; ich++)
						BinaryProcess(pSubtmp_uint8+ich*channelsz, (unsigned char *)pData+ich*channelsz, sz0, sz1, sz2, h, d  );
				}
				break;

            case V3D_UINT16:
				try
				{
					pData = (void *)(new short int [sz3*channelsz]);
				}
					catch (...)
				{
					v3d_msg("Fail to allocate memory in Distance Transform.",0);
					if (pData) {delete []pData; pData=0;}
					return false;
				}

				{
                    short int * pSubtmp_uint16 = (short int *)subject->getRawData();

					for (V3DLONG ich=0; ich<sz3; ich++)
						BinaryProcess(pSubtmp_uint16+ich*channelsz, (short int *)pData+ich*channelsz, sz0, sz1, sz2, h, d );
				}

				break;

            case V3D_FLOAT32:
				try
				{
					pData = (void *)(new float [sz3*channelsz]);
				}
					catch (...)
				{
					v3d_msg("Fail to allocate memory in Distance Transform.", 0);
					if (pData) {delete []pData; pData=0;}
					return false;
				}

				{
                    float * pSubtmp_float32 = (float *)subject->getRawData();

					for (V3DLONG ich=0; ich<sz3; ich++)
						BinaryProcess(pSubtmp_float32+ich*channelsz, (float *)pData+ich*channelsz, sz0, sz1, sz2, h, d );
				}

				break;

			default:
				break;

			}

//----------------------------------------------------------------------------------------------------------------------------------

clock_t end_t = clock();
printf("time eclapse %d s for dist computing!\n", (end_t-start_t)/1000000);


//-------------------------------------------------------------------------------------------
// Histogram equalization
//-------------------------------------------------------------------------------------------

printf("\nhistogram equalization ...\n");
int lowerbound = 30, higherbound = 255;
if (!hist_eq_range_uint8( (unsigned char *)pData, sz0*sz1*sz2*sz3, lowerbound, higherbound))
{
		v3d_msg("Error happens in proj_general_hist_equalization();\n");
		return(false);
}

//-------------------------------------------------------------------------------------------
// SIGEN Tracing
//-------------------------------------------------------------------------------------------
input_PARA PARA;
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
int k = 0;
PARA.channel = ((int)paras.size() >= k + 1) ? atoi(paras[k]) : 1;
k++;

//reconstruction_func((unsigned char*)pData, parent, PARA, /* via_gui = */ false);
//const V3DPluginArgList & input, V3DPluginArgList & output)


Image4DSimple p4DImage;
p4DImage.setData((unsigned char*)pData, sz0, sz1, sz2, sz3, subject->getDatatype());

//reconstruction_func((unsigned char*)pData, sz0, sz1, sz2, sz3, subject->getDatatype(), /* via_gui = */ false);
reconstruction_func((unsigned char*)pData, /* via_gui = */ false);
//p4DImage.saveImage(outimg_file);
callback.saveImage(&p4DImage,outimg_file);


// moving data from Image4DSimple image format to
//My4DImage my4DImage_test;
//my4DImage_test.setNewImageData((unsigned char*)pData, sz0, sz1, sz2, sz3, subject->getDatatype());
//my4DImage_test.proj_general_hist_equalization(0,255);

//v3dhandle newwin;
//if(QMessageBox::Yes == QMessageBox::question (0, "", QString("Do you want to use the existing window?"), QMessageBox::Yes, QMessageBox::No))
	//newwin = callback.currentImageWindow();
//else
	//newwin = callback.newImageWindow();

//callback.setImage(newwin, &p4DImage);
//callback.setImageName(newwin, QString("thresholded image"));
//callback.updateImageWindow(newwin);
return(true);

}

void AdaTDialog::update()
{
	  //get current data
	  Dn = Dnumber->text().toLong()-1;
		Dh = Ddistance->text().toLong()-1;
		//printf("channel %ld val %d x %ld y %ld z %ld ind %ld \n", c, data1d[ind], nx, ny, nz, ind);
}

	//----------------------------------------------------------------------------------------------------------------------------------
