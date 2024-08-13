#include "login.h"
#include "cards.h"
#include <QApplication>
#include <QFile>
#include <QResource>
#include "aescrypto.h"
#include "rsacrypto.h"
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    qRegisterMetaType<Cards>("Cards&");
    qRegisterMetaType<Cards>("Cards");
    //QResource::registerResource("./resource.rcc");
    // 加载资源文件 - 文件必须是utf8编码
    QFile file(":/conf/style.qss");
    file.open(QFile::ReadOnly);
    QByteArray all = file.readAll();
    a.setStyleSheet(all);
    file.close();

    Login w;
    int ret = w.exec();
    if(ret == QDialog::Accepted)
    {
        // test();
        return a.exec();
    }
}
