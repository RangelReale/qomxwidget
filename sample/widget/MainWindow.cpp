#include "MainWindow.h"

#include <QMenu>
#include <QMenuBar>
#include <QAction>
#include <QFileDialog>
#include <QDebug>
#include <QBoxLayout>

MainWindow::MainWindow() : QMainWindow(), _player(NULL)
{
    QWidget *central = new QWidget(this);
    setCentralWidget(central);

    QVBoxLayout *layout = new QVBoxLayout;
    central->setLayout(layout);

    _player = new QOMXWidget::Widget::Widget(this);

    connect(_player, SIGNAL(error(QString)), this, SLOT(error(QString)));
    connect(_player, SIGNAL(started()), this, SLOT(started()));
    connect(_player, SIGNAL(finished()), this, SLOT(finished()));
    layout->addWidget(_player, 3);

    _list = new QListWidget(this);
    layout->addWidget(_list, 1);

	QMenu* actmenu = menuBar()->addMenu("&Actions");

	QAction *startmenu = actmenu->addAction("&Start");
	connect(startmenu, SIGNAL(triggered(bool)), this, SLOT(menuStart(bool)));

	QAction *pausemenu = actmenu->addAction("&Pause");
	connect(pausemenu, SIGNAL(triggered(bool)), this, SLOT(menuPause(bool)));

	QAction *playmenu = actmenu->addAction("P&lay");
	connect(playmenu, SIGNAL(triggered(bool)), this, SLOT(menuPlay(bool)));

	QAction *stopmenu = actmenu->addAction("S&top");
	connect(stopmenu, SIGNAL(triggered(bool)), this, SLOT(menuStop(bool)));

	_timer = new QTimer(this);
	connect(_timer, SIGNAL(timeout()), this, SLOT(timer()));

	_timer->start(1000);
}

void MainWindow::menuStart(bool)
{
    //QString mediaFile = QFileDialog::getOpenFileName(this, "Load media");
    QString mediaFile = "/home/pi/Documents/prog/work/edgecontents/ebclient2/build/bin/clientwork/files/f_400250.bin.mp4";
    if (!mediaFile.isEmpty())
    {
        _player->load(mediaFile);
    }
}

void MainWindow::menuStop(bool)
{
    _player->stop();
}

void MainWindow::menuPause(bool)
{
    _player->pause();
}

void MainWindow::menuPlay(bool)
{
    _player->play();
}

void MainWindow::error(QString message)
{
    new QListWidgetItem(QString("ERROR: %1").arg(message), _list);
}

void MainWindow::started()
{
    new QListWidgetItem("STARTED", _list);
}

void MainWindow::finished()
{
    new QListWidgetItem("FINISHED", _list);
}

void MainWindow::timer()
{
    if (_player)
    {
        qDebug() << "POSITION: " << _player->position() << " / " << _player->duration();
    }
}
