#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <random>
#include <ctime>
#include <algorithm>
using namespace std;
string my_dna_text;
int SIZE;
class Edge {
public:
    int src;
    int dest;
    bool visited;

    Edge(int _src, int _dest) {
        src = _src;
        dest = _dest;
        visited = false;
    }

};

class Vertex {
public:
    bool visited;
    static int id;
    int number;
    vector<Edge> edges;
    int in;
    int k;
    int out;
    string data;

    Vertex(string _data) {
        number = id++;
        in = 0;
        out = 0;
        data = _data;
        visited = false;
    }

    void add_edge(Edge newEdge) {

        if (newEdge.dest == id) {
            in++;
        }
        else if (newEdge.src == id) {
            out++;
        }
        edges.push_back(newEdge);

    }
};

int Vertex::id = 0;

class DeBruijn {
public:
    int k; int n;
    int start;
    string route;
    vector<int> euler_path;
    vector<string> start_vertex;
    vector<Vertex> vertex_list;

    DeBruijn() {
        start = 0;
        route = "";
    }

    void add_vertex(string s) {
        if (get_vertex_index(s) != -1) {
            return;
        }
        Vertex new_vertex(s);
        vertex_list.push_back(new_vertex);
    }

    int get_vertex_index(string s) {
        for (int i = 0; i < vertex_list.size(); i++) {
            if (s == vertex_list[i].data) {
                return i;
            }
        }
        return -1;
    }

    void add_edge(int _src, int _desc) {

        if (_src != -1 && _desc != -1) {
            Edge new_edge = Edge(_src, _desc);
            vertex_list[new_edge.src].add_edge(new_edge);
        }
    }

    void find_start() {

        int min = 0;
        for (int i = 0; i < vertex_list.size(); i++) {
            //cout << vertex_list[i].out << "  " << vertex_list[i].in << endl;
            if (vertex_list[i].out - vertex_list[i].in > min) {
                start_vertex.push_back(vertex_list[i].data);
                //start_vertex[0] = vertex_list[i].data;
                min = vertex_list[i].out - vertex_list[i].in;
            }

            //if (vertex_list[i].out - vertex_list[i].in > 0) {
            //    start_vertex.push_back(vertex_list[i].data);
            //}
        }
        if (start_vertex.size() == 0) {
            start_vertex.push_back(vertex_list[0].data);
        }
    }

    void dfs(int start_index) {

        //cout << "Start index : " << start_index << endl;
        int size = vertex_list[start_index].edges.size();
        if (route.size() >= my_dna_text.size()) {
            return;
        }

        if (size == 0) {
            return;
        }
        for (int i = 0; i < size; i++) {

            int x = vertex_list[start_index].edges.size();

            if (route.size() >= my_dna_text.size()) {
                return;
            }
            if (vertex_list[start_index].edges[i].visited == false) {
                vertex_list[start_index].edges[i].visited = true;
                euler_path.push_back(start_index);
                string d = vertex_list[start_index].data;
                route += d[d.size() - 1];
                //cout << "Route : " << route << endl;
                vertex_list[start_index].edges.pop_back();
                dfs(vertex_list[start_index].edges[i].dest);
            }

            //vertex_list[start_index].edges[i].visited = false;
        }
    }
};


void make_reference_DNA() {
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

    SIZE = 10000;
    cout << referenceFile << "의 총 길이 : " << SIZE << endl;
    //cout << "Ref : " << reference << endl;
    // n = SIZE;
    //n = SIZE;
    ofstream writeFile;
    string title = "reference_DNA.txt";

    cout << title << " 생성 중 . . ." << endl;

    writeFile.open(title);
    writeFile.write(reference.c_str(), reference.size());
    writeFile.close();

    cout << title << " 생성 완료 . . ." << endl;
}

