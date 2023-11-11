/**
* @file main.cpp
* @brief This file contains the main function, responsible for starting the application
* @see mainwindow.h
* IDK who developed the "default" main.cpp, but it was not me. 
* @b Don't touch this unless you know what you are doing, theres very "few" things to "properly" implement here.
**/
#ifdef __LART_T14__
#include "src/tfortwindow.h"
#endif

#ifdef __LART_T24__
#include "mainwindow.h"
#endif

#include <QApplication>
#include <QLocale>
#include <QTranslator>

	
int main(int argc, char *argv[]){

    QApplication a(argc, argv);

    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "LukHudX_1_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }
	
	#ifdef __LART_T24__
		MainWindow w;
		w.show();
	#endif
	#ifdef __LART_T14__
		TfortWindow w; 
		w.show();
    #endif
	return a.exec();

}
