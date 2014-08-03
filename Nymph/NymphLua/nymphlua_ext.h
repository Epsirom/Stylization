#ifndef NYMPHLUA_EXT_H
#define NYMPHLUA_EXT_H

#include "nymphlua_global.h"

#define LUA_EXT_FUNC_PARAM struct lua_State* l
#define LUA_EXT_FUNC(f) int luaext_##f(LUA_EXT_FUNC_PARAM)

typedef int (*nymph_print_func) (const char* name, const char* str);
typedef void (*nymph_syncview_func) ();

void NYMPHLUASHARED_EXPORT install_new_print(nymph_print_func func);
void NYMPHLUASHARED_EXPORT install_new_syncview(nymph_syncview_func func);

LUA_EXT_FUNC(cprint);
LUA_EXT_FUNC(loadimg);
LUA_EXT_FUNC(syncview);

#endif // NYMPHLUA_EXT_H
