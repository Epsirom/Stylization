#ifndef NYMPHLUA_EXT_H
#define NYMPHLUA_EXT_H

#include "nymphlua_global.h"

#define LUA_EXT_FUNC_PARAM struct lua_State* l
#define LUA_EXT_FUNC(f) int luaext_##f(LUA_EXT_FUNC_PARAM)

#define LUA_EXT_GET_NYMPH_NAME(param) \
    lua_getglobal(l, "___nymphlua_name"); \
    const char* param = lua_tostring(l, -1); \
    lua_pop(l, 1)

#define LUA_EXT_GET_NYMPH_ID(param) \
    lua_getglobal(l, "___nymphlua_id"); \
    const int param = lua_tointeger(l, -1); \
    lua_pop(l, 1)

typedef int (*nymph_print_func) (int id, const char* name, const char* str);
typedef void (*nymph_syncview_func) (int id);
typedef nymph_syncview_func nymph_showresult_func;

void NYMPHLUASHARED_EXPORT install_new_print(nymph_print_func func);
void NYMPHLUASHARED_EXPORT install_new_syncview(nymph_syncview_func func);
void NYMPHLUASHARED_EXPORT install_new_showresult(nymph_showresult_func func);

LUA_EXT_FUNC(cprint);
LUA_EXT_FUNC(loadimg);
LUA_EXT_FUNC(syncview);
LUA_EXT_FUNC(showresult);
LUA_EXT_FUNC(copymat);
LUA_EXT_FUNC(assignmat);

#endif // NYMPHLUA_EXT_H
