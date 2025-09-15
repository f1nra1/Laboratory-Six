#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <iomanip>
#include <sstream>
#include "main.h"

unsigned char galois_multiply(unsigned char a, unsigned char b);
void sub_bytes(unsigned char state[4][4]);
void shift_rows(unsigned char state[4][4]);
void mix_columns(unsigned char state[4][4]);
void add_round_key(unsigned char state[4][4], unsigned char round_key[4][4]);
void key_expansion(const unsigned char* key, unsigned char expanded_keys[11][4][4]);
void aes_encrypt_block(const unsigned char* input, unsigned char* output, unsigned char expanded_keys[11][4][4]);
vector<unsigned char> aes_ofb_crypt(const vector<unsigned char>& input, const unsigned char* key, const unsigned char* iv);
vector<unsigned char> hex_to_bytes(const string& hex);
string bytes_to_hex(const vector<unsigned char>& bytes);
vector<unsigned char> read_file(const string& filename);
bool write_file(const string& filename, const vector<unsigned char>& data);
void input_key_iv(unsigned char key[16], unsigned char iv[16]);

void AES() {
    cout << "=== AES-128 OFB Шифрование/Дешифрование ===" << endl;

    int choice;
    cout << "1 - Шифрование" << endl;
    cout << "2 - Дешифрование" << endl;
    cout << "Выберите действие: ";
    cin >> choice;

    unsigned char key[16], iv[16];
    input_key_iv(key, iv);

    int source_choice;
    cout << "1 - Ввод с терминала" << endl;
    cout << "2 - Чтение из файла" << endl;
    cout << "Выберите источник данных: ";
    cin >> source_choice;

    vector<unsigned char> input_data;

    if (source_choice == 1) {
        string input_text;
        cout << "Введите текст: ";
        cin.ignore();
        getline(cin, input_text);

        if (choice == 1) {
            // Для шифрования - конвертируем строку в байты
            for (char c : input_text) {
                input_data.push_back(static_cast<unsigned char>(c));
            }
        } else {
            // Для дешифрования - парсим hex строку
            input_data = hex_to_bytes(input_text);
        }
    } else {
        string filename;
        cout << "Введите имя файла: ";
        cin >> filename;

        input_data = read_file(filename);
        if (input_data.empty()) {
            cout << "Ошибка при чтении файла. Операция прервана." << endl;
            return;
        }
    }

    // Выполняем шифрование/дешифрование
    auto result = aes_ofb_crypt(input_data, key, iv);

    // Вывод результата
    if (choice == 1) {
        cout << "Зашифрованный текст (hex): " << bytes_to_hex(result) << endl;

        // Сохранение в файл
        string output_filename;
        cout << "Введите имя файла для сохранения (или нажмите Enter для пропуска): ";
        cin.ignore();
        getline(cin, output_filename);

        if (!output_filename.empty()) {
            if (write_file(output_filename, result)) {
                cout << "Зашифрованные данные сохранены в файл: " << output_filename << endl;
            }
        }
    } else {
        cout << "Расшифрованный текст: ";
        for (unsigned char byte : result) {
            cout << static_cast<char>(byte);
        }
        cout << endl;

        // Сохранение в файл
        string output_filename;
        cout << "Введите имя файла для сохранения (или нажмите Enter для пропуска): ";
        cin.ignore();
        getline(cin, output_filename);

        if (!output_filename.empty()) {
            if (write_file(output_filename, result)) {
                cout << "Расшифрованные данные сохранены в файл: " << output_filename << endl;
            }
        }
    }
}

// S-box для AES
const unsigned char sbox[256] = {
        0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76,
        0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0,
        0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15,
        0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75,
        0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84,
        0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf,
        0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8,
        0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2,
        0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73,
        0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb,
        0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79,
        0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08,
        0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a,
        0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e,
        0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf,
        0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16
};

// Rcon для генерации ключей
const unsigned char rcon[11] = {
        0x00, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36
};

