#include "ada_thresholding.h"
#include <basic_surf_objs.h> //in V3DMAINPATH/basic_c_fun
#include <histeq.h> //in V3DMAINPATH/jba/c++
#include <v_neuronswc.h> //in V3DMAINPATH/neuron_editing
#include <iostream>
#include <vector>
#include <QMainWindow>
#include <mainwindow.h> //in V3DMAINPATH/v3d
#include <v3d_core.h> //in V3DMAINPATH/v3d
#include <mdichild.h> //in V3DMAINPATH/v3d
#include <xformwidget.h> //in V3DMAINPATH/v3d
#include <v3dr_mainwindow.h> //in V3DMAINPATH/3drenderer
#include <qtr_widget.h> //in V3DMAINPATH/3drenderer


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

bool adaptive_thresholding(V3DPluginCallback2 &callback, const V3DPluginArgList & input, V3DPluginArgList & output, unsigned char * pData1, V3DLONG* sz0_address, V3DLONG* sz1_address, V3DLONG* sz2_address, V3DLONG* sz3_address, Image4DSimple *p4DImage)
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
	cout<<"before size assignment"<<endl;
	//sz_data[0]=*sz0; sz_data[1]=*sz1; sz_data[2]=*sz2; sz_data[3]=1;
  *sz0_address = sz0;
	*sz1_address = sz1;
	*sz2_address = sz2;
	*sz3_address = sz3;
	cout<<"after size assignment"<<endl;

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
										cout<<unsigned(pSubtmp_uint8[200])<<endl;

					for (V3DLONG ich=0; ich<sz3; ich++)
						BinaryProcess(pSubtmp_uint8+ich*channelsz, (unsigned char *)pData+ich*channelsz, sz0, sz1, sz2, h, d  );
						cout<<"After binary process for uint8"<<endl;
						//cout<<unsigned(pData[200])<<endl;

					unsigned int sum = 0;

					for (V3DLONG i=0; i<sz0*sz1*sz2;i++)
						sum = sum + unsigned(((unsigned char *)pData)[i]);
					cout<<"sum"<<sum<<endl;

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
printf("time elapse %d s for dist computing!\n", (end_t-start_t)/1000000);





//*pData1 = &(unsigned char *)pData;

pData1 = (unsigned char*)(new unsigned char [sz0*sz1*sz2*sz3]);

//pData = (void *)(new unsigned char [sz3*channelsz]);
cout<<"before memcpy"<<endl;
//unsigned char temp[sz0*sz1*sz2];
memcpy(pData1, (unsigned char *)pData, sz0*sz1*sz2*sz3);
cout<<"after memcpy"<<endl;



/*void changeArray(unsigned char **arr)
{
    unsigned char ptr[3] = {100, 101, 102};
    unsigned char *p = malloc(sizeof ptr);
    memcpy(p, ptr, sizeof ptr);
    *arr = p;
    printf("%d\n", **(arr+0)); // This prints out the correct value of 100
}
*/



cout<<"Asigned Pointer Data"<<endl;
Image4DSimple p4DImage2;

//p4DImage2.setData((unsigned char *)pData, sz0, sz1, sz2, sz3, subject->getDatatype());
p4DImage->setData((unsigned char *)pData, sz0, sz1, sz2, sz3, subject->getDatatype());
//p4DImage->setRawDataPointer(pData);
//p4DImage->setDataType();

cout<<unsigned(((unsigned char *)pData)[0])<<endl;

//callback.saveImage(&p4DImage2,outimg_file);
callback.saveImage(p4DImage,outimg_file);
cout<<"Here"<<endl;
//Image4DSimple *p4DImage3 = &p4DImage2;
//p4DImage = &p4DImage2;
//cout<<"Here"<<endl;
//unsigned char *pData3 = p4DImage->getRawData();
//unsigned int sum = 0;
//for (V3DLONG i=0; i<sz0*sz1*sz2;i++)
//	sum = sum + pData3[i];
//cout<<"sum"<<sum<<endl;
cout<<p4DImage->getXDim()<<endl;
cout<<"getDatatype"<<p4DImage->getDatatype()<<endl;
//cout<<"address"<<pData3[0]<<endl;
//p4DImage->deleteRawDataAndSetPointerToNull();
p4DImage->setRawDataPointer((unsigned char *)pData);

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
