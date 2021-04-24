# 2. abov prefix

Date: 2021-04-25

## Status

Proposed

## Context

libabov 는 사용자의 응용 프로그램 또는 RTOS 와 같은 미들웨어 아래에서 호출되는
저수준 드라이버다. 따라서 libabov 의 API 는 상위 계층 프로그램과 충돌하지 않도록
일반적인 이름을 피하고 구체적인 이름으로 정의해야 한다. 예컨대, `gpio_write` 와
같은 generic 한 함수명은 상위 계층 프로그램 naming convention 과 충돌할 여지가
많다.

## Decision

모든 public API 에 `abov_` prefix 를 추가한다.

## Consequences

- 사용자 추상화에 걸림돌이 되지 않는다
- readability 를 해친다
- 타이핑 수를 늘린다
