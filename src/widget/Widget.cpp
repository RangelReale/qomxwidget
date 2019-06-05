#include "qomxwidget/widget/Widget.h"

namespace QOMXWidget {
namespace Widget {

Widget::Widget(QWidget *parent) : QWidget(parent), _player(this)
{
    QPalette p(palette());
    p.setColor(backgroundRole(), Qt::black);
    setPalette(p);
    setAutoFillBackground(true);

    connect(&_player, SIGNAL(error(QString)), this, SIGNAL(error(QString)));
    connect(&_player, SIGNAL(started()), this, SIGNAL(started()));
    connect(&_player, SIGNAL(loaded()), this, SIGNAL(loaded()));
    connect(&_player, SIGNAL(playing()), this, SIGNAL(playing()));
    connect(&_player, SIGNAL(paused()), this, SIGNAL(paused()));
    connect(&_player, SIGNAL(finished()), this, SIGNAL(finished()));
    connect(&_player, SIGNAL(trace(QString)), this, SIGNAL(trace(QString)));
}

Widget::~Widget()
{

}

void Widget::load(const QString &filename)
{
    QPoint p = mapToGlobal(QPoint(0, 0));
    _player.setBounds(p.x(), p.y(), width(), height());

    _player.load(filename);
}

void Widget::play()
{
    _player.play();
}

void Widget::pause()
{
    _player.pause();
}

void Widget::stop()
{
    _player.stop();
}

qint64 Widget::position() const
{
    return _player.position();
}

qint64 Widget::duration() const
{
    return _player.duration();
}

Player::Player &Widget::player()
{
    return _player;
}

} }
