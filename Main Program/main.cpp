#include "hotel_lib.h"
#include <regex>
#include <climits>

Kamar kamarList[7] = {
    {101, "Standar",        137000, 1},
    {102, "Standar",        137000, 1},
    {103, "Standar Couple", 250000, 1},
    {201, "Deluxe",         200000, 1},
    {202, "Deluxe",         200000, 1},
    {203, "Deluxe Couple",  500000, 1},
    {301, "Family",         750000, 1}
};
User akun[10];
Reservasi dataReservasi[100];
int jumlahKamar     = 7;
int jumlahReservasi = 0;
int jumlahUser      = 0;

void cekInputPositif(int nilai) {
    if (nilai <= 0) throw invalid_argument("Input harus lebih dari 0!");
}

bool validasiNIK(const string& nik) {
    regex pola("^[0-9]{16}$");
    return regex_match(nik, pola);
}

bool validasiHP(const string& hp) {
    regex pola("^08[0-9]{8,11}$");
    return regex_match(hp, pola);
}

bool validasiNama(const string& nama) {
    regex pola("^[A-Za-z ]{2,}$");
    return regex_match(nama, pola);
}

bool validasiTanggal(const string& tgl) {
    regex pola("^(0[1-9]|[12][0-9]|3[01])-(0[1-9]|1[0-2])-([0-9]{4})$");
    return regex_match(tgl, pola);
}

bool validasiUsername(const string& u) {
    regex pola("^[A-Za-z0-9_]{4,20}$");
    return regex_match(u, pola);
}

bool validasiPassword(const string& p) {
    return p.size() >= 6;
}

string trimString(const string& s) {
    size_t awal = s.find_first_not_of(" \t\r\n");
    if (awal == string::npos) return "";
    size_t akhir = s.find_last_not_of(" \t\r\n");
    return s.substr(awal, akhir - awal + 1);
}

bool inputInt(int& val, int minVal = 1, int maxVal = INT_MAX) {
    string s;
    if (!getline(cin, s)) {
        cin.clear();
        return false;
    }
    s = trimString(s);
    if (s.empty()) return false;

    size_t mulai = (s[0] == '-' || s[0] == '+') ? 1 : 0;
    if (mulai == s.size()) return false;
    for (size_t i = mulai; i < s.size(); i++) {
        if (!isdigit((unsigned char)s[i])) return false;
    }

    try { val = stoi(s); }
    catch (...) { return false; }

    return (val >= minVal && val <= maxVal);
}

void safeClear() {
    cout << "\033[2J\033[H" << flush;
}

void tampilWelcome() {
    cout << "\n";
    cout << Color::BMAGENTA << Color::BOLD;
    cout << "  ╔══════════════════════════════════════════════════════╗\n";
    cout << "  ║                                                      ║\n";
    cout << "  ║        ★  GRAND RIZUNA ESTATE HOTEL  ★               ║\n";
    cout << "  ║         Hotel Management System  v2.1                ║\n";
    cout << "  ║                                                      ║\n";
    cout << "  ╚══════════════════════════════════════════════════════╝\n";
    cout << Color::RESET;
}

void tampilHeader() {
    cout << "\n";
    cout << Color::BCYAN << Color::BOLD;
    cout << "  ╔══════════════════════════════════════════════╗\n";
    cout << "  ║        SISTEM RESERVASI HOTEL                ║\n";
    cout << "  ║        Grand Rizuna Estate                   ║\n";
    cout << "  ╚══════════════════════════════════════════════╝\n";
    cout << Color::RESET;
}

void cetakMenuItem(const string& warna, const string& num,
                const string& ikon, const string& label) {
    cout << warna << "  │  " << Color::RESET
        << Color::BWHITE << Color::BOLD << "[" << num << "] " << Color::RESET
        << ikon << " " << left << setw(37) << label
        << warna << "│\n" << Color::RESET;
}

void tampilMenuAdmin(string username) {
    tampilHeader();
    cout << Color::BYELLOW << Color::BOLD;
    cout << "  ┌──────────────────────────────────────────────┐\n";
    cout << "  │  " << Color::BGREEN << "👤 Login sebagai: "
        << Color::BWHITE << left << setw(15) << username
        << Color::BYELLOW << "  [ADMIN]  │\n";
    cout << "  ├──────────────────────────────────────────────┤\n";
    cout << Color::RESET;
    cout << Color::BYELLOW << "  │" << Color::RESET << Color::BOLD
        << "  MENU ADMIN                                  "
        << Color::BYELLOW << "│\n" << Color::RESET;
    cout << Color::BYELLOW << "  ├──────────────────────────────────────────────┤\n" << Color::RESET;

    cetakMenuItem(Color::BYELLOW, "1",  "🛎 ", "Reservasi Kamar Baru");
    cetakMenuItem(Color::BYELLOW, "2",  "📋", "Lihat Daftar Reservasi");
    cetakMenuItem(Color::BYELLOW, "3",  "✏ ", "Ubah Detail Reservasi");
    cetakMenuItem(Color::BYELLOW, "4",  "🚪", "Batalkan Reservasi (Check-out)");
    cetakMenuItem(Color::BYELLOW, "5",  "🗂 ", "Kelola & Urutkan Daftar Kamar");
    cetakMenuItem(Color::BYELLOW, "6",  "🔍", "Cari Data Tamu");
    cetakMenuItem(Color::BYELLOW, "7",  "🔎", "Cek Ketersediaan (Cari ID)");
    cetakMenuItem(Color::BYELLOW, "8",  "🏷 ", "Filter Kamar (by Tipe)");
    cetakMenuItem(Color::BYELLOW, "9",  "👤", "Tambah Akun Admin Baru");
    cetakMenuItem(Color::BYELLOW, "0", "🚶", "Keluar (Logout)");

    cout << Color::BYELLOW << "  └──────────────────────────────────────────────┘\n" << Color::RESET;
    cout << Color::BWHITE << "  Pilih Layanan (0-9): " << Color::RESET;
}

