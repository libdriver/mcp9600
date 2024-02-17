[English](/README.md) | [ 简体中文](/README_zh-Hans.md) | [繁體中文](/README_zh-Hant.md) | [日本語](/README_ja.md) | [Deutsch](/README_de.md) | [한국어](/README_ko.md)

<div align=center>
<img src="/doc/image/logo.svg" width="400" height="150"/>
</div>

## LibDriver MCP9600

[![MISRA](https://img.shields.io/badge/misra-compliant-brightgreen.svg)](/misra/README.md) [![API](https://img.shields.io/badge/api-reference-blue.svg)](https://www.libdriver.com/docs/mcp9600/index.html) [![License](https://img.shields.io/badge/license-MIT-brightgreen.svg)](/LICENSE)

MCP9600은 냉접점 보상이 통합된 완전히 통합된 열전대 기전력(EMF)-섭씨 변환기입니다. MCP9600은 8개의 열전대 유형(K, J, T, N, S, E, B 및 R)을 지원합니다. MCP9600은 사용자 프로그래밍 가능 레지스터를 제공하여 다양한 온도 감지 애플리케이션을 위한 설계 유연성을 추가합니다. 레지스터는 배터리 구동 애플리케이션을 위한 저전력 모드, 빠른 과도 온도를 위한 조정 가능한 디지털 필터 및 여러 온도 영역을 감지하는 데 사용할 수 있는 4개의 개별적으로 프로그래밍 가능한 온도 경고 출력과 같은 사용자 선택 가능한 설정을 허용합니다. 석유 화학 열 관리, 휴대용 측정 장비, 산업 장비 열 관리, 상업용 및 산업용 오븐, 산업용 엔진 열 모니터, 온도 감지 랙 등에서.

LibDriver MCP9600은 LibDriver에서 출시한 MCP9600의 전체 기능 드라이버입니다. 샷 읽기, 연속 읽기, 인터럽트 읽기 모드 및 기타 기능을 제공합니다. LibDriver는 MISRA를 준수합니다.

### 콘텐츠

  - [설명](#설명)
  - [설치](#설치)
  - [사용](#사용)
    - [example basic](#example-basic)
    - [example shot](#example-shot)
    - [example interrupt](#example-interrupt)
  - [문서](#문서)
  - [기고](#기고)
  - [저작권](#저작권)
  - [문의하기](#문의하기)

### 설명

/src 디렉토리에는 LibDriver MCP9600의 소스 파일이 포함되어 있습니다.

/interface 디렉토리에는 LibDriver MCP9600용 플랫폼 독립적인 IIC버스 템플릿이 포함되어 있습니다.

/test 디렉토리에는 LibDriver MCP9600드라이버 테스트 프로그램이 포함되어 있어 칩의 필요한 기능을 간단히 테스트할 수 있습니다.

/example 디렉토리에는 LibDriver MCP9600프로그래밍 예제가 포함되어 있습니다.

/doc 디렉토리에는 LibDriver MCP9600오프라인 문서가 포함되어 있습니다.

/datasheet 디렉토리에는 MCP9600데이터시트가 있습니다.

/project 디렉토리에는 일반적으로 사용되는 Linux 및 마이크로컨트롤러 개발 보드의 프로젝트 샘플이 포함되어 있습니다. 모든 프로젝트는 디버깅 방법으로 셸 스크립트를 사용하며, 자세한 내용은 각 프로젝트의 README.md를 참조하십시오.

/misra 에는 LibDriver misra 코드 검색 결과가 포함됩니다.

### 설치

/interface 디렉토리에서 플랫폼 독립적인 IIC버스 템플릿을 참조하여 지정된 플랫폼에 대한 IIC버스 드라이버를 완성하십시오.

/src 디렉터리, 플랫폼용 인터페이스 드라이버 및 자체 드라이버를 프로젝트에 추가합니다. 기본 예제 드라이버를 사용하려면 /example 디렉터리를 프로젝트에 추가합니다.

### 사용

/example 디렉터리의 예제를 참조하여 자신만의 드라이버를 완성할 수 있습니다. 기본 프로그래밍 예제를 사용하려는 경우 사용 방법은 다음과 같습니다.

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

### 문서

온라인 문서: [https://www.libdriver.com/docs/mcp9600/index.html](https://www.libdriver.com/docs/mcp9600/index.html).

오프라인 문서: /doc/html/index.html.

### 기고

CONTRIBUTING.md 를 참조하십시오.

### 저작권

저작권 (c) 2015 - 지금 LibDriver 판권 소유

MIT 라이선스(MIT)

이 소프트웨어 및 관련 문서 파일("소프트웨어")의 사본을 얻은 모든 사람은 이에 따라 무제한 사용, 복제, 수정, 통합, 출판, 배포, 2차 라이선스를 포함하여 소프트웨어를 처분할 수 있는 권리가 부여됩니다. 소프트웨어의 사본에 대한 라이선스 및/또는 판매, 그리고 소프트웨어가 위와 같이 배포된 사람의 권리에 대한 2차 라이선스는 다음 조건에 따릅니다.

위의 저작권 표시 및 이 허가 표시는 이 소프트웨어의 모든 사본 또는 내용에 포함됩니다.

이 소프트웨어는 상품성, 특정 목적에의 적합성 및 비침해에 대한 보증을 포함하되 이에 국한되지 않는 어떠한 종류의 명시적 또는 묵시적 보증 없이 "있는 그대로" 제공됩니다. 어떤 경우에도 저자 또는 저작권 소유자는 계약, 불법 행위 또는 기타 방식에 관계없이 소프트웨어 및 기타 소프트웨어 사용으로 인해 발생하거나 이와 관련하여 발생하는 청구, 손해 또는 기타 책임에 대해 책임을 지지 않습니다.

### 문의하기

연락주세요lishifenging@outlook.com.