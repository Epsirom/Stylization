#include "nymphimagemanager.h"

using std::map;
using std::pair;

NymphImageManager* NymphImageManager::instance()
{
    static NymphImageManager* _instance = 0;
    if (_instance == 0) {
        _instance = new NymphImageManager();
    }
    return _instance;
}

NymphImageManager::NymphImageManager()
    : imgs(),
      counter(0)
{
}

int NymphImageManager::applySpace()
{
    NymphImagePack* pack = new NymphImagePack;
    int new_id = counter++;
    imgs.insert(pair<int, NymphImagePack*>(new_id, pack));
    return new_id;
}

NymphImagePack* NymphImageManager::getPack(int id)
{
    map<int, NymphImagePack*>::iterator iter = imgs.find(id);
    if (iter != imgs.end())
    {
        return iter->second;
    }
    else
    {
        return 0;
    }
}

int NymphImageManager::releasePack(int id)
{
    map<int, NymphImagePack*>::iterator iter = imgs.find(id);
    if (iter != imgs.end())
    {
        delete iter->second;
    }
    return imgs.erase(id);
}
