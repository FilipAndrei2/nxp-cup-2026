extern "C" void _exit(int status) {
	while (1) {};
}

int main(void) {
	int a = 2;
	int b = 2;
	return a - b;
}