void tampilMenuUser(string username) {
    tampilHeader();
    cout << Color::BBLUE << Color::BOLD;
    cout << "  ┌──────────────────────────────────────────────┐\n";
    cout << "  │  " << Color::BGREEN << "👤 Login sebagai: "
        << Color::BWHITE << left << setw(17) << username
        << Color::BBLUE << "  [USER] │\n";
    cout << "  ├──────────────────────────────────────────────┤\n";
    cout << Color::RESET;
    cout << Color::BBLUE << "  │" << Color::RESET << Color::BOLD
        << "  MENU USER                                   "
        << Color::BBLUE << "│\n" << Color::RESET;
    cout << Color::BBLUE << "  ├──────────────────────────────────────────────┤\n" << Color::RESET;

    cetakMenuItem(Color::BBLUE, "1", "🛎 ", "Reservasi Kamar Baru");
    cetakMenuItem(Color::BBLUE, "2", "📋", "Lihat Reservasi Saya");
    cetakMenuItem(Color::BBLUE, "3", "🔎", "Cek Ketersediaan (Cari ID)");
    cetakMenuItem(Color::BBLUE, "4", "🏷 ", "Filter Kamar (by Tipe)");
    cetakMenuItem(Color::BBLUE, "0", "🚶", "Keluar (Logout)");

    cout << Color::BBLUE << "  └──────────────────────────────────────────────┘\n" << Color::RESET;
    cout << Color::BWHITE << "  Pilih Layanan (0-4): " << Color::RESET;
}

void tampilKamar(Kamar* k, int* n) {
    cout << "\n";
    cout << Color::BCYAN << Color::BOLD;
    cout << "  ╔═════════════════════════════════════════════════════════╗\n";
    cout << "  ║              DAFTAR KAMAR HOTEL                         ║\n";
    cout << "  ╠══════════╦════════════════════╦══════════════╦══════════╣\n";
    cout << "  ║ No Kamar ║     Tipe Kamar     ║    Harga     ║  Status  ║\n";
    cout << "  ╠══════════╬════════════════════╬══════════════╬══════════╣\n";
    cout << Color::RESET;

    for (int i = 0; i < *n; i++) {
        string status      = (k[i].tersedia == 1) ? "Tersedia" : "Terisi  ";
        string statusColor = (k[i].tersedia == 1) ? Color::BGREEN : Color::BRED;

        cout << Color::BCYAN << Color::BOLD << "  ║ " << Color::RESET;
        cout << Color::BWHITE << setw(8) << left << k[i].noKamar;
        cout << Color::BCYAN << Color::BOLD << " ║ " << Color::RESET;
        cout << Color::BWHITE << setw(18) << left << k[i].tipe;
        cout << Color::BCYAN << Color::BOLD << " ║ " << Color::RESET;
        cout << Color::BYELLOW << "Rp " << setw(9) << left << k[i].harga;
        cout << Color::BCYAN << Color::BOLD << " ║ " << Color::RESET;
        cout << statusColor << Color::BOLD << setw(8) << left << status;
        cout << Color::BCYAN << Color::BOLD << " ║\n" << Color::RESET;
    }

    cout << Color::BCYAN << Color::BOLD;
    cout << "  ╚══════════╩════════════════════╩══════════════╩══════════╝\n";
    cout << Color::RESET;
}

void tampilKamar(Kamar* k, int n, string filterTipe) {
    string filterLower = filterTipe;
    transform(filterLower.begin(), filterLower.end(), filterLower.begin(), ::tolower);

    cout << "\n";
    cout << Color::BYELLOW << Color::BOLD;
    cout << "  ┌─────────────────────────────────────────┐\n";
    cout << "  │  🏷  FILTER TIPE: " << Color::BWHITE << left << setw(23) << filterTipe << Color::BYELLOW << "│\n";
    cout << "  └─────────────────────────────────────────┘\n";
    cout << Color::RESET;

    bool ditemukan = false;
    for (int i = 0; i < n; i++) {
        string tipeKamar = k[i].tipe;
        transform(tipeKamar.begin(), tipeKamar.end(), tipeKamar.begin(), ::tolower);
        if (tipeKamar == filterLower) {
            string statusColor = (k[i].tersedia == 1) ? Color::BGREEN : Color::BRED;
            string statusStr   = (k[i].tersedia == 1) ? "[Tersedia]" : "[Terisi]";
            cout << "  " << Color::BBLUE << "►" << Color::RESET
                << " No: " << Color::BWHITE << Color::BOLD << k[i].noKamar << Color::RESET
                << "  |  Harga: " << Color::BYELLOW << "Rp " << k[i].harga << Color::RESET
                << "  |  " << statusColor << Color::BOLD << statusStr << Color::RESET << "\n";
            ditemukan = true;
        }
    }

    if (!ditemukan) {
        cout << "  " << Color::BRED << Color::BOLD
            << "[!] Tipe kamar '" << filterTipe << "' tidak ditemukan.\n" << Color::RESET;
    }
}

