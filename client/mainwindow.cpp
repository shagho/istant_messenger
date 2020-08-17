#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connectedTohost = false;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::socketConnected()
{
    qDebug() << "Connected to server.";
    printMessage("<font color=\"Green\">Connected to server.</font>");

    QString name = ui->nameInput->text();
    socket->write("<font color=\"Purple\">" + name.toUtf8() + " has joined the chat room.</font>");

    ui->connect_button->setText("Discoonect");
    connectedTohost = true;
}

void MainWindow::socketDisconnected()
{
    qDebug() << "Disconnected from server.";

    printMessage("<font color=\"Red\">Disconnected from server.</font>");

    ui->connect_button->setText("Connect");
    connectedTohost = false;
}

void MainWindow::socketReadyRead()
{
    ui->chatDisplay->append(socket->readAll());
}

void MainWindow::on_connect_button_clicked()
{
    if(not connectedTohost)
    {
        socket = new QTcpSocket();

        connect(socket, SIGNAL(connected()), this,
                SLOT(socketConnected()));
        connect(socket, SIGNAL(disconnected()), this,
                SLOT(socketDisconnected()));
        connect(socket, SIGNAL(readyRead()), this,
                SLOT(socketReadyRead()));

        socket->connectToHost("127.0.0.1", 8001);
    }
    else
    {
        QString name = ui->nameInput->text();
        socket->write("<font color=\"Orange\">" + name.toUtf8() + " has left the chat room.</font>");

        socket->disconnectFromHost();
    }

}

void MainWindow::printMessage(QString message)
{
    ui->chatDisplay->append(message);
}

void MainWindow::on_send_button_clicked()
{
    QString name = ui->nameInput->text();
    QString message = ui->sendInput->text();
    socket->write("<font color=\"Blue\">" + name.toUtf8() + "</font>: "+
                  message.toUtf8());
    ui->sendInput->clear();
}
