#include "lua.hpp"
#include <QtCore/QDebug>
#include "nymphlua_ext.h"
#include <QtCore/QString>
#include "nymphmanager.h"

int ___nymphlua_default_print_func(int id, const char* name, const char* str)
{
    qDebug() << QString("[%1 - %2] %3").arg(id).arg(name).arg(str).toLatin1().data();
    return 0;
}

void ___nymphlua_default_syncview_func(int id)
{
    qWarning() << "You should call insall_new_syncview to overload syncview in Lua. id:" << id;
}

void ___nymphlua_default_showresult_func(int id)
{
    qWarning() << "You should call insall_new_showresult to overload syncview in Lua. id:" << id;
}

nymph_print_func ___nymphlua_customed_print_func = ___nymphlua_default_print_func;
nymph_syncview_func ___nymphlua_customed_syncview_func = ___nymphlua_default_syncview_func;
nymph_showresult_func ___nymphlua_customed_showresult_func = ___nymphlua_default_showresult_func;

void install_new_print(nymph_print_func func)
{
    ___nymphlua_customed_print_func = func;
}

void install_new_syncview(nymph_syncview_func func)
{
    ___nymphlua_customed_syncview_func = func;
}

void install_new_showresult(nymph_showresult_func func)
{
    ___nymphlua_customed_showresult_func = func;
}

LUA_EXT_FUNC(cprint)
{
    const char* str = lua_tostring(l, -1);
    lua_pop(l, 1);
    LUA_EXT_GET_NYMPH_NAME(name);
    LUA_EXT_GET_NYMPH_ID(nymph_id);
    return ___nymphlua_customed_print_func(nymph_id, name, str);
}

LUA_EXT_FUNC(loadimg)
{
    const char* target = lua_tostring(l, -2);
    const char* filename = lua_tostring(l, -1);
    lua_pop(l, 2);
    LUA_EXT_GET_NYMPH_ID(nymph_id);
    NymphManager::instance()->loadFile(std::string(filename), nymph_id, std::string(target));
    return 0;
}

LUA_EXT_FUNC(syncview)
{
    LUA_EXT_GET_NYMPH_ID(nymph_id);
    ___nymphlua_customed_syncview_func(nymph_id);
    return 0;
}

LUA_EXT_FUNC(showresult)
{
    LUA_EXT_GET_NYMPH_ID(nymph_id);
    ___nymphlua_customed_showresult_func(nymph_id);
    return 0;
}

LUA_EXT_FUNC(copymat)
{
    const char* new_name = lua_tostring(l, -2);
    const char* old_name = lua_tostring(l, -1);
    lua_pop(l, 2);
    LUA_EXT_GET_NYMPH_ID(nymph_id);
    NymphManager::instance()->copyMat(nymph_id, std::string(new_name), std::string(old_name));
    return 0;
}

LUA_EXT_FUNC(assignmat)
{
    const char* new_name = lua_tostring(l, -2);
    const char* old_name = lua_tostring(l, -1);
    lua_pop(l, 2);
    LUA_EXT_GET_NYMPH_ID(nymph_id);
    NymphManager::instance()->aliasMat(nymph_id, std::string(new_name), std::string(old_name));
    return 0;
}
