#include "include/definitions.h"
#include "qc/hdlc.h"
#include "util/hexdump.h"
#include <vector>
#include <iostream>
#include <stdio.h>

using namespace std;

int main();
void test_basic();
void test_escape();
void test_unescape();
void test_vector_basic();
void test_vector_escape();
void test_vector_unescape();


static const uint8_t test_hdlc_basic[] = { 0x01, 0x02, 0x03, 0x04 };
static const uint8_t test_hdlc_basic_encapsulated[] = { HDLC_CONTROL_CHAR, 0x01, 0x02, 0x03, 0x04, 0x91, 0x39, HDLC_CONTROL_CHAR };
static const uint8_t test_hdlc_escape[] = { 0x01, 0x02, 0x03, HDLC_CONTROL_CHAR, 0x04, HDLC_ESC_CHAR, 0x05, 0x06, HDLC_CONTROL_CHAR, 0x07 };
static const uint8_t test_hdlc_escape_encapsulated[] = { HDLC_CONTROL_CHAR, 0x01, 0x02, 0x03, HDLC_ESC_CHAR, 0x5e, 0x04, HDLC_ESC_CHAR, 0x5d, 0x05, 0x06, HDLC_ESC_CHAR, 0x5e, 0x07, 0xd6, 0x16, HDLC_CONTROL_CHAR };


void test_vector_basic()
{
	printf("Starting Basic Vector Encapsulation Test\n");

	vector<uint8_t> test_hdlc_basic_v(test_hdlc_basic, test_hdlc_basic + 4);

	printf("Testing Data:\n");

	hexdump(&test_hdlc_basic_v[0], test_hdlc_basic_v.size());

	hdlc_request(test_hdlc_basic_v);

	printf("Resulting Encapsulated Data:\n");

	hexdump(&test_hdlc_basic_v[0], test_hdlc_basic_v.size());

	if (test_hdlc_basic_v.size() != sizeof(test_hdlc_basic_encapsulated)) {
		printf("Test Failed. Encapsulated data is not the expected size\n");
		return;
	}

	for (int i = 0; i < test_hdlc_basic_v.size(); i++) {
		if (test_hdlc_basic_v[i] != test_hdlc_basic_encapsulated[i]) {
			printf("Test Failed. Encapsulated data contains unexpected data\n");
			return;
		}
	}

	printf("Basic Vector Encapsulation: PASS\n");

}

void test_vector_escape()
{
	printf("Starting Vector Escaping Test\n");

	vector<uint8_t> test_hdlc_escape_v(test_hdlc_escape, test_hdlc_escape + 10);

	printf("Testing Data:\n");

	hexdump(&test_hdlc_escape_v[0], test_hdlc_escape_v.size());

	hdlc_request(test_hdlc_escape_v);

	printf("Resulting Encapsulated Data\n");

	hexdump(&test_hdlc_escape_v[0], test_hdlc_escape_v.size());
	
	if (test_hdlc_escape_v.size() != sizeof(test_hdlc_escape_encapsulated)) {
		printf("Test Failed. Encapsulated data is not the expected size - %lu %li", test_hdlc_escape_v.size(), sizeof(test_hdlc_escape_encapsulated));
		return;
	}

	for (int i = 0; i < test_hdlc_escape_v.size(); i++) {
		if (test_hdlc_escape_v[i] != test_hdlc_escape_encapsulated[i]) {
			printf("Test Failed. Encapsulated data contains unexpected data");
			return;
		}
	}
	
	printf("Escaping Vector: PASS\n"); 
}

void test_vector_unescape()
{
	printf("Starting Vector Unescape Test\n");

	vector<uint8_t> test_hdlc_escape_v(test_hdlc_escape, test_hdlc_escape + 10);

	printf("Testing Data:\n");

	hexdump(&test_hdlc_escape_v[0], test_hdlc_escape_v.size());

	hdlc_request(test_hdlc_escape_v);
	
	printf("Resulting Encapsulated Data\n");

	hexdump(&test_hdlc_escape_v[0], test_hdlc_escape_v.size());

	hdlc_response(test_hdlc_escape_v);

	printf("Resulting Unescaped Data\n");

	hexdump(&test_hdlc_escape_v[0], test_hdlc_escape_v.size());

	if (test_hdlc_escape_v.size() != sizeof(test_hdlc_escape)) {
		printf("Test Failed. Encapsulated data is not the expected size - %lu %li", test_hdlc_escape_v.size(), sizeof(test_hdlc_escape));
		return;
	}

	for (int i = 0; i < test_hdlc_escape_v.size(); i++) {
		if (test_hdlc_escape_v[i] != test_hdlc_escape[i]) {
			printf("Test Failed. Encapsulated data contains unexpected data");
			return;
		}
	}

	printf("Unescaping Vector: PASS\n");
}

