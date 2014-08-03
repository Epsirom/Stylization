#ifndef NYMPHCVBASE_H
#define NYMPHCVBASE_H

#include "nymphcore_global.h"


#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <QtGui/QImage>
#include <QtGui/QPixmap>

namespace Nymph
{
    enum
    {
        // 8bit, color or not
        IMREAD_UNCHANGED  =-1,
        // 8bit, gray
        IMREAD_GRAYSCALE  =0,
        // ?, color
        IMREAD_COLOR      =1,
        // any depth, ?
        IMREAD_ANYDEPTH   =2,
        // ?, any color
        IMREAD_ANYCOLOR   =4
    };
    QImage cvMatToQImage( const cv::Mat &inMat );
    QPixmap cvMatToQPixmap( const cv::Mat &inMat );
}


#endif // NYMPHCVBASE_H
