#include "lua.hpp"
#include <QtCore/QDebug>
#include "nymphlua_ext.h"
#include <QtCore/QString>
#include "nymphmanager.h"
#include "nymphalgorithm.h"
#include <vector>


int LUA_EXT_INSTALL_FUNC_DEFAULT(print)(int id, const char* name, const char* str)
{
    qDebug() << QString("[%1 - %2] %3").arg(id).arg(name).arg(str).toLatin1().data();
    return 0;
}
LUA_EXT_INSTALL_FUNC_DEFINITION(print)

void LUA_EXT_INSTALL_FUNC_DEFAULT(syncview)(int id)
{
    qWarning() << "You should call insall_new_syncview to overload syncview in Lua. id:" << id;
}
LUA_EXT_INSTALL_FUNC_DEFINITION(syncview)

void LUA_EXT_INSTALL_FUNC_DEFAULT(showresult)(int id)
{
    qWarning() << "You should call insall_new_showresult to overload syncview in Lua. id:" << id;
}
LUA_EXT_INSTALL_FUNC_DEFINITION(showresult)


LUA_EXT_FUNC(empty)
{
    return 0;
}

LUA_EXT_FUNC(cprint)
{
    LUA_EXT_GET_PARAM_START(1);
    LUA_EXT_GET_STRING(str);
    LUA_EXT_GET_PARAM_END;

    LUA_EXT_GET_NYMPH_NAME(name);
    LUA_EXT_GET_NYMPH_ID(nymph_id);

    LUA_EXT_INSTALL_FUNC_CUSTOMED(print)(nymph_id, name, str);
    return 0;
}

LUA_EXT_FUNC(loadimage)
{
    LUA_EXT_GET_PARAM_START(2);
    LUA_EXT_GET_STRING(target);
    LUA_EXT_GET_STRING(filename);
    LUA_EXT_GET_PARAM_END;

    LUA_EXT_GET_NYMPH_ID(nymph_id);
    NymphManager::instance()->loadImage(filename, nymph_id, target);
    return 0;
}

LUA_EXT_FUNC(syncview)
{
    LUA_EXT_GET_NYMPH_ID(nymph_id);
    LUA_EXT_INSTALL_FUNC_CUSTOMED(syncview)(nymph_id);
    return 0;
}

LUA_EXT_FUNC(showresult)
{
    LUA_EXT_GET_NYMPH_ID(nymph_id);
    LUA_EXT_INSTALL_FUNC_CUSTOMED(showresult)(nymph_id);
    return 0;
}

LUA_EXT_FUNC(copymat)
{
    LUA_EXT_GET_PARAM_START(2);
    LUA_EXT_GET_STRING(new_name);
    LUA_EXT_GET_STRING(old_name);
    LUA_EXT_GET_PARAM_END;

    LUA_EXT_GET_NYMPH_ID(nymph_id);
    NymphManager::instance()->copyMat(nymph_id, new_name, old_name);
    return 0;
}

LUA_EXT_FUNC(assignmat)
{
    LUA_EXT_GET_PARAM_START(2);
    LUA_EXT_GET_STRING(new_name);
    LUA_EXT_GET_STRING(old_name);
    LUA_EXT_GET_PARAM_END;

    LUA_EXT_GET_NYMPH_ID(nymph_id);
    NymphManager::instance()->aliasMat(nymph_id, new_name, old_name);
    return 0;
}

LUA_EXT_FUNC(imgsize)
{
    LUA_EXT_GET_PARAM_START(1);
    LUA_EXT_GET_STRING(img_name);
    LUA_EXT_GET_PARAM_END;

    LUA_EXT_GET_NYMPH_ID(nymph_id);
    NymphMatSize sz = NymphManager::instance()->getSize(nymph_id, img_name);
    LUA_EXT_RETURN_START;
    LUA_EXT_RETURN_INT(sz.rows);
    LUA_EXT_RETURN_INT(sz.cols);
    LUA_EXT_RETURN_END;
}

