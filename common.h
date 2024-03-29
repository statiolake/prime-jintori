﻿#ifndef COMMON_H
#define COMMON_H

#ifdef __MINGW32__
#define QSTRING_FROM(strlit) (QString::fromUtf8(strlit))
#else
#define QSTRING_FROM(strlit) (QString::fromLocal8Bit(strlit))
#endif

#include <QColor>
#include <array>

// 縦横のサイズ
constexpr const unsigned SIZE = 9;

// ターン。
constexpr const unsigned MAX_TURN = 5;

// 乱数をどこまで生成するか。とりあえず篩で PRIME_LIMIT 以下の乱数を生成するが、
// SIZE * SIZE に足りないとエラーとなる。
constexpr const unsigned PRIME_LIMIT = 800;

constexpr const unsigned PLAYERS = 4;
typedef unsigned State;
constexpr const State UNOWNED = 0;

const std::array<QColor, PLAYERS + 1> PLAYER_COLOR {
    {
        Qt::transparent,
        QColor(0xff, 0x63, 0x47), // red
        QColor(0x2e, 0x8b, 0x57), // green
        QColor(0xb0, 0xc4, 0xde), // blue
        QColor(0xf0, 0xe6, 0x8c), // yellow
    }
};

#ifdef ANDROID
#include <sstream>
// fallback
namespace std {
template <typename T>
std::string to_string(T value) {
    std::ostringstream ss;
    ss << value;
    return ss.str();
}
}
#endif

#endif // COMMON_H
