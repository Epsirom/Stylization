#ifndef NYMPHALGORITHM_H
#define NYMPHALGORITHM_H

#include <iostream>
#include <cstring>
#include <vector>

namespace cv {
    class Mat;
}

typedef struct NymphPoint {
    int row, col;
} NymphPoint;

typedef NymphPoint NymphCor;
typedef NymphPoint NymphOffset;

namespace Nymph {
    void nymph_imshow(std::string window_name, const cv::Mat& mat);
    void nymph_imhide(std::string window_name = std::string());
    void nymph_imwrite(std::string file_name, const cv::Mat& mat);

    void PatchANN(const cv::Mat& src, const cv::Mat& dst, int patch_radius, cv::Mat& cor);
    double Energy(const cv::Mat& src, const cv::Mat& dst, int patch_radius, cv::Mat& cor, std::vector<NymphPoint>& centers);
    double Energy(const cv::Mat& src, const cv::Mat& dst, int patch_radius, NymphOffset off, std::vector<NymphPoint> &centers);
    double Energy(const cv::Mat& src, const cv::Mat& dst, int patch_radius, int center_src_row, int center_src_col, int center_dst_row, int center_dst_col);

    namespace Test {
        void MarkPatch(const cv::Mat& src, cv::Mat& dst, int patch_radius, std::vector<NymphPoint>& centers);
        void MarkCorPatch(const cv::Mat& src, cv::Mat& dst, int patch_radius, const cv::Mat& cor, std::vector<NymphPoint>& centers);
        void MarkCorPatch(const cv::Mat &src, cv::Mat &dst, int patch_radius, NymphOffset off, std::vector<NymphPoint> &centers);
    }
}

#endif // NYMPHALGORITHM_H
