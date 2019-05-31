#ifndef H_MAINWINDOW_H
#define H_MAINWINDOW_H

#include <QMainWindow>
#include <QListWidget>
#include <QTimer>

#include <qomxwidget/widget/Widget.h>

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow();
public Q_SLOTS:
    void error(QString message);
    void started();
    void finished();

    void menuStart(bool);
    void menuStop(bool);
    void menuPause(bool);
    void menuPlay(bool);

    void timer();
private:
    QOmxWidget::Widget::Widget* _player;
    QListWidget *_list;
    QTimer *_timer;
};

#endif // H_MAINWINDOW_H
