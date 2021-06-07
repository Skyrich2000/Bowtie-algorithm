# DNA assemble

BWT 를 활용한 Bowtie algorithm 

## install

reference dna 파일 다운로드
```
wget http://skyrich3.synology.me/~Huni/chr1.fna
```

## how to use

* 폴더에서 bwt_index_table 와 reference_dna 파일을 가져옵니다. (.txt 파일로 변환해서 써야함)
* main.cpp 에서 n, m, l 값 조정
* BWT_outer.cpp 에서 외부정렬시 사용할 파일 개수 설정
* BW matrix 가 완성되면, BWT_outer.cpp 241번째 줄 주석처리 하여 사용

---
[참고](https://www.youtube.com/watch?v=kvVGj5V65io)
