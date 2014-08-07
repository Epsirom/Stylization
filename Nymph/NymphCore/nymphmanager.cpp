#include "nymphmanager.h"
#include <QPixmap>
#include <opencv2/opencv.hpp>
#include "nymphcvbase.h"

using std::string;
using std::map;
using namespace cv;

NymphManager* NymphManager::instance()
{
    static NymphManager* _instance = 0;
    if (!_instance) {
        _instance = new NymphManager();
    }
    return _instance;
}

NymphManager& NymphManager::insobj()
{
    return *(NymphManager::instance());
}

NymphManager::NymphManager()
    : _storage()
{
}

void NymphManager::initNymph(int id)
{
    _storage[id] = map<string, Mat>();
}

void NymphManager::releaseNymph(int id)
{
    _storage.erase(id);
}

void NymphManager::resetNymph(int id)
{
    _storage[id].clear();
}

void NymphManager::store(Mat& m, int id, const string image_name)
{
    _storage[id][image_name] = m;
}

void NymphManager::loadFile(const string filename, int id, const string image_name)
{
    Mat tmp = imread(filename);
    _storage[id][image_name] = tmp;
}

Mat NymphManager::getMat(int id, const string image_name)
{
    return _storage[id][image_name];
}

QPixmap NymphManager::getPixmap(int id, const string image_name)
{
    return Nymph::cvMatToQPixmap(_storage[id][image_name]);
}

void NymphManager::aliasMat(int id, const string new_name, const string old_name)
{
    _storage[id][new_name] = _storage[id][old_name];
}

void NymphManager::copyMat(int id, const string new_name, const string old_name)
{
    _storage[id][old_name].copyTo(_storage[id][new_name]);
}

NymphMatSize NymphManager::getSize(int id, const string name)
{
    NymphMatSize sz;
    Mat& m = _storage[id][name];
    sz.rows = m.rows;
    sz.cols = m.cols;
    return sz;
}

map<string, Mat> & NymphManager::operator[] (int id)
{
    return _storage[id];
}

Mat & NymphManager::operator() (int id, const string name)
{
    return _storage[id][name];
}
