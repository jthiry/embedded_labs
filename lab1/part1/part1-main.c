#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/* This is the function to focus on for optimization */
extern void strTable(char *s, char *d, int sl, int dl);

int main()
{
   /* Assume that dst[i] is always >= 23 (decimal) */
   char src[] = "theinitialstring\n";
   char dst[] = "PENGUINS";
   int i, srcLen, dstLen, srcLen0, dstLen0; 

   srcLen0 = strlen(src);
   dstLen0 = strlen(dst);
   
   srcLen = ((strlen(src) >> 2) << 2);
   dstLen = ((strlen(dst) >> 2) << 2);

   printf("PRE: src: %s (%d bytes), dst: %s (%d bytes)\n", 
	  src, srcLen, dst, dstLen); 
   
   for (i=0; i<1000; i++)
     strTable(src, dst, srcLen, dstLen); 
   
   printf("POST: src: %s (%d bytes), dst: %s (%d bytes)\n", 
	  src, srcLen, dst, dstLen);
   
   printf("src: %d => %d, dst: %d => %d\n",
      srcLen0, srcLen, dstLen0, dstLen);
   return 1;
}

