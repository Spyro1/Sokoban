# Szokoban - Programozói dokumentáció

> Készítette: Szenes Mártons

## Adatszerkezetek

### Enumerátorok

#### Mezőtípus (CellType)

```c
/* A pálya egyes mezőinek lehetséges értékei.*/
typedef enum celltype { null, EMPTY, WALL, TARGET, PLAYER, PLAYERONTARGET, BOX, BOXONTARGET } CellType;
```

#### Állapotok (State)

```c
/* A menü lehetséges állapotértékei */
typedef enum State { mainMenu, newPlayer, chosePlayer, rankList, exitApp, deletePlayer, editPlayer, game, exitGame, winGame } State;
```

### Struktúrák

#### Pozíció (Point)

```c
/* Egy koordinátát eltároló struktúra, mely láncolt listába fűzhető */
typedef struct position{
    int x, y;           // Koordinátái
    struct Point *next; // A következő pontra mutató pointer a láncolt listában
} Point;
```

#### Méret (Size)

```c
/* A pálya méretét eltároló struktúra */
typedef struct size{
    int width;  // A pálya szélessége
    int height; // A pálya magassága
} Size;
```

#### Lépés (Move)

```c
/* A játékos egy lépését tároló struktúra, mely láncolt listába fűzhető */
typedef struct move{
    Point from;         // A játékos által elhagyott mező koordinátája
    Point to;           // A játékos által meglépett mező koordinátája
    bool boxPushed;     // Logikai, eltolt-e a játékos a lépés során dobozt
    struct Move *next;  // a lépéseket tároló láncolt listában a következő elemre mutató pointer
} Move;
```

#### Szint statisztika (Statistics)

```c
/* A játékos egy szinten megtett lépéseinek számát tároló struktúra, mely láncolt listába fűzhető */
typedef struct statistic{
    int stepCount;           // Egy szinten a játékos által megtett lépések száma
    struct Statistics *next; // A statisztika láncolt listában a következő elemre mutató pointer
} Statistics;
```

#### Játékos (Player)

```c
/* A játékos adatait eltároló struktúra, mely láncolt listába fűzhető */
typedef struct player {
    char name[nameLenght*2+1];      // Játékosnév: maximum nameLenght hosszú (*2+1 az ékezetes karakterek és a \0 miatt)
    int numOfCompletedLevels;       // Teljesített szintek száma
    struct Statistics *levelStats;  // A teljesített szinteken megtett lépések száma láncolt listában
    struct Player *next;            // A játékos láncolt listában a következő elemre mutató pointer
} Player;
```

!!! bug Ez egy bug
!!! attention
!!! deprecated
!!! warning

## Algoritmusok

## Kód szerekzete

## Kód részeltes dokumentációja
