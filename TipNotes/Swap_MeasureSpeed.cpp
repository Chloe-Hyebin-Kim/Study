//[C++]두 정수를 교환하는 여러 가지 알고리즘 속도 비교

/*
두 값을 교환하는 swap 연산은 여러 가지 알고리즘에 두루 쓰인다.

동작은 간단하지만 호출 횟수가 많아 이 함수가 빨라야 전체 알고리즘도 빨라진다.

또한 교환 타입에 따라 여러 벌의 함수가 필요하다.

C++ 표준 라이브러리인 STL은 템플릿으로 타입 독립성을 확보한다.

표준 swap 함수와 직접 만든 함수, 매크로 함수 등의 속도를 비교해 보았다.



---------------------------------------

​

각 알고리즘으로 두 개의 정수를 교환하되 1억 1번 반복하여 실제 값이 바뀌도록 했다. 이 예제를 만들 때 STL의 고도로 최적화된 알고리즘이 좀 더 빠르지 않을까 내심 기대했다. 그러나 실행 결과를 보면 완전 뒷통수를 맞는 느낌이다.

STL의 swap 함수 : left=4, right = 3, 시간=54초

Inline에 직접 작성한 함수 Swap : left=3, right = 4, 시간=14초

매크로에 정의한 직접교환 SWAP : left=4, right = 3, 시간=2초

함수 불릴 때 직접교환 : left=3, right = 4, 시간=2초

+/- 이용: left=4, right = 3, 시간=4초

XOR 이용용: left=3, right = 4, 시간=3초

​
STL의 swap 함수가 제일 느리다. 소스를 찾아 보면 템플릿으로 타입에 독립적이며 inline 지정까지 되어 있고 레퍼런스 인수를 받는다. 그러나 내부에 이상한 매크로로 떡칠이 되어 있는데 아마도 객체인 경우 필요한 조치를 취하기 위한 코드인 것 같다. 타입 독립적으로 만들다 보니 속도는 그야 말로 형편없다. 원래부터 STL을 좋아하지 않았지만 이렇게 엉망일 줄은 예상하지 못했다.

그렇다면 STL의 불필요한 코드를 걷어 내고 직접 int, char 타입에 대한 Swap만 두 벌 만들면 어떨까 싶어 만들어 보았다. STL에 비해서는 세 배 정도 더 빠른데 불필요한 코드가 제외되어서 그렇다. 이 결과를 보고 타입 독립이고 뭐고 STL은 개나 줘 버려라는 생각이 더 굳어지게 되었다.

SWAP 매크로는 단 2초밖에 걸리지 않는다. 역시 단순 무식한게 최고다. 다만 매크로는 임시 변수 t를 외부에 별도로 선언해 놓아야 하고 인수로도 전달해야 한다는 점에서 깔끔하지 못하다. 그러나 매번 지역 변수를 새로 만드는 것보다는 확실히 빠르다. 매크로 대신 직접 코드를 전개하는 것도 결과는 당연히 같다. 이게 다르면 말이 안된다.

다음으로 두 값을 +, - 연산자로 더하고 빼는 방법과 XOR 연산의 특수함을 이용하는 방법이 있는데 임시 변수가 없어도 된다는 장점은 있지만 그 뿐이다. 코드가 짧은 것도 아니고 속도가 더 빠른 것도 아니다. 좀 신기해 보일 뿐이다. 결론적으로 두 값을 교환할 때는 역시 temp를 거쳐 상호 대입하는 방법이 가장 빠르다.

 */
#include <iostream>
#include <algorithm>
#include <time.h>
using namespace std;
 
#define SWAP(x,y,t) {t=x;x=y;y=t;}
 
void inline Swap(int& left, int& right)
{
     int t;
     t = left;
     left = right;
     right = t;
}


int main()
{
     time_t t1, t2;
     int left = 3, right = 4;
 
     // STL의 swap 함수
     time(&t1);
     for (int i = 0; i < 1000000001; i++) {
          std::swap(left, right);
     }
     time(&t2);
     printf("[****STL****] swap : left=%d, right = %d, Time=%d Sec\n", left, right, (int)difftime(t2, t1));
 
     // 직접 만든 Swap 함수
     time(&t1);
     for (int i = 0; i < 1000000001; i++) {
          Swap(left, right);
     }
     time(&t2);
     printf("[***Custom**] Swap : left=%d, right = %d, Time=%d Sec\n", left, right, (int)difftime(t2, t1));
 
     // 매크로 함수
     time(&t1);
     int t;
     for (int i = 0; i < 1000000001; i++) {
          SWAP(left, right, t);
     }
     time(&t2);
     printf("[***Macro***] SWAP : left=%d, right = %d, Time=%d Sec\n", left, right, (int)difftime(t2, t1));
 
     // 직접 교환
     time(&t1);
     for (int i = 0; i < 1000000001; i++) {
          t = left; left = right; right = t;
     }
     time(&t2);
     printf("[**Directly*] swap : left=%d, right = %d, Time=%d Sec\n", left, right, (int)difftime(t2, t1));
 
     // +/-
     time(&t1);
     for (int i = 0; i < 1000000001; i++) {
          left = left + right; right = left - right; left = left - right;
     }
     time(&t2);
     printf("[****+/-****] swap : left=%d, right = %d, Time=%d Sec\n", left, right, (int)difftime(t2, t1));

     // XOR
     time(&t1);
     for (int i = 0; i < 1000000001; i++) {
          left = left ^ right; right = left ^ right; left = left ^ right;
     }
     time(&t2);
     printf("[****XOR****] swap : left=%d, right = %d, Time=%d Sec\n", left, right, (int)difftime(t2, t1));
}



