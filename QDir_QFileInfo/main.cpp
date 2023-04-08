
#include <QCoreApplication>
#include <QDir>
#include <QFileInfo>
#include <QFileInfoList>
#include <QTextStream>
#include <QFile>


// function to get the current folder
void test_current(){
    qInfo()<<"Current Dir:"<<QDir::currentPath();
    qInfo()<<"Application current dir:"<<QCoreApplication::applicationDirPath();
    QDir dir;
    qInfo() << dir.absolutePath();

}
// function to get the current folder contents recursively
void test_current_recursive(QString path, bool recursive=false){
    QDir dir(path);
    if(!dir.exists()){
        qWarning()<< "Path not found!";
        return;
    }
    QFileInfoList flist = dir.entryInfoList(QDir::AllEntries|QDir::NoDotAndDotDot);
    foreach(QFileInfo f, flist){
        if(f.isDir()){
            qInfo() <<"Type:Dir";
            if(recursive){
                test_current_recursive(f.absoluteFilePath());
            }
        }
        else if(f.isFile()){
            qInfo() <<"File Name:"<<f.fileName();
            qInfo() <<"File Path:"<< f.absoluteFilePath();
            qInfo() <<"File Created On:"<< f.birthTime();
            qInfo() <<"File Size:"<<f.size();
            qInfo() <<"File Modified On:"<<f.lastModified();
        }
    }
}


//basics of file handling

void basics_open_write(){
    QFile qfile("Test.txt");
    if(!qfile.open(QIODeviceBase::Append)){
        qCritical()<<"File cannot be opened in append mode!" ;
        qCritical() << qfile.errorString();
    }
    else{
        QByteArray data;
        QTextStream input(stdin);
        QTextStream output(stdout);
        output <<"Enter the text to append to the file" << Qt::endl;
        data.push_back(input.readLine().toLocal8Bit());
        qfile.write(data);
        qfile.close();
    }

}

void basics_open_read(){
    QFile qfile("Test.txt");
    if(!qfile.open(QIODeviceBase::ReadOnly)){
        qCritical()<<"File cannot be opened in append mode!" ;
        qCritical() << qfile.errorString();
    }
    else{
        QByteArray data;
        data = qfile.readLine();
        qInfo() <<"Data read from the file "<< data << Qt::endl;
        qfile.close();
    }

}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    test_current();
    test_current_recursive(QDir::currentPath());
    basics_open_write();
    basics_open_read();
    return a.exec();
}
