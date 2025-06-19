#pragma once

struct Point {
    int x;
    int y;
};

inline Point point_add(const Point p1, const Point p2) {
    return {
        .x = p1.x + p2.x,
        .y = p1.y + p2.y
    };
}