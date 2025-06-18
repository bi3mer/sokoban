# Sokoban

This is a reimplementation of [Sokoban](https://en.wikipedia.org/wiki/Sokoban) with bitboards and ncurses. To run, you first need to build it. If you do not have [ncurses](https://invisible-island.net/ncurses/) installed, then you first need to do that. On mac, it is pretty easy: `brew install ncurses`.

```bash
make build
```

Then you, once it is built, you can run the game:

```bash
./a.out [[level-file]]
```

An example of the `[[level-file]]` on mac/linux is `levels/level-0.txt`.
