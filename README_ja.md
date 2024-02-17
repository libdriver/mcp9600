[English](/README.md) | [ 简体中文](/README_zh-Hans.md) | [繁體中文](/README_zh-Hant.md) | [日本語](/README_ja.md) | [Deutsch](/README_de.md) | [한국어](/README_ko.md)

<div align=center>
<img src="/doc/image/logo.svg" width="400" height="150"/>
</div>

## LibDriver MCP9600

[![MISRA](https://img.shields.io/badge/misra-compliant-brightgreen.svg)](/misra/README.md) [![API](https://img.shields.io/badge/api-reference-blue.svg)](https://www.libdriver.com/docs/mcp9600/index.html) [![License](https://img.shields.io/badge/license-MIT-brightgreen.svg)](/LICENSE)

MCP9600は、完全に統合された熱電対起電力（EMF）から摂氏までのコンバーターであり、冷接点補償が統合されています。 MCP9600は、8種類の熱電対（K、J、T、N、S、E、B、およびR）をサポートします。MCP9600は、ユーザーがプログラム可能なレジスタを提供し、さまざまな温度検出アプリケーションの設計の柔軟性を追加します。レジスタにより、バッテリ駆動アプリケーション用の低電力モード、高速過渡温度用の調整可能なデジタルフィルタ、複数の温度ゾーンの検出に使用できる4つの個別にプログラム可能な温度アラート出力などのユーザー選択可能な設定が可能になります。石油化学熱管理、携帯型測定装置、産業機器熱管理、業務用および工業用オーブン、工業用エンジン熱モニター、温度検出ラックなど。

LibDriver MCP9600は、LibDriverによって起動されたMCP9600の全機能ドライバーであり、ショット読み取り、連続読み取り、割り込み読み取りモードなどの機能を提供します。 LibDriverはMISRAに準拠しています。

### 目次

  - [説明](#説明)
  - [インストール](#インストール)
  - [使用](#使用)
    - [example basic](#example-basic)
    - [example shot](#example-shot)
    - [example interrupt](#example-interrupt)
  - [ドキュメント](#ドキュメント)
  - [貢献](#貢献)
  - [著作権](#著作権)
  - [連絡して](#連絡して)

### 説明

/ srcディレクトリには、LibDriver MCP9600のソースファイルが含まれています。

/ interfaceディレクトリには、LibDriver MCP9600用のプラットフォームに依存しないIICバステンプレートが含まれています。

/ testディレクトリには、チップの必要な機能を簡単にテストできるLibDriver MCP9600ドライバーテストプログラムが含まれています。

/ exampleディレクトリには、LibDriver MCP9600プログラミング例が含まれています。

/ docディレクトリには、LibDriver MCP9600オフラインドキュメントが含まれています。

/ datasheetディレクトリには、MCP9600データシートが含まれています。

/ projectディレクトリには、一般的に使用されるLinuxおよびマイクロコントローラー開発ボードのプロジェクトサンプルが含まれています。 すべてのプロジェクトは、デバッグ方法としてシェルスクリプトを使用しています。詳細については、各プロジェクトのREADME.mdを参照してください。

/ misraはLibDriver misraコードスキャン結果を含む。

### インストール

/ interfaceディレクトリにあるプラットフォームに依存しないIICバステンプレートを参照して、指定したプラットフォームのIICバスドライバを完成させます。

/src ディレクトリ、プラットフォームのインターフェイス ドライバー、および独自のドライバーをプロジェクトに追加します。デフォルトのサンプル ドライバーを使用する場合は、/example ディレクトリをプロジェクトに追加します。

### 使用

/example ディレクトリ内のサンプルを参照して、独自のドライバーを完成させることができます。 デフォルトのプログラミング例を使用したい場合の使用方法は次のとおりです。

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

```c
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

```c
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

### ドキュメント

オンラインドキュメント: [https://www.libdriver.com/docs/mcp9600/index.html](https://www.libdriver.com/docs/mcp9600/index.html)。

オフラインドキュメント: /doc/html/index.html。

### 貢献

CONTRIBUTING.mdを参照してください。

### 著作権

著作権（c）2015-今 LibDriver 全著作権所有

MITライセンス（MIT）

このソフトウェアおよび関連するドキュメントファイル（「ソフトウェア」）のコピーを取得した人は、無制限の使用、複製、変更、組み込み、公開、配布、サブライセンスを含む、ソフトウェアを処分する権利を制限なく付与されます。ソフトウェアのライセンスおよび/またはコピーの販売、および上記のようにソフトウェアが配布された人の権利のサブライセンスは、次の条件に従うものとします。

上記の著作権表示およびこの許可通知は、このソフトウェアのすべてのコピーまたは実体に含まれるものとします。

このソフトウェアは「現状有姿」で提供され、商品性、特定目的への適合性、および非侵害の保証を含むがこれらに限定されない、明示または黙示を問わず、いかなる種類の保証もありません。 いかなる場合も、作者または著作権所有者は、契約、不法行為、またはその他の方法で、本ソフトウェアおよび本ソフトウェアの使用またはその他の廃棄に起因または関連して、請求、損害、またはその他の責任を負わないものとします。

### 連絡して

お問い合わせくださいlishifenging@outlook.com。