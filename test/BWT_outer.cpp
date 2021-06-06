//2019111982 차영훈
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <fstream>
#include <stdlib.h>
#include <math.h>
#include <sstream>
#include "main.h"
#define SIZE 10
using namespace std;

static pair<string, string> split(string input) {
    pair<string, string> answer;
    stringstream ss(input);

    getline(ss, answer.first, ' ');
	getline(ss, answer.second, ' ');

    return answer;
}

static string ft_itoa(int i) {
	return to_string(i);
}

void BWT_print(vector<pair<int, string>> &table) {
	for (int i = 0 ; i < table.size(); i++) {
		cout << table[i].first << " " << table[i].second << endl;
	}
	cout << endl;
}

static bool compare(pair<string, string> t1, pair<string, string> t2) { // pair 이기 떄문에 정렬에 필요한 함수
	return (t1.second < t2.second);
}

void sort_one(string input, string output) {
	string str1;
	string str2;
	ifstream f_in;
	ofstream f_out;
	vector<pair<string, string>> table;

	f_in.open(input);
	f_out.open(output);

	cout << input << " : sort start" << endl;
	while(f_in) {
		getline(f_in, str1);
		if (str1 == "")
			continue;
		table.push_back(split(str1));
	}
	sort(table.begin(), table.end(), compare);
	for (auto i : table) {
		str1 = i.first + " " + i.second + "\n";
		f_out.write(str1.c_str(), str1.size());
	}
	cout << input << " : sort done" << endl;
	f_in.close();
	f_out.close();
}

void sort_merge(string input1, string input2, string output) {
	string str1;
	string str2;
	ifstream f_in1;
	ifstream f_in2;
	ofstream f_out;

	f_in1.open(input1);
	f_in2.open(input2);
	f_out.open(output);

	cout << "merge " << input1 << " " << input2 << " to " << output << endl;

	getline(f_in1, str1);
	getline(f_in2, str2);
	while (f_in1 && f_in2) {
		if (compare(split(str1), split(str2))) {
			f_out.write(str1.c_str(), str1.size());
			f_out.write("\n", 1);
			getline(f_in1, str1);
			if (str1 == "")
				continue;
		} else {
			f_out.write(str2.c_str(), str2.size());
			f_out.write("\n", 1);
			getline(f_in2, str2);
			if (str2 == "")
				continue;
		}
	}
	while(f_in1) {
		f_out.write(str1.c_str(), str1.size());
		f_out.write("\n", 1);
		getline(f_in1, str1);
		if (str1 == "")
			continue;
	}
	while(f_in2) {
		f_out.write(str2.c_str(), str2.size());
		f_out.write("\n", 1);
		getline(f_in2, str2);
		if (str2 == "")
			continue;
	}
	f_in1.close();
	f_in2.close();
	f_out.close();
}

static void BWT_sort(int n) { // 정렬
	int process_num;
	int file_index;
	int file_num;
	string file_name;

	file_num = n;
	process_num = 2;
	file_name = "bwt_table_";
	for (file_index = 0 ; file_index < file_num; file_index++) {
		sort_one(file_name + "unsorted_" + ft_itoa(file_index) + ".txt", \
				 file_name + "2_" + ft_itoa(file_index) + ".txt");
	}
	while (process_num < n) {
		for (file_index = 0 ; file_index < file_num; file_index += process_num) {
			sort_merge(file_name + ft_itoa(process_num) + "_" + ft_itoa(file_index) + ".txt", \
						file_name + ft_itoa(process_num) + "_" + ft_itoa(file_index + process_num / 2) + ".txt", \
						file_name + ft_itoa(process_num * 2) + "_" + ft_itoa(file_index) + ".txt");
		}
		process_num *= 2;
	}
	sort_merge(file_name + ft_itoa(process_num) + "_" + ft_itoa(0) + ".txt", \
				file_name + ft_itoa(process_num) + "_" + ft_itoa(process_num / 2) + ".txt", \
				"bwt_sort_result.txt");
}

