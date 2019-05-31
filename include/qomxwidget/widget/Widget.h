#ifndef H__QOMXWIDGET_WIDGET_WIDGET__H
#define H__QOMXWIDGET_WIDGET_WIDGET__H

#include <QWidget>

namespace QOmxWidget {
namespace Widget {

class Widget : public QWidget
{
    Q_OBJECT
public:
    Widget(QWidget *parent=0);
};

} }

#endif // H__QOMXWIDGET_WIDGET_WIDGET__H
