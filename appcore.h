#ifndef APPCORE_H
#define APPCORE_H

#include <QObject>
#include <databaseprocessor.h>

class ScreenshotsProcessor;
class QTimer;

class AppCore : public QObject
{
    Q_OBJECT
public:
    explicit AppCore(QObject *parent = nullptr);
    ~AppCore();

    void loadFromDatabase();

private:
    ScreenshotsProcessor *screenshotProcessor;
    DatabaseProcessor *databaseProcessor;
    QTimer *timer;

public slots:
    void makeCompare();
    void comparasionResultReceived(const QString &path, const QByteArray &imageData, const QString &hash, const QString &percentage);
    void screenshotDataFromDbReceived(const QVector<ScreenshotData> &screenshotsDatas);

signals:
    void saveToDb(const QByteArray &imageData, const QString &hash, const QString &percentage);
    void loadFromDb();
    void sendScreenshotData(QString path, QString hash, QString percentage);
    void setPreviousScreenshot(const QPixmap &previous);
    void compare();
};

#endif // APPCORE_H
