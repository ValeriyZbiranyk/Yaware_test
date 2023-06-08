#include "screenshotsprocessor.h"
#include <QBuffer>
#include <QPixmap>
#include <QApplication>
#include <QDesktopWidget>
#include <QGuiApplication>
#include <QCryptographicHash>
#include <QScreen>
#include <QRect>
#include <QtConcurrent>
#include <QDebug>
#include <QFile>
#include <QDir>

ScreenshotsProcessor::ScreenshotsProcessor(QObject *parent) : QObject{parent}
{
    QRect screenGeometry = QGuiApplication::screens().at(0)->geometry();
    previousScreenshot = new QPixmap(screenGeometry.size());
    previousScreenshot->fill(Qt::white);
    currentScreenshot = new QPixmap(screenGeometry.size());
    currentScreenshot->fill(Qt::white);

    imagesDirPath = QApplication::applicationDirPath() + "/images";
    if(!QDir(imagesDirPath).exists())
    {
        QDir().mkdir(imagesDirPath);
    }
}

ScreenshotsProcessor::~ScreenshotsProcessor()
{
    if(QDir(imagesDirPath).exists())
    {
        QDir dir(imagesDirPath);
        dir.setNameFilters(QStringList() << "*.*");
        dir.setFilter(QDir::Files);
        foreach(QString dirFile, dir.entryList())
        {
            dir.remove(dirFile);
        }
    }
}

QString ScreenshotsProcessor::getImagesDirPath()
{
    return imagesDirPath;
}

void ScreenshotsProcessor::compare()
{
    QScreen *screen = QGuiApplication::screens().at(0);
    QRect screenGeometry = screen->geometry();
    currentScreenshot = new QPixmap(screen->grabWindow(QApplication::desktop()->winId(),
                                                       screenGeometry.x(),
                                                       screenGeometry.y(),
                                                       screenGeometry.width(),
                                                       screenGeometry.height()));

    double diffentPixels = 0;

    for(int i = 0; i < screenGeometry.width(); ++i)
    {
        for(int j = 0; j < screenGeometry.height(); ++j)
        {
            if(previousScreenshot->toImage().pixel(i, j) != currentScreenshot->toImage().pixel(i, j))
            {
                ++diffentPixels;
            }
        }
    }

    double differencePercentage = diffentPixels / (screenGeometry.width() * screenGeometry.height()) * 100;
    QString percentage = "Difference: " + QString::number(differencePercentage) + "%";

    QByteArray currentScreenshotBytes = QByteArray();
    QBuffer buffer(&currentScreenshotBytes);
    buffer.open(QIODevice::WriteOnly);
    currentScreenshot->save(&buffer, "PNG");
    QString hash = QString(QCryptographicHash::hash(currentScreenshotBytes, QCryptographicHash::Md5).toHex());
    buffer.close();

    QString path = imagesDirPath + "/" + hash + ".png";
    QFile currentScreenshotFile(path);
    currentScreenshotFile.open(QIODevice::WriteOnly);
    currentScreenshot->save(&currentScreenshotFile, "PNG");
    currentScreenshotFile.close();

    emit sendComparasionResult("file:/" + path, currentScreenshotBytes, hash, percentage);

    previousScreenshot = currentScreenshot;
}

void ScreenshotsProcessor::process()
{
    QtConcurrent::run(this, &ScreenshotsProcessor::compare);
}

void ScreenshotsProcessor::setPreviousScreenshot(const QPixmap &previous)
{
    previousScreenshot = new QPixmap(previous);
}
