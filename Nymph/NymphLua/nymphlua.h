#ifndef NYMPHLUA_H
#define NYMPHLUA_H

#include "nymphlua_global.h"

#include <QPixmap>

class NYMPHLUASHARED_EXPORT NymphLua
{

public:
    static NymphLua* instance();
    NymphLua(int id = -1);
    ~NymphLua();

    QString run(const QString& script, const QString &name);

    QPixmap getPixmap(const QString& name);

private:
    void reset();
    void close();

    struct lua_State* L;
    int nymph_id;
};

#endif // NYMPHLUA_H
