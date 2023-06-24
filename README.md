# 拍手游戏 (Clapping Games)

本仓库收录了两款拍手游戏，分别为"__运__"(fortune)和"__攒豆__"(accumulating beans)，支持多人分队游戏。

## 游戏玩法

### 基本玩法

拍手游戏为回合制游戏，每一回合每个玩家同时使出一种"__技能__"(option)，并进行结算，死亡玩家将会出局。玩家间可进行分队，当场上只剩余一个队伍时，该队伍获胜，游戏结束。

拍手游戏一般会有一种类似"__能量__"的物品，会有一种技能用于积攒"能量"，"能量"可用来释放攻击，并有相应的防御可以抵挡这些攻击，可能会有一些其它的辅助技能与特殊机制。一般游戏开始时，所有玩家的"能量"均为0。

### 运

该游戏的"能量"为"`运`"(fortune)，该游戏的技能如下：
- `运`(fortune)：生产，获得1个`运`，害怕各种攻击
- `挡`(defend)：防御，无消耗
- `矛`(spear)：攻击，消耗1个`运`，会被`挡`抵消
- `双矛`(double spear)：攻击，消耗2个`运`，会被`挡`抵消，能打死`矛`
- `跟踪`(trace)：攻击，消耗3个`运`，能打死`挡`、`矛`、`双矛`
- `黄金`(gold)：防御，无消耗，能抵挡`双矛`和`跟踪`，会被`矛`打死

特殊游戏机制：
- 第一回合所有玩家必须出`运`
- 当场上有一位玩家出局时，剩余玩家的`运`归零，并从第一回合开始
- 每名玩家的攻击同时指向所有非队友玩家
- 对同一玩家的多个攻击分别结算

### 攒豆

该游戏的"能量"为"`豆`"(bean)，该游戏的技能如下：
- `攒豆`(accumulate)：生产，获得1个豆，害怕各种攻击
- `单枪`(single shot)：普通攻击，消耗1个豆，指定1个目标打出1点攻击
- `双枪`(double shot)：普通攻击，消耗2个豆，指定1个或多个目标共打出2点攻击
- `三枪`(triple shot)：普通攻击，消耗3个豆，指定1个或多个目标共打出3点攻击
- `小防`(small defense)：防御，无消耗，能防住1点攻击
- `中防`(medium defense)：防御，无消耗，能防住恰好2点攻击
- `大防`(big defense)：防御，无消耗，能防住3点及以上攻击
- `超防`(super defense)：防御，消耗1个豆，能防住任何普通攻击
- `破超防`(break super defense)：特殊攻击，消耗1个豆，指定1个目标，若目标使用超防，则目标死亡，攻击者在此基础上获得2个豆，否则无法造成攻击；能防住1点攻击
- `kill`：特殊攻击，消耗5个豆，本回合无敌，并指定1个目标，若该目标非无敌状态，则秒杀

特殊游戏机制：
- 当场上有玩家出局时，所有玩家的`豆`不变，游戏继续，直到最终分出胜负
- 所有攻击需指明目标，可以攻击自己和队友，攻击自己不会造成伤害，攻击队友会造成伤害
- 若两玩家之间有相互的普通攻击，则会对等抵消至一方纯攻击另一方。抵消后，所有对同一玩家的普通攻击将会累加，并与其使用的防御进行结算

## 代码结构

整个项目由cmake连接

游戏玩家由`Player`类维护，在`player.h`和`player.cpp`中

游戏内核的基类为`Game`，在`game.h`和`game.cpp`中

"运"由`Fortune`类维护，由`Game`继承而来，在`games/fortune.h`和`games/fortune.cpp`中

"攒豆"由`Bean`类维护，由`Game`继承而来，在`games/accumulating_beans.h`和`games/accumulating_beans.cpp`中

main函数在`main/main_fortune.cpp`和`main/main_accumulating_beans.cpp`中

## 发展方向

- 添加新的游戏
- 给已有游戏增加新的技能、规则
- 设计GUI界面
- 设计AI，实现人机模式
- 使用rpc，实现联机对战模式