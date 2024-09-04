#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "store.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr, QString serialDev=nullptr);
    store* getStore();
    ~MainWindow();
    inline void setStyleSheets();

private:

    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
