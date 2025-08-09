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
#include <print>

#include "ring_buffer.hpp"

int main() {
    RingBuffer<float, 1> rb1;

    assert(rb1.back == 0);

    rb1.push_back(32.f);
    assert(rb1.back == 0);
    assert(rb1[0] == 32.f);

    rb1.push_back(-1.1f);
    assert(rb1.back == 0);
    assert(rb1[0] == -1.1f);

    RingBuffer<int, 10> rb2;
    for(int i = 0; i < 10; ++i) {
        rb2.push_back(i);

        assert(rb2.back == (i+1) % 10);
        assert(rb2[i] == i);
    }

    rb2.push_back(10);
    assert(rb2.back == 1);
    assert(rb2[rb2.capacity() - 1] == 10);
}
```

## Ring Buffer vs Pooled Linked List Speed Test

```c++
#include "PooledLinkedList.hpp"
#include "command.hpp"
#include "constants.hpp"
#include "sokoban.hpp"
#include "ring_buffer.hpp"
#include "sokoban.hpp"

#include <chrono>
#include <cstddef>
#include <print>

// show a version where we pass the containers already constructed
void test_linked_list(Sokoban& state, PooledLinkedList<Command>& commands) {
    // PooledLinkedList<Command> commands(MAX_COMMANDS);
    std::size_t i;
    for (i= 0; i < 1000; ++i) {
        commands.push_back(sokoban_update(state, {-1, 0}));
        if (commands.count >= MAX_COMMANDS) {
            commands.pop_front();
        }
    }

    for(i = 0; i < 100; ++i) {
        commands.pop_back();
    }

    for (i= 0; i < 1000; ++i) {
        commands.push_back(sokoban_update(state, {-1, 0}));
        if (commands.count >= MAX_COMMANDS) {
            commands.pop_front();
        }
    }

    for(i = 0; i < 100; ++i) {
        commands.pop_back();
    }
}

void test_ring_buffer(Sokoban& state, RingBuffer<Command, MAX_COMMANDS>& commands) {
    // RingBuffer<Command, MAX_COMMANDS> commands;

    std::size_t i;
    for (i= 0; i < 1000; ++i) {
        commands.push_back(sokoban_update(state, {-1, 0}));
    }

    for(i = 0; i < 100; ++i) {
        commands.pop_back();
    }

    for (i= 0; i < 1000; ++i) {
        commands.push_back(sokoban_update(state, {-1, 0}));
    }

    for(i = 0; i < 100; ++i) {
        commands.pop_back();
    }
}

int main(int argc, char* argv[]) {
    const char* files[8] = {
        "levels/00.txt",
        "levels/01.txt",
        "levels/02.txt",
        "levels/03.txt",
        "levels/04.txt",
        "levels/05.txt",
        "levels/06.txt",
        "levels/07.txt",
    };

    RingBuffer<Command, MAX_COMMANDS> rb;
    PooledLinkedList<Command> ll(MAX_COMMANDS);

    for (std::size_t i = 0; i < 8; ++i) {
        Sokoban level;
        sokoban_init_from_level(level, files[i]);

        double rb_ms_played = 0;
        double ll_ms_played = 0;

        for (std::size_t i = 0; i < 50000; ++i) {
            sokoban_restart(level);
            rb.clear();

            const auto start = std::chrono::steady_clock::now();
            test_ring_buffer(level, rb);
            const auto end = std::chrono::steady_clock::now();
            std::chrono::duration<double> duration = end - start;
            rb_ms_played += std::chrono::duration<double>(end - start).count();
        }

        std::println("{} :: RB :: {}", i, rb_ms_played);

        for (std::size_t i = 0; i < 50000; ++i) {
            sokoban_restart(level);
            ll.clear();

            const auto start = std::chrono::steady_clock::now();
            test_linked_list(level, ll);
            const auto end = std::chrono::steady_clock::now();
            std::chrono::duration<double> duration = end - start;
            ll_ms_played += std::chrono::duration<double>(end - start).count();
        }

        std::println("{} :: LL :: {}", i, ll_ms_played);
        std::println("---------\n");
    }
}
```

Without pre-initialization of commands:

```
0 :: RB :: 0.13944264600003037
0 :: LL :: 0.4807033420000271
---------

1 :: RB :: 0.1967984360000794
1 :: LL :: 0.501009051999928
---------

2 :: RB :: 0.1171782660000034
2 :: LL :: 0.4877247290000035
---------

3 :: RB :: 0.11815249400000684
3 :: LL :: 0.4850949109999765
---------

4 :: RB :: 0.11622457399999808
4 :: LL :: 0.4839723399999946
---------

5 :: RB :: 0.11752556199999858
5 :: LL :: 0.48551821699998554
---------

6 :: RB :: 0.11711630399999416
6 :: LL :: 0.48217555499997977
---------

7 :: RB :: 0.116127668999997
7 :: LL :: 0.4863683730000234
---------
```

With pre-initialization of commands:

```
0 :: RB :: 0.27752394899996174
0 :: LL :: 0.34718928800000426
---------

1 :: RB :: 0.26417184500008023
1 :: LL :: 0.375724840999987
---------

2 :: RB :: 0.2519864819999229
2 :: LL :: 0.3456509490000068
---------

3 :: RB :: 0.2506242889998991
3 :: LL :: 0.34381699900001345
---------

4 :: RB :: 0.2516522019999095
4 :: LL :: 0.35810339599999064
---------

5 :: RB :: 0.2530990899999283
5 :: LL :: 0.3487735419999974
---------

6 :: RB :: 0.254171441999931
6 :: LL :: 0.34815720900000474
---------

7 :: RB :: 0.2549551989999359
7 :: LL :: 0.34796430299999614
---------
```

I am still a bit confused about why the ring buffer is slower than the linked list when everything is pre-initialized, but I haven't looked into it yet.