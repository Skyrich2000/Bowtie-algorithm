/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   make_txts.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ycha <ycha@gmail.com>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/05 09:39:39 by ycha              #+#    #+#             */
/*   Updated: 2021/06/05 09:39:39 by ycha             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <random>
#include <vector>
#include "main.h"

using namespace std;

void make_reference_DNA() {
    int SIZE;
	ifstream readFile;
	string referenceFile = "chr1.fna";

    cout << referenceFile << " 읽는 중 . . ." << endl;

	readFile.open(referenceFile);
	string description;
	stringstream ss;
	getline(readFile, description); // 첫줄은 해당 파일에 대한 description
    cout << description << endl;
	ss << readFile.rdbuf(); // 한번에 파일을 읽고
	string text = ss.str(); // 텍스트로 저장
	readFile.close();


    string reference = "";
	for (int i = 0; i < text.size(); i++) {
		if (text[i] == 'A' || text[i] == 'G'
			|| text[i] == 'C' || text[i] == 'T') {
			reference += text[i];
		}
	}
    cout << referenceFile << " 읽기 완료 . . ." << endl;

    cout << referenceFile << "의 총 길이 : " <<  reference.size() << endl;
    SIZE = reference.size();
	ofstream writeFile;
    string title = "reference_DNA.txt";

    cout << title << " 생성 중 . . ." << endl;

	writeFile.open(title);
	writeFile.write(reference.c_str(), 100000);
    writeFile.close();

    cout << title << " 생성 완료 . . ." << endl;
}

void make_my_DNA(int k) {
    int SIZE;
    std::random_device rd;
    std::mt19937 gen(rd());
    ofstream writeFile;
    string title = "reference_DNA.txt";

    ifstream file(title);
    stringstream ss;
    ss << file.rdbuf(); // 한번에 파일을 읽고
    string text = ss.str(); // 텍스트로 저장
    SIZE = text.size();

    string outputTitle = "my_DNA.txt";
    cout << outputTitle << " 생성중 . . . (k : " << k << ")" << endl;
    writeFile.open(outputTitle);


    for (int i = 0; i < SIZE; i += k) { // k만큼 건너뛰면서 0~2개 바꾸기
        if (i + k - 1 >= SIZE) // 랜덤 인덱스 더해서 SIZE 넘어가는 상황 방지
            break;

        std::uniform_int_distribution<int> distribution2(0, 2); // 바꿀 개수 0~2개 랜덤
        int rand_snip = distribution2(gen); // 바꿀 개수
        for (int j = 0; j < rand_snip; j++) {

            std::uniform_int_distribution<int> distribution3(0, k-1); // 0 ~ k-1까지 랜덤 인덱스
            int snip_index = i + distribution3(gen); // i + 랜덤 인덱스 해서 바꿀 인덱스
            char snip = text[snip_index]; // snip
            //cout << i <<  "에서 " << snip_index << "변함" << endl;
            switch (snip) {

            case 'A': {
                char new_snip;
                int rand_new = distribution2(gen);
                if (rand_new == 0) new_snip = 'C'; else if (rand_new == 1) new_snip = 'G'; else if (rand_new == 2) new_snip = 'T';
                text[snip_index] = new_snip;
                break;
            }
            case 'C': {
                char new_snip;
                int rand_new = distribution2(gen);
                if (rand_new == 0) new_snip = 'A'; else if (rand_new == 1) new_snip = 'G'; else if (rand_new == 2) new_snip = 'T';
                text[snip_index] = new_snip;
                break;
            }
            case 'G': {
                char new_snip;
                int rand_new = distribution2(gen);
                if (rand_new == 0) new_snip = 'C'; else if (rand_new == 1) new_snip = 'A'; else if (rand_new == 2) new_snip = 'T';
                text[snip_index] = new_snip;
                break;
            }
            case 'T': {
                char new_snip;
                int rand_new = distribution2(gen);
                if (rand_new == 0) new_snip = 'C'; else if (rand_new == 1) new_snip = 'G'; else if (rand_new == 2) new_snip = 'A';
                text[snip_index] = new_snip;
                break;
            }
            }

        }

    }

    writeFile.write(text.c_str(), text.size());

    writeFile.close();

    cout << outputTitle << " 생성 완료 . . . (k : " << k << ")" << endl;

}

void make_shortread(int k, int n) {
    std::random_device rd;
    std::mt19937 gen(rd());

    ofstream writeFile;
    string title = "my_DNA.txt";

    ifstream file(title);
    stringstream ss;
    ss << file.rdbuf(); // 한번에 파일을 읽고
    string text = ss.str(); // 텍스트로 저장

    int SIZE = text.size();
    std::uniform_int_distribution<int> distribution(0, SIZE - k); // 랜덤 0 ~ SIZE - k

    string outputTitle = "short_read.txt";
    writeFile.open(outputTitle);
    cout << outputTitle << " 생성중 . . . (k : " << k << ", n : " << n << ")" << endl;


    for (int i = 0; i < n; i++) {
        int rand_index = distribution(gen);
        string read = text.substr(rand_index, k); // rand index에서 k개만큼 읽기
        writeFile.write(read.c_str(), read.size());
        if (i != n - 1) // 마지막에만 \n 추가 안하기
            writeFile.write("\n", 1);
    }

    cout << outputTitle << " 생성 완료 . . . (k : " << k << ", n : " << n << ")" << endl;
}