LUA_EXT_FUNC(patchANN)
{
    LUA_EXT_GET_PARAM_START(5);
    LUA_EXT_GET_STRING(src_img);
    LUA_EXT_GET_STRING(dst_img);
    LUA_EXT_GET_INT(patch_radius);
    LUA_EXT_GET_STRING(cor_img);
    LUA_EXT_GET_INT(iterations);
    LUA_EXT_GET_PARAM_END;

    LUA_EXT_GET_NYMPH_ID(nymph_id);

    NymphManager& img = NymphManager::insobj();
    Nymph::PatchANN(img.getPatchEnergy(nymph_id), img(nymph_id, src_img), img(nymph_id, dst_img), patch_radius, img(nymph_id, cor_img), iterations);

    return 0;
}

LUA_EXT_FUNC(energy)
{
    LUA_EXT_GET_PARAM_START(6);
    LUA_EXT_GET_STRING(src_img);
    LUA_EXT_GET_STRING(dst_img);
    LUA_EXT_GET_INT(patch_radius);
    LUA_EXT_GET_INT(off_row);
    LUA_EXT_GET_INT(off_col);

    int len = lua_objlen(l, -1);
    std::vector<NymphPoint> centers(len);
    for (int i = 1; i <= len; ++i)
    {
        NymphPoint& p = centers[i - 1];
        lua_pushinteger(l, i);
        lua_gettable(l, -2);
        {
            lua_pushinteger(l, 1);
            lua_gettable(l, -2);
            p.row = lua_tointeger(l, -1);
            lua_pop(l, 1);

            lua_pushinteger(l, 2);
            lua_gettable(l, -2);
            p.col = lua_tointeger(l, -1);
            lua_pop(l, 1);
        }
        //qDebug() << "energy {" << p.row << "," << p.col << "}";
        lua_pop(l, 1);
    }

    LUA_EXT_GET_PARAM_END;

    LUA_EXT_GET_NYMPH_ID(nymph_id);

    NymphManager& img = NymphManager::insobj();
    NymphOffset off;
    off.row = off_row;
    off.col = off_col;
    double energy = Nymph::EnergyWrapper(img.getEnergy(nymph_id), img(nymph_id, src_img), img(nymph_id, dst_img), patch_radius, off, centers);

    LUA_EXT_RETURN_START;
    LUA_EXT_RETURN_DOUBLE(energy);
    LUA_EXT_RETURN_END;
}

LUA_EXT_FUNC(markpatch)
{
    LUA_EXT_GET_PARAM_START(4);
    LUA_EXT_GET_STRING(src_img);
    LUA_EXT_GET_STRING(dst_img);
    LUA_EXT_GET_INT(patch_radius);

    int len = lua_objlen(l, -1);
    std::vector<NymphPoint> centers(len);
    for (int i = 1; i <= len; ++i)
    {
        NymphPoint& p = centers[i - 1];
        lua_pushinteger(l, i);
        lua_gettable(l, -2);
        {
            lua_pushinteger(l, 1);
            lua_gettable(l, -2);
            p.row = lua_tointeger(l, -1);
            lua_pop(l, 1);

            lua_pushinteger(l, 2);
            lua_gettable(l, -2);
            p.col = lua_tointeger(l, -1);
            lua_pop(l, 1);
        }
        //qDebug() << "markpatch {" << p.row << "," << p.col << "}";
        lua_pop(l, 1);
    }

    LUA_EXT_GET_PARAM_END;

    LUA_EXT_GET_NYMPH_ID(nymph_id);

    NymphManager& img = NymphManager::insobj();
    Nymph::Test::MarkPatch(img(nymph_id, src_img), img(nymph_id, dst_img), patch_radius, centers);

    return 0;
}

