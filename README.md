# Követelmények

## Funkcionális követelmények

* Guest
    * Regisztráció
* Alap user
    * Belépés
        * Termék lista böngészése
        * Termékek keresése cím alapján
        * Részletes termék nézet megnyitása/nézése
            * Kommentek megtekintése és hozzáadása
            * Vásárlás/Letöltés
        * CAFF feltöltése
            * Nevet, árat adni
        * Felhasználói adatok módosítása
            * Fiók törlése (összes CAFF törlése)
        * saját CAFF-ok kezelése
            * törlés
            * ár/név változtatás
* Admin user
    * Belépés
    * User kezelés
        * törlés
        * módosítás
        * létrehozás
        * egyenleg kezelés: feltöltés, módosítás
    * Admin user kezelés
        * Új admin hozzáadása
        * Admin töröl
    * CAFF kezelése
        * törlés
        * adatmódosítás
    
[Use-case diagram + függőségi nyilak]

## Biztonsági követelmények és célok

A biztonsági követelményeket és célokat az alábbi kategóriák szerint csoportosíthatjuk. A fentebb részletezettek szerint három féle felhasználót különböztetünk meg: vendéget (nem bejelentkezett), felhasználót (bejelentkezett, normál), és adminisztrátor felhasználót. 

* Bizalmasság
    * Személyes adatokokat és az egyenleget csak az adott felhasználó illetve az adminisztrátorok láthatják.
    * A feltöltött CAFF fájlok csak bejelentkezés után tekinhetők meg.
* Integritás
    * Egy felhasználó személyes adatait csak adott felhasználó vagy admin módosíthaja.
    * Egy felhasználó egyenlegét közvetlenül csak egy admin módosíthatja. 
    * Egy feltöltött CAFF fájl adatait (nevét, árát) csak a tulajdonosa (aki feltötte) illetve admin módosíthatja.
* Elérhetőség
    * Az admininsztrátoroknak mindig el kell tudni érniük a rendszert.
    * A felhasználók is mindig eléhetik a rendszert, leszámítva az esetleges karbantartásokat.
* Autentikáció
    * Csak bejelentkezés után lehet elérni a rendszer felhasználói funkcióit (CAFF böngészés, vásárlás, feltöltés) és adminisztrátori funkcióit. 
    * Adminisztrátori fiókot csak meglévő Adminisztrátori fiókkal lehet létrehozni.
* Autorizáció
    * [[[]]]  admininisztrátori jogosultsághoz kötött
    * A webáruház elérése bejelentkezéshez kötött
* Auditiálás
    * Minden felhasználó minden tevékenységet naplózni kell.

## Threat assessment

### Asset-ek

A rendszerben az alábbi értéket képviselő tényezők (asset-ek) jelennek meg.

* Fizikai
    * Hardware
* Logikai
    * A felhasználók minden személyes adata
    * CAFF file-ok
    * a felhasználók egyenlegei

<p>
    <img src="figures/adatfolyam.png" width="800" title="Adatfolyam diagram">
</p>

### Támadó modell kidolgozása

A támadómodell kidolgozását a STRIDE kereterendszer alapján tehetjük meg. 

* Spoofing:
    * Social engineering támadás az adminok ellen.
    * Felhasználó admininsztrátori funkciókat próbál elérni.
    * Vendég próbál felhasználói funkciókhoz hozzáférni.
    * Adatbázis hozzáférés történik a rendszeren kívülről.
* Tampering
    * Sérül kommunikáció az adatbázisokkal.
    * Webes kommunikáció manipulálása.
    * Komponensek közötti kommunikáció módosítása.
    * Kártevő bejutása CAFF fájlokkal. (veszélyes a szerverre és arra is aki letölti a CAFF-ot) 
    * CAFF feldolgozásból adódó sérülékenységek (nem megfelelő parser logika, buffer overflow lehetőség az implementációban)
    * Logok módosítása. 
    * Egy felhasználó módosítja az egyenlegét. 
* Letagadás 
    * Egy user letagadja, hogy megkapta a pénzt egy másiktól.
    * User letagadja, hogy ő letörölte az értéket CAFF fájlját.
    * Admin kitöröl mindent és letagadja.
* Denial of Service
    * Fizikai támadás éri a szervert. 
    * Túl nagy fájl feltöltése.
    * DDoS támadás
    * Túl sok komment
    * Túl sok regisztráció.
* Information disclosure.
    * Felhasználói adatok kiszívárognak.
    * Egy felhasználó hozzáfér más adataihoz.
    * Jelszó kiszvirgás.
    * Admin hozzáfér a plaintext jelszavakhaz
    * Vendég felhasználó tudja böngészni a feltöltött CAFF listát.
    * Admin jelsző kiszivárog. 
* Elevation of privilige
    * Vendég, felhasználói jogosultságot szerez (bejelentkezés nélkül használja a rendszert).
    * Egy felhasználó adminisztrátori jogosultságot szerez.  

