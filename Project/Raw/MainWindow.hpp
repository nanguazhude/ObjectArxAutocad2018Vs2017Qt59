#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <memory>
#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow final : public QMainWindow{
    Q_OBJECT

public:
    explicit MainWindow(const QString &, QWidget *parent = 0);
    ~MainWindow();

protected:
    void closeEvent(QCloseEvent *event) override;

private:
    Ui::MainWindow *ui;
    class ThisPrivate;
    std::unique_ptr< ThisPrivate > thisp;
    using Parent = QMainWindow;
    void _p_save();
};

#endif // MAINWINDOW_HPP
