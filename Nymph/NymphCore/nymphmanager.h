#ifndef NYMPHMANAGER_H
#define NYMPHMANAGER_H

#include <map>
#include <string>

namespace cv {
    class Mat;
}

class QPixmap;

class NymphManager
{
public:
    static NymphManager* instance();
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

private:
    std::map<int, std::map<std::string, cv::Mat> > _storage;
};

#endif // NYMPHMANAGER_H
