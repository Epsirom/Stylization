#ifndef NYMPHRUNNER_H
#define NYMPHRUNNER_H

#include <QThread>
#include "nymphlua.h"

class NymphRunner : public QThread
{
    Q_OBJECT
public:
    explicit NymphRunner(QObject *parent = 0);

    QString script_buffer;
    QString script_name;

    QString result_err;

    NymphImagePack* getImages();

protected:
    void run();

private:
    NymphLua runner;

signals:

public slots:

};

#endif // NYMPHRUNNER_H
