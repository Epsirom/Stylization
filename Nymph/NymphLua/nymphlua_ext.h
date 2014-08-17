#ifndef NYMPHLUA_EXT_H
#define NYMPHLUA_EXT_H

#include "nymphlua_global.h"
#include <cstring>

#define LUA_EXT_FUNC_PARAM      struct lua_State* l
#define LUA_EXT_FUNC(f)         int luaext_##f(LUA_EXT_FUNC_PARAM)
#define LUA_EXT_PUSH_FUNC(f)    lua_pushcfunction(L, luaext_##f); \
                                lua_setglobal(L, #f)

#define LUA_EXT_INSTALL_FUNC_TYPE(name)     nymph_##name##_func
#define LUA_EXT_INSTALL_FUNC_DEFAULT(name)  ___nymphlua_default_##name##_func
#define LUA_EXT_INSTALL_FUNC_CUSTOMED(name) ___nymphlua_customed_##name##_func
#define LUA_EXT_INSTALL_FUNC(f)             void NYMPHLUASHARED_EXPORT install_new_##f(LUA_EXT_INSTALL_FUNC_TYPE(f) func)
#define LUA_EXT_INSTALL_FUNC_DEFINITION(f)  LUA_EXT_INSTALL_FUNC_TYPE(f) LUA_EXT_INSTALL_FUNC_CUSTOMED(f) = LUA_EXT_INSTALL_FUNC_DEFAULT(f); \
                                            LUA_EXT_INSTALL_FUNC(f){LUA_EXT_INSTALL_FUNC_CUSTOMED(f) = func;}

#define LUA_EXT_GET_PARAM_START(param_count)    int ___nymph_lua_param_id = -param_count; \
                                                const int ___nymph_lua_param_count = param_count
#define LUA_EXT_GET_PARAM_END                   lua_pop(l, ___nymph_lua_param_count)
#define LUA_EXT_GET_STRING(param)               const char* param = lua_tostring(l, ___nymph_lua_param_id++)
#define LUA_EXT_GET_INT(param)                  int param = lua_tointeger(l, ___nymph_lua_param_id++)
#define LUA_EXT_GET_DOUBLE(param)               double param = lua_tonumber(l, ___nymph_lua_param_id++)

#define LUA_EXT_RETURN_START        int ___nymph_lua_rtn_param_count = 0
#define LUA_EXT_RETURN_END          return ___nymph_lua_rtn_param_count
#define LUA_EXT_RETURN_INT(val)     ++___nymph_lua_rtn_param_count;lua_pushinteger(l, val)
#define LUA_EXT_RETURN_STRING(val)  ++___nymph_lua_rtn_param_count;lua_pushstring(l, val)
#define LUA_EXT_RETURN_DOUBLE(val)  ++___nymph_lua_rtn_param_count;lua_pushnumber(l, val)


#define LUA_EXT_GET_NYMPH_NAME(param) \
    lua_getglobal(l, "___nymphlua_name"); \
    const char* param = lua_tostring(l, -1); \
    lua_pop(l, 1)

#define LUA_EXT_GET_NYMPH_ID(param) \
    lua_getglobal(l, "___nymphlua_id"); \
    int param = lua_tointeger(l, -1); \
    lua_pop(l, 1)

typedef int (*LUA_EXT_INSTALL_FUNC_TYPE(print)) (int id, const char* name, const char* str);
typedef void (*LUA_EXT_INSTALL_FUNC_TYPE(syncview)) (int id);
typedef LUA_EXT_INSTALL_FUNC_TYPE(syncview) LUA_EXT_INSTALL_FUNC_TYPE(showresult);

LUA_EXT_INSTALL_FUNC(print);
LUA_EXT_INSTALL_FUNC(syncview);
LUA_EXT_INSTALL_FUNC(showresult);

LUA_EXT_FUNC(empty);
LUA_EXT_FUNC(cprint);
LUA_EXT_FUNC(loadimage);
LUA_EXT_FUNC(syncview);
LUA_EXT_FUNC(showresult);
LUA_EXT_FUNC(copymat);
LUA_EXT_FUNC(assignmat);
LUA_EXT_FUNC(imgsize);
LUA_EXT_FUNC(patchANN);
LUA_EXT_FUNC(energy);
LUA_EXT_FUNC(markpatch);
LUA_EXT_FUNC(markcorpatch);
LUA_EXT_FUNC(imshow);
LUA_EXT_FUNC(saveimage);
LUA_EXT_FUNC(set_energy);
LUA_EXT_FUNC(set_patch_energy);
LUA_EXT_FUNC(draw_pm_prob);     // draw patch match probability map
LUA_EXT_FUNC(draw_pm_result);   // draw patch match result
LUA_EXT_FUNC(get_offset);
LUA_EXT_FUNC(mat_2i);
LUA_EXT_FUNC(mat_3b);
LUA_EXT_FUNC(loadmat);
LUA_EXT_FUNC(savemat);

void imhide(std::string window_name = std::string());

#define LUA_EXT_DECLARE_FUNCTIONS \
    LUA_EXT_PUSH_FUNC(empty); \
    LUA_EXT_PUSH_FUNC(cprint); \
    LUA_EXT_PUSH_FUNC(loadimage); \
    LUA_EXT_PUSH_FUNC(syncview); \
    LUA_EXT_PUSH_FUNC(showresult); \
    LUA_EXT_PUSH_FUNC(copymat); \
    LUA_EXT_PUSH_FUNC(assignmat); \
    LUA_EXT_PUSH_FUNC(imgsize); \
    LUA_EXT_PUSH_FUNC(patchANN); \
    LUA_EXT_PUSH_FUNC(energy); \
    LUA_EXT_PUSH_FUNC(markpatch); \
    LUA_EXT_PUSH_FUNC(markcorpatch); \
    LUA_EXT_PUSH_FUNC(imshow); \
    LUA_EXT_PUSH_FUNC(saveimage); \
    LUA_EXT_PUSH_FUNC(set_energy); \
    LUA_EXT_PUSH_FUNC(set_patch_energy); \
    LUA_EXT_PUSH_FUNC(draw_pm_prob); \
    LUA_EXT_PUSH_FUNC(draw_pm_result); \
    LUA_EXT_PUSH_FUNC(get_offset); \
    LUA_EXT_PUSH_FUNC(mat_2i); \
    LUA_EXT_PUSH_FUNC(mat_3b); \
    LUA_EXT_PUSH_FUNC(loadmat); \
    LUA_EXT_PUSH_FUNC(savemat); \



#endif // NYMPHLUA_EXT_H
