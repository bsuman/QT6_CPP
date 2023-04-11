
#include <QCoreApplication>
#include <QDebug>
#include <iostream>
#include <QSysInfo>
#include <QStorageInfo>
#include "hungrycat.h"

using namespace std;
/*
 * how to build a logger in QT.
 * Using a messagehandler
 * Qdebug class provides different levels of logging messages.
    * qDebug for developers
    * qInfo for users
    * qWarning to indicate warning
    * qCritical to indicate critical messages
    * qFatal for indicating non-recoverable failure)
 * The messagehandler should be installed before the logger can function and uninstalled in case of logging needs to be disabled
 * The messages output using any one of the QDebug messages types will be intercept after the message handler is installed
 * The messagehandler during installation takes the messagehandler function as an input and to uninstall takes 0 as an input argument.
 * The classes which use the QDebug class are not aware of the messagehandler and the logging happens automatically once the messagehandler is installed
*/


/*
 * OS determination
 * Compile Time
 * using the predefined macros to determine if the current OS matches the expected
 * Run Time
 * using QSysInfo

*/

/*
* use of QStorageInfo class to get information about mounted file systems and devices



*/
void myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    QByteArray localMsg = msg.toLocal8Bit();
    const char *file = context.file ? context.file : "";
    const char *function = context.function ? context.function : "";
    switch (type) {
    case QtDebugMsg:
        fprintf(stderr, "Debug: %s (%s:%u, %s)\n", localMsg.constData(), file, context.line, function);
        break;
    case QtInfoMsg:
        fprintf(stderr, "Info: %s (%s:%u, %s)\n", localMsg.constData(), file, context.line, function);
        break;
    case QtWarningMsg:
        fprintf(stderr, "Warning: %s (%s:%u, %s)\n", localMsg.constData(), file, context.line, function);
        break;
    case QtCriticalMsg:
        fprintf(stderr, "Critical: %s (%s:%u, %s)\n", localMsg.constData(), file, context.line, function);
        break;
    case QtFatalMsg:
        fprintf(stderr, "Fatal: %s (%s:%u, %s)\n", localMsg.constData(), file, context.line, function);
        break;
    }
}

void OS_check_compiletime(){

// compile time detection using predefined macros
#ifdef Q_OS_WIN64
    qInfo()<< "Windows machine detected using Q_OS_WIN64!";
    QStorageInfo sInfo = QStorageInfo::root();
    qInfo()<<"Root Display Name:" <<sInfo.displayName();
    qInfo()<<"Root Path:" <<sInfo.rootPath();
#endif

#ifdef Q_OS_WINDOWS
    qInfo()<< "Windows machine detected using Q_OS_WINDOWS !";
#endif

// disabled because the current machine is windows
#ifdef Q_OS_LINUX
    qInfo()<< "LINUX machine detected using Q_OS_LINUX !";
#endif

}

void OS_check_Runtime(){
    qInfo()<<QSysInfo::prettyProductName();

}

void test(){
    qDebug()<<"Test function:Debug call";
    qInfo()<<"Test function:Info call";
    qWarning()<<"Test function:Warning call";
    qCritical()<<"Test function:Critical call";
    qFatal()<<"Test function:Fatal call";

    HungryCat hc;
    hc.meow();
}

int main(int argc, char **argv)
{
    QCoreApplication a(argc, argv);
    QTextStream input(stdin);
    QString option;
    OS_check_Runtime();
    OS_check_compiletime();

    qInfo() <<"In the main function";
    bool isRunning(true);
    do{
        cout<<"Enter your choice of action (start,stop,test,quit)";
        option = input.readLine();
        if(option.toUpper() == "START")
            qInstallMessageHandler(myMessageOutput);
        else if(option.toUpper() == "STOP")
            qInstallMessageHandler(0);
        else if(option.toUpper() == "TEST")
            test();
        else if(option.toUpper() == "QUIT")
            isRunning = false;
    }while(isRunning);

    return a.exec();

}
