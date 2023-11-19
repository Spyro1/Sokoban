# Szokoban - Felhasználói dokumentáció
> Készítette: Szenes Márton

## Játékról
A Szokoban egy olyan játék, ahol a játékosnak ládákat kell tologatni a játékterületen úgy, hogy azok a megfelelő helyre kerüljenek. A pálya viszont olyan, hogy könnyű betolni olyan helyre a ládákat, ahonnan már nem lehet elmozdítani őket. A pálya egy 2 dimenziós felülnézetű térben helyezkedik el. A játékterület (amin belül a ládák vannak elhelyezve és a játékos) mindig fallal van körbe véve, hogy ne tudjon azon kívül mozdulni egyik objektum se. Egy adott pálya akkor van teljesítve, ha az összes célmezőre egy-egy láda lett tolva. A Pályák nehézség szerinti sorrendben következnek egymás után.

## Program használata

### Főmenü
A program elindításakor a __[Főmenü](#főmenü)__ jelenik meg a terminálon. A programban mindig a bal alsó sarokban találhatóak meg a navigációs billentyűk amikkel lehet a kiválaszást vagy a játékost mozgatni vagy belemenni egy menüpontba, vagy kilépni belőle.  
A főmenüben a következő menüpontokat lehet találni:
- [Új játékos](#új-játékos)
- [Játék indítása](#játék-indítása)
- [Dicsőséglista](#dicsőséglista) 
- [Kilépés](#kilépés)

![Főmenü](./docs/mainmenu.PNG "Főmenü képernyőkép")

A menüben a `↑`(fel), `↓` (le), `→` (jobbra), `←` (balra) kurzor billentyűkkel lehet navigálni. Egy menüpont  kiválasztása az `Enter` billentyű lenyomásával lehetéséges. Visszalépni egy menüpontból az `Esc` vagy a `Backspace` billentyűkkel lehetséges. Ha a főképernyőn nyomjuk meg utóbbi billentyűk valamelyikét, akkor a [Kilépés](#kilépés) menü jön fel. 


#### Új játékos
Az *Új játékos* menüt kiválasztva megjelenik egy 20 karakter széles beivetli mező. Ide lehet bevinni a billentyűzetről az új játékos nevét, ami tartalmazhat ékezetes- és nem ékezetes karaktereket is. A kívánt név beírása után az `Enter`-t lenyomva elmenti a program a játékosnevet, és visszalép a [Főmenü](#főmenü)-be. Ha mégsem szeretne a felhasználó új nevet bevinni, akkor üres sornál nyomja meg az `Enter`-t. 

![Új játékos](./docs/addnewplayer.PNG "Új játékos képernyőkép")


#### Játék indítása
A _Játék indítása_ menüt kiválasztva megjelennek az adatbázisban szereplő játékosok és teljesített szintjeik száma. Amennyiben még egy játékos sem lett felvéve az adatbázisba, akkor a _"Nincs egy játékos sem még."_ felirat jelenik meg. A Játékos felvételéhez navigáljon az [Új játékos](#új-játékos) menübe. A játékosok között a kurzor billentyűkkel ( `↑`(fel), `↓` (le) ) lehet navigálni, és az `Enter` billentyűt lenyomva lehet kiválasztani egy játékost és elindítani a játékot.

| Üres játékoslista                                                                     | Nem üres játékoslista                                  |
|---------------------------------------------------------------------------------------|--------------------------------------------------------|
| ![Nincs egy játékos sem még](./docs/noexistingplayer.PNG "Nincs egy játékos sem még") | ![Játékos lista](./docs/playerlist.PNG "Játékoslista") |


#### Dicsőséglista
A _Dicsőséglista_ menüt kiválasztva megjelenik egy táblázat amiben a játékosok a egyes szinteken megtett lépések számát jeleníti meg. A fejlécben a játékosok nevei találhatóak, a bal szélső oszlopban pedig a szint sorszáma. 

![Dicsőséglista](./docs/ranklist.png "Dicsőséglista")


#### Kilépés
