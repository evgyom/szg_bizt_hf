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
        * (saját CAFF-ok kezelése)
            * törlés
            * ár/név változtatás
* Admin user
    * Belépés
    * User kezelés
        * törlés
        * módosítás
        * (létrehozás)
    * Admin user generálás
    * CAFF kezelése
        * törlés
        * adatmódosítás
    

Use-case diagram

## Biztonsági követelmények és célok

CIA és AAA alapján követelménye lista:
* Confidentiality
    * Személyes adatok titkossága.
* Integrity
* Availability
* Authentication
* Authorization
* Auditing

Biztonsági célok: 
* Biztonságos adattárolás
* Naplózás

Use-case diagram-ra függőségi nyilak.

## Threat assessment

### Assettek megállapítása
* Fizikai
* Emberi
* Logikai

Adatfolyam ábra

### Veszélyek
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
    * Admin hozzáfér a plaintext jelszavakhaz
    * Guest bármit lát
    * Admin jelsző kiszivárog
* Elevation of privilige --> access control
    * Adminná változtatja magát

### Attacks

### Mitigációs lépések

## Architektúra tervek

### Komponens diagram

### UMLsec kiegészítés a komponens diagramhoz

### Rendszer viselkedése - szekvencia diagram
* Regisztráció
* Belépés
* Feltöltés
* Letöltés
* Admin feladatok
    * Adat módosítás

# Kérdések:
* Pénz kell?
* Bejelentkezési lib szabad-e?

# Feladatok:
* Árpi: 
    * Komponens diagram
    * Architektúra tervek szövege
* Heni:
    * Use case diagram 
    * Funkcionális követelmények
    * Biztonsági követelmények
* Máté:
    * Szekvencia diagramok
* Misi:
    * Adatfolyam diagram
    * CAFF parser veszélyei