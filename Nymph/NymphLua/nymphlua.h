#ifndef NYMPHLUA_H
#define NYMPHLUA_H

#include "nymphlua_global.h"
#include "nymphimagemanager.h"

#include <QStringList>

class NYMPHLUASHARED_EXPORT NymphLua
{

public:
    static NymphLua* instance();
    NymphLua();
    ~NymphLua();

    QString run(const QString& script, const QString &name);

    NymphImagePack* getImagePack();

private:
    void reset();
    void close();

    struct lua_State* L;
    int image_id;
};

#endif // NYMPHLUA_H
