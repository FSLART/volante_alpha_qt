#ifndef FWINDOW_H
#define FWINDOW_H


#include <QMainWindow>
#include "store.h"




class FWindow : public QMainWindow
{

    Q_OBJECT

public:
    FWindow(QWidget *parent = nullptr, QString serialDev=nullptr);
    store* getStore();
    ~FWindow();

};


#endif // FWINDOW_H
