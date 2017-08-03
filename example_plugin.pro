
#an example plugin project file
TEMPLATE	= lib
CONFIG	+= qt plugin warn_off
#CONFIG	+= x86_64

#set the Vaa3D main path
V3DMAINPATH   =  /Users/valentina/projects/Vaa3dbuild_new/v3d_external/v3d_main
VAA3DPATH     =  /Users/valentina/projects/Vaa3dbuild_new/v3d_external
V3D_PLUGINS   =  /Users/valentina/projects/Vaa3dbuild_new/vaa3d_tools/released_plugins/v3d_plugins

#include necessary paths
INCLUDEPATH	+= $$V3DMAINPATH/basic_c_fun
INCLUDEPATH += $$V3DMAINPATH/common_lib/include
INCLUDEPATH += $$V3D_PLUGINS/ada_threshold
INCLUDEPATH += /Users/valentina/projects/Vaa3dbuild_new/vaa3d_tools/released_plugins/v3d_plugins/ada_threshold
LIBS += -L. -lv3dtiff -L$$V3DMAINPATH/common_lib/lib

#include the headers used in the project
HEADERS	+= example_plugin.h
HEADERS	+= example_func.h
HEADERS += ada_thresholding.h


#include the source files used in the project
SOURCES	+= example_plugin.cpp
SOURCES	+= example_func.cpp
SOURCES	+= $$V3DMAINPATH/basic_c_fun/v3d_message.cpp
SOURCES += $$V3DMAINPATH/basic_c_fun/stackutil.cpp
SOURCES += $$V3DMAINPATH/basic_c_fun/mg_image_lib.cpp
SOURCES += $$V3DMAINPATH/basic_c_fun/mg_utilities.cpp
SOURCES += $$V3DMAINPATH/basic_c_fun/basic_memory.cpp
SOURCES += ada_thresholding.cpp
#SOURCES += $$V3D_PLUGINS/ada_threshold/moc_ada_threshold.cpp


#specify target name and directory
TARGET	= $$qtLibraryTarget(example_plugin)
DESTDIR	= $$VAA3DPATH/bin/plugins/example_plugin/
