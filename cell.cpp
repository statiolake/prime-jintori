#include <cassert>
#include <QDebug>
#include <QString>
#include "common.h"
#include "cell.h"
#include "field.h"

Cell::Cell(Field *field, int y, int x, unsigned number)
    : QLabel(QString::fromStdString(std::to_string(number))),
      field(field), x(x), y(y), number(number), state(UNOWNED) {
    setup_QLabel();
}

Cell::Cell(Cell const &other)
    : QLabel(QString::fromStdString(std::to_string(other.number))),
      field(other.field), x(other.x), y(other.y), number(other.number), state(other.state) {
    setup_QLabel();
}

void Cell::setup_QLabel() {
    setAlignment(Qt::AlignCenter);
    setFont(QFont("Arial", 48));
    set_background_according_to_state();
}

void Cell::set_background_according_to_state() {
    set_background_color(this, state);
}

void Cell::paintEvent(QPaintEvent *event) {
    set_background_according_to_state();
    QLabel::paintEvent(event);
}

void Cell::mousePressEvent(QMouseEvent *event) {
    field->clicked(y, x);
    QLabel::mousePressEvent(event);
}
