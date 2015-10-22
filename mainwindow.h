#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "SerialCommHandler.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
private:
    void appendDialogWindow(const QString &text);
    void setCbOptions();
    bool startSerialComm();

private slots:
    void on_pbSendData_clicked();

    void on_pbStartDataRead_clicked();

    void on_StopDataRead_clicked();

    void on_cbPortComm_currentIndexChanged(const QString &port);

private:
    Ui::MainWindow *ui;
    SerialCommHandler spiHandler;
};

#endif // MAINWINDOW_H