// Умножение в поле Галуа
unsigned char galois_multiply(unsigned char a, unsigned char b) {
    unsigned char p = 0;
    for (int i = 0; i < 8; i++) {
        if (b & 1) p ^= a;
        if (a & 0x80) a = (a << 1) ^ 0x1b;
        else a <<= 1;
        b >>= 1;
    }
    return p;
}

// SubBytes операция
void sub_bytes(unsigned char state[4][4]) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            state[i][j] = sbox[state[i][j]];
        }
    }
}

// ShiftRows операция
void shift_rows(unsigned char state[4][4]) {
    unsigned char temp[4];

    // Вторая строка - циклический сдвиг на 1
    temp[0] = state[1][1]; temp[1] = state[1][2]; temp[2] = state[1][3]; temp[3] = state[1][0];
    for (int i = 0; i < 4; i++) state[1][i] = temp[i];

    // Третья строка - циклический сдвиг на 2
    temp[0] = state[2][2]; temp[1] = state[2][3]; temp[2] = state[2][0]; temp[3] = state[2][1];
    for (int i = 0; i < 4; i++) state[2][i] = temp[i];

    // Четвертая строка - циклический сдвиг на 3
    temp[0] = state[3][3]; temp[1] = state[3][0]; temp[2] = state[3][1]; temp[3] = state[3][2];
    for (int i = 0; i < 4; i++) state[3][i] = temp[i];
}

// MixColumns операция
void mix_columns(unsigned char state[4][4]) {
    unsigned char temp[4];
    for (int c = 0; c < 4; c++) {
        temp[0] = galois_multiply(0x02, state[0][c]) ^ galois_multiply(0x03, state[1][c]) ^ state[2][c] ^ state[3][c];
        temp[1] = state[0][c] ^ galois_multiply(0x02, state[1][c]) ^ galois_multiply(0x03, state[2][c]) ^ state[3][c];
        temp[2] = state[0][c] ^ state[1][c] ^ galois_multiply(0x02, state[2][c]) ^ galois_multiply(0x03, state[3][c]);
        temp[3] = galois_multiply(0x03, state[0][c]) ^ state[1][c] ^ state[2][c] ^ galois_multiply(0x02, state[3][c]);

        for (int i = 0; i < 4; i++) {
            state[i][c] = temp[i];
        }
    }
}

// AddRoundKey операция
void add_round_key(unsigned char state[4][4], unsigned char round_key[4][4]) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            state[i][j] ^= round_key[i][j];
        }
    }
}

// Генерация раундовых ключей
void key_expansion(const unsigned char* key, unsigned char expanded_keys[11][4][4]) {
    // Копирование исходного ключа
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            expanded_keys[0][j][i] = key[i * 4 + j];
        }
    }

    // Генерация остальных раундовых ключей
    for (int round = 1; round <= 10; round++) {
        // Копирование предыдущего ключа
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                expanded_keys[round][i][j] = expanded_keys[round - 1][i][j];
            }
        }

        // Трансформация последнего столбца
        unsigned char temp[4];
        for (int i = 0; i < 4; i++) {
            temp[i] = expanded_keys[round][i][3];
        }

        // RotWord
        unsigned char t = temp[0];
        temp[0] = temp[1];
        temp[1] = temp[2];
        temp[2] = temp[3];
        temp[3] = t;

        // SubWord
        for (int i = 0; i < 4; i++) {
            temp[i] = sbox[temp[i]];
        }

        // Применение Rcon и XOR с первым столбцом
        temp[0] ^= rcon[round];

        for (int i = 0; i < 4; i++) {
            expanded_keys[round][i][0] ^= temp[i];
        }

        // XOR остальных столбцов
        for (int col = 1; col < 4; col++) {
            for (int i = 0; i < 4; i++) {
                expanded_keys[round][i][col] ^= expanded_keys[round][i][col - 1];
            }
        }
    }
}

