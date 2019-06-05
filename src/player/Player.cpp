#include "qomxwidget/player/Player.h"

#include <QDBusMessage>
#include <QTime>

#include <qdbusdaemon/Process.h>


namespace QOMXWidget {
namespace Player {

Player::Player(QObject *parent) : QObject(parent), _usedbus(true), _started(false), _connected(false), _process(NULL),
    _executable("/usr/bin/omxplayer.bin"), _executableParams(), _dbd(NULL), _dbconn(NULL),
    _left(100), _top(100), _width(200), _height(160), _dbusnamesuffix(), _autodbusnamesuffix(false)
{

}

Player::~Player()
{
    if (_process)
    {
        processStop();
        _process->waitForFinished();
    }

    setDBusDaemon(NULL);
}

bool Player::isStarted()
{
    return _started;
}

bool Player::isConnected()
{
    return _connected;
}

Q_PID Player::pid() const
{
    if (_process)
    {
        return _process->pid();
    }
    return 0;
}

const QString &Player::executable() const
{
    return _executable;
}

void Player::setExecutable(const QString &value)
{
    _executable = value;
}

QStringList &Player::executableParams()
{
    return _executableParams;
}

void Player::setDBusDaemon(QDBusDaemon::Base *dbd)
{
    if (!_usedbus) return;

    if (_dbd)
    {
        if (_dbdOwned)
        {
            delete _dbd;
        }
        _dbd = NULL;
        _dbdOwned = false;
    }

    if (dbd)
    {
        _dbd = dbd;
        connect(_dbd, SIGNAL(connected()), this, SLOT(dbdConnected()));
        _dbdOwned = false;
    }
}

void Player::load(const QString &filename)
{
    _filename = filename;
    emit dbdStart();
}

void Player::play()
{
    if (_usedbus && _dbconn)
    {
        QDBusMessage message = QDBusMessage::createMethodCall(currentDBusName(), "/org/mpris/MediaPlayer2", "org.mpris.MediaPlayer2.Player", "Play");
        QDBusMessage reply = _dbconn->call(message);
        if (reply.type() == QDBusMessage::ErrorMessage)
        {
            emit trace(QString("Error in play message: %1").arg(reply.errorMessage()));
        }
        //qDebug() << reply;
    }
}

void Player::pause()
{
    if (_usedbus && _dbconn)
    {
        QDBusMessage message = QDBusMessage::createMethodCall(currentDBusName(), "/org/mpris/MediaPlayer2", "org.mpris.MediaPlayer2.Player", "Pause");
        QDBusMessage reply = _dbconn->call(message);
        if (reply.type() == QDBusMessage::ErrorMessage)
        {
            emit trace(QString("Error in pause message: %1").arg(reply.errorMessage()));
        }
        //qDebug() << reply;
    }
}

void Player::stop()
{
    if (_usedbus && _dbconn)
    {
        QDBusMessage message = QDBusMessage::createMethodCall(currentDBusName(), "/org/mpris/MediaPlayer2", "org.mpris.MediaPlayer2.Player", "Stop");
        QDBusMessage reply = _dbconn->call(message);
        if (reply.type() == QDBusMessage::ErrorMessage)
        {
            emit trace(QString("Error in stop message: %1").arg(reply.errorMessage()));
        }
        //qDebug() << reply;
    }
    else if (_process)
    {
        _process->terminate();
    }
}

qint64 Player::position() const
{
    if (_usedbus && _dbconn)
    {
        QDBusMessage message = QDBusMessage::createMethodCall(currentDBusName(), "/org/mpris/MediaPlayer2", "org.freedesktop.DBus.Properties", "Get");
        message.setArguments(QList<QVariant>() << "org.mpris.MediaPlayer2.Player" << "Position");
        QDBusMessage reply = _dbconn->call(message);
        if (reply.type() == QDBusMessage::ReplyMessage) {
            return reply.arguments().at(0).toULongLong();
        }
        else if (reply.type() == QDBusMessage::ErrorMessage)
        {
            emit trace(QString("Error in position message: %1").arg(reply.errorMessage()));
        }
    }
    return 0;
}

qint64 Player::duration() const
{
    if (_usedbus && _dbconn)
    {
        QDBusMessage message = QDBusMessage::createMethodCall(currentDBusName(), "/org/mpris/MediaPlayer2", "org.freedesktop.DBus.Properties", "Get");
        message.setArguments(QList<QVariant>() << "org.mpris.MediaPlayer2.Player" << "Duration");
        QDBusMessage reply = _dbconn->call(message);
        //qDebug() << reply;
        if (reply.type() == QDBusMessage::ReplyMessage) {
            return reply.arguments().at(0).toULongLong();
        }
        else if (reply.type() == QDBusMessage::ErrorMessage)
        {
            emit trace(QString("Error in duration message: %1").arg(reply.errorMessage()));
        }
    }
    return 0;
}

int Player::left() const
{
    return _left;
}

int Player::top() const
{
    return _top;
}

int Player::width() const
{
    return _width;
}

int Player::height() const
{
    return _height;
}

void Player::setBounds(int left, int top, int width, int height)
{
    _left = left;
    _top = top;
    _width = width;
    _height = height;
}

const QString &Player::dbusNameSuffix() const
{
    return _dbusnamesuffix;
}

void Player::setDBusNameSuffix(const QString &value)
{
    _dbusnamesuffix = value;
}

bool Player::useDBus() const
{
    return _usedbus;
}

void Player::setUseDBus(bool value)
{
    _usedbus = value;
}

bool Player::autoDBusNameSuffix() const
{
    return _autodbusnamesuffix;
}

void Player::setAutoDBusNameSuffix(bool value)
{
    if (value != _autodbusnamesuffix)
    {
        _autodbusnamesuffix = value;
        if (value && _dbusnamesuffix.isEmpty())
        {
            _dbusnamesuffix = genRandomName();
        }
    }
}

void Player::dbdStart()
{
    if (!_usedbus)
    {
        emit processStart();
        return;
    }

    if (!_dbd)
    {
        _dbd = new QDBusDaemon::Process(this);
        connect(_dbd, SIGNAL(connected()), this, SLOT(dbdConnected()));
    }

    if (!_dbd->isStarted())
    {
        _dbd->start();
    }
    else if (_dbd->isConnected())
    {
        emit processStart();
    }
}

void Player::processStart()
{
    if (!_started)
    {
        if (_process) delete _process;
        _process = new QProcess();

        connect(_process, SIGNAL(error(QProcess::ProcessError)), this, SLOT(processError(QProcess::ProcessError)));
        connect(_process, SIGNAL(started()), this, SLOT(processStarted()));
        connect(_process, SIGNAL(finished(int,QProcess::ExitStatus)), this, SLOT(processFinished(int,QProcess::ExitStatus)));
        connect(_process, SIGNAL(readyReadStandardError()), this, SLOT(processReadyReadStandardError()));
        connect(_process, SIGNAL(readyReadStandardOutput()), this, SLOT(processReadyReadStandardOutput()));

        QStringList params;
        params.append(_executableParams);
        params
            << "--win" << QString("%1,%2,%3,%4").arg(_left).arg(_top).arg(_left+_width).arg(_top+_height)
            << _filename;
        if (_usedbus && !_dbusnamesuffix.isEmpty())
        {
            params << "--dbus_name" << currentDBusName();
            //qDebug() << "DBUS NAME: " << currentDBusName();
            emit trace(QString("DBUS name: %1").arg(currentDBusName()));
        }

        if (_usedbus)
        {
            _process->setEnvironment(_dbd->environmentVariables());
        }
        //qDebug() << "ENV: " << _dbd->environmentVariables();
        _started = true;
        _process->start(_executable, params);
    }
}

void Player::processStop()
{
    if (_started)
    {
        _started = false;
        _process->terminate();
    }
}

void Player::dbdConnected()
{
    if (_usedbus)
    {
        //qDebug() << "DBD CONNECTED: " << _dbd->address();
        emit trace(QString("DBUS daemon connected: %1").arg(_dbd->address()));

        emit processStart();
    }
}

void Player::processError(QProcess::ProcessError err)
{
    QString errorMessage("Player Process error");
    switch (err) {
    case QProcess::ProcessError::FailedToStart: errorMessage += ": Failed to start"; break;
    case QProcess::ProcessError::Crashed: errorMessage += ": Crashed"; break;
    case QProcess::ProcessError::Timedout: errorMessage += ": Timed out"; break;
    case QProcess::ProcessError::ReadError: errorMessage += ": Read error"; break;
    case QProcess::ProcessError::WriteError: errorMessage += ": Write error"; break;
    default: break;
    }

    emit error(errorMessage);
}

void Player::processStarted()
{
    if (_started)
    {
        if (_usedbus)
        {
            _dbconn = new QDBusConnection(QDBusConnection::connectToBus(_dbd->address(), "qomxwidget_player"));
        }

        _connected = true;
        emit loaded();
        emit started();
    }
}

void Player::processFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
    // qDebug() << "processFinished: " << exitCode << " --- " << static_cast<int>(exitStatus);
    emit trace(QString("Process finished: exitCode %1, exitStatus %2").arg(exitCode).arg(static_cast<int>(exitStatus)));

