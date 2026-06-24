# linorobot2_hardware with pico

이 프로젝트는 [linorobot/linorobot2_hardware](https://github.com/linorobot/linorobot2_hardware)를 fork하여 pico 및 esp32 지원을 추가한 [hippo5329/linorobot2_hardware](https://github.com/hippo5329/linorobot2_hardware/tree/humble)의 코드를 베이스로 작업하였습니다.

ROS2 Humble 환경에서 Raspberry Pi Pico 2와 `linorobot2_hardware` 펌웨어를 사용하여 4륜 구동 모바일 로봇의 low-level 제어 시스템을 구축합니다.

통신 흐름은 다음과 같습니다.
```
Ubuntu 22.04(Host)
<-> Docker(micro-ROS Agent)
<-> USB Serial
<-> Pico 2(micro-ROS Node)
<-> TB6612FNG(Motor Driver)
```
