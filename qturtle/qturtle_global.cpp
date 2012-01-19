#include "qturtle_global.hpp"
#include "turtleModuleLib/GlobalFunctions.hpp"

#include <QDebug>
#include <QDateTime>
#include <QThread>

#include <fstream>

static std::ofstream* logFile = nullptr;

void debugOut(QtMsgType /*type*/, const char *msg)
{
    *logFile << QDateTime::currentDateTime().toString().toStdString() << " [" << (int)QThread::currentThreadId() << "] " << msg << std::endl;
    logFile->flush();
}

extern "C"
{

PyMODINIT_FUNC
PyInit_qturtle()
{
    logFile = new std::ofstream("C:\\users\\julia\\qturtle.log", std::ios_base::out | std::ios_base::trunc);
    *logFile << QDateTime::currentDateTime().toString().toStdString() <<" Hello" << std::endl;
    logFile->flush();

    qInstallMsgHandler(debugOut);

    return TurtleModule::creteModule();
}

/*
PyMODINIT_FUNC initqturtle()
{
    return PyInit_qturtle();
}
*/


}