    if (_dbconn)
    {
        delete _dbconn;
        _dbconn = NULL;
    }

    _connected = false;
    _started = false;

    _process->deleteLater();
    _process = NULL;

    if (exitStatus == QProcess::NormalExit && (exitCode == 0 || exitCode == 3)) // when using the Stop command, the player returns 3
    {
        emit finished();
    }
    else if (!_usedbus && exitCode == 0)
    {
        // terminate was sent to the process
        emit finished();
    }
    else
    {
        emit error(QString("OMXPlayer process %1 with exit code %2")
            .arg(exitStatus==QProcess::NormalExit?QString("finished"):QString("crashed"))
            .arg(exitCode));
    }
}

void Player::processReadyReadStandardError()
{
    if (_started)
    {
        emit error(QString("Player error output: %1").arg(QString::fromLocal8Bit(_process->readAllStandardError())));
    }
}

void Player::processReadyReadStandardOutput()
{
}

QString Player::genRandomName() const
{
    const int length = 12;
    const QString allow_symbols = "ABCDEFGHIJKLMNOPQRSTUWXYZabcdefghijklmnopqrtsuwxyz";

    QString result;
    qsrand(QTime::currentTime().msec());
    for (int i=0; i<length; ++i) {
        result.append(allow_symbols.at(qrand() % (allow_symbols.length())));
    }
    return result;
}

QString Player::currentDBusName() const
{
    QString name("org.mpris.MediaPlayer2.omxplayer");
    if (!_dbusnamesuffix.isEmpty())
    {
        name.append(QString("-%1").arg(_dbusnamesuffix));
    }
    return name;
}

} }
