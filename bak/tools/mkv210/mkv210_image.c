
/* BL0,Irom code read nandflash || SD pre 16K content
 * and compare the pre 16 bytes checksum,if error occur ,stop
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define BUFSIZE                 (16*1024)
#define IMG_SIZE                (16*1024)
#define SPL_HEADER_SIZE         16
#define SPL_HEADER              "S5PC110 HEADER  "

int main (int argc, char *argv[]){
	FILE		*fp;
	char		*Buf, *a;
	int		BufLen;
	int		nbytes, fileLen;
	unsigned int	checksum, count;
	int		i;
	
	if (argc != 3){
		printf("Usage: mkbl1 <source file> <destination file>\n");
		return -1;
	}

	// 2. alloc 16K buffer
	BufLen = BUFSIZE;
	Buf = (char *)malloc(BufLen);
	if (!Buf){
		printf("Alloc buffer failed!\n");
		return -1;
	}

	memset(Buf, 0x00, BufLen);

	// 3. load bin to buffer
	// 3.1 open bin
	fp = fopen(argv[1], "rb");
	if( fp == NULL){
		printf("source file open error\n");
		free(Buf);
		return -1;
	}

	// 3.2 get bin size
	fseek(fp, 0L, SEEK_END);
	fileLen = ftell(fp);
	fseek(fp, 0L, SEEK_SET);

	// 3.3 original bin should not over 16K-16byte
	count = (fileLen < (IMG_SIZE - SPL_HEADER_SIZE))
		? fileLen : (IMG_SIZE - SPL_HEADER_SIZE);

	// 3.4 buffer[0~15] store "S5PC110 HEADER  "
	memcpy(&Buf[0], SPL_HEADER, SPL_HEADER_SIZE);

	// 3.5 load original bin to buffer[16]
	nbytes = fread(Buf + SPL_HEADER_SIZE, 1, count, fp);
	if ( nbytes != count ){
		printf("source file read error\n");
		free(Buf);
		fclose(fp);
		return -1;
	}
	fclose(fp);

	// 4. calculate checksum

 	// 4.1 how many 1 in buffer from 16byte
	a = Buf + SPL_HEADER_SIZE;
	for(i = 0, checksum = 0; i < IMG_SIZE - SPL_HEADER_SIZE; i++)
		checksum += (0x000000FF) & *a++;

	// 4.2 store checksum in buffer[8~15]
	a = Buf + 8;
	*( (unsigned int *)a ) = checksum;

	// 5. copy buffer content to dest bin

	// 5.1 open dest bin
	fp = fopen(argv[2], "wb");
	if (fp == NULL){
		printf("destination file open error\n");
		free(Buf);
		return -1;
	}

	// 5.2 16k buffer copy to dest bin
	a = Buf;
	nbytes	= fwrite( a, 1, BufLen, fp);
	if ( nbytes != BufLen ){
		printf("destination file write error\n");
		free(Buf);
		fclose(fp);
		return -1;
	}

	free(Buf);
	fclose(fp);

	return 0;
}

