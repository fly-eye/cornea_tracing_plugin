
#an example plugin project file
TEMPLATE	= lib
CONFIG	+= qt plugin warn_off
#CONFIG	+= x86_64

#set the Vaa3D main path
V3DMAINPATH   =  /Users/valentina/projects/Vaa3dbuild_new/v3d_external/v3d_main
VAA3DPATH     =  /Users/valentina/projects/Vaa3dbuild_new/v3d_external
V3D_PLUGINS   =  /Users/valentina/projects/Vaa3dbuild_new/vaa3d_tools/released_plugins/v3d_plugins
QT_DIR        =  /usr/local/Trolltech/Qt-4.7.1
QT_SHARED_FOLDER = $$QT_DIR/demos/shared # for arthurwidgets


#include necessary paths
INCLUDEPATH	+= $$V3DMAINPATH/basic_c_fun
INCLUDEPATH += $$V3DMAINPATH/common_lib/include
INCLUDEPATH += $$V3D_PLUGINS/ada_threshold
INCLUDEPATH += /Users/valentina/projects/Vaa3dbuild_new/vaa3d_tools/released_plugins/v3d_plugins/ada_threshold
INCLUDEPATH += $$QT_SHARED_FOLDER
INCLUDEPATH  += $$V3DMAINPATH/v3d

LIBS += -L. -lv3dtiff -L$$V3DMAINPATH/common_lib/lib

#include the headers used in the project
HEADERS	+= example_plugin.h
HEADERS	+= example_func.h
HEADERS += ada_thresholding.h
#HEADERS += $$V3DMAINPATH/v3d/mainwindow.h
#HEADERS += $$V3DMAINPATH/3drenderer/v3dr_mainwindow.h
#HEADERS += $$V3DMAINPATH/3drenderer/v3dr_glwidget.h
#HEADERS += $$V3DMAINPATH/3drenderer/qtr_widget.h

macx{
    LIBS += -L$$V3DMAINPATH/jba/c++ -lv3dnewmat
#    CONFIG += x86_64
}

win32{
    LIBS += -L$$V3DMAINPATH/common_lib/winlib64 -llibnewmat
}

unix:!macx {
    LIBS += -L$$V3DMAINPATH/jba/c++ -lv3dnewmat
}


#include the source files used in the project
SOURCES	+= example_plugin.cpp
SOURCES	+= example_func.cpp
SOURCES	+= $$V3DMAINPATH/basic_c_fun/v3d_message.cpp
SOURCES += $$V3DMAINPATH/basic_c_fun/stackutil.cpp
SOURCES += $$V3DMAINPATH/basic_c_fun/mg_image_lib.cpp
SOURCES += $$V3DMAINPATH/basic_c_fun/mg_utilities.cpp
SOURCES += $$V3DMAINPATH/basic_c_fun/basic_memory.cpp
SOURCES += $$V3DMAINPATH/basic_c_fun/basic_surf_objs.cpp
SOURCES += ada_thresholding.cpp
#SOURCES += $$V3DMAINPATH/v3d/my4dimage.cpp
#SOURCES += $$V3D_PLUGINS/ada_threshold/moc_ada_threshold.cpp
SOURCES += $$V3DMAINPATH/jba/c++/histeq.cpp
SOURCES += $$V3DMAINPATH/neuron_editing/v_neuronswc.cpp
SOURCE += $$V3DMAINPATH/v3d/mainwindow.cpp
SOURCE += $$V3DMAINPATH/v3d/mainwindow_interface.cpp
SOURCE += $$V3DMAINPATH/v3d/mainwindow_wsi.cpp
#SOURCE += $$V3DMAINPATH/v3d/*
SOURCE += $$V3DMAINPATH/v3d/mdichild.cpp
SOURCE += $$V3DMAINPATH/v3d/3drenderer/v3dr_mainwindow.cpp
#SOURCE += $$V3DMAINPATH/v3d/3drenderer/v3dr_glwidget.cpp

## Source files needed for SIGEN
SOURCE += SIGEN.cpp



#specify target name and directory
TARGET	= $$qtLibraryTarget(example_plugin)
DESTDIR	= $$VAA3DPATH/bin/plugins/example_plugin/
