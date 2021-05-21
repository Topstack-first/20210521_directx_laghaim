#ifndef _G_STRING_MANAGER_H_
#define _G_STRING_MANAGER_H_

#include "Config.h"

#include "string_res.h"

#define STRING_MAX_BUFFER 1024
//----------------------------------------------------------------------------
//-- String Manager
//----------------------------------------------------------------------------
//-- 2004.06.30
//-- Lyul 추가

//-- 게임상에 사용될 모든 Global String 을 관리한다.
//-- 타 소스와의 컴파일 시 결합도를 제거하기 위하여 독립 파일로 분류
//-- 내부의 member 에 Set 된뒤 Return
//-- 일정하지 않은 시간안에 복사 수행해야 한다.

/*
 ▶ String manager 를 사용할 때 주의할 사항

  : 늘 그렇듯 String 문제는 Memory에 예민하다.
  2003.06.30 방식은 해당 String 을 Loading 한 뒤 Member 로 보존하고 그 주소를
  return 하여 외부에서 복사 할 수 있도록 하는 방식이다.

  단점은 manager 를 호출한 루틴이 return 되는 string 을 바로 deep복사 하지 않는
  경우, 다른 routine 으로 인해 member string 의 값이 변경 될 수 있다는 점이다.

  기타 방식의 단점  :
  임시 변수는 return 불가
  외부 변수를 가져 와서 복사 할 경우 Size 가변 불가, 생성 제거 외부에 의존
  외부 변수를 가져 와서 생성,복사 할 경우 , 제거 외부에 의존
  New/delete 의 비용 증가.
*/

char const * const G_STRING(const int a_index);


#endif // _G_STRING_MANAGER_H_
