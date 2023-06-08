#ifndef SCREENSHOTSPROCESSOR_H
#define SCREENSHOTSPROCESSOR_H

#include <QObject>

class QPixmap;

class ScreenshotsProcessor : public QObject
{
    Q_OBJECT
public:
    explicit ScreenshotsProcessor(QObject *parent = nullptr);
    ~ScreenshotsProcessor();

    QString getImagesDirPath();

private:
    QPixmap *previousScreenshot;
    QPixmap *currentScreenshot;
    QString imagesDirPath;

    void compare();

public slots:
    void process();
    void setPreviousScreenshot(const QPixmap &previous);

signals:
    void sendComparasionResult(const QString &path, const QByteArray &imageData, const QString &hash, const QString &percentage);
};

#endif // SCREENSHOTSPROCESSOR_H
