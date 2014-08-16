#ifndef NYMPHMANAGER_H
#define NYMPHMANAGER_H

#include "nymphcore.h"
#include "nymphenergy.h"

#include <map>
#include <vector>
#include <string>

class QPixmap;

typedef struct NymphMatSize {
    int cols;
    int rows;
} NymphMatSize;

typedef struct NymphStoragePack {
    std::map<std::string, cv::Mat> mat;
    NymphCore core;
    NymphEnergyPack energy;
    NymphPatchEnergyPack patch_energy;
} NymphStoragePack;

class NymphManager
{
public:
    static NymphManager* instance();
    static NymphManager& insobj();
    NymphManager();

    void initNymph(int id);
    void releaseNymph(int id);
    void resetNymph(int id);
    void store(cv::Mat& m, int id, const std::string image_name);
    void loadFile(const std::string filename, int id, const std::string image_name);
    cv::Mat& getMat(int id, const std::string image_name);
    QPixmap getPixmap(int id, const std::string image_name);
    void aliasMat(int id, const std::string new_name, const std::string old_name);
    void copyMat(int id, const std::string new_name, const std::string old_name);
    NymphMatSize getSize(int id, const std::string name);

    NymphCore& getCore(int id);
    NymphEnergyPack& getEnergy(int id);
    int setEnergy(int id, const std::string func_name, std::vector<int>& func_param);
    NymphPatchEnergyPack& getPatchEnergy(int id);
    int setPatchEnergy(int id, const std::string func_name, std::vector<int>& func_param);

    std::map<std::string, cv::Mat> & operator[] (int id);
    cv::Mat & operator() (int id, const std::string name);

private:
    std::map<int, NymphStoragePack> _storage;
};

#endif // NYMPHMANAGER_H
