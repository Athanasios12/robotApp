#include <QCoreApplication>
#include <QXmlStreamWriter>
#include <QXmlStreamReader>
#include <iostream>
#include <QFile>
#include <QTextStream>
#include <QDateTime>

using namespace std;

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QFile file("C:\\Users\\Hubertos\\Documents\\xmlTest\\log.xml");
    if (!file.open(QIODevice::ReadWrite | QIODevice::Text | QIODevice::Append))
    {
        cout << "File open error!" << endl;
    }
    QString oldContent;
    QTextStream tStream(&file);
    tStream.seek(0);
    while(!tStream.atEnd())
    {
        QString line = tStream.readLine();
        if(!line.contains("?xml"))
        {
            oldContent.append(line + "\n");
        }
    }
    file.resize(0);
    QXmlStreamWriter stream(&file);
    stream.setAutoFormatting(true);

    QDateTime timeStamp = QDateTime::currentDateTime();

    stream.writeStartDocument();
    stream.writeStartElement("Session");
    stream.writeAttribute("Date", timeStamp.toString());
    stream.writeStartElement("Positions");
    stream.writeTextElement("Position", "X = 54; Y = 12; Z = 20");
    stream.writeTextElement("Position", "X = 54; Y = 12; Z = 22");
    stream.writeTextElement("Position", "X = 54; Y = 12; Z = 24");
    stream.writeTextElement("Position", "X = 54; Y = 12; Z = 25");
    stream.writeEndElement();
    stream.writeStartElement("Alarms");
    stream.writeTextElement("Alarm", "12:45 : Error 1201 : Robot work space breach!");
    stream.writeEndElement();
    stream.writeEndElement(); // bookmark

    tStream << oldContent;
    stream.writeEndDocument();

    file.close();
    return a.exec();
}