void create(Kamar* kList, Reservasi* rList, int* nK, int* nR, string usernameAktif) {
    tampilKamar(kList, nK);

    int pilihNo;
    cout << "\n  " << Color::BWHITE << Color::BOLD
        << "Masukkan Nomor Kamar yang ingin dipesan: " << Color::RESET;
    if (!inputInt(pilihNo, 1, 9999)) {
        cout << "\n  " << Color::BRED << Color::BOLD
            << "[ERROR] Input nomor kamar tidak valid! Harus angka positif tanpa simbol/spasi.\n"
            << Color::RESET;
        return;
    }

    bool ditemukan = false;
    for (int i = 0; i < *nK; i++) {
        if (kList[i].noKamar == pilihNo && kList[i].tersedia == 1) {
            ditemukan = true;
            try {
                cout << "\n";
                cout << Color::BBLUE << Color::BOLD;
                cout << "  ┌───────────────────────────────────────┐\n";
                cout << "  │         ISI DATA TAMU                 │\n";
                cout << "  └───────────────────────────────────────┘\n";
                cout << Color::RESET;

                string nama;
                do {
                    cout << "  " << Color::BCYAN << "Nama Tamu        : " << Color::RESET;
                    getline(cin, nama);
                    nama = trimString(nama);
                    if (nama.empty()) {
                        cout << "  " << Color::BRED
                            << "[!] Nama tidak boleh kosong atau hanya spasi.\n" << Color::RESET;
                        continue;
                    }
                    if (!validasiNama(nama))
                        cout << "  " << Color::BRED
                            << "[!] Nama hanya boleh huruf & spasi, minimal 2 karakter.\n" << Color::RESET;
                } while (!validasiNama(nama));
                rList[*nR].nama = nama;

                string nik;
                cout << "  " << Color::DIM << "    (NIK: 16 digit angka, tanpa spasi)\n" << Color::RESET;
                do {
                    cout << "  " << Color::BCYAN << "NIK / No. KTP    : " << Color::RESET;
                    getline(cin, nik);
                    nik = trimString(nik);
                    if (nik.empty()) {
                        cout << "  " << Color::BRED
                            << "[!] NIK tidak boleh kosong atau hanya spasi.\n" << Color::RESET;
                        continue;
                    }
                    if (!validasiNIK(nik))
                        cout << "  " << Color::BRED
                            << "[!] NIK harus tepat 16 digit angka (tanpa spasi/simbol).\n" << Color::RESET;
                } while (!validasiNIK(nik));
                rList[*nR].data.ktp = nik;

                string hp;
                do {
                    cout << "  " << Color::BCYAN << "No HP Tamu       : " << Color::RESET;
                    getline(cin, hp);
                    hp = trimString(hp);
                    if (hp.empty()) {
                        cout << "  " << Color::BRED
                            << "[!] No HP tidak boleh kosong atau hanya spasi.\n" << Color::RESET;
                        continue;
                    }
                    if (!validasiHP(hp))
                        cout << "  " << Color::BRED
                            << "[!] No HP harus diawali 08 dan 10-13 digit (tanpa spasi/tanda -).\n" << Color::RESET;
                } while (!validasiHP(hp));
                rList[*nR].data.noHP = hp;

                string tglCheckin;
                do {
                    cout << "  " << Color::BCYAN << "Tanggal Check-in : " << Color::RESET;
                    cout << Color::DIM << "(format DD-MM-YYYY) " << Color::RESET;
                    getline(cin, tglCheckin);
                    tglCheckin = trimString(tglCheckin);
                    if (tglCheckin.empty()) {
                        cout << "  " << Color::BRED
                            << "[!] Tanggal tidak boleh kosong atau hanya spasi.\n" << Color::RESET;
                        continue;
                    }
                    if (!validasiTanggal(tglCheckin))
                        cout << "  " << Color::BRED
                            << "[!] Format tanggal tidak valid. Contoh: 25-12-2025\n" << Color::RESET;
                } while (!validasiTanggal(tglCheckin));
                rList[*nR].tanggal = tglCheckin;

                int lama = 0;
                const int MAX_LAMA = 365;
                do {
                    cout << "  " << Color::BCYAN << "Lama Inap (hari) : " << Color::RESET;
                    if (!inputInt(lama, 1, MAX_LAMA)) {
                        cout << "  " << Color::BRED
                            << "[!] Input tidak valid. Masukkan angka 1-" << MAX_LAMA
                            << " (tanpa minus/simbol/spasi).\n" << Color::RESET;
                        lama = 0;
                    }
                } while (lama <= 0);

                rList[*nR].lama       = lama;
                rList[*nR].noKamar    = pilihNo;
                rList[*nR].totalHarga = kList[i].harga * lama;
                rList[*nR].pemilik    = usernameAktif;

                cout << "\n";
                cout << Color::BGREEN << Color::BOLD;
                cout << "  ╔══════════════════════════════════════════╗\n";
                cout << "  ║            NOTA RESERVASI                ║\n";
                cout << "  ╠══════════════════════════════════════════╣\n";
                cout << Color::RESET;
                cout << Color::BGREEN << "  ║ " << Color::RESET << Color::BWHITE << "Nama Tamu    : " << Color::BYELLOW << left << setw(25) << rList[*nR].nama        << Color::BGREEN << " ║\n" << Color::RESET;
                cout << Color::BGREEN << "  ║ " << Color::RESET << Color::BWHITE << "NIK / KTP    : " << Color::BWHITE  << left << setw(25) << rList[*nR].data.ktp     << Color::BGREEN << " ║\n" << Color::RESET;
                cout << Color::BGREEN << "  ║ " << Color::RESET << Color::BWHITE << "No. Kamar    : " << Color::BYELLOW << left << setw(25) << rList[*nR].noKamar      << Color::BGREEN << " ║\n" << Color::RESET;
                cout << Color::BGREEN << "  ║ " << Color::RESET << Color::BWHITE << "Tipe Kamar   : " << Color::BYELLOW << left << setw(25) << kList[i].tipe            << Color::BGREEN << " ║\n" << Color::RESET;
                cout << Color::BGREEN << "  ║ " << Color::RESET << Color::BWHITE << "Check-in     : " << Color::BYELLOW << left << setw(25) << rList[*nR].tanggal      << Color::BGREEN << " ║\n" << Color::RESET;
                cout << Color::BGREEN << "  ║ " << Color::RESET << Color::BWHITE << "Lama Inap    : " << Color::BYELLOW << lama << " hari" << string(19, ' ')             << Color::BGREEN << " ║\n" << Color::RESET;
                cout << Color::BGREEN << "  ║ " << Color::RESET << Color::BWHITE << "Total Biaya  : " << Color::BYELLOW << "Rp " << left << setw(22) << rList[*nR].totalHarga << Color::BGREEN << " ║\n" << Color::RESET;
                cout << Color::BGREEN << Color::BOLD;
                cout << "  ╚══════════════════════════════════════════╝\n";
                cout << Color::RESET;
                cout << "  " << Color::BGREEN << Color::BOLD << "✔  Terima kasih telah memesan di Grand Rizuna!\n" << Color::RESET;

                kList[i].tersedia = 0;
                (*nR)++;
                cout << "\n  " << Color::BGREEN << Color::BOLD
                    << "[BERHASIL] Reservasi Kamar " << pilihNo << " telah diproses.\n" << Color::RESET;
            } catch (const invalid_argument& e) {
                cout << "\n  " << Color::BRED << Color::BOLD << "[ERROR] " << e.what() << "\n" << Color::RESET;
            }
            return;
        }
    }
    if (!ditemukan) {
        cout << "\n  " << Color::BRED << Color::BOLD
            << "[GAGAL] Kamar tidak ditemukan atau sudah terisi!\n" << Color::RESET;
    }
}

