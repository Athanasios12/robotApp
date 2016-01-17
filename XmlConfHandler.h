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
    ~XmlConfHandler();

    bool isLogOpen();
    bool startNewSession();
    bool endCurrentSession();

    bool startNewTeachingSequence();
    bool addPositionToTeach(const QString &position);
    bool endCurrentTeachingSequence();

    bool extractPositionList(quint64 sessionID, quint64 sequenceID, QVector<QString> &positions);
    bool getSessionSequenceIDs(QVector<QPair<quint64, quint64> > &IDsTab);

    struct XmlTag
    {
        QString tagName;
        QString tagCharacters;
        QXmlStreamAttributes attribiutes;
    };
private:
    QString m_fileName;
    QScopedPointer<QFile> m_filePtr;
    bool newSequenceStarted;
    bool newSessionStarted;
    quint64 teachingSequenceID;
    quint64 currentSessionID;
    QScopedPointer<QXmlStreamWriter> m_stream;
    QVector<XmlTag> oldContent;
    QVector<quint64> oldEndElements;

    bool writeXmlOldContent(const QVector<XmlTag> &tags,
                            QVector<quint64> &endElementsNumbers);


    bool getXmlFileOldContent(QVector<XmlTag> &tags,
                              QVector<quint64> &endElementsNumbers);
    bool findLineNumber(quint64 num, const QVector<quint64> &lineNumbers);
};


#endif // XMLCONFHANDLER_H
