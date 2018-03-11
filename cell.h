#ifndef CELL_H
#define CELL_H

#include <QDebug>
#include <QLabel>
#include "common.h"

class Field;

class Cell : public QLabel {
    Q_OBJECT

private:
    Field *field;
    int x, y;
    unsigned number;
    State state;

    void setup_QLabel();
    void set_background_according_to_state();

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;

public:
    Cell(Field *field, int y, int x, unsigned number);
    Cell(Cell const &other);
    virtual ~Cell() { qDebug() << "running destructor for " << y << x; }

    unsigned get_number() const { return number; }

    void set_state(State s) { state = s; }
    State get_state() const { return state; }

signals:

public slots:
};

#endif // CELL_H
