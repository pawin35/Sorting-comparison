#include <iostream>
#include <cstdlib>
#include <random>
#include <chrono>
#include <vector>
#include <algorithm>
#include <queue>
#include <ctime>

#define VEC_SIZE 100000
#define NUM_TEST 1
#define NEARLY_RATIO 0.9

using namespace std;

vector<int> gen_random()
{
    random_device dev;
    mt19937 rng(dev());
    uniform_int_distribution<int> dist(1, 1000000);
    vector<int> res(VEC_SIZE, 0);
    for (int i = 0; i < VEC_SIZE; i++)
    {
        res[i] = dist(rng);
    }
    return res;
}

vector<int> gen_nearly()
{
    random_device dev;
    mt19937 rng(dev());
    uniform_int_distribution<int> dist(1, 1000000);
    int INIT_SIZE = VEC_SIZE * NEARLY_RATIO;
    vector<int> res(INIT_SIZE, 0);
    for (int i = 0; i < INIT_SIZE; i++)
    {
        res[i] = dist(rng);
    }
    sort(res.begin(), res.end());
    int FILL_SIZE = VEC_SIZE - INIT_SIZE;
    //cout <<FILL_SIZE <<endl;
    for (int i = 0; i < FILL_SIZE; i++)
    {
        //cout <<"inside\n";
        uniform_int_distribution<int> vec_pos(0, res.size() - 1);
        int pos = vec_pos(rng);
        //cout <<pos <<endl;
        auto itt = res.begin() + pos;
        auto pre_itt = itt - 1;
        if (*itt >= 500000)
        {
            uniform_int_distribution<int> rand_val(0, (*pre_itt) - 1);
            int insert_val = rand_val(rng);
            res.insert(itt, insert_val);
        }
        else
        {
            uniform_int_distribution<int> rand_val((*itt) + 1, 1000000);
            int insert_val = rand_val(rng);
            res.insert(itt, insert_val);
        }
    }
    return res;
}

void print_vector(vector<int> vp)
{
    for (int i = 0; i < vp.size(); i++)
    {
        cout << vp[i] << ",";
    }
    cout << endl;
}

void insertionSort(vector<int> vec)
{
    for (auto it = vec.begin(); it != vec.end(); it++)
    {
        auto const insertion_point =
            upper_bound(vec.begin(), it, *it);

        rotate(insertion_point, it, it + 1);
    }
    //print_vector(vec);
}

void radixSort(vector<int> data)
{
    int n = data.size();
    int d, k, factor;
    auto j = data.begin();
    const int radix = 10;
    const int digits = 10;
    queue<int> queues[radix];
    for (d = 0, factor = 1; d < digits; factor *= radix, d++)
    {
        for (j = data.begin(); j != data.end(); j++)
            queues[(*j / factor) % radix].push(*j);
        for (k = 0, j = data.begin(); k < radix, j != data.end(); k++)
            while (!queues[k].empty())
            {
                *(j++) = queues[k].front();
                queues[k].pop();
            }
    }
}

void timeSort(vector<int> data, void (*sortingFunct)(vector<int>))
{
    auto start = chrono::high_resolution_clock::now();
    sortingFunct(data);
    auto finish = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed = finish - start;
    cout << "Elapsed time: " << elapsed.count() << " s\n";
}

vector<int>::iterator partition(const vector<int> &A,
                                const vector<int>::iterator &p,
                                const vector<int>::iterator &r)
{
    // Get a random element within A[p:r].
    auto seed = clock() * clock() * clock();
    default_random_engine dre(seed);
    uniform_int_distribution<size_t> di(0, r - p);

    auto random_it = p;
    random_it = p + di(dre);

    // Swap values of random_it and r.
    auto tmp = *random_it;
    *random_it = *r;
    *r = tmp;

    auto pivot = *r;

    int iless = -1;
    for (int i = 0; i < r - p; i++)
    {
        if (*(p + i) <= pivot)
        {
            if (iless != i)
            iless++;
            {
                // Swap *(p+iless) and *(p+i)
                tmp = *(p + iless);
                *(p + iless) = *(p + i);
                *(p + i) = tmp;
            }
        }
    }

    // Swap *(p+iless+1) and *r
    *r = *(p + iless + 1);
    *(p + iless + 1) = pivot;
    return p + iless + 1;
}

void quickSortRecur(const vector<int> &A, const vector<int>::iterator &p,
                    const vector<int>::iterator &r)
{
    if (p < r)
    {
        auto q = partition(A, p, r);
        quickSortRecur(A, p, q - 1);
        quickSortRecur(A, q + 1, r);
    }
}

void quickSort(vector<int> data)
{
    quickSortRecur(data, data.begin(), data.end());
}

void heapSort(vector<int> v)
{
    priority_queue<int> heap;
    for (auto i = v.begin(); i != v.end(); i++)
    {
        heap.push((*i) * -1);
    }
    v.clear();
    while (!heap.empty())
    {
        v.push_back(heap.top());
        heap.pop();
    }
}

vector<int> mergeSortRecur(const vector<int> &v, int left, int right) {
    if (left == right) return {v[left]};
    int mid = (left + right) / 2;
    vector<int> v1 = mergeSortRecur(v, left, mid);
    vector<int> v2 = mergeSortRecur(v, mid+1, right);
    vector<int> newv;
    auto it1 = v1.begin();
    auto it2 = v2.begin();
    while (it1 != v1.end() && it2 != v2.end()) {
        if (*it1 < *it2) {
            newv.push_back(*it1);
            it1++;
        } else {
            newv.push_back(*it2);
            it2++;
        }
    }
    while (it1 != v1.end()) {
        newv.push_back(*it1);
        it1++;
    }
    while (it2 != v2.end()) {
        newv.push_back(*it2);
        it2++;
    }
    return newv;
}

void mergeSort(vector<int> data) {
    data = mergeSortRecur(data, 0, data.size()-1);
}
int main()
{
    for (int i = 0; i < NUM_TEST; i++)
    {
        vector<int> a = gen_random();
        vector<int> b = gen_nearly();
        vector<int> c(b.rbegin(), b.rend());
        //print_vector(a);
        //print_vector(b);
        //print_vector(c);
        timeSort(a, insertionSort);
        timeSort(b, insertionSort);
        timeSort(c, insertionSort);
        timeSort(a, radixSort);
        timeSort(b, radixSort);
        timeSort(c, radixSort);
        timeSort(a, quickSort);
        timeSort(b, quickSort);
        timeSort(c, quickSort);
        timeSort(a, heapSort);
        timeSort(b, heapSort);
        timeSort(c, heapSort);
        timeSort(a, mergeSort);
        timeSort(b, mergeSort);
        timeSort(c, mergeSort);
    }
}