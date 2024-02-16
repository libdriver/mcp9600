[English](/README.md) | [ 简体中文](/README_zh-Hans.md) | [繁體中文](/README_zh-Hant.md) | [日本語](/README_ja.md) | [Deutsch](/README_de.md) | [한국어](/README_ko.md)

<div align=center>
<img src="/doc/image/logo.svg" width="400" height="150"/>
</div>

## LibDriver MCP9600

[![MISRA](https://img.shields.io/badge/misra-compliant-brightgreen.svg)](/misra/README.md) [![API](https://img.shields.io/badge/api-reference-blue.svg)](https://www.libdriver.com/docs/mcp9600/index.html) [![License](https://img.shields.io/badge/license-MIT-brightgreen.svg)](/LICENSE) 

Der MCP9600 ist ein vollintegrierter Thermoelement-Wandler von elektromotorischer Kraft (EMF) in Grad Celsius mit integrierter Kaltstellenkompensation. Das MCP9600 unterstützt 8 Thermoelementtypen (K, J, T, N, S, E, B und R). Der MCP9600 bietet benutzerprogrammierbare Register und erhöht die Designflexibilität für verschiedene Temperaturmessanwendungen. Die Register ermöglichen vom Benutzer wählbare Einstellungen, wie z. B. Energiesparmodi für batteriebetriebene Anwendungen, einen einstellbaren Digitalfilter für schnelle Übergangstemperaturen und vier individuell programmierbare Temperaturwarnausgänge, die zur Erkennung mehrerer Temperaturzonen verwendet werden können im petrochemischen Wärmemanagement, in Handmessgeräten, im Wärmemanagement von Industrieanlagen, in gewerblichen und industriellen Öfen, in der Wärmeüberwachung von Industriemotoren, in Temperaturerfassungsgestellen und so weiter.

LibDriver MCP9600 ist ein voll funktionsfähiger Treiber von MCP9600, der von LibDriver gestartet wurde. Er bietet Schusslesen, kontinuierliches Lesen, Interrupt-Lesemodus und andere Funktionen. LibDriver ist MISRA-konform.

### Inhaltsverzeichnis

  - [Anweisung](#Anweisung)
  - [Installieren](#Installieren)
  - [Nutzung](#Nutzung)
    - [example basic](#example-basic)
    - [example shot](#example-shot)
    - [example interrupt](#example-interrupt)
  - [Dokument](#Dokument)
  - [Beitrag](#Beitrag)
  - [Lizenz](#Lizenz)
  - [Kontaktieren Sie uns](#Kontaktieren-Sie-uns)

### Anweisung

/src enthält LibDriver MCP9600-Quelldateien.

/interface enthält die plattformunabhängige Vorlage LibDriver MCP9600 IIC.

/test enthält den Testcode des LibDriver MCP9600-Treibers und dieser Code kann die erforderliche Funktion des Chips einfach testen.

/example enthält LibDriver MCP9600-Beispielcode.

/doc enthält das LibDriver MCP9600-Offlinedokument.

/Datenblatt enthält MCP9600-Datenblatt.

/project enthält den allgemeinen Beispielcode für Linux- und MCU-Entwicklungsboards. Alle Projekte verwenden das Shell-Skript, um den Treiber zu debuggen, und die detaillierten Anweisungen finden Sie in der README.md jedes Projekts.

/misra enthält die Ergebnisse des LibDriver MISRA Code Scans.

### Installieren

Verweisen Sie auf eine plattformunabhängige IIC-Schnittstellenvorlage und stellen Sie Ihren Plattform-IIC-Treiber fertig.

Fügen Sie das Verzeichnis /src, den Schnittstellentreiber für Ihre Plattform und Ihre eigenen Treiber zu Ihrem Projekt hinzu. Wenn Sie die Standardbeispieltreiber verwenden möchten, fügen Sie das Verzeichnis /example zu Ihrem Projekt hinzu.

### Nutzung

Sie können auf die Beispiele im Verzeichnis /example zurückgreifen, um Ihren eigenen Treiber zu vervollständigen. Wenn Sie die Standardprogrammierbeispiele verwenden möchten, erfahren Sie hier, wie Sie diese verwenden.

#### example basic

```C
#include "driver_mcp9600_basic.h"

uint8_t res;
uint16_t i, times;

/* basic init */
res = mcp9600_basic_init(MCP9600_ADDRESS_0, MCP9600_THERMOCOUPLE_TYPE_K);
if (res != 0)
{
    return 1;
}

times = 3;
for (i = 0; i < times; i++)
{
    int16_t hot_raw;
    float hot_s;
    int16_t delta_raw;
    float delta_s;
    int16_t cold_raw;
    float cold_s;

    /* basic read */
    res = mcp9600_basic_read((int16_t *)&hot_raw, (float *)&hot_s,
                             (int16_t *)&delta_raw, (float *)&delta_s,
                             (int16_t *)&cold_raw, (float *)&cold_s);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: basic read failed.\n");
        (void)mcp9600_basic_deinit();

        return 1;
    }

    /* 1000 ms */
    mcp9600_interface_delay_ms(1000);

    /* print */
    mcp9600_interface_debug_print("mcp9600: %d / %d hot %0.2f delta %0.2f cold %0.2f.\n", times - i, times, hot_s, delta_s, cold_s);
}

/* deinit */
res = mcp9600_basic_deinit();
if (res != 0)
{
    return 1;
}
else
{
    return 0;
}
```
#### example shot

```C
#include "driver_mcp9600_shot.h"

uint8_t res;
uint16_t i, times;

/* shot init */
res = mcp9600_shot_init(MCP9600_ADDRESS_0, MCP9600_THERMOCOUPLE_TYPE_K);
if (res != 0)
{
    return 1;
}

times = 3;
for (i = 0; i < times; i++)
{
    int16_t hot_raw;
    float hot_s;
    int16_t delta_raw;
    float delta_s;
    int16_t cold_raw;
    float cold_s;

    /* shot read */
    res = mcp9600_shot_read((int16_t *)&hot_raw, (float *)&hot_s,
                            (int16_t *)&delta_raw, (float *)&delta_s,
                            (int16_t *)&cold_raw, (float *)&cold_s);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: shot read failed.\n");
        (void)mcp9600_shot_deinit();

        return 1;
    }

    /* 1000 ms */
    mcp9600_interface_delay_ms(1000);

    /* print */
    mcp9600_interface_debug_print("mcp9600: %d / %d hot %0.2f delta %0.2f cold %0.2f.\n", times - i, times, hot_s, delta_s, cold_s);
}

/* deinit */
res = mcp9600_shot_deinit();
if (res != 0)
{
    return 1;
}
else
{
    return 0;
}
```

#### example interrupt

```C
#include "driver_mcp9600_interrupt.h"

uint8_t res;
uint16_t i, times;
uint8_t g_flag;

void gpio_callback(uint16_t pin)
{
    g_flag = 1;
}

/* gpio init */
g_flag = 0;
res = gpio_interrupt_init();
if (res != 0)
{
    return 1;
}

/* interrupt init */
res = mcp9600_interrupt_init(MCP9600_ADDRESS_0, MCP9600_THERMOCOUPLE_TYPE_K);
if (res != 0)
{
    (void)gpio_interrupt_deinit();

    return 1;
}

/* clear all */
res = mcp9600_interrupt_clear(MCP9600_ALERT_1);
if (res != 0)
{
    mcp9600_interface_debug_print("mcp9600: interrupt clear failed.\n");
    (void)mcp9600_interrupt_deinit();
    (void)gpio_interrupt_deinit();
}
res = mcp9600_interrupt_clear(MCP9600_ALERT_2);
if (res != 0)
{
    mcp9600_interface_debug_print("mcp9600: interrupt clear failed.\n");
    (void)mcp9600_interrupt_deinit();
    (void)gpio_interrupt_deinit();
}
res = mcp9600_interrupt_clear(MCP9600_ALERT_3);
if (res != 0)
{
    mcp9600_interface_debug_print("mcp9600: interrupt clear failed.\n");
    (void)mcp9600_interrupt_deinit();
    (void)gpio_interrupt_deinit();
}
res = mcp9600_interrupt_clear(MCP9600_ALERT_4);
if (res != 0)
{
    mcp9600_interface_debug_print("mcp9600: interrupt clear failed.\n");
    (void)mcp9600_interrupt_deinit();
    (void)gpio_interrupt_deinit();
}

times = 3;
for (i = 0; i < times; i++)
{
    int16_t hot_raw;
    float hot_s;
    int16_t delta_raw;
    float delta_s;
    int16_t cold_raw;
    float cold_s;

    /* interrupt read */
    res = mcp9600_interrupt_read((int16_t *)&hot_raw, (float *)&hot_s,
                                 (int16_t *)&delta_raw, (float *)&delta_s,
                                 (int16_t *)&cold_raw, (float *)&cold_s);
    if (res != 0)
    {
        mcp9600_interface_debug_print("mcp9600: interrupt read failed.\n");
        (void)mcp9600_interrupt_deinit();
        (void)gpio_interrupt_deinit();

        return 1;
    }

    /* 1000 ms */
    mcp9600_interface_delay_ms(1000);

    /* print */
    mcp9600_interface_debug_print("mcp9600: %d / %d hot %0.2f delta %0.2f cold %0.2f.\n", times - i, times, hot_s, delta_s, cold_s);

    if (g_flag != 0)
    {
        mcp9600_interface_debug_print("mcp9600: find interrupt.\n");

        break;
    }
}

/* deinit */
res = mcp9600_interrupt_deinit();
if (res != 0)
{
    (void)gpio_interrupt_deinit();

    return 1;
}
else
{
    (void)gpio_interrupt_deinit();

    return 0;
}
```

### Dokument

Online-Dokumente: [https://www.libdriver.com/docs/mcp9600/index.html](https://www.libdriver.com/docs/mcp9600/index.html).

Offline-Dokumente: /doc/html/index.html.

### Beitrag

Bitte beachten Sie CONTRIBUTING.md.

### Lizenz

Urheberrechte © (c) 2015 - Gegenwart LibDriver Alle Rechte vorbehalten



Die MIT-Lizenz (MIT)



Hiermit wird jeder Person kostenlos die Erlaubnis erteilt, eine Kopie zu erhalten

dieser Software und zugehörigen Dokumentationsdateien (die „Software“) zu behandeln

in der Software ohne Einschränkung, einschließlich, aber nicht beschränkt auf die Rechte

zu verwenden, zu kopieren, zu modifizieren, zusammenzuführen, zu veröffentlichen, zu verteilen, unterzulizenzieren und/oder zu verkaufen

Kopien der Software und Personen, denen die Software gehört, zu gestatten

dazu eingerichtet werden, unter folgenden Bedingungen:



Der obige Urheberrechtshinweis und dieser Genehmigungshinweis müssen in allen enthalten sein

Kopien oder wesentliche Teile der Software.



DIE SOFTWARE WIRD "WIE BESEHEN" BEREITGESTELLT, OHNE JEGLICHE GEWÄHRLEISTUNG, AUSDRÜCKLICH ODER

STILLSCHWEIGEND, EINSCHLIESSLICH, ABER NICHT BESCHRÄNKT AUF DIE GEWÄHRLEISTUNG DER MARKTGÄNGIGKEIT,

EIGNUNG FÜR EINEN BESTIMMTEN ZWECK UND NICHTVERLETZUNG VON RECHTEN DRITTER. IN KEINEM FALL DARF DAS

AUTOREN ODER URHEBERRECHTSINHABER HAFTEN FÜR JEGLICHE ANSPRÜCHE, SCHÄDEN ODER ANDERE

HAFTUNG, OB AUS VERTRAG, DELIKT ODER ANDERWEITIG, ENTSTEHEND AUS,

AUS ODER IM ZUSAMMENHANG MIT DER SOFTWARE ODER DER VERWENDUNG ODER ANDEREN HANDLUNGEN MIT DER

SOFTWARE.

### Kontaktieren Sie uns

Bitte senden Sie eine E-Mail an lishifenging@outlook.com.