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

typedef struct Nymph3B {
    unsigned char c1;
    unsigned char c2;
    unsigned char c3;
} Nymph3B;

typedef NymphPoint NymphCor;
typedef NymphPoint NymphOffset;
typedef NymphPoint Nymph2I;

typedef std::vector<double> NymphEnergyParam;
typedef double (*NymphEnergyFunc) (NymphEnergyParam& param, const cv::Mat& , const cv::Mat& , int , NymphOffset , std::vector<NymphPoint> &);
typedef double (*NymphPatchEnergyFunc) (NymphEnergyParam& param, const cv::Mat& , const cv::Mat& , int , int , int , int , int );


namespace Nymph {
namespace Energy {
double rgb (NymphEnergyParam& param, const cv::Mat& , const cv::Mat& , int , NymphOffset , std::vector<NymphPoint> &);
double rgb_patch (NymphEnergyParam& param, const cv::Mat& , const cv::Mat& , int , int , int , int , int);
}
}

typedef struct NymphEnergyPack {
    NymphEnergyFunc func = Nymph::Energy::rgb;
    NymphEnergyParam param;
} NymphEnergyPack;

typedef struct NymphPatchEnergyPack {
    NymphPatchEnergyFunc func = Nymph::Energy::rgb_patch;
    NymphEnergyParam param;
} NymphPatchEnergyPack;

namespace Nymph {
    void nymph_imshow(std::string window_name, const cv::Mat& mat);
    void nymph_imhide(std::string window_name = std::string());
    void nymph_imwrite(std::string file_name, const cv::Mat& mat);
    Nymph2I nymph_mat_at_2i(const cv::Mat& m, int row, int col);
    Nymph3B nymph_mat_at_3b(const cv::Mat& m, int row, int col);

    void PatchANN(NymphPatchEnergyPack& energy, const cv::Mat& src, const cv::Mat& dst, int patch_radius, cv::Mat& cor, int iterations = 5);
    double EnergyWrapper(NymphEnergyPack& energy, const cv::Mat& src, const cv::Mat& dst, int patch_radius, cv::Mat& cor, std::vector<NymphPoint>& centers);
    double EnergyWrapper(NymphEnergyPack& energy, const cv::Mat &src, const cv::Mat &dst, int patch_radius, NymphOffset off, std::vector<NymphPoint> &centers);
    //double Energy(const cv::Mat& src, const cv::Mat& dst, int patch_radius, cv::Mat& cor, std::vector<NymphPoint>& centers);
    //double Energy(const cv::Mat& src, const cv::Mat& dst, int patch_radius, NymphOffset off, std::vector<NymphPoint> &centers);
    //double Energy(const cv::Mat& src, const cv::Mat& dst, int patch_radius, int center_src_row, int center_src_col, int center_dst_row, int center_dst_col);

    namespace Test {
        void DrawPatchMatchCor(const cv::Mat& dst, const cv::Mat& cor, int patch_radius, cv::Mat& show);
        void DrawPatchMatchCorResult(const cv::Mat& dst, const cv::Mat& cor, int patch_radius, cv::Mat& show, int offset_row = 0, int offset_col = 0);


        void MarkPatch(const cv::Mat& src, cv::Mat& dst, int patch_radius, std::vector<NymphPoint>& centers);
        void MarkCorPatch(const cv::Mat& src, cv::Mat& dst, int patch_radius, const cv::Mat& cor, std::vector<NymphPoint>& centers);
        void MarkCorPatch(const cv::Mat &src, cv::Mat &dst, int patch_radius, NymphOffset off, std::vector<NymphPoint> &centers);
    }
}

#endif // NYMPHALGORITHM_H