void read(Reservasi* rList, int* nR) {
    if (*nR == 0) {
        cout << "\n  " << Color::BYELLOW << Color::BOLD << "[INFO] Belum ada data reservasi.\n" << Color::RESET;
        return;
    }
    cout << "\n";
    cout << Color::BCYAN << Color::BOLD;
    cout << "  ╔═══╦═══════════════════╦══════════════════╦═══════════════╦════════════╦═══════╦═════════════╗\n";
    cout << "  ║ No║ Nama              ║ NIK/KTP          ║ No HP         ║Check-in    ║ Kamar ║ Total       ║\n";
    cout << "  ╠═══╬═══════════════════╬══════════════════╬═══════════════╬════════════╬═══════╬═════════════╣\n";
    cout << Color::RESET;
    for (int i = 0; i < *nR; i++) {
        cout << Color::BCYAN << "  ║ " << Color::RESET
            << Color::BWHITE << setw(2) << left << i+1
            << Color::BCYAN << "║ " << Color::RESET
            << setw(17) << left << rList[i].nama.substr(0, 17)
            << Color::BCYAN << " ║ " << Color::RESET
            << setw(16) << left << rList[i].data.ktp.substr(0, 16)
            << Color::BCYAN << " ║ " << Color::RESET
            << setw(13)  << left << rList[i].data.noHP.substr(0, 13)
            << Color::BCYAN << " ║ " << Color::RESET
            << Color::BWHITE << setw(10) << left << rList[i].tanggal.substr(0, 10)
            << Color::BCYAN << " ║ " << Color::RESET
            << Color::BYELLOW << setw(5)  << left << rList[i].noKamar
            << Color::BCYAN << " ║ " << Color::RESET
            << Color::BGREEN << "Rp " << setw(8) << left << rList[i].totalHarga
            << Color::BCYAN << " ║\n" << Color::RESET;
    }
    cout << Color::BCYAN << Color::BOLD;
    cout << "  ╚═══╩═══════════════════╩══════════════════╩═══════════════╩════════════╩═══════╩═════════════╝\n";
    cout << Color::RESET;
}

void readUser(Reservasi* rList, int* nR, string usernameAktif) {
    bool ada = false;
    int urut = 1;
    cout << "\n";
    cout << Color::BBLUE << Color::BOLD;
    cout << "  ╔═════════════════════════════════════════════════════════╗\n";
    cout << "  ║          RESERVASI SAYA - " << left << setw(28) << usernameAktif << "  ║\n";
    cout << "  ╠═════════════════════════════════════════════════════════╣\n";
    cout << Color::RESET;
    for (int i = 0; i < *nR; i++) {
        if (rList[i].pemilik == usernameAktif) {
            ada = true;
            cout << Color::BBLUE << "  ║ " << Color::RESET
                << Color::BWHITE << Color::BOLD << "Reservasi #" << urut++ << Color::RESET
                << "                                           "
                << Color::BBLUE << " ║\n" << Color::RESET;
            cout << Color::BBLUE << "  ║  " << Color::RESET
                << "Nama     : " << Color::BWHITE << left << setw(18) << rList[i].nama << Color::RESET
                << "  Kamar: " << Color::BYELLOW << rList[i].noKamar
                << "             " << Color::BBLUE << " ║\n" << Color::RESET;
            cout << Color::BBLUE << "  ║  " << Color::RESET
                << "Check-in : " << Color::BWHITE << left << setw(18) << rList[i].tanggal << Color::RESET
                << "  Lama : " << Color::BWHITE << rList[i].lama << " hari"
                << "          " << Color::BBLUE << " ║\n" << Color::RESET;
            cout << Color::BBLUE << "  ║  " << Color::RESET
                << "NIK/KTP  : " << left << setw(18) << rList[i].data.ktp
                << "  Total: " << Color::BGREEN << "Rp " << rList[i].totalHarga
                << "       " << Color::BBLUE << " ║\n" << Color::RESET;
            cout << Color::BBLUE << "  ╠═════════════════════════════════════════════════════════╣\n" << Color::RESET;
        }
    }
    if (!ada) {
        cout << Color::BBLUE << "  ║  " << Color::RESET
            << Color::BYELLOW << "Kamu belum memiliki reservasi." << Color::RESET
            << "                              "
            << Color::BBLUE << " ║\n";
        cout << "  ╚═════════════════════════════════════════════════════════╝\n" << Color::RESET;
    } else {
        cout << Color::BBLUE << "  ╚═════════════════════════════════════════════════════════╝\n" << Color::RESET;
    }
}

