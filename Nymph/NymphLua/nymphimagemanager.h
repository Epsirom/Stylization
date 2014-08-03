#ifndef NYMPHIMAGEMANAGER_H
#define NYMPHIMAGEMANAGER_H

#include <map>
#include <cstdlib>
#include <QPixmap>

typedef struct NymphImagePack {
    QPixmap style_in;
    QPixmap style_out;
    QPixmap nymph_in;
    QPixmap nymph_out;
} NymphImagePack;

class NymphImageManager
{
public:
    static NymphImageManager* instance();
    NymphImageManager();
    int applySpace();
    NymphImagePack* getPack(int id);
    int releasePack(int id);

private:
    std::map<int, NymphImagePack*> imgs;
    int counter;
};

#endif // NYMPHIMAGEMANAGER_H
