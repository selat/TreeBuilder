#include <QApplication>
#include <QSplashScreen>
#include <QStyleFactory>
#include <QThread>

#include "mainwindow.h"

int main(int argc, char **argv)
{
	QApplication app(argc, argv);
    QApplication::setStyle(QStyleFactory::create("Fusion"));

//    QPixmap pixmap("splash.png");
//    QSplashScreen splash(pixmap);
//    splash.show();
//    app.processEvents();
//
//    QThread::currentThread() -> sleep(5);
	MainWindow window;
	window.show();
//    splash.finish(&window);
	return app.exec();
}
