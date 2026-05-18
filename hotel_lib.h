#ifndef HOTEL_LIB_H
#define HOTEL_LIB_H

#include <iostream>
#include <string>
#include <iomanip>
#include <stdexcept>
#include <algorithm>
#include <cctype>
#include <regex>

#ifdef _WIN32
#include <windows.h>
#endif

using namespace std;

namespace Color {
    const string RESET   = "\033[0m";

    const string BLACK   = "\033[30m";
    const string RED     = "\033[31m";
    const string GREEN   = "\033[32m";
    const string YELLOW  = "\033[33m";
    const string BLUE    = "\033[34m";
    const string MAGENTA = "\033[35m";
    const string CYAN    = "\033[36m";
    const string WHITE   = "\033[37m";

    const string BRED    = "\033[91m";
    const string BGREEN  = "\033[92m";
    const string BYELLOW = "\033[93m";
    const string BBLUE   = "\033[94m";
    const string BMAGENTA= "\033[95m";
    const string BCYAN   = "\033[96m";
    const string BWHITE  = "\033[97m";

    const string BG_BLACK  = "\033[40m";
    const string BG_BLUE   = "\033[44m";
    const string BG_CYAN   = "\033[46m";
    const string BG_WHITE  = "\033[47m";

    const string BOLD      = "\033[1m";
    const string DIM       = "\033[2m";
    const string UNDERLINE = "\033[4m";
}

namespace Box {
    const string TL  = "╔";
    const string TR  = "╗";
    const string BL  = "╚";
    const string BR  = "╝";
    const string H   = "═";
    const string V   = "║";
    const string ML  = "╠";
    const string MR  = "╣";
    const string MH  = "═";

    const string STL = "┌";
    const string STR = "┐";
    const string SBL = "└";
    const string SBR = "┘";
    const string SH  = "─";
    const string SV  = "│";
    const string SML = "├";
    const string SMR = "┤";
}

// Semua fungsi helper dideklarasikan tanpa inline.
// Aman karena header ini hanya di-include oleh satu file (main.cpp).

void enableANSI() {
#ifdef _WIN32
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode = 0;
    GetConsoleMode(hOut, &dwMode);
    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(hOut, dwMode);
    SetConsoleOutputCP(CP_UTF8);
#endif
}

void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void printLine(const string& ch, int n) {
    for (int i = 0; i < n; i++) cout << ch;
}

void printCentered(const string& text, int width) {
    int pad = (width - (int)text.size()) / 2;
    cout << string(pad > 0 ? pad : 0, ' ') << text;
}

void printBoxHeader(const string& title, int width = 58) {
    cout << Color::BCYAN << Color::BOLD;
    cout << Box::TL; printLine(Box::H, width); cout << Box::TR << "\n";
    cout << Box::V;
    printCentered(title, width);
    int rem = width - (int)title.size() - ((width - (int)title.size()) / 2);
    cout << string(rem > 0 ? rem : 0, ' ') << Box::V << "\n";
    cout << Box::ML; printLine(Box::H, width); cout << Box::MR << "\n";
    cout << Color::RESET;
}

void printBoxFooter(int width = 58) {
    cout << Color::BCYAN << Color::BOLD;
    cout << Box::BL; printLine(Box::H, width); cout << Box::BR << "\n";
    cout << Color::RESET;
}

void printBoxLine(const string& content, int width = 58) {
    cout << Color::BCYAN << Color::BOLD << Box::V << Color::RESET;
    cout << " " << content;
    int used = 1 + (int)content.size();
    int pad  = width - used;
    cout << string(pad > 0 ? pad : 0, ' ');
    cout << Color::BCYAN << Color::BOLD << Box::V << Color::RESET << "\n";
}

void printSectionLine(int width = 58) {
    cout << Color::CYAN;
    cout << Box::SML; printLine(Box::SH, width); cout << Box::SMR << "\n";
    cout << Color::RESET;
}

// ─── Struct Data ──────────────────────────────────────────────────────────────

struct DataTamu {
    string ktp;   // NIK (16 digit)
    string noHP;
};

struct User {
    string username, nim;
    int role;     // 1 = admin, 0 = user biasa
};

struct Reservasi {
    string nama;
    DataTamu data;
    string tanggal;       // tanggal check-in format DD-MM-YYYY
    int noKamar, lama, totalHarga;
    string pemilik;
};

struct Kamar {
    int noKamar;
    string tipe;
    int harga;
    int tersedia;
};

// ─── Variabel Global ──────────────────────────────────────────────────────────

extern Kamar kamarList[7];
extern User akun[10];
extern Reservasi dataReservasi[100];
extern int jumlahKamar;
extern int jumlahReservasi;
extern int jumlahUser;

// ─── Deklarasi Fungsi ─────────────────────────────────────────────────────────

// Validasi
void cekInputPositif(int nilai);
bool validasiNIK(const string& nik);
bool validasiHP(const string& hp);
bool validasiNama(const string& nama);
bool validasiTanggal(const string& tgl);
bool validasiUsername(const string& u);
bool validasiPassword(const string& p);

// Tampil
void tampilKamar(Kamar* k, int* n);
void tampilKamar(Kamar* k, int n, string filterTipe);

// CRUD Reservasi
void create(Kamar* kList, Reservasi* rList, int* nK, int* nR, string usernameAktif);
void read(Reservasi* rList, int* nR);
void readUser(Reservasi* rList, int* nR, string usernameAktif);
void update(Reservasi* rList, int* nR, Kamar* kList, int* nK);
void hapus(Reservasi* rList, int* nR, Kamar* kList, int* nK);

// Sorting
void sortingMenu(Kamar* k, int n);
void sortKamarByNumber(Kamar* k, int n);
void insertionSortHarga(Kamar* k, int n);

// Search
int  binarySearchRekursif(Kamar* k, int low, int high, int cari);
void cariNama(Reservasi* rList, int* nR);
void cariKamar(Kamar* kList, int* nK);

// Auth
void registrasi();
void registrasiAdmin();
int  login(string& usernameOut, int& roleOut);

// Menu
void tampilHeader();
void tampilWelcome();
void tampilMenuAdmin(string username);
void tampilMenuUser(string username);

#endif