LUA_EXT_FUNC(markcorpatch)
{
    LUA_EXT_GET_PARAM_START(6);
    LUA_EXT_GET_STRING(src_img);
    LUA_EXT_GET_STRING(dst_img);
    LUA_EXT_GET_INT(patch_radius);
    LUA_EXT_GET_INT(off_row);
    LUA_EXT_GET_INT(off_col);

    int len = lua_objlen(l, -1);
    std::vector<NymphPoint> centers(len);
    for (int i = 1; i <= len; ++i)
    {
        NymphPoint& p = centers[i - 1];
        lua_pushinteger(l, i);
        lua_gettable(l, -2);
        {
            lua_pushinteger(l, 1);
            lua_gettable(l, -2);
            p.row = lua_tointeger(l, -1);
            lua_pop(l, 1);

            lua_pushinteger(l, 2);
            lua_gettable(l, -2);
            p.col = lua_tointeger(l, -1);
            lua_pop(l, 1);
        }
        if (p.row == 0 && p.col == 0)
            qDebug() << i << "markcorpatch {" << p.row << "," << p.col << "}";
        lua_pop(l, 1);
    }

    LUA_EXT_GET_PARAM_END;

    LUA_EXT_GET_NYMPH_ID(nymph_id);

    NymphManager& img = NymphManager::insobj();
    NymphOffset off;
    off.row = off_row;
    off.col = off_col;
    Nymph::Test::MarkCorPatch(img(nymph_id, src_img), img(nymph_id, dst_img), patch_radius, off, centers);

    return 0;
}

LUA_EXT_FUNC(imshow)
{
    LUA_EXT_GET_PARAM_START(2);
    LUA_EXT_GET_STRING(window_name);
    LUA_EXT_GET_STRING(img_name);
    LUA_EXT_GET_PARAM_END;

    LUA_EXT_GET_NYMPH_ID(nymph_id);
    NymphManager& img = NymphManager::insobj();

    Nymph::nymph_imshow(window_name, img(nymph_id, img_name));

    return 0;
}

LUA_EXT_FUNC(saveimage)
{
    LUA_EXT_GET_PARAM_START(2);
    LUA_EXT_GET_STRING(file_name);
    LUA_EXT_GET_STRING(img_name);
    LUA_EXT_GET_PARAM_END;

    LUA_EXT_GET_NYMPH_ID(nymph_id);
    NymphManager& img = NymphManager::insobj();

    Nymph::nymph_imwrite(file_name, img(nymph_id, img_name));

    return 0;
}

LUA_EXT_FUNC(set_energy)
{
    LUA_EXT_GET_PARAM_START(2);
    LUA_EXT_GET_STRING(func_name);

    int len = lua_objlen(l, -1);
    NymphEnergyParam params(len);
    for (int i = 1; i <= len; ++i)
    {
        lua_pushinteger(l, i);
        lua_gettable(l, -2);
        params[i - 1] = lua_tonumber(l, -1);
        lua_pop(l, 1);
    }

    LUA_EXT_GET_PARAM_END;

    LUA_EXT_GET_NYMPH_ID(nymph_id);

    auto mgr = NymphManager::instance();
    int result = mgr->setEnergy(nymph_id, func_name, params);

    LUA_EXT_RETURN_START;
    LUA_EXT_RETURN_INT(result);
    LUA_EXT_RETURN_END;
}

LUA_EXT_FUNC(set_patch_energy)
{
    LUA_EXT_GET_PARAM_START(2);
    LUA_EXT_GET_STRING(func_name);

    int len = lua_objlen(l, -1);
    NymphEnergyParam params(len);
    for (int i = 1; i <= len; ++i)
    {
        lua_pushinteger(l, i);
        lua_gettable(l, -2);
        params[i - 1] = lua_tonumber(l, -1);
        lua_pop(l, 1);
    }

    LUA_EXT_GET_PARAM_END;

    LUA_EXT_GET_NYMPH_ID(nymph_id);

    auto mgr = NymphManager::instance();
    int result = mgr->setPatchEnergy(nymph_id, func_name, params);

    LUA_EXT_RETURN_START;
    LUA_EXT_RETURN_INT(result);
    LUA_EXT_RETURN_END;
}

LUA_EXT_FUNC(draw_pm_prob)
{
    LUA_EXT_GET_PARAM_START(4);
    LUA_EXT_GET_STRING(dst_name);
    LUA_EXT_GET_STRING(cor_name);
    LUA_EXT_GET_INT(patch_radius);
    LUA_EXT_GET_STRING(prob_name);
    LUA_EXT_GET_PARAM_END;

    LUA_EXT_GET_NYMPH_ID(nymph_id);
    auto& img = NymphManager::insobj();
    Nymph::Test::DrawPatchMatchCor(img(nymph_id, dst_name), img(nymph_id, cor_name), patch_radius, img(nymph_id, prob_name));

    return 0;
}

