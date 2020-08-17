#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QTcpSocket>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void socketConnected();
    void socketDisconnected();
    void socketReadyRead();

private slots:
    void on_connect_button_clicked();

    void on_send_button_clicked();

private:
    Ui::MainWindow *ui;
    bool connectedTohost;
    QTcpSocket* socket;

    void printMessage(QString message);
};

#endif // MAINWINDOW_H
