#include <smmintrin.h>
#include <immintrin.h>
#include <algorithm>
#include <chrono>
#include <iostream>
#include <vector>
using namespace std;
#define int long long
struct bs {//큰 수 구조체
	vector<int>arr;
	bs() {

	}
	bs(int n) {
		arr.resize((n >> 3) + 1);
	}
	//생성자(n비트 큰 수 만듦)
	void setnb(int n, int k)
	{
		arr[n >> 3] &= (255 - (1 << (n & 7)));
		if (k)
		{
			arr[n >> 3] += (1 << (n & 7));
		}
	}
	//n번째 비트를 k로 바꿈
	int getnb(int n)
	{
		return !!(arr[n >> 3] & (1 << (n & 7)));
	}
	//n번째 비트 가져옴
	void mod(int n)
	{
		int s = arr.size();
		int i;
		for (i = s - 1; i >= 0; i--)
		{
			int j;
			for (j = 7; j >= 0; j--)
			{
				int v = i * 8 + j;
				if (v < n)
					goto T;
				if (getnb(v))
				{
					setnb(v, 0);
					v -= n;
					while (getnb(v))
					{
						setnb(v, 0);
						v++;
					}
					setnb(v, 1);
				}
			}
		}
	T:
		int r = n - 1;
		for (i = r; i >= 0; i--)
		{
			if (getnb(i) == 0)
				break;
		}
		if (i < 0)
		{
			for (i = r; i >= 0; i--)
			{
				setnb(i, 0);
			}
		}
	}
	//2^p-1로 나눈 나머지를 비트연산으로 빠르게 구함
};
bs mult(bs a, bs b, int n)
{
	vector<int>r(a.arr.size()+b.arr.size()+5);
	int i;
	for (i = 0; i < a.arr.size(); i++)
	{
		int j;
		for (j = 0; j < b.arr.size(); j++)
		{
			r[i + j] += a.arr[i] * b.arr[j];
		}
	}
	r.resize(r.size() + 5);
	for (i = 0; i < r.size() - 1; i++)
	{
		r[i + 1] += r[i] >> 8;
		r[i] &= 255;
	}
	bs t;
	t.arr = r;
	t.mod(n);
	t.arr.resize((n / 8) + 1);
	return t;
}//큰 수 a와 b 곱함
bs min2(bs a, int n)
{
	int i;
	a.arr[0] += 1;
	for (i = 2; i < n; i++)
	{
		a.arr[i >> 3] += (1 << (i & 7));
	}
	a.arr.resize(a.arr.size() + 5);
	for (i = 0; i < a.arr.size() - 1; i++)
	{
		a.arr[i + 1] += a.arr[i] >> 8;
		a.arr[i] &= 255;
	}
	a.mod(n);
	a.arr.resize((n / 8) + 1);
	return a;
}
//2를 빼주는 함수
signed main()
{
	while (1)
	{
		int N;
		cin >> N;
		auto start = chrono::high_resolution_clock::now();
		int i;
		bs an(N);
		an.arr[0] = 4;
		for (i = 0; i < N - 2; i++)//루카스-레머 방법 메인 반복문
		{
			an = mult(an, an, N);
			an = min2(an, N);
			if (i % 1000 == 0)
				cout << i << '\n';//현재 상황 표시기
		}
		for (i = 0; i < 5; i++)
			cout << an.arr[i] << ' ';//이 5개가 모두 0이면 소수
		cout << '\n';
		auto finish = chrono::high_resolution_clock::now();
		cout << chrono::duration_cast<chrono::nanoseconds>(finish - start).count() / (1000000.0) << "밀리초\n";
	}

}
