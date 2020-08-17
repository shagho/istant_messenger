#include "server.h"

server::server(QObject *parent) : QObject(parent)
{

}

void server::startServer()
{
    clients = new std::vector<QTcpSocket*>;

    chatserver = new QTcpServer();
    chatserver->setMaxPendingConnections(10);
    connect(chatserver, SIGNAL(newConnection()), this,
            SLOT(newClientConnection()));

    if(chatserver->listen(QHostAddress::Any, 8001))
    {
        qDebug() << "server has  started. Listening to port 8001";
    }

    else
    {
        qDebug() << "Server failed to start. Error: " + chatserver->errorString();
    }

}

void server::sendMessageToClients(QString message)
{
    if(clients->size() > 0)
    {
        for(int i = 0; i < clients->size(); i++)
        {
            if(clients->at(i)->isOpen() and clients->at(i)->isWritable())
                clients->at(i)->write(message.toUtf8());
        }
    }
}

void server::newClientConnection()
{
    QTcpSocket* client = chatserver->nextPendingConnection();
    QString ipAddress = client->peerAddress().toString();
    int port = client->peerPort();

    connect(client, &QTcpSocket::disconnected, this,
            &server::socketDissconnected);
    connect(client, &QTcpSocket::readyRead, this,
             &server::socketReadyRead);
    connect(client, &QTcpSocket::stateChanged, this,
            &server::socketStateChange);
    clients->push_back(client);

    qDebug() << "Socket connected from " + ipAddress + ":" +
                QString::number(port);
}

void server::socketDissconnected()
{
    QTcpSocket* client = qobject_cast<QTcpSocket*>(QObject::sender());
    QString socketIpAddress = client->peerAddress().toString();
    int port = client->peerPort();

    qDebug() << "Socket desconnected from " + socketIpAddress + ":"
                + QString::number(port);
}

void server::socketReadyRead()
{
    QTcpSocket* client = qobject_cast<QTcpSocket*>(QObject::sender());
    QString socketIpAddress = client->peerAddress().toString();
    int port = client->peerPort();

    QString data = QString(client->readAll());

    qDebug() << "Message: " + data + " (" + socketIpAddress + ":" +
                QString::number(port) + ")";
    sendMessageToClients(data);
}

void server::socketStateChange(QAbstractSocket::SocketState state)
{
    QTcpSocket* client = qobject_cast<QTcpSocket*>(QObject::sender());
    QString socketIpaddress = client->peerAddress().toString();
    int port = client->peerPort();

    QString desc;

    if(state == QAbstractSocket::UnconnectedState)
        desc = "socket is not connected.";
    else if (state == QAbstractSocket::HostLookupState)
        desc = "The socket is perfirming a host name lookup.";
    else if (state == QAbstractSocket::ConnectingState)
        desc = "The socket has started establishing a connection.";
    else if(state == QAbstractSocket::ConnectedState)
        desc = "A connection is stablished";
    else if(state == QAbstractSocket::BoundState)
        desc = "The socket is bound to an address and port.";
    else if(state == QAbstractSocket::ClosingState)
        desc = "The socket is about to close (data may still be waiiting to be written";
    else if(state == QAbstractSocket::ListeningState)
        desc = "For initial use only";

    qDebug() << "socket state changed (" + socketIpaddress + ":" +
                QString::number(port) + "):" + desc;


}
