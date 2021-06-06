#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <random>
#include <vector>
#include "main.h"

using namespace std;

static int check_valid_index(string &ref, string &short_read, int index, int mismatch) {
	int mis;

	mis = mismatch;
	for (int j = 0; j < short_read.size(); j++)
		if (ref[index + j] != short_read[j]) { // 다른게 있는지 확인
			if (mis == 0)
				return 0;
			mis--; // mis 만큼 허용
		}
	return (mismatch - mis);
}


void restore_DNA(int k, int n) { // 10-3 BruteForce 버전입니다.
	int size;
	ofstream writeFile;
	string title = "reference_DNA.txt"; // 왜 my_DNA 를 ..?
	ifstream file(title);
	stringstream ss;
	ss << file.rdbuf(); // 한번에 파일을 읽고
	string targetText = ss.str(); // 텍스트로 저장
	size = targetText.size();

	string outputTitle = "restore.txt";
	cout << outputTitle << " 생성중 . . . (k : " << k << ", n : " << n << ")" << endl;

	pair<vector<int>, string> bwt;
	vector<int> charset_index(127, 0);
	vector<vector<int>> tally;
	vector<int> start_index;

	cout << "build BWT index" << endl;
	bwt = BWT(targetText);
	BWT_find_pre(bwt, charset_index, tally, start_index);
	cout << "done!" << endl;

	int flag;
	int mismatch = 3;
	int little_k = k / (mismatch + 1);
	printf("mismatch : %d , k_per_mismatch : %d\n", mismatch, little_k);
	string shortRead;
	string restoreText(size, '^');
	ifstream fin;
	fin.open("short_read.txt");
	for (int i = 0; i < n; i++) {
		getline(fin, shortRead);
		if (i % 100 == 0)
			cout << i << " 번째 shortRead :" << shortRead << " 처리중 . . ." << " (" << (double)i / n * 100 << ")" <<  endl;
		for (int j = 0; j < mismatch + 1; j++) {
			flag = 0;
			for (auto index : BWT_find(bwt, shortRead.substr(little_k * j, little_k), charset_index, tally, start_index)) {
				if (index == -1)
					continue;
				if (check_valid_index(targetText, shortRead, index - little_k * j, mismatch)) {
					for (int l = 0; l < k; l++)
						restoreText[index - little_k * j + l] = shortRead[l]; // my dna에 추가
					flag = 1;
					break;
				}
				if (flag)
					break;
			}
			if (flag)
				break;
		}
	}
	fin.close();

	writeFile.open(outputTitle);
	writeFile.write(restoreText.c_str(), restoreText.size());
	writeFile.close();
	cout << outputTitle << " 생성 완료 . . . (k : " << k << ", n : " << n << ")" << endl;

}
