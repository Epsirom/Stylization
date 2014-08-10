#include "nymphenergy.h"

#include <map>
#include <opencv2/opencv.hpp>

using namespace cv;

namespace Nymph
{
namespace Energy
{


NymphEnergyFunc get_energy_func(const std::string &func_name)
{
    static std::map<std::string, NymphEnergyFunc> energy_func_map;
    if (energy_func_map.empty())
    {
        NYMPH_ENERGY_FUNC_REGISTER;
    }
    auto iter = energy_func_map.find(func_name);
    if (iter != energy_func_map.end())
    {
        return iter->second;
    }
    return 0;
}

NymphPatchEnergyFunc get_patch_energy_func(const std::string &func_name)
{
    static std::map<std::string, NymphPatchEnergyFunc> energy_func_map;
    if (energy_func_map.empty())
    {
        NYMPH_PATCH_ENERGY_FUNC_REGISTER;
    }
    auto iter = energy_func_map.find(func_name);
    if (iter != energy_func_map.end())
    {
        return iter->second;
    }
    return 0;
}

NYMPH_ENERGY_FUNC_TPL(rgb_naive)
{
    double result = 0;
    int patch_size = patch_radius * 2 + 1;
    for (auto& pt : centers)
    {
        result += rgb_naive_patch(src, dst, patch_radius, pt.row, pt.col, pt.row + off.row, pt.col + off.col);
    }
    return result - centers.size() * 150 * patch_size * patch_size;
}

NYMPH_PATCH_ENERGY_FUNC_TPL(rgb_naive_patch)
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


}
}