void update(Reservasi* rList, int* nR, Kamar* kList, int* nK) {
    read(rList, nR);
    if (*nR == 0) return;

    int idx;
    cout << "\n  " << Color::BWHITE << Color::BOLD
         << "Pilih nomor urut yang diubah (1-" << *nR << "): " << Color::RESET;
    if (!inputInt(idx, 1, *nR)) {
        cout << "  " << Color::BRED
             << "[ERROR] Input tidak valid. Masukkan angka 1-" << *nR << ".\n" << Color::RESET;
        return;
    }

    try {
        string tglBaru;
        do {
            cout << "  " << Color::BCYAN << "Tanggal Check-in Baru "
                << Color::DIM << "(DD-MM-YYYY)" << Color::RESET << ": ";
            getline(cin, tglBaru);
            tglBaru = trimString(tglBaru);
            if (tglBaru.empty()) {
                cout << "  " << Color::BRED
                    << "[!] Tanggal tidak boleh kosong atau hanya spasi.\n" << Color::RESET;
                continue;
            }
            if (!validasiTanggal(tglBaru))
                cout << "  " << Color::BRED
                    << "[!] Format tanggal tidak valid. Contoh: 25-12-2025\n" << Color::RESET;
        } while (!validasiTanggal(tglBaru));
        rList[idx-1].tanggal = tglBaru;

        int lama = 0;
        const int MAX_LAMA = 365;
        do {
            cout << "  " << Color::BCYAN << "Lama Inap Baru (hari, 1-" << MAX_LAMA << "): " << Color::RESET;
            if (!inputInt(lama, 1, MAX_LAMA)) {
                cout << "  " << Color::BRED
                    << "[!] Input tidak valid. Masukkan angka 1-" << MAX_LAMA
                    << " (tanpa minus/simbol/spasi).\n" << Color::RESET;
                lama = 0;
            }
        } while (lama <= 0);
        rList[idx-1].lama = lama;

        for (int i = 0; i < *nK; i++) {
            if (kList[i].noKamar == rList[idx-1].noKamar)
                rList[idx-1].totalHarga = kList[i].harga * lama;
        }
        cout << "  " << Color::BGREEN << Color::BOLD << "[BERHASIL] Data berhasil diupdate!\n" << Color::RESET;
    } catch (const invalid_argument& e) {
        cout << "  " << Color::BRED << "[ERROR] " << e.what() << "\n" << Color::RESET;
    }
}

void hapus(Reservasi* rList, int* nR, Kamar* kList, int* nK) {
    read(rList, nR);
    if (*nR == 0) return;

    int idx;
    cout << "\n  " << Color::BWHITE << Color::BOLD
         << "Pilih nomor urut yang dihapus (1-" << *nR << "): " << Color::RESET;
    if (!inputInt(idx, 1, *nR)) {
        cout << "  " << Color::BRED
             << "[ERROR] Input tidak valid. Masukkan angka 1-" << *nR << ".\n" << Color::RESET;
        return;
    }

    for (int i = 0; i < *nK; i++) {
        if (kList[i].noKamar == rList[idx-1].noKamar)
            kList[i].tersedia = 1;
    }
    for (int i = idx - 1; i < *nR - 1; i++) rList[i] = rList[i + 1];
    (*nR)--;
    cout << "  " << Color::BGREEN << Color::BOLD
        << "[BERHASIL] Data reservasi dihapus / check-out selesai!\n" << Color::RESET;
}

void sortingMenu(Kamar* k, int n) {
    for (int i = 0; i < n - 1; i++) {
        int m = i;
        for (int j = i + 1; j < n; j++) {
            if (k[j].tipe > k[m].tipe) m = j;
        }
        Kamar t = k[i]; k[i] = k[m]; k[m] = t;
    }
}

void sortKamarByNumber(Kamar* k, int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (k[j].noKamar > k[j+1].noKamar) {
                Kamar temp = k[j]; k[j] = k[j+1]; k[j+1] = temp;
            }
        }
    }
}

void insertionSortHarga(Kamar* k, int n) {
    for (int i = 1; i < n; i++) {
        Kamar key = k[i];
        int j = i - 1;
        while (j >= 0 && k[j].harga > key.harga) {
            k[j+1] = k[j]; j--;
        }
        k[j+1] = key;
    }
}

int binarySearchRekursif(Kamar* k, int low, int high, int cari) {
    if (low > high) return -1;
    int mid = (low + high) / 2;
    if (k[mid].noKamar == cari) return mid;
    return (k[mid].noKamar > cari) ? binarySearchRekursif(k, low, mid-1, cari)
                                : binarySearchRekursif(k, mid+1, high, cari);
}

void cariNama(Reservasi* rList, int* nR) {
    string nama;
    cout << "\n";
    cout << Color::BBLUE << Color::BOLD;
    cout << "  ┌─────────────────────────────────────┐\n";
    cout << "  │     🔍 PENCARIAN DATA TAMU          │\n";
    cout << "  └─────────────────────────────────────┘\n";
    cout << Color::RESET;

    do {
        cout << "  " << Color::BCYAN << "Masukkan nama lengkap: " << Color::RESET;
        getline(cin, nama);
        nama = trimString(nama);
        if (nama.empty())
            cout << "  " << Color::BRED
                << "[!] Nama tidak boleh kosong atau hanya spasi.\n" << Color::RESET;
    } while (nama.empty());

    bool ditemukan = false;
    int index = -1;
    for (int i = 0; i < *nR; i++) {
        if (rList[i].nama == nama) { ditemukan = true; index = i; break; }
    }

    if (ditemukan) {
        cout << "\n";
        cout << Color::BGREEN << Color::BOLD;
        cout << "  ╔══════════════════════════════════════════╗\n";
        cout << "  ║         ✔  DATA TAMU DITEMUKAN           ║\n";
        cout << "  ╠══════════════════════════════════════════╣\n";
        cout << Color::RESET;
        cout << Color::BGREEN << "  ║ " << Color::RESET << Color::BWHITE << "Nama      : " << Color::BYELLOW << left << setw(28) << rList[index].nama              << Color::BGREEN << " ║\n" << Color::RESET;
        cout << Color::BGREEN << "  ║ " << Color::RESET << Color::BWHITE << "NIK/KTP   : " << Color::BWHITE  << left << setw(28) << rList[index].data.ktp           << Color::BGREEN << " ║\n" << Color::RESET;
        cout << Color::BGREEN << "  ║ " << Color::RESET << Color::BWHITE << "No. HP    : " << Color::BWHITE  << left << setw(28) << rList[index].data.noHP           << Color::BGREEN << " ║\n" << Color::RESET;
        cout << Color::BGREEN << "  ║ " << Color::RESET << Color::BWHITE << "Check-in  : " << Color::BYELLOW << left << setw(28) << rList[index].tanggal             << Color::BGREEN << " ║\n" << Color::RESET;
        cout << Color::BGREEN << "  ║ " << Color::RESET << Color::BWHITE << "No. Kamar : " << Color::BYELLOW << left << setw(28) << rList[index].noKamar             << Color::BGREEN << " ║\n" << Color::RESET;
        cout << Color::BGREEN << "  ║ " << Color::RESET << Color::BWHITE << "Total     : " << Color::BGREEN  << "Rp " << left << setw(25) << rList[index].totalHarga << Color::BGREEN << " ║\n" << Color::RESET;
        cout << Color::BGREEN << Color::BOLD << "  ╚══════════════════════════════════════════╝\n" << Color::RESET;
    } else {
        cout << "\n  " << Color::BRED << Color::BOLD << "[!] Nama '" << nama << "' tidak ditemukan.\n" << Color::RESET;
        cout << "  " << Color::BYELLOW << "[i] Pastikan penulisan nama sudah benar (huruf kapital sesuai data).\n" << Color::RESET;
    }
}

