#include <iostream>
#include <vector>
#include <cstdlib>
#include <windows.h>
#include <ctime>
#include <fstream>
#include <string>

using namespace std;

#ifdef _WIN32
    #define OPEN_CMD "start"
#elif __APPLE__
    #define OPEN_CMD "open"
#else
    #define OPEN_CMD "xdg-open"
#endif

void consolePrintArray(const vector<int>& arr, int low, int high, int pivotIdx) {
    for (int i = 0; i < (int)arr.size(); i++) {
        if (i < low || i > high) {
            cout << arr[i] << " ";
        } else if (i == pivotIdx) {
            cout << "(" << arr[i] << ") ";
        } else {
            cout << arr[i] << " ";
        }
    }
    cout << endl;
}

int consolePartition(vector<int>& arr, int low, int high) {
    int pivot = arr[high];
    int i = low - 1;
    cout << "\n  -> Partition [" << low << ".." << high << "], pivot = " << pivot << endl;
    cout << "     Массив ДО:    ";
    consolePrintArray(arr, low, high, high);
    for (int j = low; j < high; j++) {
        if (arr[j] <= pivot) {
            i++;
            if (i != j) {
                cout << "     Обмен: " << arr[i] << " <-> " << arr[j] << endl;
                swap(arr[i], arr[j]);
            }
        }
    }
    swap(arr[i + 1], arr[high]);
    cout << "     Обмен pivot: " << arr[i + 1] << " <-> " << pivot << endl;
    cout << "     Массив ПОСЛЕ: ";
    consolePrintArray(arr, low, high, i + 1);
    return i + 1;
}

void consoleQuickSort(vector<int>& arr, int low, int high, int depth = 0) {
    if (low < high) {
        string indent(depth * 4, ' ');
        cout << indent << " quickSort [" << low << ".." << high << "] " << endl;
        int pi = consolePartition(arr, low, high);
        consoleQuickSort(arr, low, pi - 1, depth + 1);
        consoleQuickSort(arr, pi + 1, high, depth + 1);
    } else {
        string indent(depth * 4, ' ');
        cout << indent << " quickSort [" << low << ".." << high << "] — базовый случай, выход " << endl;
    }
}

struct Frame {
    vector<int> arr;
    int low, high, pivot, i, j;
};

vector<Frame> frames;

void recordFrame(const vector<int>& arr, int low, int high, int pivot, int i, int j) {
    Frame f;
    f.arr = arr;
    f.low = low;
    f.high = high;
    f.pivot = pivot;
    f.i = i;
    f.j = j;
    frames.push_back(f);
}

int htmlPartition(vector<int>& arr, int low, int high) {
    int pivot = arr[high];
    int i = low - 1;

    recordFrame(arr, low, high, high, i, -1);

    for (int j = low; j < high; j++) {
        recordFrame(arr, low, high, high, i, j);
        if (arr[j] <= pivot) {
            i++;
            if (i != j)
                swap(arr[i], arr[j]);
            recordFrame(arr, low, high, high, i, j);
        }
     }
    swap(arr[i + 1], arr[high]);
    recordFrame(arr, low, high, i + 1, i + 1, high);
    return i + 1;
}

void htmlQuickSort(vector<int>& arr, int low, int high) {
    if (low < high) {
        int pi = htmlPartition(arr, low, high);
        htmlQuickSort(arr, low, pi - 1);
        htmlQuickSort(arr, pi + 1, high);
    }
}

