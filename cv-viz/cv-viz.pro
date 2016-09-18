TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
    main.cpp

DEFINES += _GLIBCXX_USE_CXX11_ABI=0


unix:!macx: LIBS += -L/opt/develop/libs/lib/ \
                    -lopencv_aruco \
                    -lopencv_bgsegm \
                    -lopencv_bioinspired \
                    -lopencv_calib3d \
                    -lopencv_ccalib \
                    -lopencv_core \
                    -lopencv_cudaarithm \
                    -lopencv_cudabgsegm \
                    -lopencv_cudacodec \
                    -lopencv_cudafeatures2d \
                    -lopencv_cudafilters \
                    -lopencv_cudaimgproc \
                    -lopencv_cudalegacy \
                    -lopencv_cudaobjdetect \
                    -lopencv_cudaoptflow \
                    -lopencv_cudastereo \
                    -lopencv_cudawarping \
                    -lopencv_cudev \
                    -lopencv_datasets \
                    -lopencv_dnn \
                    -lopencv_dpm \
                    -lopencv_face \
                    -lopencv_features2d \
                    -lopencv_flann \
                    -lopencv_fuzzy \
                    -lopencv_hdf \
                    -lopencv_highgui \
                    -lopencv_imgcodecs \
                    -lopencv_imgproc \            
                    -lopencv_line_descriptor \
                    -lopencv_ml \
                    -lopencv_objdetect \
                    -lopencv_optflow \
                    -lopencv_photo \
                    -lopencv_plot \
                    -lopencv_reg \
                    -lopencv_rgbd \
                    -lopencv_saliency \
                    -lopencv_shape \
                    -lopencv_stereo \
                    -lopencv_stitching \
                    -lopencv_structured_light \
                    -lopencv_superres \
                    -lopencv_surface_matching \
                    -lopencv_text \
                    -lopencv_tracking \
                    -lopencv_videoio \
                    -lopencv_video \
                    -lopencv_videostab \
                    -lopencv_viz \
                    -lopencv_xfeatures2d \
                    -lopencv_ximgproc \
                    -lopencv_xobjdetect \
                    -lopencv_xphoto \
                    -lboost_atomic \
                    -lboost_chrono \
                    -lboost_container \
                    -lboost_context \
                    -lboost_coroutine \
                    -lboost_date_time \
                    -lboost_exception \
                    -lboost_filesystem \
                    -lboost_graph \
                    -lboost_locale \
                    -lboost_log \
                    -lboost_log_setup \
                    -lboost_math_c99 \
                    -lboost_math_c99f \
                    -lboost_math_c99l \
                    -lboost_math_tr1 \
                    -lboost_math_tr1f \
                    -lboost_math_tr1l \
                    -lboost_prg_exec_monitor \
                    -lboost_program_options \
                    -lboost_python \
                    -lboost_random \
                    -lboost_regex \
                    -lboost_serialization \
                    -lboost_signals \
                    -lboost_system \
                    -lboost_test_exec_monitor \
                    -lboost_thread \
                    -lboost_timer \
                    -lboost_type_erasure \
                    -lboost_unit_test_framework \
                    -lboost_wave \
                    -lboost_wserialization


INCLUDEPATH += $$PWD/../../../../../../opt/develop/libs/include
DEPENDPATH += $$PWD/../../../../../../opt/develop/libs/include

DISTFILES += \
    main.cpp.bak

HEADERS += \
    main.cpp.bak.2
