#include <iostream>
#include <raylib.h>
#include <vector>

void null(int, const char*, va_list);

using namespace std;

int main() {
    SetTraceLogCallback(null);

    unsigned int fps, oSize;
    cout << "INPUT ] FPS of original video: ";
    cin >> fps;
    cout << "INPUT ] Amount of frames of the original video (check frames folder for file amount): ";
    cin >> oSize;

    if (!FileExists(TextFormat("./frames/%d.png", oSize))) {
        cout << "Error, size is incorrect!!" << endl;
        return 1;
    }

    if (FileExists("./out.vpr")) {
        cout << "Delete your previous file (out.vpr) before creating a new one!" << endl;
        return 2;
    }

    vector<uint8_t> data;

    data.push_back(((unsigned char*) (&fps))[0]);
    data.push_back(((unsigned char*) (&fps))[1]);
    data.push_back(((unsigned char*) (&fps))[2]);
    data.push_back(((unsigned char*) (&fps))[3]);

    data.push_back(((unsigned char*) (&oSize))[0]);
    data.push_back(((unsigned char*) (&oSize))[1]);
    data.push_back(((unsigned char*) (&oSize))[2]);
    data.push_back(((unsigned char*) (&oSize))[3]);

    for (unsigned i = 0; i < oSize; i++) {
        uint32_t sz;
        if (!FileExists(TextFormat("./frames/%d.png", i+1))) {
            cout << "A frame is missing from the ./frames folder!" << "\n";
            exit(1);
        }
        unsigned char *f = LoadFileData(TextFormat("./frames/%d.png", i+1), &sz);
        auto thisFrame = (uint16_t) i;
        data.push_back(((unsigned char*) (&thisFrame))[0]);
        data.push_back(((unsigned char*) (&thisFrame))[1]);

        data.push_back(((unsigned char*) (&sz))[0]);
        data.push_back(((unsigned char*) (&sz))[1]);
        data.push_back(((unsigned char*) (&sz))[2]);
        data.push_back(((unsigned char*) (&sz))[3]);

        data.reserve(data.size()+sz);
        for (unsigned int j = 0; j < sz; j++) {
            data.push_back(f[j]);
        }
        cout << "Saved: " << thisFrame << " | Size: " << sz << "\n";
        UnloadFileData(f);
    }

    SaveFileData("./out.vpr", data.data(), data.size());

    data.clear();
    data.shrink_to_fit();

    return 0;
}

void null(int, const char*, va_list) {}