int main() {
    SetConsoleOutputCP(CP_UTF8);
    srand(static_cast<unsigned>(time(0)));

    int n = 40;
    vector<int> arr1(n);
    for (int i = 0; i < n; i++) {
        arr1[i] = rand() % 90 + 10;
    }

    vector<int> arr2 = arr1;

    cout << " Изначальный массив: " << endl;
    for (int x : arr1) cout << x << " ";
    cout << "\n\n";


    consoleQuickSort(arr1, 0, n - 1);

    cout << "\n Итоговый отсортированный массив (консоль): " << endl;
    for (int x : arr1) cout << x << " ";
    cout << "\n\n";

    cout << " Выполняется сортировка и запись кадров...\n";

    htmlQuickSort(arr2, 0, n - 1);

    cout << " Сортировка завершена. Генерация HTML-файла...\n";

    ofstream out("visualization.html");
    out << "<!DOCTYPE html>\n<html lang='ru'>\n<head>\n<meta charset='UTF-8'>\n<title>Quick Sort Visualization</title>\n<style>\n";
    out << "body { background: #1e1e1e; color: #fff; font-family: 'Segoe UI', sans-serif; display: flex; flex-direction: column; align-items: center; margin: 0; padding: 20px; }\n";
    out << "h2 { margin-bottom: 10px; }\n";
    out << "#container { display: flex; align-items: flex-end; height: 400px; width: 800px; border-bottom: 3px solid #555; margin-top: 50px; padding-bottom: 5px; }\n"; // Исправлены двоеточия в CSS
    out << ".bar { width: 35px; margin: 0 2px; background: #888; border-radius: 4px 4px 0 0; transition: height 0.15s ease, background-color 0.15s ease; display: flex; align-items: flex-end; justify-content: center; font-size: 12px; font-weight: bold; padding-bottom: 5px; color: #000; }\n";
    out << ".bar.pivot { background: #ff4d4d !important; color: white; } /* Красный: Опорный */\n";
    out << ".bar.compare { background: #ffd700 !important; color: black; } /* Желтый: Сравнение */\n";
    out << ".bar.swap { background: #4dff4d !important; color: black; } /* Зеленый: Обмен */\n";
    out << ".bar.sorted { background: #4dffff !important; color: black; } /* Голубой: Отсортирован */\n";
    out << ".legend { display: flex; gap: 20px; margin-top: 20px; font-size: 14px; }\n";
    out << ".dot { width: 15px; height: 15px; display: inline-block; border-radius: 3px; vertical-align: middle; margin-right: 5px; }\n";
    out << "</style>\n</head>\n<body>\n";
    out << "<h2>Визуализация быстрой сортировки (Quick Sort)</h2>\n";
    out << "<div id='container'></div>\n";
    out << "<p id='status' style='margin-top: 20px; font-size: 18px; color: #aaa;'>Подготовка...</p>\n";
    out << "<div class='legend'>\n";
    out << "<div><span class='dot' style='background:#ff4d4d'></span>Опорный (Pivot)</div>\n";
    out << "<div><span class='dot' style='background:#ffd700'></span>Сравнение (j)</div>\n";
    out << "<div><span class='dot' style='background:#4dff4d'></span>Обмен (i)</div>\n";
    out << "<div><span class='dot' style='background:#4dffff'></span>Отсортировано</div>\n";
    out << "</div>\n";

    out << "<script>\nconst frames = [\n";
    for (size_t k = 0; k < frames.size(); k++) {
        out << "  {\n";
        out << "    \"arr\": [";
        for (size_t m = 0; m < frames[k].arr.size(); m++) {
            out << frames[k].arr[m] << (m + 1 == frames[k].arr.size() ? "" : ",");
        }
        out << "],\n";
        out << "    \"low\": " << frames[k].low << ",\n";
        out << "    \"high\": " << frames[k].high << ",\n";
        out << "    \"pivot\": " << frames[k].pivot << ",\n";
        out << "    \"i\": " << frames[k].i << ",\n";
        out << "    \"j\": " << frames[k].j << "\n";
        out << "  }" << (k + 1 == frames.size() ? "" : ",") << "\n";
    }
    out << "];\n";

    out << "let currentFrame = 0;\n";
    out << "const container = document.getElementById('container');\n";
    out << "const status = document.getElementById('status');\n";
    out << "const initialArr = frames[0].arr;\n";
    out << "const n = initialArr.length;\n";
    out << "\n// Создаем столбики\n";
    out << "initialArr.forEach((val, idx) => {\n";
    out << "    const bar = document.createElement('div');\n";
    out << "    bar.className = 'bar';\n";
    out << "    bar.id = 'bar-' + idx;\n";
    out << "    bar.style.height = (val * 3.5) + 'px';\n";
    out << "    bar.innerText = val;\n";
    out << "    container.appendChild(bar);\n";
    out << "});\n";
    out << "\nfunction animate() {\n";
    out << "    if (currentFrame >= frames.length) {\n";
    out << "        status.innerText = 'Сортировка успешно завершена!';\n";
    out << "        status.style.color = '#4dff4d';\n";
    out << "        document.querySelectorAll('.bar').forEach(b => b.className = 'bar sorted');\n";
    out << "        return;\n";
    out << "    }\n";
    out << "    const f = frames[currentFrame];\n";
    out << "    f.arr.forEach((val, idx) => {\n";
    out << "        const bar = document.getElementById('bar-' + idx);\n";
    out << "        bar.style.height = (val * 3.5) + 'px';\n";
    out << "        bar.innerText = val;\n";
    out << "        bar.className = 'bar'; // Сброс классов\n";
    out << "        if (idx === f.pivot) bar.classList.add('pivot');\n";
    out << "        else if (idx === f.j && f.j !== -1) bar.classList.add('compare');\n";
    out << "        else if (idx === f.i && f.i >= f.low) bar.classList.add('swap');\n";
    out << "    });\n";
    out << "    currentFrame++;\n";
    out << "    setTimeout(animate, 60); // Скорость анимации (60 мс)\n";
    out << "}\n";
    out << "status.innerText = 'Идет сортировка...';\n";
    out << "setTimeout(animate, 500);\n";
    out << "</script>\n</body>\n</html>\n";
    out.close();
    cout << " Готово! Открываю визуализацию в браузере...\n";
    string cmd = string(OPEN_CMD) + " visualization.html";
    system(cmd.c_str());

}