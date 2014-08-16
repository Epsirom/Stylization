#include "nymphalgorithm.h"

#include "nymphpatchmatch.h"

#include <cmath>
#include <QDebug>
#include <iostream>

using namespace cv;

namespace Nymph {


void nymph_imshow(std::string window_name, const Mat &mat)
{
    imshow(window_name, mat);
}

void nymph_imhide(std::string window_name)
{
    if (window_name == "")
    {
        cvDestroyAllWindows();
    }
    else
    {
        cvDestroyWindow(window_name.data());
    }
}

void nymph_imwrite(std::string file_name, const Mat &mat)
{
    imwrite(file_name, mat);
}

NymphPoint nymph_corpoint(const Mat &cor, int row, int col)
{
    auto& pt = cor.at<Vec2i>(row, col);
    NymphPoint npt;
    npt.row = pt[0];
    npt.col = pt[1];
    return npt;
}

void PatchANN(NymphPatchEnergyFunc energy, const Mat &src, const Mat &dst, int patch_radius, Mat &cor, int iterations)
{
    // Here implement an Approximate Nearest Neighbor algorithm of PatchMatch
    qDebug() << "PatchANN Start...";
    cor.create(src.rows, src.cols, CV_32SC2);
    PatchMatch::InitCor(cor, dst.size(), patch_radius);
    //Test::DrawCor(dst, cor, patch_radius); return;

    int random_search_radius = dst.rows < dst.cols ? dst.cols : dst.rows;
    random_search_radius -= (2 * patch_radius);

    //imshow("Source", src);

    for (int it = 0; it < iterations; ++it)
    {
        // An iteration of PatchMatch ANN.
        qDebug() << "Iteration" << it;
        for (int i = patch_radius; i < src.rows - patch_radius; ++i)
        {
            //qDebug() << "Iteration" << it << ", processing row" << i;
            for (int j = patch_radius; j < src.cols - patch_radius; ++j)
            {
                //qDebug() << "Processing {" << i << "," << j << "}";
                PatchMatch::Propagation(energy, src, dst, cor, patch_radius, i, j);
                //auto& cor_pt = cor.at<Vec2i>(i, j);
                //if (cor_pt[0] >= dst.rows - patch_radius || cor_pt[1] >= dst.cols - patch_radius)
                //    qDebug() << "Propagation error" << i << j;
                PatchMatch::RandomSearch(energy, src, dst, cor, patch_radius, i, j, random_search_radius, 0.5);
                //auto& cor_pt2 = cor.at<Vec2i>(i, j);
                //if (cor_pt2[0] >= dst.rows - patch_radius || cor_pt2[1] >= dst.cols - patch_radius)
                //    qDebug() << "RandomSearch error" << i << j;
            }
        }

        //Test::DrawCorResult(dst, cor, patch_radius, 0, 0);
    }

    qDebug() << "PatchANN Finished.";

}


double EnergyWrapper(NymphEnergyFunc func, const Mat &src, const Mat &dst, int patch_radius, Mat &cor, std::vector<NymphPoint> &centers)
{
    NymphOffset offset;
    NymphPoint pt = centers[0];
    //qDebug() << "centers[0] {" << pt.row << "," << pt.col << "}";
    auto corpt = cor.at<Vec2i>(pt.row, pt.col);
    offset.row = corpt[0] - pt.row;
    offset.col = corpt[1] - pt.col;
    //qDebug() << "Offset {" << offset.row << "," << offset.col << "}";
    return func(src, dst, patch_radius, offset, centers);
}

double EnergyWrapper(NymphEnergyFunc func, const Mat &src, const Mat &dst, int patch_radius, NymphOffset off, std::vector<NymphPoint> &centers)
{
    return func(src, dst, patch_radius, off, centers);
}

/*
double Energy(const Mat &src, const Mat &dst, int patch_radius, Mat &cor, std::vector<NymphPoint> &centers)
{
    NymphOffset offset;
    NymphPoint pt = centers[0];
    //qDebug() << "centers[0] {" << pt.row << "," << pt.col << "}";
    auto corpt = cor.at<Vec2i>(pt.row, pt.col);
    offset.row = corpt[0] - pt.row;
    offset.col = corpt[1] - pt.col;
    //qDebug() << "Offset {" << offset.row << "," << offset.col << "}";
    return Energy(src, dst, patch_radius, offset, centers);
}

double Energy(const Mat &src, const Mat &dst, int patch_radius, NymphOffset off, std::vector<NymphPoint> &centers)
{
    double result = 0;
    int patch_size = patch_radius * 2 + 1;
    for (auto& pt : centers)
    {
        result += Energy(src, dst, patch_radius, pt.row, pt.col, pt.row + off.row, pt.col + off.col);
    }
    return result - centers.size() * 150 * patch_size * patch_size;
}

double Energy(const Mat &src, const Mat &dst, int patch_radius, int center_src_row, int center_src_col, int center_dst_row, int center_dst_col)
{
    if (center_src_row < patch_radius || center_src_row >= src.rows - patch_radius
            || center_src_col < patch_radius || center_src_col >= src.cols - patch_radius
            || center_dst_row < patch_radius || center_dst_row >= src.rows - patch_radius
            || center_dst_col < patch_radius || center_dst_col >= src.cols - patch_radius
            )
    {
        return 1e100;
    }

    double result = 0;
    /*
    auto& src_pt = src.at<Vec3b>(center_src_row, center_src_col);
    auto& dst_pt = dst.at<Vec3b>(center_dst_row, center_dst_col);
    for (int k = 0; k < 3; ++k)
    {
        result += fabs(src_pt[k] - dst_pt[k]);
    }
    */
/*
    for (int i = -patch_radius; i <= patch_radius; ++i)
    {
        for (int j = -patch_radius; j <= patch_radius; ++j)
        {
            auto& src_pt = src.at<Vec3b>(center_src_row + i, center_src_col + j);
            auto& dst_pt = dst.at<Vec3b>(center_dst_row + i, center_dst_col + j);
            for (int k = 0; k < 3; ++k)
            {
                result += fabs(src_pt[k] - dst_pt[k]);
            }
        }
    }

    return result;
}
*/


namespace Test {



void DrawPatchMatchCor(const Mat &dst, const Mat &cor, int patch_radius, Mat& show)
{
    DrawCor(dst, cor, patch_radius, show);
}

void DrawPatchMatchCorResult(const Mat &dst, const Mat &cor, int patch_radius, Mat& show, int offset_row, int offset_col)
{
    DrawCorResult(dst, cor, patch_radius, show, offset_row, offset_col);
}

void MarkPatch(const Mat &src, Mat &dst, int patch_radius, std::vector<NymphPoint> &centers)
{
    src.copyTo(dst);
    Mat flags(src.rows, src.cols, CV_8UC1, Scalar::all(0));
    for (auto& pt : centers)
    {
        flags.at<uchar>(pt.row, pt.col) = 1;
    }
    int patch_size = patch_radius * 2 + 1;
    int dirmap[4][2] = {
        {1, 0}, {-1, 0}, {0, 1}, {0, -1}
    };
    int linemap[3][2] = {   // {from, to}
        {-1, -1}, {-1, 1}, {1, 1}
    };
    for (auto& pt : centers)
    {
        for (int i = 0; i < 4; ++i)
        {
            int tmp_row = pt.row + dirmap[i][0] * patch_size, tmp_col = pt.col + dirmap[i][1] * patch_size;
            if (tmp_row < 0 || tmp_row >= flags.rows
                    || tmp_col < 0 || tmp_col >= flags.cols
                    || flags.at<uchar>(tmp_row, tmp_col) == 0)
            {
                Point fromPoint(
                            pt.col + linemap[dirmap[i][1] + 1][0] * patch_radius,
                            pt.row + linemap[dirmap[i][0] + 1][0] * patch_radius
                        );
                Point toPoint(
                            pt.col + linemap[dirmap[i][1] + 1][1] * patch_radius,
                            pt.row + linemap[dirmap[i][0] + 1][1] * patch_radius
                        );
                line(dst, fromPoint, toPoint, Scalar(0, 0, 255), 2);
            }
        }
    }
}

void MarkCorPatch(const Mat &src, Mat &dst, int patch_radius, const Mat &cor, std::vector<NymphPoint> &centers)
{
    NymphOffset offset;
    NymphPoint pt = centers[0];
    auto corpt = cor.at<Vec2i>(pt.row, pt.col);
    offset.row = corpt[0] - pt.row;
    offset.col = corpt[1] - pt.col;
    //qDebug() << "Offset {" << offset.row << "," << offset.col << "}";
    MarkCorPatch(src, dst, patch_radius, offset, centers);
}

void MarkCorPatch(const Mat &src, Mat &dst, int patch_radius, NymphOffset off, std::vector<NymphPoint> &centers)
{
    src.copyTo(dst);
    Mat flags(src.rows, src.cols, CV_8UC1, Scalar::all(0));
    for (auto& pt : centers)
    {
        pt.row += off.row;
        pt.col += off.col;
        flags.at<uchar>(pt.row, pt.col) = 1;
    }
    int patch_size = patch_radius * 2 + 1;
    int dirmap[4][2] = {
        {1, 0}, {-1, 0}, {0, 1}, {0, -1}
    };
    int linemap[3][2] = {   // {from, to}
        {-1, -1}, {-1, 1}, {1, 1}
    };
    for (auto& pt : centers)
    {
        for (int i = 0; i < 4; ++i)
        {
            int tmp_row = pt.row + dirmap[i][0] * patch_size, tmp_col = pt.col + dirmap[i][1] * patch_size;
            if (tmp_row < 0 || tmp_row >= flags.rows
                    || tmp_col < 0 || tmp_col >= flags.cols
                    || flags.at<uchar>(tmp_row, tmp_col) == 0)
            {
                Point fromPoint(
                            pt.col + linemap[dirmap[i][1] + 1][0] * patch_radius,
                            pt.row + linemap[dirmap[i][0] + 1][0] * patch_radius
                        );
                Point toPoint(
                            pt.col + linemap[dirmap[i][1] + 1][1] * patch_radius,
                            pt.row + linemap[dirmap[i][0] + 1][1] * patch_radius
                        );
                line(dst, fromPoint, toPoint, Scalar(0, 0, 255), 2);
            }
        }
    }
}


}

}
