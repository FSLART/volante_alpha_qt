#ifndef VOIDSTERDEBUGWINDOW2_H
#define VOIDSTERDEBUGWINDOW2_H

#include <QMainWindow>
#include "store.h"

QT_BEGIN_NAMESPACE
namespace Ui { class VoidsterdebugWindow2; }
QT_END_NAMESPACE

class VoidsterdebugWindow2 : public QMainWindow
{
    Q_OBJECT

public:
    VoidsterdebugWindow2(QWidget *parent = nullptr, QString serialDev=nullptr);
    store* getStore();
    ~VoidsterdebugWindow2();

private:
    Ui::VoidsterdebugWindow2 *ui;
};

#endif // VOIDSTERDEBUGWINDOW2_H
