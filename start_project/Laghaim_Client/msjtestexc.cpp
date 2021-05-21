#include "stdafx.h"
//
// Test code to demonstrate MSJEXHND
//
void YetAnotherFunction( int i, float j, char * pszString )
{
	*(int *)i = 5;
}

void MyWonderfulFunction( int i, float j )
{
	YetAnotherFunction( i * 2, j, "Hello" );
}

int main()
{
	MyWonderfulFunction( 4, float(5.6) );

	return 0;
}
