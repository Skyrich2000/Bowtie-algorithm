# DNA assemble

BWT 를 활용한 Bowtie algorithm 

## install

reference dna 파일 다운로드
```
wget http://skyrich3.synology.me/~Huni/chr1.fna
```

## how to use

* main.cpp 에서 n, m, l 값 조정
* BWT_outer.cpp 에서 외부정렬시 사용할 파일 개수 설정
* BW matrix 가 완성되면, BWT_outer.cpp 241번째 줄 주석처리 하여 사용
