/*
 * MappedElementTest.cpp
 *
 *  Created on: 27 Mar 2012
 *      Author: cjones
 */

#include <iostream>
#include "slic++/Engine.hpp"

using namespace std;
using namespace slic;

SLIC_DESIGN(MappedElementTest)

int main() {
	MaxFile maxfile(&MappedElementTest_init);
	Actions actions(maxfile, "default");

	auto N = maxfile.getConstant("N");

	// inputs
	srand(12345);

	double a = rand();
	vector<double> b(N);

	for (int n = 0; n < N; ++n)
		b[n] = rand();

	actions.set("MappedElementTestKernel", "a", a);
	actions.setMem("MappedElementTestKernel", "b", b);
	actions.ignoreMemInput("MappedElementTestKernel", "result"); // this is an output

	// # ticks is set implicitly by using the 'default' mode for this bitstream

	// outputs
	double first, second;
	vector<double> fromStream(N);
	vector<double> fromRAM(N);

	actions.get("MappedElementTestKernel", "first",  &first);
	actions.get("MappedElementTestKernel", "second", &second);

	// the output vectors are sized correctly above
	for (uint i = 0; i < N; ++i) {
		actions.getMem("MappedElementTestKernel", "result", i, &fromRAM[n]);
	}

	actions.queueOutput("output", fromStream.data(), fromStream.size()*sizeof(double));

	Engine(maxfile).run(actions);

	// check output
	vector<double> expected(N);
	for (int n = 0; n < N; ++n)
		expected[n] = a * b[n];

	for (int n = 0; n < N; ++n) {
		if (fromStream[n] != expected[n])
			goto FAILED;

		if (fromRAM[n] != expected[n])
			goto FAILED;
	}

	if (first != expected[0])
		goto FAILED;

	if (second != expected[1])
		goto FAILED;

	cout << "Test PASSED" << endl;
	return 0;

	FAILED:
	cout << "Test FAILED" << endl;
	return 1;
}
