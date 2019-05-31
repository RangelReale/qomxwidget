#ifndef H__QOMXWIDGET_PLAYER_PLAYER__H
#define H__QOMXWIDGET_PLAYER_PLAYER__H

#include <QObject>

namespace QOmxWidget::Player
{

class Player : public QObject
{
public:
    Player(QObject *parent=0);
};

}

#endif // H__QOMXWIDGET_PLAYER_PLAYER__H
