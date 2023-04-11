
#include <QCoreApplication>
#include <QBuffer>
#include <QTextStream>
#include <QFile>
/*
* compression and decompression of data/files/etc.
* compression formats:
* QuaZip: wrapper between Qt and CPP for zipping and unzipping (zip and 7z file formats supported by 3rd Party lib)
* GZ format is builtin into Qt.
*/

// custom file

bool makeFileForCompression(QString path){
    QFile file(path);
    if(!file.open(QIODevice::WriteOnly))
    {
        qWarning()<<"File cannot be opened in write mode!!";
        return false;
    }
    QByteArray byteData("A company’s greatest potential and most important factor for success and innovation is its people. At Siemens we look for diverse talent from all walks of life with an open, self-reflective mindset - people, who have the desire to continue learning and growing. As part of our company growth culture, we believe in building on the unique strengths of our existing and future people. A positive focus on strengths allows us to all be authentic, energized and motivated in life. And this is what our gift is all about.A company’s greatest potential and most important factor for success and innovation is its people. At Siemens we look for diverse talent from all walks of life with an open, self-reflective mindset - people, who have the desire to continue learning and growing. As part of our company growth culture, we believe in building on the unique strengths of our existing and future people. A positive focus on strengths allows us to all be authentic, energized and motivated in life. And this is what our gift is all about.A company’s greatest potential and most important factor for success and innovation is its people. At Siemens we look for diverse talent from all walks of life with an open, self-reflective mindset - people, who have the desire to continue learning and growing. As part of our company growth culture, we believe in building on the unique strengths of our existing and future people. A positive focus on strengths allows us to all be authentic, energized and motivated in life. And this is what our gift is all about.A company’s greatest potential and most important factor for success and innovation is its people. At Siemens we look for diverse talent from all walks of life with an open, self-reflective mindset - people, who have the desire to continue learning and growing. As part of our company growth culture, we believe in building on the unique strengths of our existing and future people. A positive focus on strengths allows us to all be authentic, energized and motivated in life. And this is what our gift is all about.A company’s greatest potential and most important factor for success and innovation is its people. At Siemens we look for diverse talent from all walks of life with an open, self-reflective mindset - people, who have the desire to continue learning and growing. As part of our company growth culture, we believe in building on the unique strengths of our existing and future people. A positive focus on strengths allows us to all be authentic, energized and motivated in life. And this is what our gift is all about.");
    file.write(byteData);
    file.close();
    return true;
}

QByteArray getHeader(){
    QByteArray header;
    header.append("==========xxxxxxxxxxxxx===========");
    return header;
}

bool CompressFile(QString opath,QString npath){
    QFile file(opath);
    QFile nfile(npath);
    if(!file.open(QIODevice::ReadOnly) || !nfile.open(QIODevice::WriteOnly)){
        qWarning()<<"File cannot be opened in required mode!!";
        return false;
    }
    int size= 1024;
    QByteArray header = getHeader();
    while(!file.atEnd()){
        QByteArray buf = file.read(size);
        QByteArray compress = qCompress(buf);
        nfile.write(header);
        nfile.write(compress);
    }
    file.close();
    nfile.close();
    return true;
}


bool DeCompressFile(QString opath,QString npath){
    QFile file(opath);
    QFile nfile(npath);
    if(!file.open(QIODevice::ReadOnly) || !nfile.open(QIODevice::WriteOnly)){
        qWarning()<<"File cannot be opened in required mode!!";
        return false;
    }
    int size= 1024;
    QByteArray buf= file.peek(size);
    QByteArray header = getHeader();
    if(!buf.startsWith(header)){
        qWarning()<<"File not created with the expected compression method!!";
        file.close();
        nfile.close();
        return false;
    }
    file.seek(header.length());
    while(!file.atEnd()){
        buf = file.peek(size);
        qint64 index= buf.indexOf(header);
        qInfo() <<"Header at index" << index;
        if(index > -1)
        {
            qint64 mBytes = index ;
            qInfo() <<"Reading:" << mBytes;
            buf = file.read(mBytes);
            file.read(header.length());
        }
        else{
            qInfo() <<"No Header!";
            buf =  file.readAll();
        }
        QByteArray decompress = qUncompress(buf);
        nfile.write(decompress);
        nfile.flush();
    }

    file.close();
    nfile.close();
    return true;
}


void testFunc(){
    QString originalFile = "original.txt";
    QString clFile = "compressed.txt";
    QString deFile = "decompress.txt";
    if(makeFileForCompression(originalFile)){
        qInfo()<< "Original Created!! Now calling compression function ...";
        if(CompressFile(originalFile,clFile)){
            qInfo()<< "Compressed File Created!! Now calling decompression function ...";
            if(DeCompressFile(clFile,deFile)){
                qInfo()<< "Decompressed File Created!";
            }
            else
                qWarning()<< "Decompressed Failed!";
        }
        else
             qWarning()<< "Compressed Failed!";
    }
    else
        qWarning()<< "File Creation Failed!";
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QByteArray byteData("A company’s greatest potential and most important factor for success and innovation is its people. At Siemens we look for diverse talent from all walks of life with an open, self-reflective mindset - people, who have the desire to continue learning and growing. As part of our company growth culture, we believe in building on the unique strengths of our existing and future people. A positive focus on strengths allows us to all be authentic, energized and motivated in life. And this is what our gift is all about.");
    qInfo()<<"Before Compression file size" <<byteData.size();
    QByteArray compress = qCompress(byteData,9);
    qInfo()<<"Compressed file size" << compress.size();
    QByteArray decompress = qUncompress(compress);
    qInfo()<<"After Decompression file size" << decompress.size();
    testFunc();
    return a.exec();
}
