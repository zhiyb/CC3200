#include "simplelink.h"

// Standard includes
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

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

void receiveFile(char *cmd)
{
	_i32 size = atol(cmd);
	if (size <= 0)
		return;
	while (!isspace(*cmd) && *cmd != '\0')
		cmd++;
	while (isspace(*cmd) && *cmd != '\0')
		cmd++;

	uart0_write_string(ESC_YELLOW "Opening file ");
	uart0_write_string(cmd);
	uart0_write_string("for read...\n");
	_i32 handle;
	_i32 ret;
	if ((ret = sl_FsOpen(cmd, FS_MODE_OPEN_CREATE(size, FS_MODE_OPEN_WRITE), NULL, &handle)) != 0) {
		sprintf(buffer, ESC_GREY "Error: %d\n", ret);
		uart0_write_string(buffer);
	}

	uart0_write_string(ESC_CYAN "Receiving file data...\n" ESC_DEFAULT);
	_u32 offset = 0;
	while (size) {
		_u32 block = sizeof(buffer) > size ? size : sizeof(buffer);
		uart0_read_data(buffer, block);
		if ((ret = sl_FsWrite(handle, offset, buffer, block)) != block) {
			sprintf(buffer, ESC_GREY "Error: %d\n", ret);
			uart0_write_string(buffer);
		}
		size -= block;
		offset += block;
	}
	sprintf(buffer, ESC_CYAN "%u bytes received.\n", offset);
	uart0_write_string(buffer);
	sl_FsClose(handle, NULL, NULL, 0);
}