### Biztonsági követelmények és mechanizmusok

A fenti támadási felületek alapján az alábbi védekezések szükségesek.

Sok támadási felületet meg tudunk szüntetni a megfelelő autentikációval (jelszavas) és hozzáférés kezeléssel.  

A fizikai támadások illetve a túlterheléses támadások problémáját átháríjük egy felhő szolgáltatóra.

A letagadás jellegű fenyegetéseket részletes naplózással küszöböljük ki. 

A konkrét fenyegetésekre adott válaszokat az alábbi táblázatban részletezzük:

### Spoofing

|   Threat   | Biztonsági követelmény  |
|---|---|
| Spoofing | --- |
| Social engineering támadás az adminok ellen.  | Kívül esik a házi feladat határain.  |
| Felhasználó admininsztrátori funkciókat ér el (pl. pénzt ad magának) | Jelszavas bejelentkezés (jelsző követelmények). Megfelelő hozzáférés kezelő logika. |
| Bejelentkezés nélküli hozzáférés a Webáruházhoz. | Jelszavas bejelentkezés. Megfelelő hozzáférés kezelő logika. |
| Támadó megszemélyesíti a rendszerünket és közvetlenül hozzáfér az adatbázishoz. | Az adatbázisok és a backend privát hálózaton kommunikálnak. |

### Tampering

| Threat | Biztonsági követelmény |
| --- | --- |
| Man-in-the-middle támadás a szerver és az adatbázisok között. | Az adatbázisok és a backend privát hálózaton kommunikálnak. Adatbázis hozzáférés authentikálása. |
| Man-in-the-middle támadás a backend és a frontend között. | Https használata |
| Komponensek közötti kommunikáció módosítása. | Az adatbázisok és a backend privát hálózaton kommunikálnak. Input sanitization. |
| Kártékony CAFF fájl bejutása. | CAFF validilás, parse-olás előtt. |
| Logok módosítása | A logok módosításának és törlésének tilalma, biztonsági mentések létrehozása. |

### Denial of Service

| Threat | Biztonsági követelmény |
| --- | --- |
| Fizikai támadás éri a szervert. (Kihúzzák, elázik, stb) | Felhőszolgáltató felelőssége. |
| Túl nagy adatmennyiség feltöltése. | Fájlméret ellenőrzése és a fájlok számának limitálása. |
| DDoS támadás | Felhőszolgáltató felelőssége. |
| Túl sok komment írása | Kommentelés limitálása felhasználónként. |
| Túl sok regisztráció | KÍvül esik a házi feladat keretein. |

### Information disclosure

| Threat | Biztonsági követelmény |
|--- | --- | 
| Felhasználó adatok (admin jelszó, jelszó, egyenleg, személyes adatok) kiszívárognak | Jelszavak titkosítása (Salted Hash) Megfelelő access kontrol. |

### Elevation of privilige

| Threat | Biztonsági követelmény |
| --- | --- |
| Vendég, felhasználói jogosultságot szeret (bejelentkezés nélkül használja a rendszert). | Jelszavas bejelentkezés (jelsző követelmények). Megfelelő hozzáférés kezelő logika. |
| Egy felhasználó adminisztrátori jogosultságot szerez. | Megfelelő hozzáférés kezelő logika. |

## Architektúra tervek

### Komoponens diagram

<p>
    <img src="figures/komponensdiagram.png" width="800" title="Komponens diagram">
</p>

### Rendszer viselkedése - szekvencia diagramok

* Regisztráció
* Belépés
* Feltöltés
* Letöltés
* Admin feladatok
    * Adat módosítás

## Tesztelési terv

A rendszer tesztelése az alábbiak szerint kerül majd megvalósításra.

* Funkcionális tesztelés
    * Komoponensek unit-tesztelése az adott nyelv eszközeivel
    * Az egész rendszer manuális tesztelése
* Biztonsági tesztelés
    * Minden komponens code review-zása.
    * Statikus analízis
        * C Parser:
            * VS Code extension linter funkciója
            * Cppcheck (static analyzer)
        * Python backend: 
            * PyCharm IDE beépített code analyzer-e
            * Mypy static analyzer és type checker
        * Mobil frontend
            * 
    * Dinamikus tesztelés
        * C parser fuzzolása: American Fuzzy Lop tool-lal
        * Flask Python Backend tesztelése: Zed Attack Proxy pentest tool-lal

# Feladatok:
* Árpi: 
    * Komponens diagram
    * Architektúra tervek szövege -> minden interfészről 1-2 mondat
    * Android tesztek !!!!
* Heni:
    * Use case diagramok (3 darab), include nyilak
    * Funkcionális követelményekhez szöveg, bevezetés
* Máté:
    * Szekvencia diagramok
* Misi:
    * adatfolyam ábra