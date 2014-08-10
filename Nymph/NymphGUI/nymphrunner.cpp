#include "nymphrunner.h"


NymphRunner::NymphRunner(int id, QObject *parent) :
    QThread(parent),
    runner(id)
{

}

void NymphRunner::run()
{
    if (isExecuteCMD)
    {
        result_err = runner.executeCMD(script_buffer, script_name);
    }
    else
        result_err = runner.run(script_buffer, script_name);
}

QPixmap NymphRunner::getPixmap(const QString name)
{
    return runner.getPixmap(name);
}
