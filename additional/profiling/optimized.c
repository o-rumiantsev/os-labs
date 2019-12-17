#include <stdio.h>

void new_func1(void);
int resultOfSum(int, int);

int func1(int a, int b) {
	int res = 0;

    for (int i = 0; i < 10; i++)	{
//		sleep(1);
		if (i > 8) {
			res = resultOfSum(a, b);
		}
		if (res > 0) {
			return res;
		}
	}

    return res;
}

int func2(int a, int b, int c) {
	int res = 0;

    for (int i = 0; i < 10; i++) {
		c++;
		if (c > b) {
		    res = func1(a, b);
		} else {
    		res = func1(a, c);
		}
		if (res > 0) return res;
	}

    return res;
}


int resultOfSum(int a, int b) {
	return a + b;
}

int main() {
    func2(51, 110, 12);
    return 0;
}