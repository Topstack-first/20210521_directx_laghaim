#include "stdafx.h"

long getFileSize(PFILE* fp)
{
	long curPos = pftell(fp);
	pfseek(fp, 0, SEEK_END);
	long endPos = pftell(fp);
	pfseek(fp, curPos, SEEK_SET);

	return endPos;
}

void readData(void* ptr, size_t size, size_t n, unsigned char** stream)
{
	size_t copySize = size * n;
	memcpy(ptr, *stream, copySize);
	*stream += copySize;
}