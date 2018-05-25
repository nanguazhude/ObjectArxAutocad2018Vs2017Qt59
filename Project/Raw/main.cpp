#include "MainWindow.hpp"
#include <QApplication>

int main(int argc, char *argv[]){

    QApplication app(argc, argv);
    MainWindow window(QString::fromLocal8Bit(argv[1]));
	window.resize(800,300);
    window.show();

    return app.exec();
}


