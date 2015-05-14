/*
 * PassthroughTest.cpp
 *
 *  Created on: 27 Mar 2012
 *      Author: cjones
 */

#include <iostream>
#include "slic++/Engine.hpp"

using namespace std;
using namespace slic;

SLIC_DESIGN(PassthroughTest)

int main() {
	MaxFile maxfile(&PassthroughTest_init);
	Actions actions(maxfile);

	const uint N = 1000;

	vector<double> input(N);
	vector<double> output(N);

	for (int n = 0; n < N; ++n)
		input[n] = n;

	actions.queueInput("input", input);
	// the output vector is sized correctly above to be large enough to receive the output
	actions.queueOutput("output", output.data(), output.size()*sizeof(double));
	actions.setTicks("PassthroughTestKernel", N);

	// open a device, run the actions, close the device...
	Engine(maxfile).run(actions);

	for (int n = 0; n < N; ++n) {
		if (output[n] != input[n])
			goto FAILED;
	}

	cout << "Test PASSED" << endl;
	return 0;

	FAILED:
	cout << "Test FAILED" << endl;
	return 1;
}