static int BWT_filltherest(int n, string T) { // 나머지 부분 채우는 함수
	int index;
	int file_index;
	int little_n;
	string file_name;
	string line;
	ofstream f;

	index = 0;
	file_name = "bwt_table_unsorted_";
	little_n = ceil(T.size() / (double)n);
	cout << T.size() << " : " << little_n << endl;
	for (file_index = 0 ; file_index < n && index < T.size(); file_index++) {
		cout << file_name + ft_itoa(file_index) + ".txt" << " doing !" << endl;
		f.open(file_name + ft_itoa(file_index) + ".txt");
		for (int j = 0; j < little_n && index < T.size(); j++) {
			line = ft_itoa(index++) + " " + T + "\n";
			f.write(line.c_str(), line.size());
			T.push_back(T[0]);
			T.erase(0, 1);
		}
		cout << file_name + ft_itoa(file_index) + ".txt" << " done !" << endl;
		f.close();
	}
	cout << file_index;
	return file_index;
}

pair<vector<int>, string> BWT(string T) {
	ifstream f_final;
	int file_num;
	string ret;
	//vector<pair<int, string>> table(T.size() + 1);
	vector<int> index;

	if (!f_final.open("bwt_index_table.txt")) {
//	cout << "T : " << T << "\n\n";
	T.push_back('$');

	cout << "Fill the rest" << "\n";
	file_num = BWT_filltherest(SIZE, T);

	cout << "Sort" << "\n";
	BWT_sort(file_num);

	cout << "Build final index table" << endl;
	string line;
	ifstream f("bwt_sort_result.txt");
	while (f) {
		getline(f, line);
		if (line == "")
			continue;
		auto _line = split(line);
		ret.push_back(_line.second.back()); // 문자열의 마지막부분을 더하여 BWT(T)를 완성한다.
		index.push_back(stoi(_line.first));
	}
	f.close();
	}
	cout << "Done " << endl;
	string line;
	ifstream f("bwt_sort_result.txt");
	while (f) {
		getline(f, line);
		if (line == "")
			continue;
		auto _line = split(line);
		ret.push_back(_line.second.back()); // 문자열의 마지막부분을 더하여 BWT(T)를 완성한다.
		index.push_back(stoi(_line.first));
	}
	f.close();
	return {index, ret};
}

void BWT_find_pre(pair<vector<int>, string> bwt, vector<int> &charset_index, vector<vector<int>> &tally, vector<int> &start_index) {
	string last;
	string front;
	string charset;

	// build charset
	front = last = bwt.second;
	sort(front.begin(), front.end());
	charset = front;
	charset.erase(0, 1);
	charset.erase(unique(charset.begin(), charset.end()), charset.end());
	for (int i = 0; i < charset.size(); i++)
		charset_index[charset[i]] = i;

	// build tally
	tally.push_back(vector<int>(charset.size(), 0));
	tally[0][charset_index[last[0]]]++;
	for (int i = 1; i < last.size(); i++) {
		tally.push_back(tally.back());
		if (last[i] == '$')
			continue;
		tally[i][charset_index[last[i]]]++;
	}

	// build start_index array
	start_index = vector<int>(charset.size(), -1);
	for (int i = 0; i < front.size(); i++) { // 해쉬에 값을 저장한다.
		if (front[i] == '$')
			continue;
		if (start_index[charset_index[front[i]]] == -1) // 이미 있는경우는 무시
			start_index[charset_index[front[i]]] = i;
	}
	start_index.push_back(front.size());
}

vector<int> BWT_find(pair<vector<int>, string> bwt, string q, vector<int> &charset_index, vector<vector<int>> &tally, vector<int> &start_index) {
	vector<int> ret;

	// querying
	int start, end, chr, _c, _start, _end;
	start = start_index[charset_index[q.back()]];
	end = start_index[charset_index[q.back()] + 1] - 1;
	//cout << start << ", " << end << endl;
	for (int i = q.size() - 2; i >= 0; i--) {
		chr = charset_index[q[i]];
		//printf("{%d %d} \n", tally[start][chr], tally[end][chr]);
		_c = tally[end][chr] - tally[start - 1][chr];
		_start = start_index[chr] + tally[end][chr] - _c;
		_end = _start + _c - 1;
		if (_end - _start < 0)
			return {-1};
		start = _start;
		end = _end;
		//cout << start << ", " << end << endl;
	}

	// get index
	for (int i = start; i <= end; i++)
		ret.push_back(bwt.first[i]);
	return (ret);
}

// int main() {
// 	pair<vector<int>, string> bwt = BWT("123456789012345678901234567890123456789"); //29
// 	cout << bwt.second << endl;
// }
