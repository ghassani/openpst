/**
* LICENSE PLACEHOLDER
*/

#include "hexdump.h"


void hexdump(unsigned char *data, unsigned int amount) {
	unsigned int    dp, p;  /* data pointer */
	for (dp = 1; dp <= amount; dp++) {
		fprintf(stderr, "%02x ", data[dp - 1]);
		if ((dp % 8) == 0)
			fprintf(stderr, " ");
		if ((dp % 16) == 0) {
			fprintf(stderr, "| ");
			p = dp;
			for (dp -= 16; dp < p; dp++)
                fprintf(stderr, "%c", hex_trans[data[dp]]);
			fflush(stderr);
			fprintf(stderr, "\n");
		}
		fflush(stderr);
	}
	// tail
	if ((amount % 16) != 0) {
		p = dp = 16 - (amount % 16);
		for (dp = p; dp > 0; dp--) {
			fprintf(stderr, "   ");
			if (((dp % 8) == 0) && (p != 8))
				fprintf(stderr, " ");
			fflush(stderr);
		}
		fprintf(stderr, " | ");
		for (dp = (amount - (16 - p)); dp < amount; dp++)
            fprintf(stderr, "%c", hex_trans[data[dp]]);
		fflush(stderr);
	}
	fprintf(stderr, "\n");

	return;
}

void hexdump(unsigned char *data, unsigned int amount, QString& out)
{
    unsigned int    dp, p;  /* data pointer */


    //hexdump(data, amount);

    QString tmp;

    for (dp = 1; dp <= amount; dp++) {
        tmp.sprintf("%02x ", data[dp - 1]);
        out.append(tmp);

        if ((dp % 8) == 0) {
            out.append(" ");
        }

        if ((dp % 16) == 0) {
            out.append("| ");
            p = dp;
            for (dp -= 16; dp < p; dp++) {
                tmp.sprintf("%c ", hex_trans[data[dp]]);
                out.append(tmp);
            }
            out.append("\n");
        }
    }

    // tail
    if ((amount % 16) != 0) {
        p = dp = 16 - (amount % 16);
        for (dp = p; dp > 0; dp--) {
            out.append("   ");
            if (((dp % 8) == 0) && (p != 8))
                out.append(" ");
        }
        out.append(" | ");
        for (dp = (amount - (16 - p)); dp < amount; dp++)
            tmp.sprintf("%c ", hex_trans[data[dp]]);
            out.append(tmp);
    }

    out.append("\n");

    return;
}
