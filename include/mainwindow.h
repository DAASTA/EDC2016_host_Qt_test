#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPainter>
#include <QTimer>

#include "logic/GameData.h"



namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void update();
    void paintEvent(QPaintEvent *);

private:
    Ui::MainWindow *ui;
    QTimer *timer;

    GameData gameData;

};

#endif // MAINWINDOW_H
