#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QDebug>
#include <vector>

class server : public QObject
{
    Q_OBJECT
public:
    explicit server(QObject *parent = nullptr);
    void startServer();
    void sendMessageToClients(QString message);

private:
    QTcpServer *chatserver;
    std::vector<QTcpSocket*> *clients;

signals:

public slots:
    void newClientConnection();
    void socketDissconnected();
    void socketReadyRead();
    void socketStateChange(QAbstractSocket::SocketState state);
};

#endif // SERVER_H
