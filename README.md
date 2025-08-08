# Sokoban

This is a reimplementation of [Sokoban](https://en.wikipedia.org/wiki/Sokoban) with bitboards and [ncurses](https://invisible-island.net/ncurses/).

## Building

You need [ncurses.](https://invisible-island.net/ncurses/) On mac, it's pretty easy: `brew install ncurses`. It will also be fairly easy to get ncurses on any Linux distro (e.g., something along the lines of `sudo apt-get install ncurses-dev` if you are on Ubuntu). However, I have no clue how easy or not it will be to install ncurses for Windows or any other OS.

```bash
make
```

## Running

After the game is succesfully built, you can run the game with:

```
./sokoban
```

## Other

Usually, I code a game and I have a repo to document the progress, and that is it. For this project, I went in a different direction and recorded myself while programming the whole thing. You can find the whole series on [YouTube.](https://www.youtube.com/watch?v=1qzPr5OpPOE&list=PLwaZncztKsRckZ0u3sKbwkZMtH1-ABkDR)


## Ring Buffer Test Code

```c++
#include <cassert>

#include "ring_buffer.hpp"

int main() {
    RingBuffer<float, 1> rb1;

    assert(rb1.index == 0);

    rb1.push(32.f);
    assert(rb1.index == 0);
    assert(rb1[0] == 32.f);

    rb1.push(-1.1f);
    assert(rb1.index == 0);
    assert(rb1[0] == -1.1f);

    RingBuffer<int, 10> rb2;
    for(int i = 0; i < 10; ++i) {
        rb2.push(i);

        assert(rb2.index == (i+1) % 10);
        assert(rb2[i] == i);
    }

    rb2.push(10);
    assert(rb2.index == 1);
    assert(rb2[0] == 10);
}
```