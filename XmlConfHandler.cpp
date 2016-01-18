#include "XmlConfHandler.h"
#include <QXmlStreamReader>
#include <QRegularExpression>
#include <QRegularExpressionMatch>
#include <QDateTime>
#include <QPair>

//tags
const QString SESSION_TAG = "Session";
const QString POSITIONS_TAG = "Positions";
const QString POSITION_TAG = "Position";

//attribiutes
const QString DATE_ATTR = "Date";
const QString SESSION_ID_ATTR = "Session_ID";
const QString SEQUENCE_ID_ATTR = "Sequence_ID";

XmlConfHandler::XmlConfHandler(const QString &fileName):
    m_fileName(fileName),
    m_filePtr(new QFile(fileName)),
    newSequenceStarted(false),
    newSessionStarted(false),
    teachingSequenceID(0),
    m_stream(new QXmlStreamWriter(m_filePtr.data()))

{

}

XmlConfHandler::~XmlConfHandler()
{
    if(newSequenceStarted)
    {
        endCurrentTeachingSequence();
    }
    if(newSessionStarted)
    {
        endCurrentSession();
    }
}

bool XmlConfHandler::isLogOpen()
{
    return m_filePtr->isOpen();
}

bool XmlConfHandler::writeXmlOldContent(const QVector<XmlTag> &tags,
                                        QVector<quint64> &endElementsNumbers)
{
    if(!tags.empty())
    {
        quint64 lineNumber = 2;
        quint32 i = 0;
        while(lineNumber <= endElementsNumbers.last())
        {
            if(findLineNumber(lineNumber, endElementsNumbers))
            {
                m_stream->writeEndElement();
            }
            else
            {
                if(!tags[i].tagCharacters.isEmpty())
                {
                    m_stream->writeTextElement(tags[i].tagName, tags[i].tagCharacters);
                }
                else
                {
                   m_stream->writeStartElement(tags[i].tagName);
                }
                if(!tags[i].attribiutes.empty())
                {
                    foreach(QXmlStreamAttribute attr, tags[i].attribiutes)
                    {
                        m_stream->writeAttribute(attr.name().toString(), attr.value().toString());
                    }
                }
                i++;
            }
            lineNumber++;
        }
    }
    else
    {
        return false;
    }
    m_stream->writeEndDocument();
    return true;
}

bool XmlConfHandler::getXmlFileOldContent(QVector<XmlTag> &tags,
                                          QVector<quint64> &endElementsNumbers)
{
    m_filePtr->seek(0);
    QXmlStreamReader xmlReader(m_filePtr.data());
    XmlTag currentTag;
    bool newTag = false;
    bool isTextElement = false;

    while (!xmlReader.atEnd())
    {
        xmlReader.readNext();
        if(xmlReader.isStartElement())
        {
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
        }
        if(xmlReader.isCharacters() && newTag)
        {
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
    }
    if(!tags.empty())
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool XmlConfHandler::startNewSession()
{
    if(!m_filePtr->isOpen() && !newSessionStarted)
    {
        if (!m_filePtr->open(QIODevice::ReadWrite | QIODevice::Text | QIODevice::Append))
        {
            return false;
        }
        //here use xml reader and writer combo to append the file and create new session
        getXmlFileOldContent(oldContent, oldEndElements);
        m_filePtr->resize(0);
        m_stream->setAutoFormatting(true);

        QDateTime timeStamp = QDateTime::currentDateTime();
        currentSessionID = QDateTime::currentMSecsSinceEpoch();
        m_stream->writeStartDocument();
        m_stream->writeStartElement(SESSION_TAG);//</Session>
        m_stream->writeAttribute(DATE_ATTR, timeStamp.toString());
        m_stream->writeAttribute(SESSION_ID_ATTR, QString::number(currentSessionID));
        newSessionStarted = true;
        return true;
    }
    return false;
}

bool XmlConfHandler::endCurrentSession()
{
    if(newSessionStarted)
    {
        m_stream->writeEndElement();//</Session>
        writeXmlOldContent(oldContent, oldEndElements);
        m_filePtr->close();
        teachingSequenceID = 0;
        newSessionStarted = false;
        return true;
    }
    return false;
}

bool XmlConfHandler::startNewTeachingSequence()
{
    if(newSessionStarted)
    {
        m_stream->writeStartElement(POSITIONS_TAG);
        m_stream->writeAttribute(SESSION_ID_ATTR, QString::number(currentSessionID));
        m_stream->writeAttribute(SEQUENCE_ID_ATTR, QString::number(teachingSequenceID));
        newSequenceStarted = true;
        return true;
    }
    else
    {
        return false;
    }

}

bool XmlConfHandler::endCurrentTeachingSequence()
{
    if(newSequenceStarted)
    {
        m_stream->writeEndElement(); // </positions>
        teachingSequenceID++;
        newSequenceStarted = false;
        return true;
    }
    return false;
}

bool XmlConfHandler::addPositionToTeach(const QString &position)
{
    if(newSequenceStarted)
    {
        m_stream->writeTextElement(POSITION_TAG, position);
        return true;
    }
    else
    {
        return false;
    }
}

bool XmlConfHandler::extractPositionList(quint64 sessionID, quint64 sequenceID, QVector<QString> &positions)
{
    if(!newSessionStarted)
    {
        if (!m_filePtr->open(QIODevice::ReadWrite | QIODevice::Text | QIODevice::Append))
        {
            return false;
        }
        getXmlFileOldContent(oldContent, oldEndElements); // get xml file content
    }
    quint64 lineNumber = 2;
    quint32 i = 0;
    while(lineNumber <= oldEndElements.last())
    {
        if(!findLineNumber(lineNumber, oldEndElements))
        {
            if(oldContent[i].tagName == POSITIONS_TAG && !oldContent[i].attribiutes.empty())
            {
                if(oldContent[i].attribiutes[0].value().toULongLong() == sessionID)
                {
                    if(oldContent[i].attribiutes[1].value().toULongLong() == sequenceID)
                    {
                        while(!findLineNumber(lineNumber, oldEndElements))
                        {
                            if(oldContent[i].tagName == POSITION_TAG && !oldContent[i].tagCharacters.isEmpty())
                            {
                                positions.push_back(oldContent[i].tagCharacters);
                            }
                            i++;
                            lineNumber++;
                        }
                    }
                }
            }
            i++;
        }
        lineNumber++;
    }
    return positions.empty();

}

bool XmlConfHandler::getSessionSequenceIDs(QVector<QPair<quint64, quint64> > &IDsTab)
{
    if(!newSessionStarted)
    {
        getXmlFileOldContent(oldContent, oldEndElements);
    }
    quint64 firstID = 0, secondID = 0;
    bool firstReady = false;
    bool secondReady = false;
    foreach(XmlTag tag, oldContent)
    {
        if(!tag.attribiutes.empty())
        {
            foreach(QXmlStreamAttribute attr, tag.attribiutes)
            {
                if(attr.name().toString() == SESSION_ID_ATTR)
                {
                    firstID = attr.value().toULongLong();
                    firstReady = true;
                }
                if(attr.name().toString() == SEQUENCE_ID_ATTR)
                {
                    secondID = attr.value().toULongLong();
                    secondReady = true;
                }
                if(firstReady && secondReady)
                {
                    firstReady = false;
                    secondReady = false;
                    IDsTab.push_back(QPair<quint64, quint64>(firstID, secondID));
                }
            }
        }
    }
    return IDsTab.empty();

}

bool XmlConfHandler::findLineNumber(quint64 num, const QVector<quint64> &lineNumbers)
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
