#ifndef NYMPHRUNNER_H
#define NYMPHRUNNER_H

#include <QThread>
#include "nymphlua.h"

class NymphRunner : public QThread
{
    Q_OBJECT
public:
    explicit NymphRunner(int id = -1, QObject *parent = 0);

    QString script_buffer;
    QString script_name;

    bool isExecuteCMD;

    QString result_err;

    QPixmap getPixmap(const QString name);

protected:
    void run();

private:
    NymphLua runner;

signals:

public slots:

};

#endif // NYMPHRUNNER_H
