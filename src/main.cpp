/**
* @file main.cpp
* @brief This file contains the main function, responsible for starting the application
* @see mainwindow.h
* IDK who developed the "default" main.cpp, but it was not me. 
* @b Don't touch this unless you know what you are doing, theres very "few" things to "properly" implement here.
**/
#include "mainwindow.h"
#include <QApplication>
#include <QLocale>
#include <QTranslator>

	
int main(int argc, char *argv[]){

    QApplication a(argc, argv);

    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "VolanteAlphaQT_1_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }
	
	MainWindow w;
    w.show();

    return a.exec();

}