void cariKamar(Kamar* kList, int* nK) {
    cout << "\n";
    cout << Color::BBLUE << Color::BOLD;
    cout << "  ┌─────────────────────────────────────┐\n";
    cout << "  │     🔎 CEK KETERSEDIAAN KAMAR       │\n";
    cout << "  └─────────────────────────────────────┘\n";
    cout << Color::RESET;
    cout << "  " << Color::BCYAN << "Nomor kamar yang dicari: " << Color::RESET;

    int no;
    if (!inputInt(no, 1, 9999)) {
        cout << "\n  " << Color::BRED << Color::BOLD
            << "[ERROR] Input nomor kamar tidak valid! Masukkan angka positif tanpa simbol/spasi.\n"
            << Color::RESET;
        return;
    }

    sortKamarByNumber(kList, *nK);
    int hasil = binarySearchRekursif(kList, 0, *nK - 1, no);

    if (hasil != -1) {
        string statusColor = (kList[hasil].tersedia == 1) ? Color::BGREEN : Color::BRED;
        string statusStr   = (kList[hasil].tersedia == 1) ? "Tersedia ✔" : "Terisi   ✗";
        cout << "\n";
        cout << Color::BCYAN << Color::BOLD;
        cout << "  ╔══════════════════════════════════════════╗\n";
        cout << "  ║         DETAIL INFORMASI KAMAR           ║\n";
        cout << "  ╠══════════════════════════════════════════╣\n";
        cout << Color::RESET;
        cout << Color::BCYAN << "  ║ " << Color::RESET << Color::BWHITE << "Nomor Kamar : " << Color::BYELLOW << left << setw(26) << kList[hasil].noKamar << Color::BCYAN << " ║\n" << Color::RESET;
        cout << Color::BCYAN << "  ║ " << Color::RESET << Color::BWHITE << "Tipe Kamar  : " << Color::BWHITE  << left << setw(26) << kList[hasil].tipe     << Color::BCYAN << " ║\n" << Color::RESET;
        cout << Color::BCYAN << "  ║ " << Color::RESET << Color::BWHITE << "Harga/Malam : " << Color::BYELLOW << "Rp " << left << setw(23) << kList[hasil].harga << Color::BCYAN << " ║\n" << Color::RESET;
        cout << Color::BCYAN << "  ║ " << Color::RESET << Color::BWHITE << "Status      : " << statusColor << Color::BOLD << left << setw(28) << statusStr   << Color::BCYAN << " ║\n" << Color::RESET;
        cout << Color::BCYAN << Color::BOLD << "  ╚══════════════════════════════════════════╝\n" << Color::RESET;
    } else {
        cout << "\n  " << Color::BRED << Color::BOLD << "[!] Nomor kamar '" << no << "' tidak terdaftar.\n" << Color::RESET;
        cout << "  " << Color::BYELLOW << "[i] Silakan cek daftar kamar pada menu utama.\n" << Color::RESET;
    }
}

void registrasi() {
    if (jumlahUser >= 10) {
        cout << "\n  " << Color::BRED << "[!] Kapasitas akun penuh.\n" << Color::RESET;
        return;
    }
    cout << "\n";
    cout << Color::BMAGENTA << Color::BOLD;
    cout << "  ┌─────────────────────────────────────┐\n";
    cout << "  │        DAFTAR AKUN BARU (USER)      │\n";
    cout << "  └─────────────────────────────────────┘\n";
    cout << Color::RESET;

    string uname, pass;

    do {
        cout << "  " << Color::BCYAN << "Username (4-20 karakter, a-z/0-9/_, tanpa spasi): " << Color::RESET;
        getline(cin, uname);
        uname = trimString(uname);
        if (!validasiUsername(uname)) {
            cout << "  " << Color::BRED
                << "[!] Username tidak valid (4-20 karakter, huruf/angka/_, tanpa spasi).\n" << Color::RESET;
        } else {
            bool duplikat = false;
            for (int i = 0; i < jumlahUser; i++) {
                if (akun[i].username == uname) { duplikat = true; break; }
            }
            if (duplikat) {
                cout << "  " << Color::BRED << "[!] Username sudah digunakan, coba yang lain.\n" << Color::RESET;
                uname = "";
            }
        }
    } while (!validasiUsername(uname));

    do {
        cout << "  " << Color::BCYAN << "Password (min. 6 karakter)                      : " << Color::RESET;
        getline(cin, pass);
        pass = trimString(pass);
        if (pass.empty())
            cout << "  " << Color::BRED << "[!] Password tidak boleh kosong atau hanya spasi.\n" << Color::RESET;
        else if (!validasiPassword(pass))
            cout << "  " << Color::BRED << "[!] Password minimal 6 karakter.\n" << Color::RESET;
    } while (!validasiPassword(pass));

    akun[jumlahUser].username = uname;
    akun[jumlahUser].nim      = pass;
    akun[jumlahUser].role     = 0;
    jumlahUser++;
    cout << "  " << Color::BGREEN << Color::BOLD << "[BERHASIL] Akun user berhasil dibuat! Silakan login.\n" << Color::RESET;
}

