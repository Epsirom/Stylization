#include "nymphrunner.h"


NymphRunner::NymphRunner(int id, QObject *parent) :
    QThread(parent),
    runner(id)
{

}

void NymphRunner::run()
{
    result_err = runner.run(script_buffer, script_name);
}

QPixmap NymphRunner::getPixmap(const QString name)
{
    return runner.getPixmap(name);
}
