//2019111982 차영훈
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include "main.h"

using namespace std;

void BWT_print(vector<pair<int, string>> &table) {
	for (int i = 0 ; i < table.size(); i++) {
		cout << table[i].first << " " << table[i].second << endl;
	}
	cout << endl;
}

static bool compare(pair<int, string> t1, pair<int, string> t2) { // pair 이기 떄문에 정렬에 필요한 함수
	return (t1.second < t2.second);
}

static void BWT_sort(vector<pair<int, string>> &table) { // 정렬
	sort(table.begin(), table.end(), compare);
}

static void BWT_filltherest(vector<pair<int, string>> &table, string T) { // 나머지 부분 채우는 함수
	for (int i = 0 ; i < table.size(); i++) {
		table[table.size() - 1 - i].first = i;
		table[table.size() - 1 - i].second = T;
		T.push_back(T[0]);
		T.erase(0, 1);
	}
}

string BWT_repair(string back) {
	string front;
	string bwt;
	int index;
	vector<int> hash(127, 0); // 해쉬
	vector<int> back_to_front(back.size());

	// index 		 : 0 1 2 3 4 5 6
	// front 		 : $ a a a c c g
	// back 		 : g c $ a a a c
	// back_to_front : 6 4 0 1 2 3 5

	front = back;
	sort(front.begin(), front.end());
	for (int i = 0; i < front.size(); i++) // 해쉬에 값을 저장한다.
		if (hash[front[i]] == 0) // 이미 있는경우는 무시
			hash[front[i]] = i;
	for (int i = 0; i < back.size(); i++)
		back_to_front[i] = hash[back[i]]++; // back_to_front 부분 만들기
	index = back_to_front[0];
	while (index) { // index 가 0, 즉 '$' 가 올떄 까지 반복한다.
		bwt.insert(bwt.begin(), front[index]);
		index = back_to_front[index];
	}
	return bwt;
}

pair<vector<int>, string> BWT(string T) {
	string ret;
	vector<pair<int, string>> table(T.size() + 1);
	vector<int> index;

	//cout << "T : " << T << "\n\n";
	T.push_back('$');

	//cout << "Fill the rest" << "\n";
	BWT_filltherest(table, T);

	//cout << "Sort" << "\n";
	BWT_sort(table);

	for (auto i : table) {
		ret.push_back(i.second[i.second.size() - 1]); // 문자열의 마지막부분을 더하여 BWT(T)를 완성한다.
		index.push_back(i.first);
	}
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
