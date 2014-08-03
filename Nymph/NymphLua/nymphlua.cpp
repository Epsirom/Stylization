#include "lua.hpp"

#include "nymphlua.h"

#include "nymphlua_ext.h"


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

NymphLua::NymphLua()
    : L(0),
      image_id(-1)
{

}

NymphLua::~NymphLua()
{
    close();
}

QString NymphLua::run(const QString &script, const QString &name)
{
    reset();
    lua_pushstring(L, name.toLatin1().data());
    lua_setglobal(L, "___nymphlua_name");

    image_id = NymphImageManager::instance()->applySpace();
    lua_pushinteger(L, image_id);
    lua_setglobal(L, "___nymphlua_image_id");

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

void NymphLua::reset()
{
    close();
    L = lua_open();
    luaL_openlibs(L);

    // Register NymphLua functions.
    lua_pushcfunction(L, luaext_cprint);
    lua_setglobal(L, "cprint");
    lua_pushcfunction(L, luaext_loadimg);
    lua_setglobal(L, "loadimage");
    lua_pushcfunction(L, luaext_syncview);
    lua_setglobal(L, "syncview");
}

void NymphLua::close()
{
    if (L)
    {
        NymphImageManager::instance()->releasePack(image_id);
        lua_close(L);
        image_id = -1;
        L = 0;
    }
}

NymphImagePack* NymphLua::getImagePack()
{
    return NymphImageManager::instance()->getPack(image_id);
}
