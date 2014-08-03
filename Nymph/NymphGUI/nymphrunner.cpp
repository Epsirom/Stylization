#include "nymphrunner.h"


NymphRunner::NymphRunner(QObject *parent) :
    QThread(parent),
    runner()
{

}

void NymphRunner::run()
{
    result_err = runner.run(script_buffer, script_name);
}

NymphImagePack* NymphRunner::getImages()
{
    return runner.getImagePack();
}
