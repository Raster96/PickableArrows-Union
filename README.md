# PickableArrows - Union Plugin

Plugin dla Gothic 2: Nocy Kruka pozwalający na zbieranie i ponowne używanie wystrzelonych strzał i bełtów.

## Funkcje

- **Odzyskiwanie strzał z NPC** - Strzały które trafiły w przeciwników są dodawane do ich ekwipunku. Można je odzyskać z martwych lub nieprzytomnych NPC.
- **Zbieranie strzał z ziemi** - Strzały które nie trafiły w cel spadają na ziemię i można je podnieść.
- **Konfigurowalne szanse** - Możliwość ustawienia osobnych szans na odzyskanie strzał z ziemi i z NPC (domyślnie 100%).

## Wymagania

- Gothic 2: Noc Kruka
- Union 1.0m lub nowszy

## Instalacja

1. Skompiluj plugin używając Visual Studio (projekt `.vcxproj`)
2. Skopiuj wygenerowany plik `PickableArrows.dll` do folderu `[Gothic2]\System\Autorun\`
3. Skopiuj ustawienia z `Gothic.ini.example` do `[Gothic2]\System\Gothic.ini`

## Konfiguracja

### Przez menu Union (zalecane)

1. Uruchom grę
2. Wejdź do menu głównego → Opcje → Union
3. Znajdź "PickableArrows" na liście
4. Skonfiguruj wszystkie opcje przez graficzne menu

### Przez plik Gothic.ini (alternatywnie)

Dodaj do pliku `Gothic.ini` sekcję:

```ini
[PICKABLE_ARROWS]
Enabled=1
GroundRecoveryChance=100
NpcRecoveryChance=100
AllowNpcInventoryPickup=1
AllowGroundPickup=1
RequireDamageForNpcRecovery=1
ChanceDependsOnSkills=0
```

### Opcje

| Opcja | Opis | Domyślnie |
|-------|------|-----------|
| `Enabled` | Włącza/wyłącz plugin | 1 |
| `GroundRecoveryChance` | Szansa na odzyskanie strzały z ziemi (0-100%) | 100 |
| `NpcRecoveryChance` | Szansa na odzyskanie strzały z NPC (0-100%) | 100 |
| `AllowNpcInventoryPickup` | Dodawanie strzał do ekwipunku trafionych NPC | 1 |
| `AllowGroundPickup` | Upuszczanie strzał na ziemię przy pudłach | 1 |
| `RequireDamageForNpcRecovery` | Wymagaj zadania obrażeń dla odzyskania z NPC | 1 |
| `ChanceDependsOnSkills` | Szansa zależy od umiejętności łuk/kusza (max 100%) | 0 | 

## Ostrzeżenie

> **UWAGA:** Użycie tego pluginu nieodwracalnie zmienia balans gry!
> Liczba dostępnych strzał w grze zostanie zredukowana, ponieważ NPC nie będą już mieli "nieskończonej" amunicji.

## Budowanie

1. Otwórz `BonusIndependentTraining.sln` w Visual Studio 2022
2. Wybierz konfigurację `G2A MT Release`
3. Zbuduj projekt (F7)
4. Plik DLL znajdziesz w folderze `Bin\`

## Licencja

MIT License
