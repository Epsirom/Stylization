#include "nymphpatchmatch.h"
#include "nymphalgorithm.h"
#include <ctime>

#include <QDebug>

using namespace cv;

RNG rng((uint64)time(nullptr));

namespace Nymph {
namespace PatchMatch {


void InitCor(Mat &cor, Size dst_size, int patch_radius)
{
    for (int i = patch_radius; i < cor.rows - patch_radius; ++i)
    {
        for (int j = patch_radius; j < cor.cols - patch_radius; ++j)
        {
            auto& cor_pt = cor.at<Vec2i>(i, j);
            cor_pt[0] = rng.uniform(patch_radius, dst_size.height - patch_radius);
            cor_pt[1] = rng.uniform(patch_radius, dst_size.width - patch_radius);
            //qDebug() << "cor{" << i << "," << j << "} = {" << cor_pt[0] << "," << cor_pt[1] << "}";
        }
    }
}

bool Propagation(NymphPatchEnergyFunc energy, const Mat &src, const Mat &dst, Mat &cor, int patch_radius, int row, int col)
{
    bool flag = false;
    auto& cor_pt = cor.at<Vec2i>(row, col);
    double old_energy = energy(src, dst, patch_radius, row, col, cor_pt[0], cor_pt[1]);
    // Propagation from left
    if (col > patch_radius)
    {
        auto& cor_left = cor.at<Vec2i>(row, col - 1);
        if (cor_left[1] + 1 < dst.cols - patch_radius)
        {
            double from_left_energy = energy(src, dst, patch_radius, row, col, cor_left[0], cor_left[1] + 1);
            if (from_left_energy <= old_energy)
            {
                cor_pt[0] = cor_left[0];
                cor_pt[1] = cor_left[1] + 1;
                //qDebug() << "Propagation left: {" << row << "," << col << "} to {" << cor_pt[0] << "," << cor_pt[1] << "}";
                flag = true;
            }
        }
    }
    // Propagation from top
    if (row > patch_radius)
    {
        auto& cor_top = cor.at<Vec2i>(row - 1, col);
        if (cor_top[0] + 1 < dst.rows - patch_radius)
        {
            double from_top_energy = energy(src, dst, patch_radius, row, col, cor_top[0] + 1, cor_top[1]);
            if (from_top_energy <= old_energy)
            {
                cor_pt[0] = cor_top[0] + 1;
                cor_pt[1] = cor_top[1];
                //qDebug() << "Propagation top: {" << row << "," << col << "} to {" << cor_pt[0] << "," << cor_pt[1] << "}";
                flag = true;
            }
        }
    }
    return flag;
}

bool RandomSearch(NymphPatchEnergyFunc energy, const Mat &src, const Mat &dst, Mat &cor, int patch_radius, int row, int col, int radius, double alpha)
{
    bool flag = false;
    auto& cor_pt = cor.at<Vec2i>(row, col);
    double old_energy = energy(src, dst, patch_radius, row, col, cor_pt[0], cor_pt[1]);

    double current_alpha = 1.0;
    int current_radius = radius;
    int hor_min, hor_max, ver_min, ver_max, new_row, new_col;
    do
    {
        // Find a patch below radius
        hor_min = cor_pt[1] - current_radius;
        if (hor_min < patch_radius)
            hor_min = patch_radius;
        hor_max = cor_pt[1] + current_radius;
        if (hor_max > dst.cols - patch_radius)  // Here is > for the usage of rng.uniform
            hor_max = dst.cols - patch_radius;  // !!! do not minus 1

        ver_min = cor_pt[0] - current_radius;
        if (ver_min < patch_radius)
            ver_min = patch_radius;
        ver_max = cor_pt[0] + current_radius;
        if (ver_max > dst.rows - patch_radius)  // Here is > for the usage of rng.uniform
            ver_max = dst.rows - patch_radius;  // !!! do not minus 1

        new_row = rng.uniform(ver_min, ver_max);
        new_col = rng.uniform(hor_min, hor_max);

        // Check if it is better
        double new_energy = energy(src, dst, patch_radius, row, col, new_row, new_col);
        if (new_energy < old_energy)    // Here is <, not <=
        {
            cor_pt[0] = new_row;
            cor_pt[1] = new_col;
            //qDebug() << "Random search: {" << row << "," << col << "} to {" << cor_pt[0] << "," << cor_pt[1] << "}";
            flag = true;
        }

        // Update alpha and radius
        current_alpha *= alpha;
        current_radius = radius * current_alpha;
    } while (current_radius >= 1);

    return flag;
}


}

namespace Test {


void DrawCor(const Mat &dst, const Mat &cor, int patch_radius)
{
    Mat show;
    dst.copyTo(show);
    for (int i = patch_radius; i < cor.rows - patch_radius; ++i)
    {
        for (int j = patch_radius; j < cor.cols - patch_radius; ++j)
        {
            auto& cor_pt = cor.at<Vec2i>(i,j);
            auto& show_pt = show.at<Vec3b>(cor_pt[0], cor_pt[1]);
            show_pt[0] = 0;
            show_pt[1] = 0;
            show_pt[2] = 255;
        }
    }
    imshow("Corresponding Map", show);
}

void DrawCorResult(const Mat &dst, const Mat &cor, int patch_radius, int offset_row, int offset_col)
{
    Mat show(cor.rows, cor.cols, CV_8UC3, Scalar::all(0));
    int patch_size = patch_radius * 2 + 1;
    for (int i = patch_radius + offset_row; i < cor.rows - patch_radius; i += patch_radius * 2)
    {
        for (int j = patch_radius + offset_col; j < cor.cols - patch_radius; j += patch_radius * 2)
        {
            auto& cor_pt = cor.at<Vec2i>(i, j);
            /*
             * (0 <= roi.x
             * && 0 <= roi.width
             * && roi.x + roi.width <= m.cols
             * && 0 <= roi.y
             * && 0 <= roi.height
             * && roi.y + roi.height <= m.rows)
             */
            //qDebug() << i << j << cor_pt[0] - patch_radius << cor_pt[1] - patch_radius << patch_size << i - patch_radius << j - patch_radius;
            dst(Rect(cor_pt[1] - patch_radius, cor_pt[0] - patch_radius, patch_size, patch_size)).
                    copyTo(show(Rect(j - patch_radius, i - patch_radius, patch_size, patch_size)));
        }
    }
    imshow("PatchMatch Result", show);
}


}
}
