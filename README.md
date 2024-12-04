# openGL_learn
- 드립커피+한모금더 강사님의 openGL 인프런 강의(https://www.inflearn.com/course/%EA%B2%8C%EC%9E%84%EA%B0%9C%EB%B0%9C-graphics-shader-opengl-1) 실습 레포입니다.

- 실습환경은 iMac (Retina 5K, 27-inch, 2019), 3.1 GHz 6-Core Intel Core i5
- openGL 4.1 로 진행하였습니다.
- Build tool : CMake

## Pyramid 회전 example (Chapter 3)
### openGL program
- EBO를 통해 효과적인 버텍스 사용 진행.
- chrono를 통한 렌더 타임 체크로 걸린 시간에 비례하여 회전되게 설정.
- 'R' 키 press에 lastTime 업데이트로 theta 값 초기화.
- `GL_CULL_FACE` enable 을 통해 파이프라인 상에 필요없는 프리미티브 축출.
- 카메라 위치 등 상황에 따라 달라질 수 있는 CullFace를 cullMode 변수 세팅을 통해 설정할 수 있도록 switch-case 문 활용.

### shader
- 기본 버텍스 위치와 각도를 vertex shader로 전달하여 쉐이더에서 회전변환 계산이 진행되도록 설정.
- openGL 특징 상 오른손, 왼손 좌표계를 혼용하는 것에 대응하여 z direction에 대해서 negate 진행.

### 결과
![Alt text](./readmeData/Screen-Recording-2024-12-04-at-1.19.38-PM.gif)

- y 축 기준으로 회전하는 피라미드 생성.


---

## 2D 회전변환 적용 example (Chapter 4)
### openGL program
- 궤도를 도는 렌더링을 위해 회전각과 시작 오프셋 값을 유니폼으로 전달

### shader
- 전달받은 버텍스 좌표에 시작점 오프셋 값을 합한 뒤 회전각 만큼 회전변환시켜서 원 궤도를 돌아가는 것으로 만듦.

### 결과

![Alt text](./readmeData/Screen-Recording-2024-12-04-at-1.54.03-PM.gif)
