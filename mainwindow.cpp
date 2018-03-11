#include <cassert>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "field.h"

void MainWindow::new_game() {
    field.reset(new Field(ui->statusBar, ui->currentPlayer));
    field->register_score_labels_to_ui(*ui->scores);
    field->register_cells_to_ui(*ui->field);
    field->update_message();
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    new_game();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_menu_new_triggered()
{
    qDebug() << "--- menu New was triggered ---";
    for (unsigned y = 0; y < SIZE; y++) {
        for (unsigned x = 0; x < SIZE; x++) {
            auto child = field->field_cells[y][x];
            ui->field->removeWidget(child);
            child->deleteLater();
            field->field_cells[y][x] = nullptr;
        }
    }

    for (unsigned i = 0; i < PLAYERS; i++) {
        auto child = field->score_labels[i];
        ui->scores->removeWidget(child);
        child->deleteLater();
        field->score_labels[i] = nullptr;
    }

    new_game();
}

