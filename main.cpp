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
    cout << "��Ȯ���� " << rate << " �Դϴ� . " << endl;

}

int main() {
	make_reference_DNA();
    make_my_DNA(100);
    make_shortread(100, 30000);

    clock_t start, end;
    start = clock();
    restore_DNA(100, 30000); // ���� �κ��� �����Ͻø� �˴ϴ�
    end = clock();
    double duration = (double)(end - start) / CLOCKS_PER_SEC;
    cout << "�ɸ��ð�(sec) : " << duration << "s" << endl;

    string title1 = "my_DNA.txt";
    string title2 = "restore.txt";
    compare(title1, title2);
    return 0;
}
