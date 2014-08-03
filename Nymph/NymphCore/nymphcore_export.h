#ifndef NYMPHCORE_EXPORT_H
#define NYMPHCORE_EXPORT_H

#include "nymphcore_global.h"

class QPixmap;

namespace Nymph
{
    QPixmap NYMPHCORESHARED_EXPORT loadImage(const char* filename, int flags=1);
}

#endif // NYMPHCORE_EXPORT_H
