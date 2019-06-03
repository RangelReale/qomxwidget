#ifndef H__QOMXWIDGET_PLAYER_PLAYER__H
#define H__QOMXWIDGET_PLAYER_PLAYER__H

#include <QObject>
#include <QProcess>
#include <QDBusConnection>

#include <qdbusdaemon/Base.h>

namespace QOMXWidget {
namespace Player {

class Player : public QObject
{
    Q_OBJECT
public:
    Player(QObject *parent=0);
    ~Player();

    bool isStarted();
    bool isConnected();
    Q_PID pid() const;
    QString address() const;

    const QString &executable() const;
    void setExecutable(const QString &value);

    QStringList &executableParams();

    void setDBusDaemon(QDBusDaemon::Base *dbd);

    void load(const QString &filename);
    void play();
    void pause();
    void stop();

    qint64 position() const;
    qint64 duration() const;

    const QString &dbusNameSuffix() const;
    void setDBusNameSuffix(const QString &value);

    bool autoDBusNameSuffix() const;
    void setAutoDBusNameSuffix(bool value);

    int left() const;
    int top() const;
    int width() const;
    int height() const;

    void setBounds(int left, int top, int width, int height);
private Q_SLOTS:
    void dbdStart();
    void processStart();
    void processStop();

    void dbdConnected();

    void processError(QProcess::ProcessError error);
    void processStarted();
    void processFinished(int exitCode, QProcess::ExitStatus exitStatus);
    void processReadyReadStandardError();
    void processReadyReadStandardOutput();
Q_SIGNALS:
    void error(QString message);
    void started();
    void loaded();
    void playing();
    void paused();
    void finished();
    void trace(QString message) const;
private:
    QString currentDBusName() const;
    QString genRandomName() const;

    bool _started;
    bool _connected;
    QProcess *_process;
    QString _executable;
    QStringList _executableParams;
    QString _filename;
    QDBusDaemon::Base *_dbd;
    QDBusConnection *_dbconn;
    bool _dbdOwned;
    int _left, _top, _width, _height;
    QString _dbusnamesuffix;
    bool _autodbusnamesuffix;
};

} }

#endif // H__QOMXWIDGET_PLAYER_PLAYER__H
