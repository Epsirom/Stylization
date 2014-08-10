#include "lua.hpp"

#include "nymphlua.h"

#include "nymphlua_ext.h"
#include "nymphmanager.h"


NymphLua* NymphLua::instance()
{
    return new NymphLua();
    /*
    static NymphLua * _instance = 0;
    if (_instance == 0)
    {
        _instance = new NymphLua();
    }
    return _instance;
    */
}

NymphLua::NymphLua(int id)
    : L(0),
      nymph_id(id)
{
    NymphManager::instance()->initNymph(id);
}

NymphLua::~NymphLua()
{
    close();
    NymphManager::instance()->releaseNymph(nymph_id);
    nymph_id = -1;
}

QString NymphLua::run(const QString &script, const QString &name)
{
    reset();
    lua_pushstring(L, name.toLatin1().data());
    lua_setglobal(L, "___nymphlua_name");

    lua_pushinteger(L, nymph_id);
    lua_setglobal(L, "___nymphlua_id");

    bool err = luaL_loadbuffer(L, script.toLatin1().data(), script.length(), name.toLatin1().data()) || lua_pcall(L, 0, 0, 0);
    if (err)
    {
        QString errinfo(lua_tostring(L, -1));
        lua_pop(L, 1);
        return errinfo;
    }
    else
    {
        return QString();
    }
}

QString NymphLua::executeCMD(const QString &cmd, const QString &name)
{
    if (!L)
    {
        reset();
        lua_pushstring(L, name.toLatin1().data());
        lua_setglobal(L, "___nymphlua_name");

        lua_pushinteger(L, nymph_id);
        lua_setglobal(L, "___nymphlua_id");
    }
    bool err = luaL_loadbuffer(L, cmd.toLatin1().data(), cmd.length(), name.toLatin1().data()) || lua_pcall(L, 0, 0, 0);
    if (err)
    {
        QString errinfo(lua_tostring(L, -1));
        lua_pop(L, 1);
        return errinfo;
    }
    else
    {
        return QString();
    }
}

void NymphLua::reset()
{
    close();
    NymphManager::instance()->resetNymph(nymph_id);
    L = lua_open();
    luaL_openlibs(L);

    // Register NymphLua functions.
    LUA_EXT_DECLARE_FUNCTIONS;
}

void NymphLua::close()
{
    if (L)
    {
        lua_close(L);
        L = 0;
    }
}

QPixmap NymphLua::getPixmap(const QString &name)
{
    return NymphManager::instance()->getPixmap(nymph_id, name.toStdString());
}
