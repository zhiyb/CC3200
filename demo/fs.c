#include "simplelink.h"

// Standard includes
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>

// Local includes
#include "uart0/uart0.h"
#include "common/escape.h"
#include "global.h"
#include "fs.h"

void printFileInfo(const char *path)
{
	SlFsFileInfo_t info;
	_i16 ret = sl_FsGetInfo(path, 0, &info);
	if (ret != 0) {
		uart0_write_string(ESC_GREY "Getting file info of ");
		uart0_write_string(path);
		uart0_write_string(" failed with: ");
		sprintf(buffer, "%d\n", ret);
		uart0_write_string(buffer);
		return;
	}

	sprintf(buffer, ESC_WHITE "Flags: 0x%04x\n", info.flags);
	uart0_write_string(buffer);
	sprintf(buffer, "Length: %lu\n", info.FileLen);
	uart0_write_string(buffer);
	sprintf(buffer, "Allocated length: %lu\n", info.AllocatedLen);
	uart0_write_string(buffer);
	uart0_write_string("Token: ");
	_u32 i;
	for (i = 0; i < 4; i++) {
		sprintf(buffer, "0x%08lx ", info.Token[i]);
		uart0_write_string(buffer);
	}
	uart0_write_string("\n");
}
