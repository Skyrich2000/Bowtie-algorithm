//2019111982 차영훈
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
    ss << file.rdbuf(); // 한번에 파일을 읽고
    string reference = ss.str(); // 텍스트로 저장


    ifstream file2(title2);
    stringstream ss2;
    ss2 << file2.rdbuf(); // 한번에 파일을 읽고
    string mine = ss2.str(); // 텍스트로 저장


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

void read_file(string my_dna, string short_read, int& N, int& m, int& k) {
    ifstream file(my_dna);
    stringstream ss;
    ss << file.rdbuf(); // 한번에 파일을 읽고
    string reference = ss.str(); // 텍스트로 저장
    string my_dna_text = ss.str();

    N = my_dna_text.size();

    ifstream fin;
    fin.open(short_read);
    string line;
    int i = 0;

    while (!fin.eof()) { // shortRead 읽기
        getline(fin, line);
        if (line.size() == 0)
            break;
        k = line.size();
        i++;
    }
    fin.close();
    m = i;
}

int main() {
	int n;
	int l;
	int m;
    int file_num = 10;

	//make_reference_DNA(n);
	//make_my_DNA(l);
	//make_shortread(l, m);

    string my_dna = "./1000/20_70/mydna.txt";
    string ref_dna ="./1000/20_70/ref.txt";
    string short_read = "./1000/20_70/shortread.txt";

    read_file(my_dna, short_read, n, m, l);

    clock_t start, end;
    start = clock();
    restore_DNA(n, m, l, file_num);
    end = clock();
    double duration = (double)(end - start) / CLOCKS_PER_SEC;
    cout << "duration(sec) : " << duration << "s" << endl;

	compare(my_dna, "restore.txt");
    return 0;
}
