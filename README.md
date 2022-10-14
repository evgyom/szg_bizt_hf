# Követelmények

## Funkcionális követelmények

Követelmények:
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
    * [insert admin feladatok] jogosultsághoz kötött tevékenység
    * A webáruház elérése bejelentkezéshez kötött
* Auditiálás
    * Minden felhasználó minden tevékenységet naplózni kell.

## Threat assessment

### Asset-ek

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

* Spoofing:
    * Social engineering of admins  --> awareness campaign
    * User admin funkciókat próbál elérni --> megfelelő access control logika
    * Guest próbál user funkcióhoz hozzáférni --> megfelelő access control logika
    * Adatbázis hozzáférés a rendszeren kívülről --> elzárt hálózaton fut a szerver
* Tampering
    * Sérül kommunikáció az adatbázisokkal --> elzárt hálózaton fut a szerver
    * Webes kommunikáció módosítása --> https
    * Komponensek közötti kommunikáció módosítása --> zárt rendszer
    * CAFF kártevőt tartalmaz --> fasza parser
    * Invalid CAFF-ok --> fasza parsser
        * Lezárás
        * ...
    * Logok módosítás --> no access, backup
* Letagadás --> logolás
    * Egy user letagadja, hogy megkapta a pénzt egy másiktól
    * User letagadja, hogy ő letörölte
    * Admin kitöröl mindent és letagadja
* Denial of Service
    * Kihúzzák a szervert --> bezárjuk a termet, kamera
    * Túl nagy fájl feltöltése --> parser
    * DDos --> public cloud
    * Túl sok komment --> limit
    * Túl sok regisztráció --> kívül esik a prioritásainkon
    * Túl sok ... --> limit
* Information disclosure --> access control
    * Felhasználó adatok kiszívárognak --> access control
        * Egy felhasználó hozzáfér máséhoz
        * Jelszó kiszvirgás -> jelszó hash-t tárolunk (sózott)
    * Admin hozzáfér a plaintext jelszavakhaz
    * Guest bármit lát
    * Admin jelsző kiszivárog
* Elevation of privilige --> access control
    * Guest user jogosultságot szerez.
    * Adminná változtatja magát. 

### Mitigációs lépések

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

* Mindentre code revirew és static analyzer
* Parserre fuzzolás
* Ha Flask lesz a backend: https://www.securecoding.com/blog/penetration-testing-in-flask-application/
* Manuális tesztelés

# Kérdések:
* Pénz kell?
* Bejelentkezési lib szabad-e?

# Feladatok:
* Árpi: 
    * Komponens diagram
    * Architektúra tervek szövege -> minden interfészről 1-2 mondat
* Heni:
    * Use case diagram 
    * Funkcionális követelmények
    * Biztonsági követelmények
* Máté:
    * Szekvencia diagramok
* Misi:
    * Adatfolyam diagram
    * CAFF parser veszélyei
    * Támadómodellek kirészletezése
    * Biztonsági követelményekből szövegalkotás

# Megjegyzések

Komponensdiagramban a parser legyen egy külön komponens a CAFF Áruházon kívül
