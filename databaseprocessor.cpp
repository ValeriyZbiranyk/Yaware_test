#include "databaseprocessor.h"
#include <QApplication>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

DatabaseProcessor::DatabaseProcessor(QObject *parent)
    : QObject{parent}
{
    QString path_to_db = QApplication::applicationDirPath() + "/" + "images.db";
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(path_to_db);
}

void DatabaseProcessor::insertIntoTable(const QByteArray &imageData, const QString &hash, const QString &percentage)
{
    if(db.open())
    {
        QSqlQuery query = QSqlQuery(db);
        query.prepare("CREATE TABLE IF NOT EXISTS imagesTable (imagedata BLOB, hash TEXT, percentage TEXT)");
        if(!query.exec())
        {
            qDebug() << query.lastError();
            return;
        }

        query.prepare("INSERT INTO imagesTable (imagedata, hash, percentage) VALUES (:imageData, :hash, :percentage)");
        query.bindValue(":imageData", imageData);
        query.bindValue(":hash", hash);
        query.bindValue(":percentage", percentage);
        if(!query.exec())
        {
            qDebug() << query.lastError();
            return;
        }
        db.close();
    }
    else
    {
        qDebug() << db.lastError();
    }
}

void DatabaseProcessor::loadFromTable()
{
    QVector<ScreenshotData> screenshotsDatas;
    if(db.open())
    {
        QSqlQuery query = QSqlQuery(db);
        query.prepare("SELECT * FROM imagesTable");
        if(!query.exec())
        {
            qDebug() << query.lastError();
            emit sendImageData(screenshotsDatas);
            return;
        }

        while(query.next())
        {
            ScreenshotData screenshotData;
            screenshotData.imageData = query.value(0).toByteArray();
            screenshotData.hash = query.value(1).toString();
            screenshotData.percentage = query.value(2).toString();
            screenshotsDatas.append(screenshotData);
        }
        db.close();
        emit sendImageData(screenshotsDatas);
    }
    else
    {
        qDebug() << db.lastError();
        emit sendImageData(screenshotsDatas);
    }
}
