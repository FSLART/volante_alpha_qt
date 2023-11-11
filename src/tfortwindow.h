#ifndef TFORTWINDOW_H
#define TFORTWINDOW_H
#include <QMainWindow>
#include "store.h"

QT_BEGIN_NAMESPACE
namespace Ui { class TfortWindow; }
QT_END_NAMESPACE

class TfortWindow : public QMainWindow
{
    Q_OBJECT

public:
    TfortWindow(QWidget *parent = nullptr, QString serialDev=nullptr);
    store* getStore();
    ~TfortWindow();

private:

    Ui::TfortWindow *ui;
};
#endif // TFORTWINDOW_H
