# Szokoban - Programozói dokumentáció
> Készítette: Szenes Mártons
## Program felépítése
A program két fő részből áll, a menürendszerbő ([menu.h](#menuh)) és a játékból ([game.h](#gameh)). Az indításkor először a menürendszer nyílik meg, és onnan tud a felhasználó tovább navigálni a játék elindításáig.
A menü állapotait egy [`enum State {...}`](#menü-állapotai-state)-ben tárolja, mivel véges számú állapota lehet a menünek, és ezáltal könnyű azonosítani az egyes menüpontokat. A menü aktuális állapotát egy `State` változó tárolja, amit az egyes menüpontok kiválasztásakor változtat a program.
A program indítása után a `main`-ben beállítja a program a karakterkódolást, és meghívja a [`void menu_MainScreen() {...}`]()-t. Ez az eljárás futtatja ciklikusan a menüt, amíg ki nem lép a felhasználó a programból.

### A menü felépítése
```c
void menu_MainScreen(){
    // Konstansok  és változók inicializálása
    ...
    // Főcím kiiratása
    menu_PrintTitle();
    // Szintek mappájának beolvasása
    level_ReadDirectory("./levels/", &levelList, &numOfLevels);
    econio_rawmode(); // Billentyűérzékelés bekapcsolása

    // == MENÜ FUTTATÁSA ==
    while(runMenu){
        p = (Point) {center, 9}; // Koordináta beállítása
        prevOption = option; // Előző kijelölés eltárolása;
        
        // Lenyomott billentyű kiértékelése
        menu_KeyPress(...);
        // Képernyőre írás választott mód szerint
        menu_EvaluateState(...);
        // Billentyűlenyomásra vár, ha fut a menü
        if (runMenu && !displayFirst) key = econio_getch();
    }
    // Játékosok adatainak mentése
    player_WriteTxtFile(...);
    // Játékosok listájának felszabadítása
    player_FreePlayerList(...); // Player lista felszabadítása
    // Lefoglalt levelLista felszabadítása
    level_FreeLevelList(...);
}
```
## Adatstruktúrák


### Enumerátorok
#### Mezőtípusok (CellType)
```c
/* A pálya egyes mezőinek lehetséges értékei.*/
typedef enum celltype { null, EMPTY, WALL, TARGET, PLAYER, PLAYERONTARGET, BOX, BOXONTARGET } CellType;
```

#### Menü állapotai (State)

```c
/* A menü lehetséges állapotértékei */
typedef enum State { mainMenu, newPlayer, chosePlayer, rankList, exitApp, deletePlayer, editPlayer, game, exitGame, winGame } State;
```

### Struktúrák

#### Pozíció (Point)

```c
/** Egy koordinátát eltároló struktúra, mely láncolt listába fűzhető */
typedef struct point{
int x, y;           // Koordináták
struct Point *next; // A következő pontra mutató pointer a láncolt listában
} Point;
```

#### Méret (Size)

```c
/** A pálya méretét eltároló struktúra */
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
## Kód szerekzete

## Kód részeltes dokumentációja

## Algoritmusok
### Menu.h

---
<!-- BEGIN DOC-COMMENT H4 headers/menu.h -->

#### `void menu_MainScreen()`

A főmenüt futtató függvény. Egyszer hivandó meg a mainben

#### `static void menu_PrintTitle()`

Kiirítja a főcímet a képernyő tetejére nagy betűkkel több sorosan

#### `static void menu_KeyPress(int key, State *state, bool *displayFirst, int *option, int *selectedPlayer, bool *runMenu, Player *currentPlayer, Player **playerListHead, int *numOfPlayers, char **levelList, int numOfLevels)`

Kiértékeli a felhasználó által lenyomott billentyőt, és megváltoztatja a program állapotát aszerint
**Parameters:**

* `A` — `key` — lenyomott billentyű kódja
* `A` — `state` — program állapotát tároló érték (Cím szerint)
* `Először` — `displayFirst` — megy-e be a ciklus a menüpontba tulajdonság (Cím szerint)
* `A` — `option` — kiválasztott opció (Cím szerint)
* `A` — `selectedPlayer` — kiválasztott játékos indexének változója (Cím szerint)
* `Menü` — `runMenu` — futtatását beállító logikai változó (Cím szerint)
* `A` — `currentPlayer` — kiválasztott játékos struktúrájára mutató pointer, playerList elem
* `A` — `playerListHead` — játékosok adatait tartalmazó láncolt lista (Cím szerint)
* `A` — `numOfPlayers` — játékosok darabszáma (Cím szerint)
* `A` — `levelList` — szintek fájlnevét tároló dinamikus string tömb

#### `static void menu_EvaluateState(int *key, State *state, bool runMenu, bool *displayFirst, int option, int prevOption, Player **playerListHead, Player **currentPlayer, int *numOfPlayers, int selectedPlayer, Point p, int *linesPrinted)`

Az aktuális állapotnak megfelelően végrehajtja a szükséges utasításokat, és kiértékeli a bemeneteket
**Parameters:**

* `A` — `key` — lenyomott billentyű kódja (Cím szerint)
* `A` — `state` — program állapotát tároló érték (Cím szerint)
* `Fut-e` — `runMenu` — a menü
* `Először` — `displayFirst` — megy-e be a ciklus a menüpontba tulajdonság (Cím szerint)
* `A` — `option` — kiválasztott opció
* `Az` — `prevOption` — előző kiválasztott opció
* `A` — `playerListHead` — játékosok adatait tartalmazó láncolt lista (Cím szerint)
* `A` — `currentPlayer` — kiválasztott játékos struktúrájára mutató pointer, playerList elem
* `A` — `numOfPlayers` — játékosok darabszáma (Cím szerint)
* `Az` — `selectedPlayer` — aktuálisan kiválasztott játékos indexe
* `A` — `p` — kiíráshoz legfelső középső pont a képernyőn
* `Az` — `linesPrinted` — előző menüpontba a képernyőre írt sorok száma

#### `static void menu_ResetMenuVars(bool *displayFirst, int *option, int *selectedPlayer)`

Visszaállítja a kezdőértékeket a menüben, ha menüpont váltás volt
**Parameters:**

* `bool*` — `displayFirst` — Először megy-e be a ciklus a menüpontba tulajdonság
* `int*` — `option` — A kiválasztott opciók változója
* `int*` — `selectedPlayer` — A kiválasztott játékos indexének változója

#### `static void menu_PrintNavControls(bool displayFirst, State state)`

Kiírja a képernyő aljára az aktuális menüpot nevigációs lehetőségeit
**Parameters:**

* `bool` — `displayFirst` — Először megy-e be a ciklus a menüpontba tulajdonság
* `State` — `state` — A program állapotát tároló érték

<!-- END DOC-COMMENT -->

### Game.h

---
<!-- BEGIN DOC-COMMENT H4 headers/game.h -->

#### `bool game_Init(Player *player, char **levelList)`

Ezzel kell meghívni a játékot. Inicializálja a játékhoz szükséges elemeket
**Parameters:**

* `Player*` — `player` — Az aktuális játékos adatait tartalmazza (Cím szerint)
* `char[][]` — `levelList` — A pályák fájlneveit tartalmazó string tömb

**Returns:** `bool` — Igaz, ha a játékos teljesítette a szintet; Hamis, ha a játékos kilépett a játékból

#### `static bool game_StartGame(Player *player, char levelName[])`

Ezzel a fügvénnyel indítja el ésfuttatja a játékot
**Parameters:**

* `Az` — `player` — aktuális játékos adatait tartalmazza (Cím szerint)
* `A` — `levelName` — betöltendő pálya fájlneve

**Returns:** `Logikai:` — Igaz, ha a játékos teljesítette a szintet; Hamis, ha a játékos kilépett a játékból

#### `static bool game_CheckWin(CellType **map, Size mapSize)`

Ellenőrzi, hogy a játékos teljesítette-e a szintet, vagyis, hogy minden doboz a helyére került-e
**Parameters:**

* `A` — `map` — pályát leíró 2D-s dinamikus mátrix
* `A` — `mapSize` — pálya szélességét és magasságát leíró struktúra

**Returns:** `Logikai:` — Igaz, ha minden doboz a helyére került; Hamis, ha van egy doboz is, ami nincs a helyén

#### `static bool game_MovePlayer(CellType ***map, Point *currentPosition, Point **boxPositions, Point direction, Move **movesListHead)`

A játékos elmozdulását tesztelő függvény a direction irányba. Ha lehetséges a lépés, vagy ha doboz van a lépés irányában akkor a dobozt eltolja, és igazat ad vissza; Hamis, ha nem lehetséges a lépés
**Parameters:**

* `A` — `map` — pályát leíró 2D-s dinamikus mátrix (Cím szerint)
* `A` — `mapSize` — pálya szélességét és magasságát leíró struktúra
* `A` — `currentPosition` — játékos aktuális koordinátája a pályán (map-en) (Cím szerint)
* `A` — `boxPositions` — dobozok koordinátáinak dinamikus tömbje (Cím szerint)
* `A` — `direction` — játékos elmozdulásvektora
* `A` — `movesListHead` — játékos lépéseit eltároló láncolt lista (Cím szerint)

**Returns:** `Logikai:` — Igaz, ha el tud mozdulni a játékos az adott irányba; Hamis, ha nem lehetséges a lépés

#### `static bool game_UndoMove(CellType ***map, Point *currentPosition, Point **boxPositions, Move **moveListHead)`

Visszavonja a játékos előző lépést. Egészen addig fut le sikeresen, amíg a moveList-ben volt elem, azaz meglépett lépés.
**Parameters:**

* `A` — `map` — pályát leíró 2D-s dinamikus mátrix (Cím szerint)
* `A` — `currentPosition` — játékos aktuális koordinátája a pályán (map-en) (Cím szerint)
* `A` — `boxPositions` — dobozok koordinátáinak dinamikus tömbje (Cím szerint)
* `A` — `moveListHead` — játékos lépéseit eltároló láncolt lista (Cím szerint)

**Returns:** `Logikai:` — Igaz, ha sikeresen visszavonta a lépést; Hamis, ha nincs több visszavonható lépés

#### `static void game_ReadXSBFile(char filename[], CellType ***map, Size *mapSize, Point *playerPosition, Point **boxPositions, int *boxCount)`

Beolvassa a kapott fájlnévben lévő pályát és eltárolja a map mátrixban
**Parameters:**

* `A` — `filename` — pálya fájlneve (Bemenet)
* `A` — `map` — pályát leíró 2D-s dinamikus mátrix (Cím szerint, kimenet)
* `A` — `mapSize` — pálya szélességét és magasságát leíró struktúra (Cím szerint, kimenet)
* `A` — `playerPosition` — játékos kezdő koordinátája a pályán (map-en) (Cím szerint, kimenet)
* `A` — `boxPositions` — dobozok koordinátáinak dinamikus tömbje (Cím szerint, kimenet)
* `A` — `boxCount` — dobozok koordinátáit tároló dinamikus tömb elemszáma, dobozok száma a pályán (Cím szerint, kimenet)

#### `static CellType game_ConvertInputCharToCellType(char character)`

A beolvasott fájl egy karakterét értelmezi és átalakítja cellType értékké
**Parameters:**

* `A` — `character` — beolvasott fájl egy karaktere

**Returns:** `A` — kapott karakter értelmezett CellType értékekké alakított értéke

#### `static void game_PrintSimpleMap(CellType **map, Size mapSize)`

Kiírja a bal felső sarokba a pályát. Csak tesztelésre való
**Parameters:**

* `A` — `map` — pályát leíró 2D-s dinamikus mátrix
* `A` — `mapSize` — pálya szélességét és magasságát leíró struktúra

#### `static void game_PrintStyledMap(CellType **map, Size mapSize)`

Kiiratja a pályát a képernyőre színesen
**Parameters:**

* `A` — `map` — pályát leíró 2D-s dinamikus mátrix
* `A` — `mapSize` — pálya szélességét és magasságát leíró struktúra

#### `static void game_PrintPosition(CellType **map, Point pos)`

Egy kapott koordinátán lévő mezőt írja ki a képernyőre színesen és a megfelelő definiált karaterrel
**Parameters:**

* `A` — `map` — pályát leíró 2D-s dinamikus mátrix
* `A` — `pos` — kiirandó karakter koordinátája

#### `static void game_PrintStatsAndNav(Size mapSize, int numOfSteps, int level)`

Kiírja a képernyőre a játéktér mellé az aktuális szintet és a lépések számát, vagy a tutorial pályánál a bevezető instrukciókat
**Parameters:**

* `A` — `mapSize` — pálya szélességét és magasságát leíró struktúra
* `A` — `numOfSteps` — szinten megtett lépések száma
* `Az` — `level` — aktuális szint száma

#### `static void game_AllocateMemoryToMap(CellType ***map, Size *mapSize)`

Memóriát foglal a pályát tároló 2D-s dinamikus mátrixnak (map-nek)
**Parameters:**

* `A` — `map` — pályát leíró 2D-s dinamikus mátrix (Cím szerint)
* `A` — `mapSize` — pálya szélességét és magasságát leíró struktúra

#### `static void game_AllocateDynamicArray(Point **newArray, int lenght)`

Memóriát foglal egy 1D-s dinamikus tömbnek
**Parameters:**

* `Az` — `newArray` — új 1D-s dinamikus tömb címe (Cím szerint)
* `A` — `lenght` — létrehozandó dinamikus tömb hossza, elemszáma

#### `static void game_FreeAllocatedMemoryFromMap(CellType ***map)`

Felszabadítja a pályát tároló 2D-s dinamikus mátrix lefoglat memóriáját, ha volt lefoglalva
**Parameters:**

* `A` — `map` — pályát leíró 2D-s dinamikus mátrix (Cím szerint)

#### `static void game_FreeDynamicArray(Point **dynamicArray)`

Felszabadítja egy 1D-s dinamikus tömbnek lefoglalt memóriáját, ha volt lefoglalva
**Parameters:**

* `1D-s` — `dynamicArray` — dinamikus tömb címe (Cím szerint)

<!-- END DOC-COMMENT -->

### Level.h

---
<!-- BEGIN DOC-COMMENT H4 headers/level.h -->

#### `void level_ReadDirectory(char directory[], char **levelList[], int *numOfFiles)`

Beolvassa a megadott mappából a fájlneveket, és eltárolja egy dinamikusan foglalt tömbben
**Parameters:**

* `char[]` — `directory` — A mappa elérési útvonala
* `*char[][]` — `levelList` — A dinamikusan foglalt string tömb (Cím szerint)
* `int*` — `numOfFiles` — A Beolvasott fájlnevek száma (Cím szerint)

#### `void level_FreeLevelList(char **levelList[], int *numOfLevels)`

Felszabadítja a dinamikusan foglalt fájlnevek string tömbjét
**Parameters:**

* `*char[][]` — `levelList` — A dinamikusan foglalt string tömb (Cím szerint)
* `int*` — `numOfLevels` — A Beolvasott fájlnevek száma (Cím szerint)

<!-- END DOC-COMMENT -->
### Lib.h

---
<!-- BEGIN DOC-COMMENT H4 headers/lib.h -->

#### `void lib_WarningWindow(const char Message[], Point p, bool *displayFirst, int option, EconioColor baseColor, EconioColor accentForeColor, EconioColor accentBgColor)`

Kiír a képernyőre egy figyelmeztető ablakot a megadott Message üzenettel, Igen/Nem válaszlehetőségekkel
**Parameters:**

* `char[]` — `Message` — Az üzenet
* `Point` — `p` — képernyő közepének koordinátája
* `bool*` — `displayFirst` — Először megy-e be a ciklus a menüpontba tulajdonság (Cím szerint)
* `int` — `option` — A kiválasztott opció
* `EconioColor` — `baseColor` — Alap betűszíne az ablaknak
* `EconioColor` — `accentForeColor` — Kijelölt opció betűszíne
* `EconioColor` — `accentBgColor` — Kijelölt opció háttérszíne

#### `void lib_ClearScrBellow()`

Letörli a cím alatt lévő területet a képernyőről

#### `void lib_ClearScreenSection(int x1, int y1, int x2, int y2, EconioColor bgColor)`

Letörli a képernyőt megadott koordinátákon belül a kapott bgColor színnel.
**Parameters:**

* `int` — `x1` — Bal felső sarok x koordinátája
* `int` — `y1` — Bal felső sarok y koordinátája
* `int` — `x2` — Jobb alsó sarok x koordinátája
* `int` — `y2` — Jobb alsó sarok y koordinátája
* `EconioColor` — `bgColor` — Törlendő terület háttérszíne

#### `void lib_printError(const char errormessage[])`

Kiírja képernyőre a hibaüzenetet
**Parameters:**

* `char[]` — `errormessage` — Hibaüzenet

#### `void print(char const str[], int x, int y)`

Kiírja a képernyőre a kapott szöveget a megadott kezdő koordinátákra
**Parameters:**

* `char[]` — `str` — Kiírandó szöveg
* `int` — `x` — x koordináta a képernyőn
* `int` — `y` — y koordniáta a képernyőn

#### `void printfc(char const str[], int x, int y, EconioColor foreColor)`

Kiírja a képernyőre a kapott szöveget a megadott kezdő koordinátákra a megadott betűszínnel
**Parameters:**

* `char[]` — `str` — Kiírandó szöveg
* `int` — `x` — x koordináta a képernyőn
* `int` — `y` — y koordniáta a képernyőn
* `EconioColor` — `foreColor` — A szöveg színe

#### `void printfbc(char const str[], int x, int y, EconioColor foreColor, EconioColor bgColor)`

Kiírja a képernyőre a kapott szöveget a megadott kezdő koordinátákra a megadott betűszínnel és háttérszínnel
**Parameters:**

* `char[]` — `str` — Kiírandó szöveg
* `int` — `x` — x koordináta a képernyőn
* `int` — `y` — y koordniáta a képernyőn
* `EconioColor` — `foreColor` — A szöveg színe
* `EconioColor` — `bgColor` — A szöveg háttérszíne

#### `Point addPoints(Point a, Point b)`

Két pont koordinátáit összeadó függvény
**Parameters:**

* `Point` — `a` — Egyik koordináta
* `Point` — `b` — Másik koordináta

**Returns:** `Point` — Koordináták összege

#### `Point subPoints(Point a, Point b)`

Két pont koordinátáinak különbségét kiszámoló függvény
**Parameters:**

* `Point` — `a` — Egyik koordináta
* `Point` — `b` — Másik koordniáta

**Returns:** `Point` — Koordináták különbsége

#### `bool comparePoints(Point a, Point b)`

Két pontot hasonlít össze, hogy egyenlőek-e
**Parameters:**

* `Point` — `a` — Egyik koordináta
* `Point` — `b` — Másik koordináta

**Returns:** `bool` — Egyenlőek e a paraméterként kapott koordináták

#### `int stringlenght(const char str[])`

Megszámolja, hogy a kapott string hány valós karakterből áll, hány krakter íródik ki a képernyőre
**Parameters:**

* `char[]` — `str` — Karaktertömb, string (Bemenet)

**Returns:** `int` — A string hossza megjelenített karakterszámban

#### `int stringlenghtMax(const char str[], int max)`

Megszámolja, hogy hány byte-on tárolódik a max karakterszámú string
**Parameters:**

* `char[]` — `str` — Karaktertömb, string (Bemenet)
* `int` — `max` — Megjelenítendő karakterek száma

**Returns:** `int` — Megjelenítendő string max karakterű byte hossza

#### `bool isBlankString(const char* str)`

Megnézi a függvény, hogy a string csak üres karaktereket tartalmaz-e (szóköz, \\n, \t)
**Parameters:**

* `char[]` — `str` — Karaktertömb, string (Bemenet)

**Returns:** `bool` — Csak üres karaktereket tartalmaz-e a string

<!-- END DOC-COMMENT -->

### Datatypes.h

---
<!-- BEGIN DOC-COMMENT H4 headers/datatypes.h -->

#### `typedef enum celltype`

A pálya egyes mezőinek lehetséges értékei

#### `typedef enum State`

A menü lehetséges állapotértékei

#### `typedef struct point`

Egy koordinátát eltároló struktúra, mely láncolt listába fűzhető

#### `int x, y`

Koordinátái

#### `struct Point *next`

< A következő pontra mutató pointer a láncolt listában
<!-- END DOC-COMMENT -->

### Player.h

---
<!-- BEGIN DOC-COMMENT H4 headers/player.h -->

#### `void player_ReadTxtFile(Player **playerListHead, int *numOfPlayers)`

Beolvassa a playerDataPath-ban megadott fájlt, és elátrolja a playerListHead láncolt listában
**Parameters:**

* `Player**` — `playerListHead` — A játékosok adatait tartalmazó láncolt lista (Cím szerint)
* `int*` — `numOfPlayers` — A játékosok darabszáma (Cím szerint)

#### `void player_WriteTxtFile(Player *playerListHead, int numOfPlayers)`

Kiírja fájlba a playeListHead-ben tárolt játékosok adatait: név;szintek;lépészámok..
**Parameters:**

* `Player*` — `playerListHead` — sA játékosok adatait tartalmazó láncolt lista (Cím szerint)
* `int` — `numOfPlayers` — A játékosok darabszáma (Cím szerint)

#### `Player *player_MakePlayer(char name[], int numOfLevels, Statistics *statsListHead)`

Létrehoz egy Player struktúrára mutató pointert a paraméterként kapott értékekből, hogy aztán Lístába lehessen fűzni.
**Parameters:**

* `char[]` — `name` — A játékos neve (max 20 karakter)
* `int` — `numOfLevels` — A játékos által teljesített szintek száma
* `Statistics` — `statsListHead` — A játékos lépésstatisztikájának láncolt listája

**Returns:** `Player*` — Player struktúrára mutató pointer a kapott adatokkal

#### `void player_FreePlayerList(Player **playerListHead)`

Felszabadítja a az egész láncolt listának foglalt memóriát
**Parameters:**

* `Player**` — `playerListHead` — A játékosok adatait tartalmazó láncolt lista (Cím szerint)

#### `static void player_FreePlayerNode(Player **playerNode)`

Felszabadítja egy elem lefoglalt memóriáját a listából
**Parameters:**

* `Player**` — `playerNode` — Egy Player struktúrára mutató pointer a láncolt listából (Cím szerint)

#### `void player_AddPlayerToEnd(Player *newPlayer, Player **playerListHead, int *numOfPlayers)`

Beszúrja a játékoslistának a végére az új játékos elemet
**Parameters:**

* `Player*` — `newPlayer` — Új játékos struktúrájára mutató pointer
* `Player**` — `playerListHead` — A játékosok adatait tartalmazó láncolt lista (Cím szerint)
* `int*` — `numOfPlayers` — A játékosok darabszáma (Cím szerint)

#### `void player_AddPlayerInOrder(Player *newPlayer, Player **playerListHead, int *numOfPlayers)`

Beszúrja a játékoslistába au új játékost a nevének a hossza szerint növekvő sorrendben
**Parameters:**

* `Player*` — `newPlayer` — Új játékos struktúrájára mutató pointer
* `Player**` — `playerListHead` — A játékosok adatait tartalmazó láncolt lista (Cím szerint)
* `int*` — `numOfPlayers` — A játékosok darabszáma (Cím szerint)

#### `bool player_RemovePlayer(Player *removablePlayer, Player **playerListHead, int *numOfPlayers)`

Törli a paramterként kapott játékost a listából
**Parameters:**

* `Player*` — `removablePlayer` — A törlendő játékos struktúrájára mutató pointer
* `Player**` — `playerListHead` — A játékosok adatait tartalmazó láncolt lista (Cím szerint)
* `int*` — `numOfPlayers` — A játékosok darabszáma (Cím szerint)

**Returns:** `bool` — Igaz, ha sikeres a törlés a listából; Hamis, ha nem sikerült törölni a játékost

#### `Player* player_GetSelectedPlayer(Player *playerListHead, int selectedPlayer)`

Megkeresi a listában a selectedPlayer-edik elemet
**Parameters:**

* `Player*` — `playerListHead` — A játékosok adatait tartalmazó láncolt lista
* `int` — `selectedPlayer` — A játékos sorszáma / indexe a listában

**Returns:** `Player*` — A keresett játékos struktúrájára mutató pointer, ha megtalálta, különben NULL pointer

#### `int player_GetIndexOfPlayer(Player *playerListHead, char name[])`

Megkeresi a listában a játékos nevét, és visszaadja a sorszámát / indexét  a listában
**Parameters:**

* `Player*` — `playerListHead` — A játékosok adatait tartalmazó láncolt lista
* `char[]` — `name` — A keresett játékos neve

**Returns:** `int` — A keresett játékos indexe, ha megtalálta, különben -1

#### `void player_PrintPlayerList(Player *playerList, int selectedPlayerIndex, Point p)`

Kiírja a képernyőre a játékoslistát (nevüket és szintjüket) egymás alá, és kiemeli az aktuálisan kiválasztott játékost
**Parameters:**

* `Player*` — `playerList` — A játékosok adatait tartalmazó láncolt lista
* `int` — `selectedPlayerIndex` — Az aktuálisan kiválasztott játékos sorszáma / indexe
* `Point` — `p` — A kiíráshoz legfelső középső pont a képernyőn

<!-- END DOC-COMMENT -->

### Statistics.h

---
<!-- BEGIN DOC-COMMENT H4 headers/statistics.h -->

#### `void stats_AddLevelStatistics(int stepCount, Statistics **statsListHead)`

Beszúrja a paraméterként kapott stepCount értéket a statsListHead láncolt lista végére
**Parameters:**

* `int` — `stepCount` — A szinten megtett lépések száma
* `Statistics**` — `statsListHead` — A lépések számát tároló láncolt lista (Cím szerint)

#### `void stats_FreeStatisticsList(Statistics **statsListHead)`

Felszabadítja a az egész láncolt listának foglalt memóriát
**Parameters:**

* `Statistics**` — `statsListHead` — A lépések számát tároló láncolt lista (Cím szerint)

#### `static void stats_FreeStatNode(Statistics **statNode)`

Felszabadítja egy elem lefoglalt memóriáját a listából
**Parameters:**

* `Statistics**` — `statNode` — Egy Statistics struktúrára mutató pointer a láncolt listából (Cím szerint)

<!-- END DOC-COMMENT -->

### Move.h

---
<!-- BEGIN DOC-COMMENT H4 headers/move.h -->

#### `Move* move_CreateMove(Point stepfrom, Point stepTo, bool boxPushed)`

Létrehoz egy Move struktúrára mutató pointert a paraméterként kapott értékekből, hogy aztán Lístába lehessen fűzni.
**Parameters:**

* `Point` — `stepfrom` — A legutolsó pozíció koordinátája
* `Point` — `stepTo` — A következő pozíció koordinátája
* `bool` — `boxPushed` — Igaz, ha eltolt doboz; Hamis, ha csak a játékos mozdult el

**Returns:** `Move*` — Move strúktúrára mutató pointer a kapott paraméterekkel

#### `void move_AddMoveToList(Move *newMove, Move **moveListHead)`

Beszúrja a paraméterként kapott newMove elemet a láncolt lista (moveListHead) elejére
**Parameters:**

* `Move*` — `newMove` — Új elmozdulást tároló struktúrára mutató pointer
* `Move**` — `moveListHead` — Az elmozdulásokat tároló láncolt lista (Verem/Stack)

#### `Move move_RemoveMoveFromList(Move **moveListHead)`

Eltávolítja az első elemet a láncolt listából (Veremből/Stack)
**Parameters:**

* `Move**` — `moveListHead` — Az elmozdulásokat tároló láncolt lista (Verem/Stack) (Cím szerint)

**Returns:** `Move` — Visszaadja az eltávolított listaelem struktúráját

#### `void move_FreeMoveList(Move **moveListHead)`

Felszabadítja a az egész láncolt listának foglalt memóriát
**Parameters:**

* `Move**` — `moveListHead` — Az elmozdulásokat tároló láncolt lista (Verem/Stack) (Cím szerint)

#### `static void move_FreeNode(Move **moveNode)`

Felszabadítja egy elem lefoglalt memóriáját a listából
**Parameters:**

* `Move**` — `moveNode` — Egy Move struktúrára mutató pointer a láncolt listából (Cím szerint)

<!-- END DOC-COMMENT -->


> powershell:
> D:\Programozas\soko>
> & "C:\Users\Szenes Márton\node_modules\.bin\docblox2md" .\SsokobanDevDoc.md