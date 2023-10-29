#include <iostream>
#include <fstream>
#include <vector>

void swap(std::vector<int>& vector, uint32_t index1, uint32_t index2) {
    int temp = vector[index1];
    vector[index1] = vector[index2];
    vector[index2] = temp;
}

uint32_t find_pivot(std::vector<int>& vector, uint32_t start, uint32_t end) {
    int pivot = vector[start];

    uint32_t count = 0;
    for (int i = start + 1; i <= end; i++) {
        if (vector[i] >= pivot)
            count++;
    }

    uint32_t pivot_index = start + count;
    swap(vector, start, pivot_index);

    int i = start, j = end;

    while (i < pivot_index && j > pivot_index) {
        while (vector[i] <= pivot) {
            i++;
        }

        while (vector[j] > pivot) {
            j--;
        }

        if (i < pivot_index && j > pivot_index) {
            swap(vector, i++, j--);
        }
    }

    return pivot_index;
}

void quick_sort(std::vector<int>& vector, uint32_t start, uint32_t end) {
    if(start >= end)
        return;

    uint32_t pivot = find_pivot(vector, start, end);

    std::cout << pivot << '\n';

    quick_sort(vector, start, pivot - 1);
    quick_sort(vector, pivot + 1, end);
}

void bubble_sort(std::vector<int>& vector) {
    if(vector.size() == 0) {
        return;
    }

    uint32_t n = vector.size() - 1;

    do {
        for(uint32_t i = 0; i < n; i++) {
            if(vector[i] > vector[i + 1]) {
                swap(vector, i, i + 1);
            }
        }

        n--;
    } while(n > 1);
}

int main() {
    const uint8_t CHUNK_COUNT = 4;
    const std::string CHUNK_FOLDER = "chunks";

    std::ifstream input("liczby.txt");

    std::fstream chunks[CHUNK_COUNT];
    std::fstream sorted_chunks[CHUNK_COUNT];

    for(int i = 0; i < CHUNK_COUNT; i++) {
        std::string chunk_name = CHUNK_FOLDER + "/chunk" + std::to_string(i);
        std::string sorted_chunk_name = CHUNK_FOLDER + "/sorted_chunk" + std::to_string(i);
        
        auto file = std::ofstream(chunk_name);
        file.close();

        file = std::ofstream(sorted_chunk_name);
        file.close();

        chunks[i].open(chunk_name);
        sorted_chunks[i].open(sorted_chunk_name);
    }

    int current;

    for(int i = 0; input >> current; i = (i + 1) % CHUNK_COUNT) {
        chunks[i] << current << ' ';
    }

    input.close();

    for(int i = 0; i < CHUNK_COUNT; i++) {
        chunks[i].seekg(0);

        std::vector<int> chunk;
        int temp;

        while(chunks[i] >> temp) {
            chunk.push_back(temp);
        }

        // quick_sort(chunk, 0, chunk.size());
        bubble_sort(chunk);

        for(int num : chunk) {
            sorted_chunks[i] << num << '\n';
        }

        chunks[i].close();

        sorted_chunks[i].seekg(0);
    }

    int currents[CHUNK_COUNT];

    for(int i = 0; i < CHUNK_COUNT; i++) {
        sorted_chunks[i] >> currents[i];
    }

    std::ofstream output("posortowane.txt");

    int smallest_not_eof = 0;

    while(true) {
        int smallest_current = smallest_not_eof;

        for(int i = 1; i < CHUNK_COUNT; i++) {
            if(currents[i] < currents[smallest_current] && !sorted_chunks[i].eof()) {
                smallest_current = i;
            }
        }

        output << currents[smallest_current] << '\n';

        sorted_chunks[smallest_current] >> currents[smallest_current];

        bool all_eof = true;

        for(int i = 0; i < CHUNK_COUNT; i++) {
            if(!sorted_chunks[i].eof()) {
                smallest_not_eof = i;
                all_eof = false;
                break;
            }
        }

        if(all_eof) {
            std::cout << currents[smallest_current] << std::endl;

            break;
        }
    }

    output.close();

    for(int i = 0; i < CHUNK_COUNT; i++) {
        sorted_chunks[i].close();
    }

    for(int i = 0; i < CHUNK_COUNT; i++) {
        std::string chunk_name = CHUNK_FOLDER + "/chunk" + std::to_string(i);
        std::string sorted_chunk_name = CHUNK_FOLDER + "/sorted_chunk" + std::to_string(i);

        std::remove(chunk_name.c_str());
        std::remove(sorted_chunk_name.c_str());
    }

    return 0;
}