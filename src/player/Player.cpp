#include "qomxwidget/player/Player.h"

#include <QDBusMessage>
#include <QDebug>

#include <qdbusdaemon/Process.h>


namespace QOmxWidget {
namespace Player {

Player::Player(QObject *parent) : QObject(parent), _started(false), _connected(false), _process(NULL),
    _executable("/usr/bin/omxplayer.bin"), _executableParams(), _dbd(NULL), _dbconn(NULL),
    _left(100), _top(100), _width(200), _height(160)
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
    if (_dbconn)
    {
        QDBusMessage message = QDBusMessage::createMethodCall("org.mpris.MediaPlayer2.omxplayer", "/org/mpris/MediaPlayer2", "org.mpris.MediaPlayer2.Player", "Play");
        QDBusMessage reply = _dbconn->call(message);
        qDebug() << reply;
    }
}

void Player::pause()
{
    if (_dbconn)
    {
        QDBusMessage message = QDBusMessage::createMethodCall("org.mpris.MediaPlayer2.omxplayer", "/org/mpris/MediaPlayer2", "org.mpris.MediaPlayer2.Player", "Pause");
        QDBusMessage reply = _dbconn->call(message);
        qDebug() << reply;
    }
}

void Player::stop()
{
    if (_dbconn)
    {
        QDBusMessage message = QDBusMessage::createMethodCall("org.mpris.MediaPlayer2.omxplayer", "/org/mpris/MediaPlayer2", "org.mpris.MediaPlayer2.Player", "Stop");
        QDBusMessage reply = _dbconn->call(message);
        qDebug() << reply;
    }
}

qint64 Player::position() const
{
    if (_dbconn)
    {
        QDBusMessage message = QDBusMessage::createMethodCall("org.mpris.MediaPlayer2.omxplayer", "/org/mpris/MediaPlayer2", "org.freedesktop.DBus.Properties", "Get");
        message.setArguments(QList<QVariant>() << "org.mpris.MediaPlayer2.Player" << "Position");
        QDBusMessage reply = _dbconn->call(message);
        qDebug() << reply;
        if (reply.type() == QDBusMessage::ReplyMessage) {
            return reply.arguments().at(0).toULongLong();
        }
    }
    return 0;
}

qint64 Player::duration() const
{
    if (_dbconn)
    {
        QDBusMessage message = QDBusMessage::createMethodCall("org.mpris.MediaPlayer2.omxplayer", "/org/mpris/MediaPlayer2", "org.freedesktop.DBus.Properties", "Get");
        message.setArguments(QList<QVariant>() << "org.mpris.MediaPlayer2.Player" << "Duration");
        QDBusMessage reply = _dbconn->call(message);
        qDebug() << reply;
        if (reply.type() == QDBusMessage::ReplyMessage) {
            return reply.arguments().at(0).toULongLong();
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

void Player::dbdStart()
{
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

        _process->setEnvironment(_dbd->environmentVariables());
        qDebug() << "IS CONNECTED: " << _dbd->isConnected();
        qDebug() << "ENV: " << _dbd->environmentVariables();
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
    qDebug() << "DBD CONNECTED: " << _dbd->address();

    emit processStart();
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
        qDebug() << _dbd->address();
        _dbconn = new QDBusConnection(QDBusConnection::connectToBus(_dbd->address(), "qomxwidget_player"));

        _connected = true;
        emit started();
    }
}

void Player::processFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
    if (_dbconn)
    {
        delete _dbconn;
        _dbconn = NULL;
    }

    _connected = false;
    _started = false;

    _process->deleteLater();
    _process = NULL;

    emit finished();
}

void Player::processReadyReadStandardError()
{
    if (_started)
    {
        emit error(QString("Player error output: %1").arg(QString::fromLocal8Bit(_process->readAll())));
    }
}

void Player::processReadyReadStandardOutput()
{
}

} }
