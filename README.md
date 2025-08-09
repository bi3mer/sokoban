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
0 :: RB :: 0.14772572300000672
0 :: LL :: 1.474623485000022
---------

1 :: RB :: 0.21522973000009496
1 :: LL :: 1.599340028000003
---------

2 :: RB :: 0.12970251300000774
2 :: LL :: 1.5227354229999746
---------

3 :: RB :: 0.1282125590000794
3 :: LL :: 1.536196489999948
---------

4 :: RB :: 0.1277690120000718
4 :: LL :: 1.534694321999982
---------

5 :: RB :: 0.1287599760000841
5 :: LL :: 1.5301706139999938
---------

6 :: RB :: 0.12903205900009512
6 :: LL :: 1.510112820999951
---------

7 :: RB :: 0.12683329100005022
7 :: LL :: 1.516441060000004
---------
```

With pre-initialization of commands:

```
0 :: RB :: 0.27078426299998193
0 :: LL :: 0.20289853399996446
---------

1 :: RB :: 0.2733968709998832
1 :: LL :: 0.2690322259999415
---------

2 :: RB :: 0.2665183969999204
2 :: LL :: 0.20646241000000956
---------

3 :: RB :: 0.2653721439999482
3 :: LL :: 0.2073878790000423
---------

4 :: RB :: 0.2674320259999196
4 :: LL :: 0.2036483749999444
---------

5 :: RB :: 0.26680295599992315
5 :: LL :: 0.2060121819999826
---------

6 :: RB :: 0.2680215979999221
6 :: LL :: 0.2057220939999832
---------

7 :: RB :: 0.2680512249999293
7 :: LL :: 0.20656693600002643
---------
```

I am still a bit confused about why the ring buffer is slower than the linked list when everything is pre-initialized, but I haven't looked into it yet.