LUA_EXT_FUNC(draw_pm_result)
{
    LUA_EXT_GET_PARAM_START(4);
    LUA_EXT_GET_STRING(dst_name);
    LUA_EXT_GET_STRING(cor_name);
    LUA_EXT_GET_INT(patch_radius);
    LUA_EXT_GET_STRING(result_name);
    LUA_EXT_GET_PARAM_END;

    LUA_EXT_GET_NYMPH_ID(nymph_id);
    auto& img = NymphManager::insobj();
    Nymph::Test::DrawPatchMatchCorResult(img(nymph_id, dst_name), img(nymph_id, cor_name), patch_radius, img(nymph_id, result_name));

    return 0;
}

LUA_EXT_FUNC(get_offset)
{
    LUA_EXT_GET_PARAM_START(3);
    LUA_EXT_GET_STRING(mat_name);
    LUA_EXT_GET_INT(row);
    LUA_EXT_GET_INT(col);
    LUA_EXT_GET_PARAM_END;

    LUA_EXT_GET_NYMPH_ID(nymph_id);
    auto& img = NymphManager::insobj();
    Nymph2I npt = Nymph::nymph_mat_at_2i(img(nymph_id, mat_name), row, col);

    LUA_EXT_RETURN_START;
    LUA_EXT_RETURN_INT(npt.row - row);
    LUA_EXT_RETURN_INT(npt.col - col);
    LUA_EXT_RETURN_END;
}

LUA_EXT_FUNC(mat_2i)
{
    LUA_EXT_GET_PARAM_START(3);
    LUA_EXT_GET_STRING(mat_name);
    LUA_EXT_GET_INT(row);
    LUA_EXT_GET_INT(col);
    LUA_EXT_GET_PARAM_END;

    LUA_EXT_GET_NYMPH_ID(nymph_id);
    auto& img = NymphManager::insobj();
    Nymph2I npt = Nymph::nymph_mat_at_2i(img(nymph_id, mat_name), row, col);

    LUA_EXT_RETURN_START;
    LUA_EXT_RETURN_INT(npt.row);
    LUA_EXT_RETURN_INT(npt.col);
    LUA_EXT_RETURN_END;
}

LUA_EXT_FUNC(mat_3b)
{
    LUA_EXT_GET_PARAM_START(3);
    LUA_EXT_GET_STRING(mat_name);
    LUA_EXT_GET_INT(row);
    LUA_EXT_GET_INT(col);
    LUA_EXT_GET_PARAM_END;

    LUA_EXT_GET_NYMPH_ID(nymph_id);
    auto& img = NymphManager::insobj();
    Nymph3B npt = Nymph::nymph_mat_at_3b(img(nymph_id, mat_name), row, col);

    LUA_EXT_RETURN_START;
    LUA_EXT_RETURN_INT(npt.c1);
    LUA_EXT_RETURN_INT(npt.c2);
    LUA_EXT_RETURN_INT(npt.c3);
    LUA_EXT_RETURN_END;
}

LUA_EXT_FUNC(loadmat)
{
    LUA_EXT_GET_PARAM_START(2);
    LUA_EXT_GET_STRING(file_name);
    LUA_EXT_GET_STRING(mat_name);
    LUA_EXT_GET_PARAM_END;

    LUA_EXT_GET_NYMPH_ID(nymph_id);

    NymphManager::instance()->loadMat(file_name, nymph_id, mat_name);

    return 0;
}

LUA_EXT_FUNC(savemat)
{
    LUA_EXT_GET_PARAM_START(2);
    LUA_EXT_GET_STRING(file_name);
    LUA_EXT_GET_STRING(mat_name);
    LUA_EXT_GET_PARAM_END;

    LUA_EXT_GET_NYMPH_ID(nymph_id);

    NymphManager::instance()->saveMat(file_name, nymph_id, mat_name);

    return 0;
}


void imhide(std::string window_name)
{
    Nymph::nymph_imhide(window_name);
}
