#ifndef XMLCONFHANDLER_H
#define XMLCONFHANDLER_H
#include <QString>
#include <QScopedPointer>
#include <QFile>
#include <QVector>
#include <QXmlStreamWriter>

class XmlConfHandler
{
public:
    XmlConfHandler(const QString &fileName);
    XmlConfHandler();
    ~XmlConfHandler();

    void setFileName(const QString &fileName);

    bool isLogOpen();
    bool startNewSession();

    bool startNewTeachingSequence();
    bool addPositionToTeach(const QString &position);

    bool extractPositionList(quint64 sessionID, quint64 sequenceID, QVector<QString> &positions);
    bool getSessionSequenceIDs(QVector<qint64> &IDsTab);

    struct XmlTag
    {
        QString tagName;
        QString tagCharacters;
        QXmlStreamAttributes attribiutes;
    };
private:
    QScopedPointer<QFile> m_filePtr;
    QString m_fileName;
    bool newSequenceStarted;
    bool newSessionStarted;
    QScopedPointer<QXmlStreamWriter> m_stream;
    QVector<XmlTag> oldContent;
    QVector<quint64> oldEndElements;
    quint64 teachingSequenceID;
    quint64 currentSessionID;

    bool writeXmlOldContent(const QVector<XmlTag> &tags,
                            QVector<quint64> &endElementsNumbers);


    bool getXmlFileOldContent(QVector<XmlTag> &tags,
                              QVector<quint64> &endElementsNumbers);
    bool findLineNumber(quint64 num, const QVector<quint64> &lineNumbers);
};


#endif // XMLCONFHANDLER_H
