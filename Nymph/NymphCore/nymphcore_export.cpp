#include "nymphcvbase.h"

#include "nymphcvbase.h"
#include "nymphcore_export.h"


using namespace cv;

namespace Nymph
{
    QPixmap loadImage(const char *filename, int flags)
    {
        //return QPixmap();
        Mat src = imread(std::string(filename), flags);
        return cvMatToQPixmap(src);
    }
}
