#ifndef H__QOMXWIDGET_WIDGET_WIDGET__H
#define H__QOMXWIDGET_WIDGET_WIDGET__H

#include <qomxwidget/player/Player.h>

#include <QWidget>

namespace QOMXWidget {
namespace Widget {

class Widget : public QWidget
{
    Q_OBJECT
public:
    Widget(QWidget *parent=0);
    ~Widget();

    void load(const QString &filename);
    void play();
    void pause();
    void stop();

    qint64 position() const;
    qint64 duration() const;

    Player::Player &player();
Q_SIGNALS:
    void error(QString message);
    void started();
    void loaded();
    void playing();
    void paused();
    void finished();
private:
    Player::Player _player;
};

} }

#endif // H__QOMXWIDGET_WIDGET_WIDGET__H
