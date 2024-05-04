# AssaultCube_Cheat
게임 치트 연습을 위해 만들어진 저장소

현재 업로드 되어 있는 코드는
`AssaultCube_v1.2.0.2` `AssaultCube_v1.3.0.2_LockdownEdition` 버전의 코드가 같이 있습니다.

AssaultCube_v1.3.0.2_LockdownEdition 버전 오프셋
내좌표, 적좌표
```cpp
#include <cmath> // sqrt 함수를 사용하기 위해 포함합니다.

// 두 점 사이의 거리를 계산하는 함수
float CalculateDistance(float x1, float z1, float x2, float z2) {
    // 피타고라스의 정리를 사용하여 거리를 계산합니다.
    float deltaX = x2 - x1;
    float deltaZ = z2 - z1;
    float distanceSquared = deltaX * deltaX + deltaZ * deltaZ;
    float distance = sqrt(distanceSquared); // 거리를 구하기 위해 제곱근을 씌웁니다.
    return distance;
}

// 예시에서 제공한 코드를 사용하여 두 점의 좌표를 입력하고 거리를 계산하는 예시입니다.
int main() {
    // 내 위치
    float myX = *(float*)mem::FindAddress(moduleBase + PlayerBase, { TeleportX });
    float myZ = *(float*)mem::FindAddress(moduleBase + PlayerBase, { TeleportZ });

    // 상대방 위치
    float enemyX = *(float*)mem::FindAddress(moduleBase + PsPlayerBase, { P1PlayerBase, P1TeleportX });
    float enemyZ = *(float*)mem::FindAddress(moduleBase + PsPlayerBase, { P1PlayerBase, P1TeleportZ });

    // 거리 계산
    float distance = CalculateDistance(myX, myZ, enemyX, enemyZ);
    
    // 결과 출력
    std::cout << "나와 상대방 사이의 거리: " << distance << std::endl;

    return 0;
}
```

탄약 무제한
```lua
[ENABLE]

alloc(newmem, 2048)
label(returnhere)
label(originalcode)
label(exit)

newmem:

originalcode:
nop
lea eax,[esp+1C]

exit:
jmp returnhere

"ac_client.exe"+C73EF:
jmp newmem
nop
returnhere:

[DISABLE]

```
```
"ac_client.exe"+0017E0A8 =   
X 축 28  
Z 축 2C  
Y 축 30  
  
34 X 축 시점  
38 Y 축 시점
```
https://youtu.be/sP7RUtyYjCY
