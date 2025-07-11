# Sokoban

This is a reimplementation of [Sokoban](https://en.wikipedia.org/wiki/Sokoban) with bitboards and ncurses. To run, you first need to build it. If you do not have [ncurses](https://invisible-island.net/ncurses/) installed, then you first need to do that. On mac, it is pretty easy: `brew install ncurses`.

## Building

```bash
make build
./a.out [[level-file]]
```

An example of the `[[level-file]]` on mac/linux is `levels/level-0.txt`.

## Coding

Instead of just coding the game, I decided to record multiple videos of me programming the game one step at a time:

- [Part 1: Bitboards](https://youtu.be/1qzPr5OpPOE?si=1ijM4O0X8vE1O1u7)
- [Part 2: ncurses](https://youtu.be/PHkmcQtTuxU?si=LB8c5zvodZrtva98)
- [Part 3: Organization](https://www.youtube.com/watch?v=ZO7vgxL8Zqo)
- [Part 4: Color and the Main Menu](https://www.youtube.com/watch?v=42ZuLIajps8)
- [Part 5: Restarting](https://www.youtube.com/watch?v=WjEhLT4OZLk)
- [Part 6: Menu Interaction](https://www.youtube.com/watch?v=CMHkWGfSPqg)
- [Part 7: State Machine Basics](https://www.youtube.com/watch?v=GkINdTax_Eg)
- [Part 8: Instructions](https://www.youtube.com/watch?v=pl2_XjE7QFA)
- [Part 9: Level Progression](https://www.youtube.com/watch?v=kqlvHfFpcj4)
- [Part 10: Grid View](https://www.youtube.com/watch?v=-jr-BNMy5K8)
- [Part 11: Saving](https://www.youtube.com/watch?v=dbooT1yTtI0)
- [Part 12: Logging](https://www.youtube.com/watch?v=UwbaxYBC5OI)