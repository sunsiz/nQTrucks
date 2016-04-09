include($$PWD/../../common.pri)

INCLUDEPATH += $$PWD/..
DEPENDPATH  += $$PWD/..

SOURCES += \
 $$PWD/alpr.cpp \
 $$PWD/alpr_impl.cpp \
 $$PWD/config.cpp \
 $$PWD/detection/detector.cpp \
 $$PWD/detection/detectorcpu.cpp \
 $$PWD/detection/detectorcuda.cpp \
 $$PWD/detection/detectorocl.cpp \
 $$PWD/detection/detectorfactory.cpp \
 $$PWD/detection/detectormorph.cpp \
 $$PWD/licenseplatecandidate.cpp \
 $$PWD/utility.cpp \
 $$PWD/ocr.cpp \
 $$PWD/postprocess/postprocess.cpp \
 $$PWD/postprocess/regexrule.cpp \
 $$PWD/binarize_wolf.cpp \
 $$PWD/segmentation/charactersegmenter.cpp \
 $$PWD/segmentation/histogram.cpp \
 $$PWD/segmentation/histogramvertical.cpp \
 $$PWD/segmentation/histogramhorizontal.cpp \
 $$PWD/edges/edgefinder.cpp \
 $$PWD/edges/platecorners.cpp \
 $$PWD/edges/platelines.cpp \
 $$PWD/edges/textlinecollection.cpp \
 $$PWD/edges/scorekeeper.cpp \
 $$PWD/colorfilter.cpp \
 $$PWD/prewarp.cpp \
 $$PWD/transformation.cpp \
 $$PWD/textdetection/characteranalysis.cpp \
 $$PWD/textdetection/platemask.cpp \
 $$PWD/textdetection/textcontours.cpp \
 $$PWD/textdetection/textline.cpp \
 $$PWD/textdetection/linefinder.cpp \
 $$PWD/pipeline_data.cpp \
 $$PWD/cjson.c \
 $$PWD/motiondetector.cpp \
 $$PWD/result_aggregator.cpp 

HEADERS += \
 $$PWD/alpr.h \
 $$PWD/alpr_impl.h \
 $$PWD/config.h \
 $$PWD/detection/detector.h \
 $$PWD/detection/detectorcpu.h \
 $$PWD/detection/detectorcuda.h \
 $$PWD/detection/detectorocl.h \
 $$PWD/detection/detectorfactory.h \
 $$PWD/detection/detectormorph.h \
 $$PWD/licenseplatecandidate.h \
 $$PWD/utility.h \
 $$PWD/ocr.h \
 $$PWD/postprocess/postprocess.h \
 $$PWD/postprocess/regexrule.h \
 $$PWD/binarize_wolf.h \
 $$PWD/segmentation/charactersegmenter.h \
 $$PWD/segmentation/histogram.h \
 $$PWD/segmentation/histogramvertical.h \
 $$PWD/segmentation/histogramhorizontal.h \
 $$PWD/edges/edgefinder.h \
 $$PWD/edges/platecorners.h \
 $$PWD/edges/platelines.h \
 $$PWD/edges/textlinecollection.h \
 $$PWD/edges/scorekeeper.h \
 $$PWD/colorfilter.h \
 $$PWD/prewarp.h \
 $$PWD/transformation.h \
 $$PWD/textdetection/characteranalysis.h \
 $$PWD/textdetection/platemask.h \
 $$PWD/textdetection/textcontours.h \
 $$PWD/textdetection/textline.h \
 $$PWD/textdetection/linefinder.h \
 $$PWD/pipeline_data.h \
 $$PWD/cjson.h \
 $$PWD/motiondetector.h \
 $$PWD/result_aggregator.h 

