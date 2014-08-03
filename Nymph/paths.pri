
# Common settings for OS X
unix {
    QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.9
}

# Library settings
use_lua {
    INCLUDEPATH += $$PWD/3rd/lua/include
    LIBS += -L$$PWD/3rd/lua/lib -llua
}

use_opencv {
    INCLUDEPATH += /usr/local/Cellar/opencv/2.4.9/include
    LIBS += -L/usr/local/Cellar/opencv/2.4.9/lib \
            -lopencv_core \
            -lopencv_highgui \
            -lopencv_imgproc \
            -lopencv_features2d \
            -lstdc++
}