// Шифрование блока AES
void aes_encrypt_block(const unsigned char* input, unsigned char* output, unsigned char expanded_keys[11][4][4]) {
    unsigned char state[4][4];

    // Инициализация state
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            state[j][i] = input[i * 4 + j];
        }
    }

    // Начальный раунд
    add_round_key(state, expanded_keys[0]);

    // 9 основных раундов
    for (int round = 1; round <= 9; round++) {
        sub_bytes(state);
        shift_rows(state);
        mix_columns(state);
        add_round_key(state, expanded_keys[round]);
    }

    // Финальный раунд
    sub_bytes(state);
    shift_rows(state);
    add_round_key(state, expanded_keys[10]);

    // Копирование результата
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            output[i * 4 + j] = state[j][i];
        }
    }
}

// OFB шифрование/дешифрование
vector<unsigned char> aes_ofb_crypt(const vector<unsigned char>& input,
                                  const unsigned char* key,
                                  const unsigned char* iv) {
    unsigned char expanded_keys[11][4][4];
    key_expansion(key, expanded_keys);

    vector<unsigned char> output;
    unsigned char keystream[16];
    unsigned char iv_copy[16];

    // Копирование IV
    for (int i = 0; i < 16; i++) {
        iv_copy[i] = iv[i];
    }

    size_t blocks = (input.size() + 15) / 16;

    for (size_t block = 0; block < blocks; block++) {
        // Генерация keystream
        aes_encrypt_block(iv_copy, keystream, expanded_keys);

        // XOR с данными
        size_t start = block * 16;
        size_t end = min(start + 16, input.size());

        for (size_t i = start; i < end; i++) {
            output.push_back(input[i] ^ keystream[i - start]);
        }

        // Обновление IV для следующего блока
        for (int i = 0; i < 16; i++) {
            iv_copy[i] = keystream[i];
        }
    }

    return output;
}

// Конвертация hex строки в байты
vector<unsigned char> hex_to_bytes(const string& hex) {
    vector<unsigned char> bytes;
    for (size_t i = 0; i < hex.length(); i += 2) {
        string byte_string = hex.substr(i, 2);
        unsigned char byte = static_cast<unsigned char>(strtol(byte_string.c_str(), nullptr, 16));
        bytes.push_back(byte);
    }
    return bytes;
}

// Конвертация байтов в hex строку
string bytes_to_hex(const vector<unsigned char>& bytes) {
    stringstream ss;
    for (unsigned char byte : bytes) {
        ss << hex << setw(2) << setfill('0') << static_cast<int>(byte);
    }
    return ss.str();
}

// Чтение файла в вектор байтов
vector<unsigned char> read_file(const string& filename) {
    ifstream file(filename, ios::binary);
    if (!file.is_open()) {
        cerr << "Ошибка: не удалось открыть файл " << filename << endl;
        return {};
    }

    vector<unsigned char> data;
    char byte;
    while (file.get(byte)) {
        data.push_back(static_cast<unsigned char>(byte));
    }

    file.close();
    return data;
}

// Запись вектора байтов в файл
bool write_file(const string& filename, const vector<unsigned char>& data) {
    ofstream file(filename, ios::binary);
    if (!file.is_open()) {
        cerr << "Ошибка: не удалось создать файл " << filename << endl;
        return false;
    }

    for (unsigned char byte : data) {
        file.put(static_cast<char>(byte));
    }

    file.close();
    return true;
}

// Функция для ввода ключа и IV
void input_key_iv(unsigned char key[16], unsigned char iv[16]) {
    string key_str, iv_str;

    cout << "Введите 128-битный ключ (32 hex символа): ";
    cin >> key_str;

    if (key_str.length() != 32) {
        cerr << "Ошибка: ключ должен содержать 32 hex символа" << endl;
        exit(1);
    }

    cout << "Введите IV (32 hex символа): ";
    cin >> iv_str;

    if (iv_str.length() != 32) {
        cerr << "Ошибка: IV должен содержать 32 hex символа" << endl;
        exit(1);
    }

    auto key_bytes = hex_to_bytes(key_str);
    auto iv_bytes = hex_to_bytes(iv_str);

    for (int i = 0; i < 16; i++) {
        key[i] = key_bytes[i];
        iv[i] = iv_bytes[i];
    }
}