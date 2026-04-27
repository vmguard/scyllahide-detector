#include <iostream>
#include <windows.h>
#include "scyllahidedetector.h"

int main() {
	std::cout << "sleeping for 15 seconds\n";
	Sleep(15000);
	if (IsScyllaHidePresent())
		std::cout << "ScyllaHide detected";
	else
		std::cout << "ScyllaHide not found";
	std::cin.get();
	return 0;

}
