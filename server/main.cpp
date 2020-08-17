#include <QCoreApplication>
#include <server.h>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    server* mserver = new server();

    mserver->startServer();
    return a.exec();
}
