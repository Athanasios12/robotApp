#include <QCoreApplication>
#include <QXmlStreamWriter>
#include <QXmlStreamReader>
#include <iostream>
#include <QFile>
#include <QTextStream>
#include <QDateTime>
#include <QDir>
#include <QRegularExpression>
#include <QRegularExpressionMatch>

using namespace std;

struct XmlTag
{
    QString tagName;
    QString tagCharacters;
    QXmlStreamAttributes attribiutes;
};

bool findLineNumber(quint64 num, const QVector<quint64> &lineNumbers)
{
    foreach(quint64 lineNumber, lineNumbers)
    {
        if(lineNumber == num)
        {
            return true;
        }
    }
    return false;
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QFile file("log.xml");
    QVector<XmlTag> tags;
    QVector<quint64> endElementsNumbers;

    if (!file.open(QIODevice::ReadWrite | QIODevice::Text))
    {
        cout << "File open error!" << endl;
    }

    file.seek(0);
    QXmlStreamReader xmlReader(&file);
    XmlTag currentTag;
    bool newTag = false;
    bool isTextElement = false;

    while (!xmlReader.atEnd())
    {
        xmlReader.readNext();
        cout << xmlReader.tokenString().toStdString() << endl;
        if(xmlReader.isStartElement())
        {
            cout << xmlReader.name().toString().toStdString() << endl;
            currentTag.tagName = xmlReader.name().toString();
            if(!xmlReader.attributes().empty())
            {
                currentTag.attribiutes = xmlReader.attributes();
            }
            else
            {
                currentTag.attribiutes = QXmlStreamAttributes();
            }
            newTag = true;
            //cout << xmlReader.text().toString().toStdString() << endl;
        }
        if(xmlReader.isCharacters() && newTag)
        {
           cout << xmlReader.text().toString().toStdString() << endl;
           QString tagCharacters = xmlReader.text().toString();
           QRegularExpression letters("\\l");
           QRegularExpression digits("\\d");
           QRegularExpressionMatch hasLetters = letters.match(tagCharacters);
           QRegularExpressionMatch hasDigits = digits.match(tagCharacters);
           if(hasLetters.hasMatch() || hasDigits.hasMatch())
           {
              currentTag.tagCharacters = tagCharacters;
              isTextElement = true;
           }
           else
           {
              currentTag.tagCharacters = QString();
           }

           tags.push_back(currentTag);
           newTag = false;
        }
        if(xmlReader.isEndElement() && !isTextElement)
        {
            endElementsNumbers.push_back(xmlReader.lineNumber());
        }
        if(xmlReader.isEndElement())
        {
           isTextElement = false;
        }
            //cout << xmlReader.text().toString().toStdString() << endl;
    }
    cout << "\n\nVector Content: " << endl;
    foreach (XmlTag tag, tags)
    {
        cout << tag.tagName.toStdString() << endl;
        if(!tag.tagCharacters.isEmpty())
        {
            cout << tag.tagCharacters.toStdString() << endl;
        }
        if(!tag.attribiutes.empty())
        {
            foreach(QXmlStreamAttribute attr, tag.attribiutes)
            {
                cout << attr.name().toString().toStdString() << "=" << attr.value().toString().toStdString() << endl;
            }
        }
    }
    file.resize(0);
    file.seek(0);
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
    stream.writeEndElement(); //</sessions>
    if(!tags.empty())
    {
        quint64 lineNumber = 2;
        quint32 i = 0;
        while(lineNumber <= endElementsNumbers.last())
        {
            if(findLineNumber(lineNumber, endElementsNumbers))
            {
                stream.writeEndElement();
            }
            else
            {
                if(!tags[i].tagCharacters.isEmpty())
                {
                    stream.writeTextElement(tags[i].tagName, tags[i].tagCharacters);
                }
                else
                {
                   stream.writeStartElement(tags[i].tagName);
                }
                if(!tags[i].attribiutes.empty())
                {
                    foreach(QXmlStreamAttribute attr, tags[i].attribiutes)
                    {
                        stream.writeAttribute(attr.name().toString(), attr.value().toString());
                    }
                }
                i++;
            }
            lineNumber++;
        }
    }
    stream.writeEndDocument();

    file.close();
    return a.exec();
}