void make_my_DNA(int k) {

    std::random_device rd;
    std::mt19937 gen(rd());
    ofstream writeFile;
    string title = "reference_DNA.txt";

    ifstream file(title);
    stringstream ss;
    ss << file.rdbuf(); // 한번에 파일을 읽고
    string text = ss.str(); // 텍스트로 저장


    string outputTitle = "my_DNA.txt";
    cout << outputTitle << " 생성중 . . . (k : " << k << ")" << endl;
    writeFile.open(outputTitle);


    for (int i = 0; i < SIZE; i += k) { // k만큼 건너뛰면서 0~2개 바꾸기
        if (i + k - 1 >= SIZE) // 랜덤 인덱스 더해서 SIZE 넘어가는 상황 방지
            break;

        std::uniform_int_distribution<int> distribution2(0, 2); // 바꿀 개수 0~2개 랜덤
        int rand_snip = distribution2(gen); // 바꿀 개수
        for (int j = 0; j < rand_snip; j++) {

            std::uniform_int_distribution<int> distribution3(0, k - 1); // 0 ~ k-1까지 랜덤 인덱스
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
    std::uniform_int_distribution<int> distribution(0, SIZE - k); // 랜덤 0 ~ SIZE - k

    ofstream writeFile;
    string title = "my_DNA.txt";

    ifstream file(title);
    stringstream ss;
    ss << file.rdbuf(); // 한번에 파일을 읽고
    string text = ss.str(); // 텍스트로 저장

    string outputTitle = "short_read.txt";
    writeFile.open(outputTitle);
    cout << outputTitle << " 생성중 . . . (k : " << k << ", n : " << n << ")" << endl;

    for (int i = 0; i < n; i++) {
        int rand_index = distribution(gen);
        string read = text.substr(rand_index, k); // rand index에서 k개만큼 읽기
        //cout << "i : " << i << "Read : " << rand_index << endl;
        writeFile.write(read.c_str(), read.size());
        if (i != n - 1) // 마지막에만 \n 추가 안하기
            writeFile.write("\n", 1);
    }

    cout << outputTitle << " 생성 완료 . . . (k : " << k << ", n : " << n << ")" << endl;
}

vector<string> read_shortread() {

    string title = "my_DNA.txt";
    ifstream file(title);
    stringstream ss;
    ss << file.rdbuf(); // 한번에 파일을 읽고
    string reference = ss.str(); // 텍스트로 저장
    my_dna_text = ss.str();
    ifstream fin; fin.open("short_read.txt");
    string line;
    vector<string> short_read;

    while (!fin.eof()) { // shortRead 읽기
        getline(fin, line);
        short_read.push_back(line);
    }
    fin.close();

    return short_read;
}

DeBruijn make_de_graph(vector<string>& short_read, int k, int n) {
    vector<string> my_short_read;
    DeBruijn de;

    for (int i = 0; i < n; i++) {
        string lsub = short_read[i].substr(0, k - 1);
        string rsub = short_read[i].substr(1, k - 1);
        my_short_read.push_back(lsub);
        my_short_read.push_back(rsub);
    }

    sort(my_short_read.begin(), my_short_read.end());
    my_short_read.erase(unique(my_short_read.begin(), my_short_read.end()), my_short_read.end()); // 중복 제거

    for (int i = 0; i < my_short_read.size(); i++) {
        de.add_vertex(my_short_read[i]);
    }


    for (int i = 0; i < short_read.size(); i++) {
        string lsub = short_read[i].substr(0, k - 1);
        string rsub = short_read[i].substr(1, k - 1);

        int src_index = de.get_vertex_index(lsub);
        int dest_index = de.get_vertex_index(rsub);
        de.add_edge(src_index, dest_index);
        //cout << "src : " << src_index << " to  " << dest_index << endl;
        de.vertex_list[src_index].out++;
        de.vertex_list[dest_index].in++;
    }


    cout << "created Debruijn Graph" << endl;


    return de;
}

void compare(string title1, string title2, vector<double>& rates) {

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
    int min_size = min(mine.size(), reference.size());
    for (int i = 0; i < reference.size(); i++) {

        if (mine[i] == NULL || reference[i] == NULL)
            break;

        if (reference[i] == mine[i]) {
            correct++;
        }
        else {
            noncorrect++;
        }
    }

    double rate = (double)correct / reference.size();
    cout << "생성된 string[" << rates.size() << "]의 정확도는 " << rate << " 입니다 . " << endl;
    rates.push_back(rate);
}


void find_route(DeBruijn& de, vector<string>& routes, int k) {



    for (int i = 0; i < de.start_vertex.size(); i++) {
        //cout << "Start Vertex : " << de.start_vertex[i] << endl;
        int start_index = de.get_vertex_index(de.start_vertex[i]);

        de.route = de.vertex_list[start_index].data.substr(0, k - 2);
        de.dfs(start_index);
        routes.push_back(de.route);
    }


    string recov; int max = 0;
    vector<double> rates;
    if (routes.size() != 0) {
        for (int i = 0; i < routes.size(); i++) {
            recov = routes[i];

            ofstream writeFile;
            string outputTitle = "recover_test.txt";
            writeFile.open(outputTitle);
            writeFile.write(recov.c_str(), recov.size());
            writeFile.close();

            string title1 = "my_dna.txt";
            string title2 = "recover_test.txt";
            compare(title1, title2, rates);
        }
    }

    int max_index = -1;
    double max_rate = -1;
    for (int i = 0; i < rates.size(); i++) {
        if (max_rate < rates[i]) {
            max_index = i;
            max_rate = rates[i];
        }
    }

    cout << "최종 복원한 스트링의 정확도 : " << max_rate << endl;

    ofstream writeFile;
    string outputTitle = "recover_test.txt";
    writeFile.open(outputTitle);
    writeFile.write(recov.c_str(), recov.size());
    writeFile.close();

}



void restore_DNA(int k, int n) {

    clock_t start, end;
    start = clock();

    string title = "my_DNA.txt";

    ifstream file(title);
    stringstream ss;
    ss << file.rdbuf(); // 한번에 파일을 읽고
    string reference = ss.str(); // 텍스트로 저장
    my_dna_text = ss.str();
    ifstream fin; fin.open("short_read.txt");
    string line;
    vector<string> short_read;



    while (!fin.eof()) { // shortRead 읽기
        getline(fin, line);
        short_read.push_back(line);
    }
    fin.close();

    vector<string> my_short_read;
    DeBruijn de;

    for (int i = 0; i < n; i++) {
        string lsub = short_read[i].substr(0, k - 1);
        string rsub = short_read[i].substr(1, k - 1);
        my_short_read.push_back(lsub);
        my_short_read.push_back(rsub);
    }

    sort(my_short_read.begin(), my_short_read.end());
    my_short_read.erase(unique(my_short_read.begin(), my_short_read.end()), my_short_read.end()); // 중복 제거

    for (int i = 0; i < my_short_read.size(); i++) {
        de.add_vertex(my_short_read[i]);
    }


    for (int i = 0; i < short_read.size(); i++) {
        string lsub = short_read[i].substr(0, k - 1);
        string rsub = short_read[i].substr(1, k - 1);

        int src_index = de.get_vertex_index(lsub);
        int dest_index = de.get_vertex_index(rsub);
        de.add_edge(src_index, dest_index);
        //cout << "src : " << src_index << " to  " << dest_index << endl;
        de.vertex_list[src_index].out++;
        de.vertex_list[dest_index].in++;
    }



    //////////////////// 까지 드브루인 만들기

    cout << "created Debruijn Graph" << endl;

    for (int i = 0; i < de.vertex_list.size(); i++) {
        //cout << de.vertex_list[i].data << " , " << de.vertex_list[i].in << ", " << de.vertex_list[i].out << endl;
    }

    de.find_start();

    vector<string> routes(1, "");





    for (int i = 0; i < de.start_vertex.size(); i++) {
        //cout << "Start Vertex : " << de.start_vertex[i] << endl;
        int start_index = de.get_vertex_index(de.start_vertex[i]);
        de.route = de.vertex_list[start_index].data.substr(0, k - 2);

        de.dfs(start_index);
        routes[i] = de.route;
    }

    string recov;
    if (routes.size() != 0) {
        recov = routes[0];
    }
    ofstream writeFile;
    string outputTitle = "recover_test.txt";
    writeFile.open(outputTitle);
    writeFile.write(recov.c_str(), recov.size());
    writeFile.close();

    end = clock();
    double duration = (double)(end - start) / CLOCKS_PER_SEC;
    cout << "걸린시간(sec) : " << duration << "s" << endl;

}

int main() {


    int k = 100;
    int n = 60000;

    make_reference_DNA();
    make_my_DNA(k);
    make_shortread(k, n);



    clock_t start, end;

    start = clock();

    //    restore_DNA(k, n);

    vector<string> short_read = read_shortread();
    cout << "read done" << endl;
    DeBruijn graph = make_de_graph(short_read, k, n);
    cout << "find_start " << endl;
    graph.find_start();

    vector<string> routes;
    cout << "find_route " << endl;
    find_route(graph, routes, k);
    end = clock();

    double duration = (double)(end - start) / CLOCKS_PER_SEC;
    cout << "걸린시간(sec) : " << duration << "s" << endl;



    return 0;
}
