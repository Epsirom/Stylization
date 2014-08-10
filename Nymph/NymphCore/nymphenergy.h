#ifndef NYMPHENERGY_H
#define NYMPHENERGY_H

#include "nymphalgorithm.h"

#define NYMPH_ENERGY_FUNC_TPL(name) double name(const cv::Mat& src, const cv::Mat& dst, int patch_radius, NymphOffset off, std::vector<NymphPoint> &centers)
#define NYMPH_PATCH_ENERGY_FUNC_TPL(name) double name(const cv::Mat& src, const cv::Mat& dst, int patch_radius, int center_src_row, int center_src_col, int center_dst_row, int center_dst_col)

namespace Nymph
{
    namespace Energy
    {
        NymphEnergyFunc get_energy_func(const std::string& func_name);
        NymphPatchEnergyFunc get_patch_energy_func(const std::string& func_name);

        NYMPH_ENERGY_FUNC_TPL(rgb_naive);
        NYMPH_PATCH_ENERGY_FUNC_TPL(rgb_naive_patch);
    }
}

#define NYMPH_ENERGY_FUNC_REG_TPL(func) energy_func_map[#func] = func
#define NYMPH_PATCH_ENERGY_FUNC_REG_TPL(func) energy_func_map[#func] = func##_patch

#define NYMPH_ENERGY_FUNC_REGISTER \
    NYMPH_ENERGY_FUNC_REG_TPL(rgb_naive); \


#define NYMPH_PATCH_ENERGY_FUNC_REGISTER \
    NYMPH_PATCH_ENERGY_FUNC_REG_TPL(rgb_naive); \


#endif // NYMPHENERGY_H
