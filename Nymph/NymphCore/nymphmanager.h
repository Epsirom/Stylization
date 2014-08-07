#ifndef NYMPHMANAGER_H
#define NYMPHMANAGER_H

#include <map>
#include <string>

namespace cv {
    class Mat;
}

class QPixmap;

typedef struct NymphMatSize {
    int cols;
    int rows;
} NymphMatSize;

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
    cv::Mat getMat(int id, const std::string image_name);
    QPixmap getPixmap(int id, const std::string image_name);
    void aliasMat(int id, const std::string new_name, const std::string old_name);
    void copyMat(int id, const std::string new_name, const std::string old_name);
    NymphMatSize getSize(int id, const std::string name);

    std::map<std::string, cv::Mat> & operator[] (int id);
    cv::Mat & operator() (int id, const std::string name);

private:
    std::map<int, std::map<std::string, cv::Mat> > _storage;
};

#endif // NYMPHMANAGER_H
