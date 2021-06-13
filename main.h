//2019111982 차영훈
#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
//#define SIZE 100000

using namespace std;

void make_reference_DNA(int n);
void make_my_DNA(int k);
void make_shortread(int k, int n);
void restore_DNA(int size, int n, int k, int file_num);

pair<vector<int>, string> BWT(string T, int file_num);
void BWT_find_pre(pair<vector<int>, string>& bwt,
                  vector<int> &charset_index, vector<vector<int>> &tally,
                  vector<int> &start_index);
vector<int> BWT_find(pair<vector<int>, string>& bwt, string q,
                     vector<int> &charset_index, vector<vector<int>> &tally,
                     vector<int> &start_index);
