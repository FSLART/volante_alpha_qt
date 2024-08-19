#ifndef PILOT_H
#define PILOT_H



#include <QMainWindow>
#include "store.h"


QT_BEGIN_NAMESPACE
namespace Ui { class pilot; }
QT_END_NAMESPACE

class PilotWindow : public QMainWindow
{
    Q_OBJECT

public:
    PilotWindow(QWidget *parent = nullptr, QString serialDev=nullptr);
    store* getStore();
    ~PilotWindow();
    void switch_screen(int menu);

private:

    Ui::pilot *ui;
};


#endif // PILOT_H
