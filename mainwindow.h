#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <memory>
#include <QMainWindow>

class Field;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void on_menu_new_triggered();

private:
    Ui::MainWindow *ui;
    std::unique_ptr<Field> field;
    void new_game();
};

#endif // MAINWINDOW_H
