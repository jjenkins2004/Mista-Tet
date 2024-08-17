# Mista Tet

A personal remake of Tetris, where there are power-up balls that spawn to aid your fight against a villain named Mista Tet. Click on these powerups to collect them, but be careful, as some of these have tradeoffs that could help you or cost you the game. Some powerups will be collected and will immediately be used while others will go into your power-up storage. If your powerup storage is full, then newly collected powerups will disappear, but powerups that don't need storage will still have an effect.

The game will get progressively more challenging as blocks move faster and Tet uses stronger powerups more frequently. On the other hand, powerups will spawn more frequently as the game progresses, so be sure to use these wisely to defeat Tet.

Try to get your score to 100,000 as fast as possible to defeat Tet.

---

### How to Install

1. clone/download this repository
2. open bin folder
3. run the executable file for your system, MistaTet_win for Windows or MistaTet_mac for Mac

---

### Controls

| Key(s)         | Action                             |
|----------------|------------------------------------|
| **← →**        | Move left and right                |
| **↑**          | Rotate block                       |
| **↓**          | Move downward                      |
| **␣**          | Drop block                         |
| **C**          | Hold current block                 |
| **1, 2, 3**    | Use power-ups in corresponding slot|
| **P**          | Open pause menu                    |

---

**Thank you for playing!**  
For a more comprehensive guide on power-ups and Tet's powers, see the sections below.

<br>
<br>

# Player Power-ups

<br>

## Multiplier Power-ups

<br>

*These powerups affect your in-game multiplier, temporarily or permanently. Using these well can provide substantial boosts to your score.*

<br>

**-Permanent multipliers are permanent and add to your base multiplier-**

![+0.1 multiplier](bin/resources/powerup/0_1_multiplier.png)
: +0.1 multiplier

![+0.2 multiplier](bin/resources/powerup/0_2_multiplier.png)
: +0.2 multiplier

<br>

**-Temporary multipliers last for 30 seconds, and multiply your base multiplier-**

![1.2x multiplier](bin/resources/powerup/1_2_multiplier.png)
: x1.2 multiplier

![1.5x multiplier](bin/resources/powerup/1_5_multiplier.png)
: x1.5 multiplier

![1.2x multiplier](bin/resources/powerup/2_multiplier.png)
: x2 multiplier

<br>

**-Trade-off/debuff multipliers-**

![- multiplier](bin/resources/powerup/0_7_multiplier.png)
: x0.7 multiplier lasts 30 seconds but gives +0.025 permanent multiplier

![- multiplier](bin/resources/powerup/negative_multiplier.png)
: x-1 multiplier lasts 15 seconds but gives +0.05 permanent multiplier

<br>

**-Tips-**
- Temporary multipliers stack multiplicatively. EX: your base multiplier is x1.5 and you used a x2 and x1.5 temporary multiplier then your final multiplier would be (1.5 base)x(2x1.5) = 4.5.
- Permanent multipliers are additive so getting a +0.2 then a +0.1 will give a base multiplier of x1.3.
- Hold on to multipliers until high-scoring moments to maximize points.
- Permament multipliers and trade-off multipliers will immediately be used regardless of a full powerup storage but temporary multipliers are put into storage.

<br>

## Block Power-ups

<br>

*These Powerups go into your storage and change the next three spawned blocks upon activation.*

<br>

![I_block](bin/resources/powerup/I_block.png)
: replaces next three with I blocks

![J_block](bin/resources/powerup/J_block.png)
: replaces next three with J blocks

![L_block](bin/resources/powerup/L_block.png)
: replaces next three with L blocks

![O_block](bin/resources/powerup/O_block.png)
: replaces next three with O blocks

![T_block](bin/resources/powerup/T_block.png)
: replaces next three with T blocks

<br>

**-Tips-**
- These powerups aren't extraordinarily good but can be situationally good.
- Besides the I-block power-up it might not be worth holding on to these for long periods because they take up a storage space.

<br>

## Speed Changing Power-ups

<br>

*Changes how fast blocks fall in the game for 30 seconds.*

<br>

**-Slow down block speed-**

![slow_1](bin/resources/powerup/slow_1.png)
: slow down by 30%

![slow_2](bin/resources/powerup/slow_2.png)
: slow down by 50%

![pause](bin/resources/powerup/pause.png)
: blocks don't automatically fall

<br>

**-Increase block speed with a trade-off-**

![fast_1](bin/resources/powerup/fast_1.png)
: speed up by 15%, gives 30 second 1.5x multiplier

![fast_2](bin/resources/powerup/fast_2.png)
: speed up by 35%, gives 30 second 2x multiplier

<br>

**-Tips-**
- Slowing power-ups can be very good against some of Tet's powers in the late-game.
- Though using the speed increase power-ups give a very good boost, it can be very risky, especially in the late-game.
- Increase speed power-ups don't take storage space and are immediately activated while slowing power-ups go into storage.

<br>

## Board Changing Powerups

<br>

*Removes taken squares on the board and gives points/multipliers accordingly.*

<br>

**-Nuke-**

![nuke](bin/resources/powerup/nuke.png)
: obliterates everything on the board
- Very rare powerup that can be game-changing.
- Gives 15 points for every grid destroyed.
- Goes into storage when collected.

<br>

**-Bomb-**

![bomb](bin/resources/powerup/bomb.png)
: explode everything in a small radius
- Player can choose explosion location using arrow keys.
- Gives 35 points for every grid destroyed.

<br>

**-Lasers-**

![lasers](bin/resources/powerup/lasers.png)
:destroys three random rows
- Squares above destroyed rows will fall down.
- Gives points and a temporary multiplier based on how many grids were destroyed.

<br>

**-Tips-**
- These powerups though decent by themselves, are very good when combined with powerups that give multipliers.
- Having a nuke on standby at all times can be life-saving.
- Using the lasers power-up before a nuke or bomb on a full board can give a huge boost to points because of the multiplier given by the lasers power-up.

<br>

## Mystery Powerups

<br>

*Collecting these power-ups will yield other power-ups*

<br>

**-Mystery-**

![mystery](bin/resources/powerup/mystery.png)
: reveals a random power-up
- Reveals and automatically collects a new power-up the moment it's collected.
- Equal chance to get any power-up.

![5_random](bin/resources/powerup/5_random.png)
: spawns 5 random power-ups
- spawned power-ups use normal chances

<br>

**-Tips-**
- the mystery powerup can be a double-edged sword. Though it has a higher chance to give rare power-ups, it can also give power-ups with negative effects, which at the wrong time could mean game over.
- The 5 random power-up is extremely good. Just collect it every time you see it.
