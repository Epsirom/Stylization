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

void PatchANN(const Mat &src, const Mat &dst, int patch_radius, Mat &cor)
{
    // Here implement an Approximate Nearest Neighbor algorithm of PatchMatch
    qDebug() << "PatchANN";
    cor.create(src.rows, src.cols, CV_32SC2);
    PatchMatch::InitCor(cor, dst.size(), patch_radius);
    //Test::DrawCor(dst, cor, patch_radius); return;

    int random_search_radius = dst.rows < dst.cols ? dst.cols : dst.rows;
    random_search_radius -= (2 * patch_radius);

    //imshow("Source", src);

    for (int it = 0; it < 5; ++it)
    {
        // An iteration of PatchMatch ANN.
        for (int i = patch_radius; i < src.rows - patch_radius; ++i)
        {
            //qDebug() << "Iteration" << it << ", processing row" << i;
            for (int j = patch_radius; j < src.cols - patch_radius; ++j)
            {
                //qDebug() << "Processing {" << i << "," << j << "}";
                PatchMatch::Propagation(src, dst, cor, patch_radius, i, j);
                //auto& cor_pt = cor.at<Vec2i>(i, j);
                //if (cor_pt[0] >= dst.rows - patch_radius || cor_pt[1] >= dst.cols - patch_radius)
                //    qDebug() << "Propagation error" << i << j;
                PatchMatch::RandomSearch(src, dst, cor, patch_radius, i, j, random_search_radius, 0.5);
                //auto& cor_pt2 = cor.at<Vec2i>(i, j);
                //if (cor_pt2[0] >= dst.rows - patch_radius || cor_pt2[1] >= dst.cols - patch_radius)
                //    qDebug() << "RandomSearch error" << i << j;
            }
        }

        //Test::DrawCorResult(dst, cor, patch_radius, 0, 0);
    }

    //qDebug() << "PatchMatch Finished.";

}

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


namespace Test {


void MarkPatch(const Mat &src, Mat &dst, int patch_radius, std::vector<NymphPoint> &centers)
{
    src.copyTo(dst);
    for (auto& pt : centers)
    {
        rectangle(dst,
                  Point(pt.col - patch_radius, pt.row - patch_radius),
                  Point(pt.col + patch_radius, pt.row + patch_radius),
                  Scalar(0, 0, 255)
                  );
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
    for (auto& pt : centers)
    {
        rectangle(dst,
                  Point(pt.col - patch_radius + off.col, pt.row - patch_radius + off.row),
                  Point(pt.col + patch_radius + off.col, pt.row + patch_radius + off.row),
                  Scalar(0, 0, 255)
                  );
    }
}


}

}