void registrasiAdmin() {
    if (jumlahUser >= 10) {
        cout << "\n  " << Color::BRED << "[!] Kapasitas akun penuh.\n" << Color::RESET;
        return;
    }
    cout << "\n";
    cout << Color::BYELLOW << Color::BOLD;
    cout << "  ┌─────────────────────────────────────┐\n";
    cout << "  │        TAMBAH AKUN ADMIN BARU       │\n";
    cout << "  └─────────────────────────────────────┘\n";
    cout << Color::RESET;

    string uname, pass;

    do {
        cout << "  " << Color::BCYAN << "Username (4-20 karakter, a-z/0-9/_, tanpa spasi): " << Color::RESET;
        getline(cin, uname);
        uname = trimString(uname);
        if (!validasiUsername(uname)) {
            cout << "  " << Color::BRED
                << "[!] Username tidak valid (4-20 karakter, huruf/angka/_, tanpa spasi).\n" << Color::RESET;
        } else {
            bool duplikat = false;
            for (int i = 0; i < jumlahUser; i++) {
                if (akun[i].username == uname) { duplikat = true; break; }
            }
            if (duplikat) {
                cout << "  " << Color::BRED << "[!] Username sudah digunakan.\n" << Color::RESET;
                uname = "";
            }
        }
    } while (!validasiUsername(uname));

    do {
        cout << "  " << Color::BCYAN << "Password (min. 6 karakter)                      : " << Color::RESET;
        getline(cin, pass);
        pass = trimString(pass);
        if (pass.empty())
            cout << "  " << Color::BRED << "[!] Password tidak boleh kosong atau hanya spasi.\n" << Color::RESET;
        else if (!validasiPassword(pass))
            cout << "  " << Color::BRED << "[!] Password minimal 6 karakter.\n" << Color::RESET;
    } while (!validasiPassword(pass));

    akun[jumlahUser].username = uname;
    akun[jumlahUser].nim      = pass;
    akun[jumlahUser].role     = 1;
    jumlahUser++;
    cout << "  " << Color::BGREEN << Color::BOLD
        << "[BERHASIL] Akun admin '" << uname << "' berhasil ditambahkan!\n" << Color::RESET;
}

int login(string& usernameOut, int& roleOut) {
    if (jumlahUser == 0) {
        cout << "\n  " << Color::BYELLOW << Color::BOLD << "[!] Belum ada akun. Silakan registrasi dulu!\n" << Color::RESET;
        return 0;
    }
    string u, p;
    cout << "\n";
    cout << Color::BMAGENTA << Color::BOLD;
    cout << "  ┌─────────────────────────────────────┐\n";
    cout << "  │          LOGIN KE SISTEM            │\n";
    cout << "  └─────────────────────────────────────┘\n";
    cout << Color::RESET;
    cout << "  " << Color::BCYAN << "Username : " << Color::RESET;
    getline(cin, u);
    cout << "  " << Color::BCYAN << "Password : " << Color::RESET;
    getline(cin, p);

    u = trimString(u);
    p = trimString(p);

    for (int i = 0; i < jumlahUser; i++) {
        if (akun[i].username == u && akun[i].nim == p) {
            usernameOut = u;
            roleOut     = akun[i].role;
            return 1;
        }
    }
    return 0;
}


