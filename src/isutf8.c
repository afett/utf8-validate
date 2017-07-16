#include <utf8-validate.h>
#include <stdio.h>

int main()
{

	FILE *in = stdin;
	utf8_state state = 0;
	for (;;) {
		if (feof(in)) {
			break;
		}

		if (ferror(in)) {
			fprintf(stderr, "read error\n");
			return 127;
		}

		char buf[4096];
		size_t read = fread(buf, 1, sizeof(buf), in);
		if (!utf8_validate_some(&state, buf, read)) {
			break;
		}
	}

	if (state < 0) {
		return 1;
	}

	if (state > 0) {
		return 2;
	}

	return 0;
}
