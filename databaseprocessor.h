#ifndef DATABASEPROCESSOR_H
#define DATABASEPROCESSOR_H

#include <QObject>
#include <QSqlDatabase>

struct ScreenshotData
{
    QByteArray imageData = QByteArray();
    QString hash = QString();
    QString percentage = QString();
};

class QSqlDatabase;

class DatabaseProcessor : public QObject
{
    Q_OBJECT
public:
    explicit DatabaseProcessor(QObject *parent = nullptr);

private:
    QSqlDatabase db;

public slots:
    void insertIntoTable(const QByteArray &imageData, const QString &hash, const QString &percentage);
    void loadFromTable();

signals:
    void sendImageData(const QVector<ScreenshotData> &sceenshotsDatas);
};

#endif // DATABASEPROCESSOR_H
