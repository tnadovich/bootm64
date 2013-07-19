#include "convert.h"

uint16_t hexstr2int(char msig, char lsig) {
	uint16_t out = 0;
	if ((msig <= '9') && (msig >= '0')) {
		out += (msig - '0') * 16;
	} else if (msig <= 'F' && msig >= 'A') {
		out += (msig - 'A' + 10) * 16;
	} else {
		return -1;
	}
	if ((lsig <= '9') && (lsig >= '0')) {
		out += lsig - '0';
	} else if (lsig <= 'F' && lsig >= 'A') {
		out += lsig - 'A' + 10;
	} else {
		return -1;
	}
	return out;
}