void test_basic()
{
	printf("Starting Basic Encapsulation Test\n");

	printf("Testing Data:\n");

	hexdump((uint8_t*)test_hdlc_basic, sizeof(test_hdlc_basic));

	uint8_t* out = NULL;
	size_t outSize = 0;

	hdlc_request((uint8_t*)test_hdlc_basic, sizeof(test_hdlc_basic), &out, outSize);
	
	printf("Resulting Encapsulated Data:\n");

	hexdump(out, outSize);

	if (outSize  != sizeof(test_hdlc_basic_encapsulated)) {
		free(out);
		printf("Test Failed. Encapsulated data is not the expected size\n");
		return;
	}

	for (int i = 0; i < outSize; i++) {
		if (out[i] != test_hdlc_basic_encapsulated[i]) {
			free(out);
			printf("Test Failed. Encapsulated data contains unexpected data\n");
			return;
		}
	}

	free(out);

	printf("Basic Encapsulation: PASS\n");
}

void test_escape()
{
	printf("Starting Escaping Test\n");

	printf("Testing Data:\n");

	hexdump((uint8_t*)test_hdlc_escape, sizeof(test_hdlc_escape));

	uint8_t* out = NULL;
	size_t outSize = 0;

	hdlc_request((uint8_t*)test_hdlc_escape, sizeof(test_hdlc_escape), &out, outSize);

	printf("Resulting Encapsulated Data\n");

	hexdump(out, outSize);

	if (outSize != sizeof(test_hdlc_escape_encapsulated)) {
		free(out); 
		printf("Test Failed. Encapsulated data is not the expected size\n");
		return;
	}

	for (int i = 0; i < outSize; i++) {
		if (out[i] != test_hdlc_escape_encapsulated[i]) {
			free(out);
			printf("Test Failed. Encapsulated data contains unexpected data\n");
			return;
		}
	}

	free(out);

	printf("Escaping Vector: PASS\n");
}

void test_unescape()
{
	printf("Starting Unescape Test\n");


	printf("Testing Data:\n");

	hexdump((uint8_t*)test_hdlc_escape, sizeof(test_hdlc_escape));

	uint8_t* out = NULL;
	size_t outSize = 0;


	hdlc_request((uint8_t*)test_hdlc_escape, sizeof(test_hdlc_escape), &out, outSize);

	printf("Resulting Encapsulated Data\n");

	hexdump(out, outSize);

	uint8_t* unescaped = NULL;
	size_t unescapedSize = 0;

	hdlc_response(out, outSize, &unescaped, unescapedSize);

	printf("Resulting Unescaped Data\n");

	hexdump(&unescaped[0], unescapedSize);

	if (unescapedSize != sizeof(test_hdlc_escape)) {
		free(out);
		free(unescaped);
		printf("Test Failed. Encapsulated data is not the expected size\n");
		return;
	}

	for (int i = 0; i < unescapedSize; i++) {
		if (unescaped[i] != test_hdlc_escape[i]) {
			free(out);
			free(unescaped); 
			printf("Test Failed. Encapsulated data contains unexpected data\n");
			return;
		}
	}

	free(out);
	free(unescaped);

	printf("Unescaping: PASS\n");
}


int main() {
	printf("\n\n------------\nStarting Standard Tests\n------------\n\n");
	test_basic();
	test_escape();
	test_unescape(); 
	/*
	printf("\n\n------------\nStarting Vector Tests\n------------\n\n");
	test_vector_basic();
	test_vector_escape();
	test_vector_unescape();

	*/
	cout << "\n\nPress Enter To Exit" << endl;
	int pause = getchar();
	return 0;
}
