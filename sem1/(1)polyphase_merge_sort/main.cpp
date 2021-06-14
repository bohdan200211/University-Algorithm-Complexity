#include <iostream>
#include <vector>
#include <algorithm>
#include "c_heap.h"
#include <chrono>
using namespace std::chrono;
using namespace std;
const int n_size = sizeof(int);
const char * ext = ".dat";
int iters = 1;


int number_of_elm () {
    FILE * oput = fopen("source.dat","rb");
    int ans = 0;
    int r;
    while (fread(&r,n_size,1,oput)) {
        ans++;
    }
    return ans;
}
bool is_sorted () {
    FILE * oput = fopen("0.dat","rb");
    int p_r = INT32_MIN;
    int r;
    while (fread(&r,n_size,1,oput)) {
        if (p_r > r)
            return false;
        p_r = r;
    }
    return true;
}
const char * pr_get_filename_by_num (int num) { ///TODO could be optimized
    int lenght = 0, temp = num;
    do {
        num /= 10;
        lenght++;
    } while (num);
    const int h_length = lenght + 5;
    char * filename = new char[h_length];
    for (int i = lenght - 1; i > -1; i--) {
        filename[i] = '0' + temp % 10;
        temp /= 10;
    }
    for (int i = lenght, j = 0; i < h_length; i++,j++) {
        filename[i] = ext[j];
    }
    filename[lenght+4] = '\0';
    return filename;
}
vector<FILE *> pr_create_files_by_amount(const int n) { //creating files from 0 to (n - 1)
    vector<FILE *> set_of_filenames(n);
    for (int i = 0; i < n; i++) {
        set_of_filenames[i] = fopen(pr_get_filename_by_num(i), "wb");
    }
    return set_of_filenames;
}
int * create_split_list (const int num_of_files, const int num_of_chunks) {
    int *arr = new int[num_of_files], sum = num_of_files - 1, curr_index_max = 1, max_n = 1;
    for (int i = 1; i < num_of_files; i++) arr[i] = 1;
    arr[0] = 0;
    //init
    while (sum < num_of_chunks) {
        sum += (num_of_files - 2) * max_n;
        for (int i = 0; i < num_of_files; i++) arr[i] += max_n;
        arr[curr_index_max] = 0;
        curr_index_max = (curr_index_max + 1) % num_of_files;
        max_n = arr[curr_index_max];
        iters++;
    }
    return arr;
}
void files_init (const char * source_file, vector<FILE*> & set_of_files, const int * f_split, const int num_el_in_chunk, const int num_of_elements) {
    int temp[num_el_in_chunk], prev = INT32_MAX, curr,pos;
    int i = 0, amount_of_full_chunks = num_of_elements / num_el_in_chunk;
    FILE * source = fopen(source_file,"rb");


    while (i < set_of_files.size()) {
        int j = 0;
        while (j++ < f_split[i]) {
            if (amount_of_full_chunks-- < 1) goto l_out;
            fread(&temp,n_size,num_el_in_chunk,source);
            sort(temp,temp + num_el_in_chunk);
            curr = temp[0];

            if (prev != INT32_MAX && prev < curr) {
                temp[0] = prev;
                fseek(set_of_files[i], -n_size,SEEK_CUR);
                fwrite(&curr,n_size,1,set_of_files[i]);
            }
            fwrite(&temp,n_size,num_el_in_chunk,set_of_files[i]);
            prev = temp[num_el_in_chunk - 1];
        }
        prev = INT32_MAX;
        i++;
    }
    l_out:
    int s = num_of_elements % num_el_in_chunk;
    if (s > 0) {

        int temper, j = 0, temp1[s];
        while (fread(&temper,n_size,1,source)) {
            temp1[j++] = temper;
        }
        sort(temp1,temp1 + s);

        curr = temp1[0];
        if (prev != INT32_MAX && prev < curr) {
            temp1[0] = prev;
            fseek(set_of_files[i], -n_size,SEEK_CUR);
            fwrite(&curr,n_size,1,set_of_files[i]);
        }

        fwrite(&temp1,n_size,s,set_of_files[i]);
    }
}
void polyphase_merge_sort (const char * sourcefile, const int num_of_files, const int num_of_elements, const int num_el_in_chunk) {
    const int num_of_chunks = num_of_elements / num_el_in_chunk + (num_of_elements % num_el_in_chunk ? 1 : 0);
    int * split_arr = create_split_list(num_of_files,num_of_chunks);
    vector<FILE*> set_files = pr_create_files_by_amount(num_of_files);
    files_init(sourcefile,set_files,split_arr,num_el_in_chunk,num_of_elements);

    int indx_empty_file = (iters - 1) % num_of_files;
    int indx_min_file = (indx_empty_file - 1) % num_of_files;
    if (indx_min_file == -1) indx_min_file = num_of_files - 1;
    int temp;
    int min_val;
    c_heap heap = c_heap(num_of_files);
    for (int i = 0; i < num_of_files; i++) {
        fclose(set_files[i]);
        set_files[i] = fopen(pr_get_filename_by_num(i), "rb");
    }

    for (int c = 0; c < iters; c++) {
        fclose(set_files[indx_empty_file]);
        set_files[indx_empty_file] = fopen(pr_get_filename_by_num(indx_empty_file), "wb");
        min_val = split_arr[indx_min_file];

        for (int a = 0; a < min_val; a++) {
            for (int i = 0; i < num_of_files; i++) {
                if(fread(&temp,n_size,1,set_files[i]))
                    heap.insert(temp, set_files[i],min_val);
                else
                    heap.insert(INT32_MAX, set_files[i],0);
            }
            while (!heap.is_empty()) {
                temp = heap.get_min();
                fwrite(&temp,n_size,1,set_files[indx_empty_file]);
            }
            heap.set_size_zero();
        }

        for (int i = 0; i < num_of_files; i++) if(split_arr[i]) split_arr[i] -= min_val;
        split_arr[indx_empty_file] = min_val;

        fclose(set_files[indx_empty_file]);
        set_files[indx_empty_file] = fopen(pr_get_filename_by_num(indx_empty_file),"rb");

        indx_empty_file = indx_min_file;
        indx_min_file = (indx_empty_file - 1) % num_of_files;
        if (indx_min_file == -1) indx_min_file = num_of_files - 1;
    }
}

int main() {
    int n = 10000;



    FILE * s = fopen("source.dat","wb");
    int rad;
    for (int i = n; i > 0; i--) {
        rad = rand();
        fwrite(&rad, n_size, 1, s);
    }
    fclose(s);


    const char * s_file = "source.dat";

    auto start = steady_clock::now();
    polyphase_merge_sort(s_file,5,n,5);
    auto end = steady_clock::now();

    FILE * oput = fopen("0.dat","rb");
    int r;
    while (fread(&r,n_size,1,oput)) {
        cout << r << ", ";
    }
    cout << "\n\n\n";

    /*if (is_sorted()){
        cout << "success";
    } else {
        cout << "fault";
    }
    cout << endl;
    std::cout << "end sort " << (duration_cast<milliseconds>(end - start).count()) * 0.001 << " sec" << std::endl;*/

    return 0;
}