int main() {
    enableANSI();

    akun[0] = {"admin", "admin123", 1};
    akun[1] = {"user",  "user123",  0};
    jumlahUser = 2;

    int pilihAwal;
    bool programJalan = true;

    while (programJalan) {
        string usernameAktif = "";
        int roleAktif   = 0;
        int statusLogin = 0;
        int percobaan   = 0;

        while (statusLogin == 0) {
            safeClear();
            tampilWelcome();
            cout << Color::BWHITE << Color::BOLD;
            cout << "  ┌─────────────────────────────────────────────────────┐\n";
            cout << "  │  Percobaan Login : [" << percobaan+1 << "/3]                            │\n";
            cout << "  ├─────────────────────────────────────────────────────┤\n";
            cout << "  │  [1] Login ke Sistem                                │\n";
            cout << "  │  [2] Registrasi Akun User Baru                      │\n";
            cout << "  │  [0] Keluar dari Aplikasi                           │\n";
            cout << "  └─────────────────────────────────────────────────────┘\n";
            cout << Color::RESET;
            cout << "  " << Color::BWHITE << Color::BOLD << "Pilih Opsi (0-2): " << Color::RESET;

            if (!inputInt(pilihAwal, 0, 2)) {
                cout << "\n  " << Color::BRED << "[!] Input tidak valid. Masukkan angka 1, 2, atau 0.\n" << Color::RESET;
                cout << "  " << Color::DIM << "Tekan Enter untuk lanjut..." << Color::RESET;
                cin.ignore(1000, '\n');
                continue;
            }

            if (pilihAwal == 1) {
                if (login(usernameAktif, roleAktif) == 1) {
                    statusLogin = 1;
                    percobaan = 0;
                    cout << "\n  " << Color::BGREEN << Color::BOLD
                        << "✔  Login berhasil! Selamat datang, " << usernameAktif << "!\n" << Color::RESET;
                    cout << "  " << Color::DIM << "Tekan Enter untuk masuk ke menu..." << Color::RESET;
                    cin.ignore(1000, '\n');
                } else {
                    percobaan++;
                    cout << "\n  " << Color::BRED << Color::BOLD
                        << "[!] Gagal! Username/Password salah. (" << percobaan << "/3)\n" << Color::RESET;
                    if (percobaan >= 3) {
                        cout << "  " << Color::BRED << Color::BOLD
                            << "[!] Akun terkunci karena salah 3x. Program berhenti.\n" << Color::RESET;
                        return 0;
                    }
                    cout << "  " << Color::DIM << "Tekan Enter untuk coba lagi..." << Color::RESET;
                    cin.ignore(1000, '\n');
                }
            } else if (pilihAwal == 2) {
                registrasi();
                cout << "  " << Color::DIM << "Tekan Enter untuk lanjut..." << Color::RESET;
                cin.ignore(1000, '\n');
            } else {
                programJalan = false;
                cout << "\n  " << Color::BMAGENTA << Color::BOLD
                    << "Terima kasih telah menggunakan HMS Grand Rizuna Estate. Sampai jumpa!\n" << Color::RESET;
                break;
            }
        }

        if (!programJalan) break;

        if (roleAktif == 1) {
            while (statusLogin == 1) {
                safeClear();
                int pilih;
                tampilMenuAdmin(usernameAktif);

                if (!inputInt(pilih, 0, 9)) {
                    cout << "\n  " << Color::BRED
                        << "[!] Input tidak valid. Masukkan angka 0-9 tanpa simbol/huruf/spasi.\n" << Color::RESET;
                    cout << "  " << Color::DIM << "Tekan Enter untuk lanjut..." << Color::RESET;
                    cin.ignore(1000, '\n');
                    continue;
                }

                switch (pilih) {
                    case 1: create(kamarList, dataReservasi, &jumlahKamar, &jumlahReservasi, usernameAktif); break;
                    case 2: read(dataReservasi, &jumlahReservasi); break;
                    case 3: update(dataReservasi, &jumlahReservasi, kamarList, &jumlahKamar); break;
                    case 4: hapus(dataReservasi, &jumlahReservasi, kamarList, &jumlahKamar); break;
                    case 5: {
                        int s = 0;
                        cout << "\n";
                        cout << Color::BBLUE << Color::BOLD;
                        cout << "  ┌────────────────────────────────────────────┐\n";
                        cout << "  │      PILIH METODE PENGURUTAN KAMAR         │\n";
                        cout << "  ├────────────────────────────────────────────┤\n";
                        cout << "  │  [1] Urutkan berdasarkan Tipe Kamar        │\n";
                        cout << "  │  [2] Urutkan berdasarkan Nomor Kamar       │\n";
                        cout << "  │  [3] Urutkan berdasarkan Harga Kamar       │\n";
                        cout << "  └────────────────────────────────────────────┘\n";
                        cout << Color::RESET;
                        cout << "  " << Color::BWHITE << "Pilihan (1-3): " << Color::RESET;
                        if (!inputInt(s, 1, 3)) {
                            cout << "  " << Color::BRED << "[!] Input tidak valid. Masukkan 1, 2, atau 3.\n" << Color::RESET;
                            break;
                        }
                        if      (s == 1) { sortingMenu(kamarList, jumlahKamar);        cout << "  " << Color::BGREEN << "[INFO] Diurutkan berdasarkan Tipe.\n"  << Color::RESET; }
                        else if (s == 2) { sortKamarByNumber(kamarList, jumlahKamar);  cout << "  " << Color::BGREEN << "[INFO] Diurutkan berdasarkan Nomor.\n" << Color::RESET; }
                        else             { insertionSortHarga(kamarList, jumlahKamar); cout << "  " << Color::BGREEN << "[INFO] Diurutkan berdasarkan Harga.\n" << Color::RESET; }
                        tampilKamar(kamarList, &jumlahKamar);
                        break;
                    }
                    case 6: cariNama(dataReservasi, &jumlahReservasi); break;
                    case 7: cariKamar(kamarList, &jumlahKamar); break;
                    case 8: {
                        string t;
                        cout << "  " << Color::BCYAN
                            << "Masukkan tipe kamar (contoh: Standar / Deluxe / Family): " << Color::RESET;
                        getline(cin, t);
                        t = trimString(t);
                        if (t.empty()) {
                            cout << "  " << Color::BRED
                                << "[!] Tipe kamar tidak boleh kosong atau hanya spasi.\n" << Color::RESET;
                        } else {
                            tampilKamar(kamarList, jumlahKamar, t);
                        }
                        break;
                    }
                    case 9: registrasiAdmin(); break;
                    case 0:
                        statusLogin = 0;
                        cout << "\n  " << Color::BYELLOW << Color::BOLD
                            << "[INFO] Logout berhasil. Sampai jumpa, " << usernameAktif << "!\n" << Color::RESET;
                        break;
                    default:
                        cout << "\n  " << Color::BRED << "[!] Pilihan tidak tersedia.\n" << Color::RESET;
                }

                if (statusLogin == 1) {
                    cout << "\n  " << Color::DIM << "Tekan Enter untuk kembali ke menu..." << Color::RESET;
                    cin.ignore(1000, '\n');
                }
            }
        }

        else {
            while (statusLogin == 1) {
                safeClear();
                int pilih;
                tampilMenuUser(usernameAktif);

                if (!inputInt(pilih, 0, 4)) {
                    cout << "\n  " << Color::BRED
                        << "[!] Input tidak valid. Masukkan angka 0-4 tanpa simbol/huruf/spasi.\n" << Color::RESET;
                    cout << "  " << Color::DIM << "Tekan Enter untuk lanjut..." << Color::RESET;
                    cin.ignore(1000, '\n');
                    continue;
                }

                switch (pilih) {
                    case 1: create(kamarList, dataReservasi, &jumlahKamar, &jumlahReservasi, usernameAktif); break;
                    case 2: readUser(dataReservasi, &jumlahReservasi, usernameAktif); break;
                    case 3: cariKamar(kamarList, &jumlahKamar); break;
                    case 4: {
                        string t;
                        cout << "  " << Color::BCYAN
                            << "Masukkan tipe kamar (contoh: Standar / Deluxe / Family): " << Color::RESET;
                        getline(cin, t);
                        t = trimString(t);
                        if (t.empty()) {
                            cout << "  " << Color::BRED
                                << "[!] Tipe kamar tidak boleh kosong atau hanya spasi.\n" << Color::RESET;
                        } else {
                            tampilKamar(kamarList, jumlahKamar, t);
                        }
                        break;
                    }
                    case 0:
                        statusLogin = 0;
                        cout << "\n  " << Color::BYELLOW << Color::BOLD
                            << "[INFO] Logout berhasil. Sampai jumpa, " << usernameAktif << "!\n" << Color::RESET;
                        break;
                    default:
                        cout << "\n  " << Color::BRED << "[!] Pilihan tidak tersedia.\n" << Color::RESET;
                }

                if (statusLogin == 1) {
                    cout << "\n  " << Color::DIM << "Tekan Enter untuk kembali ke menu..." << Color::RESET;
                    cin.ignore(1000, '\n');
                }
            }
        }
    }

    return 0;
}