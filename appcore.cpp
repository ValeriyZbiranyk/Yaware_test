#include "appcore.h"
#include "screenshotsprocessor.h"
#include <QTimer>
#include <QDebug>
#include <QApplication>
#include <QFile>
#include <QPixmap>

const int ONE_MINUTE = 1000 * 60;

AppCore::AppCore(QObject *parent) : QObject{parent}
{
    screenshotProcessor = new ScreenshotsProcessor(this);
    connect(this, &AppCore::compare,
            screenshotProcessor, &ScreenshotsProcessor::process);
    connect(this, &AppCore::setPreviousScreenshot,
            screenshotProcessor, &ScreenshotsProcessor::setPreviousScreenshot);
    connect(screenshotProcessor, &ScreenshotsProcessor::sendComparasionResult,
            this, &AppCore::comparasionResultReceived);

    databaseProcessor = new DatabaseProcessor(this);
    connect(this, &AppCore::saveToDb,
            databaseProcessor, &DatabaseProcessor::insertIntoTable);
    connect(this, &AppCore::loadFromDb,
            databaseProcessor, &DatabaseProcessor::loadFromTable);
    connect(databaseProcessor, &DatabaseProcessor::sendImageData,
            this, &AppCore::screenshotDataFromDbReceived);

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &AppCore::makeCompare);

}

AppCore::~AppCore()
{
    timer->stop();
    delete timer;
    delete screenshotProcessor;
    delete databaseProcessor;
}

void AppCore::loadFromDatabase()
{
    emit loadFromDb();
}

void AppCore::makeCompare()
{
    emit compare();
}

void AppCore::comparasionResultReceived(const QString &path, const QByteArray &imageData, const QString &hash, const QString &percentage)
{
    emit saveToDb(imageData, hash, percentage);
    emit sendScreenshotData(path, hash, percentage);
}

void AppCore::screenshotDataFromDbReceived(const QVector<ScreenshotData> &screenshotsDatas)
{
    if(screenshotsDatas.isEmpty())
    {
        timer->start(ONE_MINUTE);
        return;
    }
    QPixmap screenshot;
    for(int i = 0; i < screenshotsDatas.count(); ++i)
    {
        QString path = screenshotProcessor->getImagesDirPath() + "/" + screenshotsDatas.at(i).hash + ".png";
        QFile currentScreenshotFile(path);

        screenshot.loadFromData(screenshotsDatas.at(i).imageData, "PNG");
        currentScreenshotFile.open(QIODevice::WriteOnly);
        screenshot.save(&currentScreenshotFile, "PNG");
        currentScreenshotFile.close();
        emit sendScreenshotData("file:/" + path, screenshotsDatas.at(i).hash, screenshotsDatas.at(i).percentage);
    }

    emit setPreviousScreenshot(screenshot);

    timer->start(ONE_MINUTE);
}
