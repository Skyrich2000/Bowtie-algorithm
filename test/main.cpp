#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <random>
#include <vector>
#include <ctime>
#include "main.h"

using namespace std;

void compare(string title1, string title2) {

    ofstream writeFile;

    ifstream file(title1);
    stringstream ss;
    ss << file.rdbuf(); // �ѹ��� ������ �а�
    string reference = ss.str(); // �ؽ�Ʈ�� ����


    ifstream file2(title2);
    stringstream ss2;
    ss2 << file2.rdbuf(); // �ѹ��� ������ �а�
    string mine = ss2.str(); // �ؽ�Ʈ�� ����


    int correct = 0;
    int noncorrect = 0;
    for (int i = 0; i < reference.size(); i++) {


        if (reference[i] == mine[i]) {
            correct++;
        }
        else {
            noncorrect++;
        }
    }

    double rate = (double)correct / reference.size();
	cout << "acc : " << rate << endl;

}

int main() {
	int n = 500000;
	int l = 100;
	int m = 50000;

	//make_reference_DNA(n);
	make_my_DNA(l);
	make_shortread(l, m);

    clock_t start, end;
    start = clock();
    restore_DNA(l, m); // ���� �κ��� �����Ͻø� �˴ϴ�
    end = clock();
    double duration = (double)(end - start) / CLOCKS_PER_SEC;
    cout << "duration(sec) : " << duration << "s" << endl;

    string title1 = "my_DNA.txt";
    string title2 = "restore.txt";
	compare(title1, title2);
    return 0;
}
