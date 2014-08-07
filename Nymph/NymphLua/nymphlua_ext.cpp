#include "lua.hpp"
#include <QtCore/QDebug>
#include "nymphlua_ext.h"
#include <QtCore/QString>
#include "nymphmanager.h"


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
    NymphManager::instance()->loadFile(std::string(filename), nymph_id, std::string(target));
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
    NymphManager::instance()->copyMat(nymph_id, std::string(new_name), std::string(old_name));
    return 0;
}

LUA_EXT_FUNC(assignmat)
{
    LUA_EXT_GET_PARAM_START(2);
    LUA_EXT_GET_STRING(new_name);
    LUA_EXT_GET_STRING(old_name);
    LUA_EXT_GET_PARAM_END;

    LUA_EXT_GET_NYMPH_ID(nymph_id);
    NymphManager::instance()->aliasMat(nymph_id, std::string(new_name), std::string(old_name));
    return 0;
}

LUA_EXT_FUNC(imgsize)
{
    LUA_EXT_GET_PARAM_START(1);
    LUA_EXT_GET_STRING(img_name);
    LUA_EXT_GET_PARAM_END;

    LUA_EXT_GET_NYMPH_ID(nymph_id);
    NymphMatSize sz = NymphManager::instance()->getSize(nymph_id, std::string(img_name));
    LUA_EXT_RETURN_START;
    LUA_EXT_RETURN_INT(sz.rows);
    LUA_EXT_RETURN_INT(sz.cols);
    LUA_EXT_RETURN_END;
}
