#include <stdio.h>
#include <stdlib.h>

int main()
{
	FILE *v;
	
	v = fopen("test.out", "w+");
	
	fseek(v, 5*1024-1, SEEK_SET);
	fwrite("F", 1, 1, v);
	fflush(v);
	
	fclose(v);
	
	return 0;
}

