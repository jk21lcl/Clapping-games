# 拍手游戏 (Clapping Games)

本仓库收录了两款拍手游戏，分别为"__运__"(fortune)和"__攒豆__"(accumulating beans)，目前支持**单机多人分队模式**和**人机模式**(pvc)。

## 游戏玩法

### 基本玩法

拍手游戏为回合制游戏，每一回合每个玩家同时使出一种**技能**(option)，并进行结算，死亡玩家将会出局。玩家间可进行分队，当场上只剩余一个队伍时，该队伍获胜，游戏结束。

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
- `攒豆`(accumulate)：生产，获得1个`豆`，害怕各种攻击
- `单枪`(single shot)：普通攻击，消耗1个`豆`，指定1个目标打出1点攻击
- `双枪`(double shot)：普通攻击，消耗2个`豆`，指定1个或多个目标共打出2点攻击
- `三枪`(triple shot)：普通攻击，消耗3个`豆`，指定1个或多个目标共打出3点攻击
- `小防`(small defense)：防御，无消耗，能防住1点攻击
- `中防`(medium defense)：防御，无消耗，能防住恰好2点攻击
- `大防`(big defense)：防御，无消耗，能防住3点及以上攻击
- `超防`(super defense)：防御，消耗1个`豆`，能防住任何普通攻击
- `破超防`(break super defense)：特殊攻击，消耗1个`豆`，指定1个目标，若目标使用`超防`，则目标死亡，攻击者在此基础上获得2个`豆`，否则无法造成伤害；能防住1点攻击
- `kill`：特殊攻击，消耗5个`豆`，本回合无敌，并指定1个目标，若该目标非无敌状态，则死亡
- `反弹`(rebound)：防御，无消耗，若共受到1点普通攻击，则可防住，并将其反弹
- `双反弹`(double rebound)：防御，无消耗，若共受到恰好2点普通攻击，则可防住，并将其反弹
- `反反弹`(anti rebound)：辅助，无消耗，杀死场上所有使用`反弹`和`双反弹`的玩家
- `干扰`(disturb)：辅助，消耗1个`豆`，使得敌方所有`反弹`和`双反弹`失效 (被`干扰`后的`反弹`和`双反弹`依然能被`反反弹`打死)
- `嘲讽`(taunt)：辅助，消耗2个`豆`，将己方受到的所有普通攻击和`破超防`吸引到自己身上，并且获得等于`敌方存活玩家数量/敌方存活队伍数量`的盾，可以防住小于等于该数值的普通攻击；若己方有超过1人使用`嘲讽`，则吸引攻击的能力失效，仅有护盾能力
- `净化`(purify)：辅助，消耗2个`豆`，本回合无敌，并使得敌方所有`干扰`和`嘲讽`失效 (`嘲讽`也不再有护盾加成)

特殊游戏机制：
- 当场上有玩家出局时，所有玩家的`豆`不变，游戏继续，直到最终分出胜负
- 所有攻击需指明目标，可以攻击自己和队友，且**均会造成伤害**
- `对枪`：若两玩家之间有相互的普通攻击，则会对等抵消至一方纯攻击另一方
- `判定`：所有对同一玩家的普通攻击会进行叠加，并与其使用的防御进行比较
- 结算顺序：`净化`->`干扰`&`嘲讽`->`反弹`&`对枪`->`判定`->`反反弹`
- 可能会出现同归于尽的情况

## 代码结构

整个项目由cmake连接

游戏玩家由`Player`类维护，在`player.h`和`player.cpp`中

游戏内核的基类为`Game`，在`game.h`和`game.cpp`中

"运"由`Fortune`类维护，由`Game`继承而来，在`games/fortune.h`和`games/fortune.cpp`中

"攒豆"由`Bean`类维护，由`Game`继承而来，在`games/accumulating_beans.h`和`games/accumulating_beans.cpp`中

可执行文件在`main/main_fortune.cpp`和`main/main_accumulating_beans.cpp`中，用cmake build后可进行游玩

## 发展方向

- 添加新的游戏
- 给已有游戏增加新的技能、规则
- 设计GUI界面 (目前只能用终端来玩)
- 设计AI，优化人机模式中电脑策略 (目前电脑策略为纯随机)
- 使用rpc，实现联机对战模式 (目前仅支持单机)