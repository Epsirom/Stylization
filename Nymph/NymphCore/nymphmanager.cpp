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
    _storage[id];
}

void NymphManager::releaseNymph(int id)
{
    _storage.erase(id);
}

void NymphManager::resetNymph(int id)
{
    _storage[id].mat.clear();
}

void NymphManager::store(Mat& m, int id, const string image_name)
{
    _storage[id].mat[image_name] = m;
}

void NymphManager::loadFile(const string filename, int id, const string image_name)
{
    Mat tmp = imread(filename);
    _storage[id].mat[image_name] = tmp;
}

Mat& NymphManager::getMat(int id, const string image_name)
{
    return _storage[id].mat[image_name];
}

QPixmap NymphManager::getPixmap(int id, const string image_name)
{
    return Nymph::cvMatToQPixmap(_storage[id].mat[image_name]);
}

void NymphManager::aliasMat(int id, const string new_name, const string old_name)
{
    _storage[id].mat[new_name] = _storage[id].mat[old_name];
}

void NymphManager::copyMat(int id, const string new_name, const string old_name)
{
    _storage[id].mat[old_name].copyTo(_storage[id].mat[new_name]);
}

NymphMatSize NymphManager::getSize(int id, const string name)
{
    NymphMatSize sz;
    Mat& m = _storage[id].mat[name];
    sz.rows = m.rows;
    sz.cols = m.cols;
    return sz;
}

NymphCore& NymphManager::getCore(int id)
{
    return _storage[id].core;
}

NymphEnergyPack& NymphManager::getEnergy(int id)
{
    return _storage[id].energy;
}

int NymphManager::setEnergy(int id, const std::string func_name, std::vector<int>& func_param)
{
    NymphEnergyFunc func = Nymph::Energy::get_energy_func(func_name);
    if (func)
    {
        auto& energy = _storage[id].energy;
        energy.func = func;
        energy.param = func_param;
        return 0;
    }
    else
    {
        return 1;   // Function Not Found.
    }
}

NymphPatchEnergyPack& NymphManager::getPatchEnergy(int id)
{
    return _storage[id].patch_energy;
}

int NymphManager::setPatchEnergy(int id, const std::string func_name, std::vector<int>& func_param)
{
    NymphPatchEnergyFunc func = Nymph::Energy::get_patch_energy_func(func_name);
    if (func)
    {
        auto& patch_energy = _storage[id].patch_energy;
        patch_energy.func = func;
        patch_energy.param = func_param;
        return 0;
    }
    else
    {
        return 1;   // Function Not Found.
    }
}

map<string, Mat> & NymphManager::operator[] (int id)
{
    return _storage[id].mat;
}

Mat & NymphManager::operator() (int id, const string name)
{
    return _storage[id].mat[name];
}
