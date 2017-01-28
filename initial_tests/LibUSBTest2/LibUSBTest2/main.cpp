#include "GCAdapter.h"
#include <iostream>

using namespace std;

int main() {
	GCANode::Setup();
	int result = GCANode::Load();
	cin.ignore();
	return result;
}