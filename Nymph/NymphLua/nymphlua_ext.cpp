#include "lua.hpp"
#include <QtCore/QDebug>
#include "nymphlua_ext.h"
#include "nymphimagemanager.h"
#include <QtCore/QString>
#include "nymphcore_export.h"

int ___nymphlua_default_print_func(const char* name, const char* str)
{
    qDebug() << QString("[%1] %2").arg(name).arg(str).toLatin1().data();
    return 0;
}

void ___nymphlua_default_syncview_func()
{
    qWarning() << "You should call insall_new_syncview to overload syncview in Lua.";
}

nymph_print_func ___nymphlua_customed_print_func = ___nymphlua_default_print_func;
nymph_syncview_func ___nymphlua_customed_syncview_func = ___nymphlua_default_syncview_func;

void install_new_print(nymph_print_func func)
{
    ___nymphlua_customed_print_func = func;
}

void install_new_syncview(nymph_syncview_func func)
{
    ___nymphlua_customed_syncview_func = func;
}

LUA_EXT_FUNC(cprint)
{
    const char* str = lua_tostring(l, -1);
    lua_pop(l, 1);
    lua_getglobal(l, "___nymphlua_name");
    const char* name = lua_tostring(l, -1);
    lua_pop(l, 1);
    ___nymphlua_customed_print_func(name, str);
    return 0;
}

LUA_EXT_FUNC(loadimg)
{
    const char* target = lua_tostring(l, -2);
    const char* filename = lua_tostring(l, -1);
    lua_pop(l, 2);
    lua_getglobal(l, "___nymphlua_image_id");
    const int image_id = lua_tointeger(l, -1);
    lua_getglobal(l, "___nymphlua_name");
    const char* name = lua_tostring(l, -1);
    lua_pop(l, 2);
    NymphImagePack* pack = NymphImageManager::instance()->getPack(image_id);
    QString tstr(target);
    if (tstr == "style_in") {
        pack->style_in = Nymph::loadImage(filename);
    } else if (tstr == "style_out") {
        pack->style_out = Nymph::loadImage(filename);
    } else if (tstr == "nymph_in") {
        pack->nymph_in = Nymph::loadImage(filename);
    } else if (tstr == "nymph_out") {
        ___nymphlua_customed_print_func(name, "[warning]nymph_out should not be loaded.");
    } else {
        ___nymphlua_customed_print_func(name, qPrintable(QString("[warning]Unknown target: %1").arg(tstr)));
    }
    return 0;
}

LUA_EXT_FUNC(syncview)
{
    ___nymphlua_customed_syncview_func();
    return 0;
}
