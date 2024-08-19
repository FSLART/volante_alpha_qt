#ifndef PILOT_H
#define PILOT_H



#include <QMainWindow>
#include "store.h"


QT_BEGIN_NAMESPACE
namespace Ui { class VoidsterdebugWindow; }
QT_END_NAMESPACE

class VoidsterdebugWindow : public QMainWindow
{
    Q_OBJECT

public:
    VoidsterdebugWindow(QWidget *parent = nullptr, QString serialDev=nullptr);
    store* getStore();
    ~VoidsterdebugWindow();

private:

    Ui::VoidsterdebugWindow *ui;
};


#endif // PILOT_H
