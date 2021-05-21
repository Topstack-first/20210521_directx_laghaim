#ifndef _G_STRING_MANAGER_H_
#define _G_STRING_MANAGER_H_

#include "Config.h"

#include "string_res.h"

#define STRING_MAX_BUFFER 1024
//----------------------------------------------------------------------------
//-- String Manager
//----------------------------------------------------------------------------
//-- 2004.06.30
//-- Lyul �߰�

//-- ���ӻ� ���� ��� Global String �� �����Ѵ�.
//-- Ÿ �ҽ����� ������ �� ���յ��� �����ϱ� ���Ͽ� ���� ���Ϸ� �з�
//-- ������ member �� Set �ȵ� Return
//-- �������� ���� �ð��ȿ� ���� �����ؾ� �Ѵ�.

/*
 �� String manager �� ����� �� ������ ����

  : �� �׷��� String ������ Memory�� �����ϴ�.
  2003.06.30 ����� �ش� String �� Loading �� �� Member �� �����ϰ� �� �ּҸ�
  return �Ͽ� �ܺο��� ���� �� �� �ֵ��� �ϴ� ����̴�.

  ������ manager �� ȣ���� ��ƾ�� return �Ǵ� string �� �ٷ� deep���� ���� �ʴ�
  ���, �ٸ� routine ���� ���� member string �� ���� ���� �� �� �ִٴ� ���̴�.

  ��Ÿ ����� ����  :
  �ӽ� ������ return �Ұ�
  �ܺ� ������ ���� �ͼ� ���� �� ��� Size ���� �Ұ�, ���� ���� �ܺο� ����
  �ܺ� ������ ���� �ͼ� ����,���� �� ��� , ���� �ܺο� ����
  New/delete �� ��� ����.
*/

char const * const G_STRING(const int a_index);


#endif // _G_STRING_MANAGER_H_
