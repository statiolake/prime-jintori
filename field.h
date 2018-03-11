#ifndef FIELD_H
#define FIELD_H

#include <QGridLayout>
#include <QStatusBar>
#include <QLabel>
#include <memory>
#include <array>
#include "mainwindow.h"
#include "common.h"
#include "cell.h"

void set_background_color(QLabel *label, State state);

class Field
{
private:
    std::array<std::array<Cell*, SIZE>, SIZE> field_cells; // = {{}};
    QStatusBar *status_bar;
    QLabel *current_player_label;
    std::array<QLabel*, PLAYERS> score_labels;

    State current_player = 1;
    unsigned turn_left = MAX_TURN;

    bool in_bound(int y, int x);
    bool is_valid(int y, int x);
    void switch_turn();
public:
    explicit Field(QStatusBar *status_bar, QLabel *current_player_label);
    void register_score_labels_to_ui(QHBoxLayout &layout);
    void register_cells_to_ui(QGridLayout &layout);
    void update_message();
    void clicked(int y, int x);

    friend void MainWindow::on_menu_new_triggered();
};

#endif // FIELD_H
