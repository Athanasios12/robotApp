#ifndef SERIALTHREAD_H
#define SERIALTHREAD_H
#include <QThread>
#include "serialcommhandler.h"

class SerialThread : public QThread
{
    Q_OBJECT
public:
    explicit SerialThread(QObject *parent = 0, SerialCommHandler *serial = 0);
    void run();
signals:
    void receivedData(QByteArray);
public slots:
    void sendData(const QByteArray &data);
private:
    SerialCommHandler *serialPtr;
    const QByteArray *dataPtr;
    bool dataSend;
public:
    bool Stop;
};

#endif // SERIALTHREAD_H
