#ifndef NYMPHPATCHMATCH_H
#define NYMPHPATCHMATCH_H

#include <opencv2/opencv.hpp>

namespace Nymph
{
    namespace PatchMatch {
        void InitCor(cv::Mat& cor, cv::Size dst_size, int patch_radius);
        bool Propagation(const cv::Mat& src, const cv::Mat& dst, cv::Mat& cor, int patch_radius, int row, int col);
        bool RandomSearch(const cv::Mat& src, const cv::Mat& dst, cv::Mat& cor, int patch_radius, int row, int col, int radius, double alpha);
    }

    namespace Test {
        void DrawCor(const cv::Mat& dst, const cv::Mat& cor, int patch_radius);
        void DrawCorResult(const cv::Mat& dst, const cv::Mat& cor, int patch_radius, int offset_row = 0, int offset_col = 0);
    }
}

#endif // NYMPHPATCHMATCH_H
