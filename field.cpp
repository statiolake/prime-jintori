#include "common.h"
#include "cell.h"
#include "field.h"

#include <QDebug>
#include <random>
#include <ctime>
#include <cmath>
#include <cassert>

static std::vector<unsigned> get_prime_list() {
    std::vector<unsigned> result;
    std::vector<bool> is_prime(PRIME_LIMIT + 1, true);
    for (unsigned p = 2; p <= PRIME_LIMIT; p++) {
        if (!is_prime[p]) continue;

        result.push_back(p);
        for (unsigned i = p+p; i <= PRIME_LIMIT; i += p) {
            is_prime[i] = false;
        }
    }
    return result;
}

Field::Field(QStatusBar *status_bar, QLabel *current_player_label)
    : status_bar(status_bar), current_player_label(current_player_label) {
    auto primes = get_prime_list();
    assert(primes.size() >= SIZE * SIZE);
    primes.resize(SIZE * SIZE);

    // std::random_device rnd;
    // qDebug() << "random" << rnd();
    // std::mt19937 eng(rnd());
    std::mt19937 eng((unsigned)time(NULL));
    std::shuffle(primes.begin(), primes.end(), eng);

    qDebug() << "initializing field_cells.";
    for (unsigned y = 0; y < SIZE; y++) {
        for (unsigned x = 0; x < SIZE; x++) {
            qDebug() << "now for" << y << x;
            field_cells[y][x] = new Cell(this, y, x, primes[y*SIZE + x]);
        }
    }
    qDebug() << "finished initializing field_cells";
    for (unsigned i = 0; i < PLAYERS; i++) {
        auto label = new QLabel("");
        label->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
        label->setAlignment(Qt::AlignCenter);
        label->setMargin(16);
        label->setFont(QFont("Arial", 36));
        score_labels[i] = label;
    }
}

void Field::update_message() {
    QString msg;
    State state;
    if (turn_left > 0) {
        msg = QString::fromUtf8("残り %1 ターンです！現在は %2P の手番です。").arg(turn_left).arg(current_player);
        state = current_player;
    } else {
        msg = QString::fromUtf8("ゲームは決着しました。メニューから新規ゲームを始めてください。");
        state = UNOWNED;
    }
    current_player_label->setText(msg);
    set_background_color(current_player_label, state);
    current_player_label->repaint();

    std::array<int, PLAYERS+1> scores {{}};
    for (unsigned y = 0; y < SIZE; y++) {
        for (unsigned x = 0; x < SIZE; x++) {
            scores[field_cells[y][x]->get_state()] += field_cells[y][x]->get_number();
        }
    }

    for (unsigned i = 0; i < PLAYERS; i++) {
        qDebug() << "player" << i+1 << "has" << scores[i+1] << "pts";
        score_labels[i]->setText(QString::fromStdString(std::to_string(scores[i+1])));
        set_background_color(score_labels[i], i+1);
        score_labels[i]->repaint();
    }
}

void Field::switch_turn() {
    if (turn_left > 0) {
        current_player %= PLAYERS;
        current_player += 1;
        if (current_player == 1) turn_left -= 1;
    }
    update_message();
}

bool Field::in_bound(int y, int x) {
    auto in_bound = [](int n) { return 0 <= n && static_cast<unsigned>(n) < SIZE; };
    return in_bound(y) && in_bound(x);
}

bool Field::is_valid(int y, int x) {
    assert(in_bound(y, x));
    return field_cells[y][x]->get_state() == current_player || field_cells[y][x]->get_state() == UNOWNED;
}

void Field::register_score_labels_to_ui(QHBoxLayout &layout) {
    for (unsigned i = 0; i < PLAYERS; i++) {
        layout.addWidget(score_labels[i]);
    }
}

void Field::register_cells_to_ui(QGridLayout &layout) {
    for (unsigned y = 0; y < SIZE; y++) {
        for (unsigned x = 0; x < SIZE; x++) {
            layout.addWidget(field_cells[y][x], y, x);
        }
    }
    update_message();
}

void Field::clicked(int y, int x) {
    if (turn_left == 0) return;
    qDebug() << "clicked for " << current_player;
    if (!is_valid(y, x)) {
        status_bar->showMessage(QString::fromUtf8("無効な場所です！"), 1000);
        return;
    }
    field_cells[y][x]->set_state(current_player);
    field_cells[y][x]->repaint();

    // 上
    for (int yy = y-1; in_bound(yy, x); yy--) {
        auto validness = is_valid(yy, x);
        field_cells[yy][x]->set_state(current_player);
        field_cells[yy][x]->repaint();
        if (!validness) break;
    }

    // 下
    for (int yy = y+1; in_bound(yy, x); yy++) {
        auto validness = is_valid(yy, x);
        field_cells[yy][x]->set_state(current_player);
        field_cells[yy][x]->repaint();
        if (!validness) break;
    }

    // 左
    for (int xx = x-1; in_bound(y, xx); xx--) {
        auto validness = is_valid(y, xx);
        field_cells[y][xx]->set_state(current_player);
        field_cells[y][xx]->repaint();
        if (!validness) break;
    }

    // 右
    for (int xx = x+1; in_bound(y, xx); xx++) {
        auto validness = is_valid(y, xx);
        field_cells[y][xx]->set_state(current_player);
        field_cells[y][xx]->repaint();
        if (!validness) break;
    }

    switch_turn();
}

void set_background_color(QLabel *label, State state) {
    QPalette palette = label->palette();
    label->setAutoFillBackground(true);
    palette.setColor(QPalette::Window, PLAYER_COLOR[state]);
    label->setPalette(palette);
}
