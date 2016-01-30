#include "simplelink.h"

#include "global.h"

_i16 startDevice()
{
	return status.role = sl_Start(NULL, NULL, NULL);
}

void stopDevice()
{
	sl_Stop(0xff);
	status.role = -1;
}

_i16 setDeviceRole(_i16 role)
{
	long ret;
	if (status.role != role) {
		//sprintf(buffer, ESC_YELLOW "Changing device role from %d to %d...\n", status.role, role);
		if ((ret = sl_WlanSetMode(ROLE_STA)) != 0)
			return status.role;
		//uart0_write_string(ESC_YELLOW "Stopping device...\n");
		stopDevice();
		//uart0_write_string(ESC_YELLOW "Starting device...\n");
		startDevice();
	}
	return status.role;
}
