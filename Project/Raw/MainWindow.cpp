#include "MainWindow.hpp"
#include "ui_MainWindow.h"
#include <QtCore/QtCore>

class MainWindow::ThisPrivate{
public:
    QString $FilePath;
    QFile $File;
    QString $FileData;
    ThisPrivate(const QString & varFilePath ) : $FilePath(varFilePath) {
        $File.setFileName($FilePath);
        $File.open( QIODevice::ReadWrite );
        QTextStream varStream{ &$File };
        $FileData = varStream.readAll( );
    }
    void save(){
        $File.reset();
        const QByteArray varDataAboutToWrite = $FileData.toLocal8Bit();
        $File.resize( varDataAboutToWrite.size()  );
        $File.write( varDataAboutToWrite );
        $File.close()/*final*/;
    }
};

MainWindow::MainWindow(const QString & varFilePath, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow){
    ui->setupUi(this);
    thisp.reset( new ThisPrivate(varFilePath) ) ;
    ui->textEdit->setPlainText( thisp->$FileData );
}

MainWindow::~MainWindow(){
    delete ui;
}

void MainWindow::_p_save() try {
    thisp->$FileData = ui->textEdit->toPlainText();
    thisp->save();
} catch( ... ){
/*this function should not throw */
}

void MainWindow::closeEvent(QCloseEvent *event) {
    this->_p_save();
    return Parent::closeEvent(event);
}



