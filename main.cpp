#include "hotel_lib.h"
#include <regex>
#include <ctime>

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

// ─── Validasi Input ───────────────────────────────────────────────────────────

void cekInputPositif(int nilai) {
    if (nilai <= 0) throw invalid_argument("Input harus lebih dari 0!");
}

// Validasi NIK: tepat 16 digit angka
bool validasiNIK(const string& nik) {
    regex pola("^[0-9]{16}$");
    return regex_match(nik, pola);
}

// Validasi No HP: diawali 08, panjang 10-13 digit
bool validasiHP(const string& hp) {
    regex pola("^08[0-9]{8,11}$");
    return regex_match(hp, pola);
}

// Validasi nama: hanya huruf dan spasi, minimal 2 karakter
bool validasiNama(const string& nama) {
    regex pola("^[A-Za-z ]{2,}$");
    return regex_match(nama, pola);
}

// Validasi tanggal: format DD-MM-YYYY
bool validasiTanggal(const string& tgl) {
    regex pola("^(0[1-9]|[12][0-9]|3[01])-(0[1-9]|1[0-2])-([0-9]{4})$");
    return regex_match(tgl, pola);
}

// Validasi username: alfanumerik, 4-20 karakter
bool validasiUsername(const string& u) {
    regex pola("^[A-Za-z0-9_]{4,20}$");
    return regex_match(u, pola);
}

// Validasi password: minimal 6 karakter
bool validasiPassword(const string& p) {
    return p.size() >= 6;
}

// ─── Tampil Header & Welcome ─────────────────────────────────────────────────

void tampilWelcome() {
    cout << "\n";
    cout << Color::BMAGENTA << Color::BOLD;
    cout << "  ╔══════════════════════════════════════════════════════╗\n";
    cout << "  ║                                                      ║\n";
    cout << "  ║        ★  GRAND RIZUNA ESTATE HOTEL  ★              ║\n";
    cout << "  ║         Hotel Management System  v2.1               ║\n";
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

// ─── Tampil Menu ──────────────────────────────────────────────────────────────

void tampilMenuAdmin(string username) {
    tampilHeader();
    cout << Color::BYELLOW << Color::BOLD;
    cout << "  ┌──────────────────────────────────────────────┐\n";
    cout << "  │  " << Color::BGREEN << "👤 Login sebagai: " << Color::BWHITE << left << setw(20) << username << Color::BYELLOW << "  [ADMIN]  │\n";
    cout << "  ├──────────────────────────────────────────────┤\n";
    cout << Color::RESET;
    cout << Color::BYELLOW << "  │" << Color::RESET << Color::BOLD << "  MENU ADMIN                                  " << Color::BYELLOW << "│\n" << Color::RESET;
    cout << Color::BYELLOW << "  ├──────────────────────────────────────────────┤\n" << Color::RESET;

    auto menuItem = [](string num, string ikon, string label) {
        cout << Color::BYELLOW << "  │  " << Color::RESET
            << Color::BWHITE << Color::BOLD << "[" << num << "] " << Color::RESET
            << ikon << " " << left << setw(38) << label
            << Color::BYELLOW << "│\n" << Color::RESET;
    };

    menuItem("1",  "🛎 ", "Reservasi Kamar Baru");
    menuItem("2",  "📋", "Lihat Daftar Reservasi");
    menuItem("3",  "✏ ", "Ubah Detail Reservasi");
    menuItem("4",  "🚪", "Batalkan Reservasi (Check-out)");
    menuItem("5",  "🗂 ", "Kelola & Urutkan Daftar Kamar");
    menuItem("6",  "🔍", "Cari Data Tamu");
    menuItem("7",  "🔎", "Cek Ketersediaan (Cari ID)");
    menuItem("8",  "🏷 ", "Filter Kamar (by Tipe)");
    menuItem("9",  "👤", "Tambah Akun Admin Baru");
    menuItem("10", "🚶", "Keluar (Logout)");

    cout << Color::BYELLOW << "  └──────────────────────────────────────────────┘\n" << Color::RESET;
    cout << Color::BWHITE << "  Pilih Layanan (1-10): " << Color::RESET;
}

void tampilMenuUser(string username) {
    tampilHeader();
    cout << Color::BBLUE << Color::BOLD;
    cout << "  ┌──────────────────────────────────────────────┐\n";
    cout << "  │  " << Color::BGREEN << "👤 Login sebagai: " << Color::BWHITE << left << setw(20) << username << Color::BBLUE << "  [USER]   │\n";
    cout << "  ├──────────────────────────────────────────────┤\n";
    cout << Color::RESET;
    cout << Color::BBLUE << "  │" << Color::RESET << Color::BOLD << "  MENU USER                                   " << Color::BBLUE << "│\n" << Color::RESET;
    cout << Color::BBLUE << "  ├──────────────────────────────────────────────┤\n" << Color::RESET;

    auto menuItem = [](string num, string ikon, string label) {
        cout << Color::BBLUE << "  │  " << Color::RESET
            << Color::BWHITE << Color::BOLD << "[" << num << "] " << Color::RESET
            << ikon << " " << left << setw(38) << label
            << Color::BBLUE << "│\n" << Color::RESET;
    };

    menuItem("1", "🛎 ", "Reservasi Kamar Baru");
    menuItem("2", "📋", "Lihat Reservasi Saya");
    menuItem("3", "🔎", "Cek Ketersediaan (Cari ID)");
    menuItem("4", "🏷 ", "Filter Kamar (by Tipe)");
    menuItem("5", "🚶", "Keluar (Logout)");

    cout << Color::BBLUE << "  └──────────────────────────────────────────────┘\n" << Color::RESET;
    cout << Color::BWHITE << "  Pilih Layanan (1-5): " << Color::RESET;
}

// ─── Tampil Kamar ─────────────────────────────────────────────────────────────

void tampilKamar(Kamar* k, int* n) {
    cout << "\n";
    cout << Color::BCYAN << Color::BOLD;
    cout << "  ╔══════════════════════════════════════════════════════════╗\n";
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
    cout << "  │  🏷  FILTER TIPE: " << Color::BWHITE << left << setw(21) << filterTipe << Color::BYELLOW << "│\n";
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
        cout << "  " << Color::BRED << Color::BOLD << "[!] Tipe kamar '" << filterTipe << "' tidak ditemukan.\n" << Color::RESET;
    }
}

// ─── CRUD Reservasi ──────────────────────────────────────────────────────────

void create(Kamar* kList, Reservasi* rList, int* nK, int* nR, string usernameAktif) {
    tampilKamar(kList, nK);

    int pilihNo;
    cout << "\n  " << Color::BWHITE << Color::BOLD << "Masukkan Nomor Kamar yang ingin dipesan: " << Color::RESET;
    if (!(cin >> pilihNo)) {
        cin.clear();
        cin.ignore(1000, '\n');
        cout << "\n  " << Color::BRED << Color::BOLD << "[ERROR] Input nomor kamar tidak valid!\n" << Color::RESET;
        return;
    }

    bool ditemukan = false;
    for (int i = 0; i < *nK; i++) {
        if (kList[i].noKamar == pilihNo && kList[i].tersedia == 1) {
            ditemukan = true;
            try {
                cin.ignore(1000, '\n');
                cout << "\n";
                cout << Color::BBLUE << Color::BOLD;
                cout << "  ┌───────────────────────────────────────┐\n";
                cout << "  │         ISI DATA TAMU                 │\n";
                cout << "  └───────────────────────────────────────┘\n";
                cout << Color::RESET;

                // ── Nama ──
                string nama;
                do {
                    cout << "  " << Color::BCYAN << "Nama Tamu        : " << Color::RESET;
                    getline(cin, nama);
                    if (!validasiNama(nama))
                        cout << "  " << Color::BRED << "[!] Nama hanya boleh huruf & spasi, minimal 2 karakter.\n" << Color::RESET;
                } while (!validasiNama(nama));
                rList[*nR].nama = nama;

                // ── NIK / KTP ──
                string nik;
                cout << "  " << Color::DIM << "    (NIK: Nomor Induk Kependudukan, 16 digit angka)\n" << Color::RESET;
                do {
                    cout << "  " << Color::BCYAN << "NIK / No. KTP    : " << Color::RESET;
                    getline(cin, nik);
                    if (!validasiNIK(nik))
                        cout << "  " << Color::BRED << "[!] NIK harus tepat 16 digit angka.\n" << Color::RESET;
                } while (!validasiNIK(nik));
                rList[*nR].data.ktp = nik;

                // ── No HP ──
                string hp;
                do {
                    cout << "  " << Color::BCYAN << "No HP Tamu       : " << Color::RESET;
                    getline(cin, hp);
                    if (!validasiHP(hp))
                        cout << "  " << Color::BRED << "[!] No HP harus diawali 08 dan 10-13 digit.\n" << Color::RESET;
                } while (!validasiHP(hp));
                rList[*nR].data.noHP = hp;

                // ── Tanggal Check-in ──
                string tglCheckin;
                do {
                    cout << "  " << Color::BCYAN << "Tanggal Check-in : " << Color::RESET;
                    cout << Color::DIM << "(format DD-MM-YYYY) " << Color::RESET;
                    getline(cin, tglCheckin);
                    if (!validasiTanggal(tglCheckin))
                        cout << "  " << Color::BRED << "[!] Format tanggal tidak valid. Gunakan DD-MM-YYYY.\n" << Color::RESET;
                } while (!validasiTanggal(tglCheckin));
                rList[*nR].tanggal = tglCheckin;

                // ── Lama Inap ──
                int lama;
                do {
                    cout << "  " << Color::BCYAN << "Lama Inap (hari) : " << Color::RESET;
                    if (!(cin >> lama)) {
                        cin.clear();
                        cin.ignore(1000, '\n');
                        lama = 0;
                        cout << "  " << Color::BRED << "[!] Input harus berupa angka.\n" << Color::RESET;
                        continue;
                    }
                    cin.ignore(1000, '\n');
                    if (lama <= 0)
                        cout << "  " << Color::BRED << "[!] Lama inap harus lebih dari 0.\n" << Color::RESET;
                } while (lama <= 0);
                cekInputPositif(lama);
                rList[*nR].lama = lama;

                rList[*nR].noKamar    = pilihNo;
                rList[*nR].totalHarga = kList[i].harga * lama;
                rList[*nR].pemilik    = usernameAktif;

                // ── Nota ──
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
                cout << Color::BGREEN << "  ║ " << Color::RESET << Color::BWHITE << "Lama Inap    : " << Color::BYELLOW << lama << " hari" << string(20, ' ')             << Color::BGREEN << " ║\n" << Color::RESET;
                cout << Color::BGREEN << "  ║ " << Color::RESET << Color::BWHITE << "Total Biaya  : " << Color::BYELLOW << "Rp " << left << setw(22) << rList[*nR].totalHarga << Color::BGREEN << " ║\n" << Color::RESET;
                cout << Color::BGREEN << Color::BOLD;
                cout << "  ╚══════════════════════════════════════════╝\n";
                cout << Color::RESET;
                cout << "  " << Color::BGREEN << Color::BOLD << "✔  Terima kasih telah memesan di Grand Rizuna!\n" << Color::RESET;

                kList[i].tersedia = 0;
                (*nR)++;
                cout << "\n  " << Color::BGREEN << Color::BOLD << "[BERHASIL] Reservasi Kamar " << pilihNo << " telah diproses.\n" << Color::RESET;
            } catch (const invalid_argument& e) {
                cin.ignore(1000, '\n');
                cout << "\n  " << Color::BRED << Color::BOLD << "[ERROR] " << e.what() << "\n" << Color::RESET;
            }
            return;
        }
    }
    if (!ditemukan) {
        cout << "\n  " << Color::BRED << Color::BOLD << "[GAGAL] Kamar tidak ditemukan atau sudah terisi!\n" << Color::RESET;
    }
}

void read(Reservasi* rList, int* nR) {
    if (*nR == 0) {
        cout << "\n  " << Color::BYELLOW << Color::BOLD << "[INFO] Belum ada data reservasi.\n" << Color::RESET;
        return;
    }
    cout << "\n";
    cout << Color::BCYAN << Color::BOLD;
    cout << "  ╔═══╦═══════════════════╦══════════════════╦═════════╦═════════╦═══════╦═════════════╗\n";
    cout << "  ║ No║ Nama              ║ NIK/KTP          ║ No HP   ║Check-in ║ Kamar ║ Total       ║\n";
    cout << "  ╠═══╬═══════════════════╬══════════════════╬═════════╬═════════╬═══════╬═════════════╣\n";
    cout << Color::RESET;
    for (int i = 0; i < *nR; i++) {
        cout << Color::BCYAN << "  ║ " << Color::RESET
            << Color::BWHITE << setw(2) << left << i+1
            << Color::BCYAN << "║ " << Color::RESET
            << setw(17) << left << rList[i].nama.substr(0, 17)
            << Color::BCYAN << " ║ " << Color::RESET
            << setw(16) << left << rList[i].data.ktp.substr(0, 16)
            << Color::BCYAN << " ║ " << Color::RESET
            << setw(7)  << left << rList[i].data.noHP.substr(0, 7)
            << Color::BCYAN << " ║ " << Color::RESET
            << Color::BWHITE << setw(7) << left << rList[i].tanggal.substr(0, 7)
            << Color::BCYAN << " ║ " << Color::RESET
            << Color::BYELLOW << setw(5)  << left << rList[i].noKamar
            << Color::BCYAN << " ║ " << Color::RESET
            << Color::BGREEN << "Rp " << setw(8) << left << rList[i].totalHarga
            << Color::BCYAN << " ║\n" << Color::RESET;
    }
    cout << Color::BCYAN << Color::BOLD;
    cout << "  ╚═══╩═══════════════════╩══════════════════╩═════════╩═════════╩═══════╩═════════════╝\n";
    cout << Color::RESET;
}

void readUser(Reservasi* rList, int* nR, string usernameAktif) {
    bool ada = false;
    int urut = 1;
    cout << "\n";
    cout << Color::BBLUE << Color::BOLD;
    cout << "  ╔══════════════════════════════════════════════════════════╗\n";
    cout << "  ║          RESERVASI SAYA - " << left << setw(28) << usernameAktif << "   ║\n";
    cout << "  ╠═════════════════════════════════════════════════════════╣\n";
    cout << Color::RESET;
    for (int i = 0; i < *nR; i++) {
        if (rList[i].pemilik == usernameAktif) {
            ada = true;
            cout << Color::BBLUE << "  ║ " << Color::RESET
                << Color::BWHITE << Color::BOLD << "Reservasi #" << urut++ << Color::RESET
                << "                                              "
                << Color::BBLUE << " ║\n" << Color::RESET;
            cout << Color::BBLUE << "  ║  " << Color::RESET
                << "Nama     : " << Color::BWHITE << left << setw(16) << rList[i].nama << Color::RESET
                << "  Kamar: " << Color::BYELLOW << rList[i].noKamar
                << "               " << Color::BBLUE << " ║\n" << Color::RESET;
            cout << Color::BBLUE << "  ║  " << Color::RESET
                << "Check-in : " << Color::BWHITE << left << setw(16) << rList[i].tanggal << Color::RESET
                << "  Lama : " << Color::BWHITE << rList[i].lama << " hari"
                << "        " << Color::BBLUE << " ║\n" << Color::RESET;
            cout << Color::BBLUE << "  ║  " << Color::RESET
                << "NIK/KTP  : " << left << setw(18) << rList[i].data.ktp
                << "  Total: " << Color::BGREEN << "Rp " << rList[i].totalHarga
                << "      " << Color::BBLUE << " ║\n" << Color::RESET;
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
    cout << "\n  " << Color::BWHITE << Color::BOLD << "Pilih nomor urut yang diubah: " << Color::RESET;
    if (!(cin >> idx)) {
        cin.clear();
        cin.ignore(1000, '\n');
        cout << "  " << Color::BRED << "[ERROR] Input tidak valid.\n" << Color::RESET;
        return;
    }
    if (idx > 0 && idx <= *nR) {
        try {
            cin.ignore(1000, '\n');
            // Ubah tanggal check-in
            string tglBaru;
            do {
                cout << "  " << Color::BCYAN << "Tanggal Check-in Baru " << Color::DIM << "(DD-MM-YYYY)" << Color::RESET << ": ";
                getline(cin, tglBaru);
                if (!validasiTanggal(tglBaru))
                    cout << "  " << Color::BRED << "[!] Format tanggal tidak valid.\n" << Color::RESET;
            } while (!validasiTanggal(tglBaru));
            rList[idx-1].tanggal = tglBaru;

            // Ubah lama inap
            int lama;
            do {
                cout << "  " << Color::BCYAN << "Lama Inap Baru (hari): " << Color::RESET;
                if (!(cin >> lama)) {
                    cin.clear();
                    cin.ignore(1000, '\n');
                    lama = 0;
                    cout << "  " << Color::BRED << "[!] Input harus berupa angka.\n" << Color::RESET;
                    continue;
                }
                cin.ignore(1000, '\n');
                if (lama <= 0)
                    cout << "  " << Color::BRED << "[!] Lama inap harus lebih dari 0.\n" << Color::RESET;
            } while (lama <= 0);
            cekInputPositif(lama);
            rList[idx-1].lama = lama;

            for (int i = 0; i < *nK; i++) {
                if (kList[i].noKamar == rList[idx-1].noKamar)
                    rList[idx-1].totalHarga = kList[i].harga * lama;
            }
            cout << "  " << Color::BGREEN << Color::BOLD << "[BERHASIL] Data berhasil diupdate!\n" << Color::RESET;
        } catch (const invalid_argument& e) {
            cout << "  " << Color::BRED << "[ERROR] " << e.what() << "\n" << Color::RESET;
        }
    } else {
        cout << "  " << Color::BRED << "[!] Nomor urut tidak valid.\n" << Color::RESET;
    }
}

void hapus(Reservasi* rList, int* nR, Kamar* kList, int* nK) {
    read(rList, nR);
    if (*nR == 0) return;
    int idx;
    cout << "\n  " << Color::BWHITE << Color::BOLD << "Pilih nomor urut yang dihapus: " << Color::RESET;
    if (!(cin >> idx)) {
        cin.clear();
        cin.ignore(1000, '\n');
        cout << "  " << Color::BRED << "[ERROR] Input tidak valid.\n" << Color::RESET;
        return;
    }
    cin.ignore(1000, '\n');
    if (idx > 0 && idx <= *nR) {
        for (int i = 0; i < *nK; i++) {
            if (kamarList[i].noKamar == rList[idx-1].noKamar)
                kamarList[i].tersedia = 1;
        }
        for (int i = idx - 1; i < *nR - 1; i++) rList[i] = rList[i + 1];
        (*nR)--;
        cout << "  " << Color::BGREEN << Color::BOLD << "[BERHASIL] Data reservasi dihapus / check-out selesai!\n" << Color::RESET;
    } else {
        cout << "  " << Color::BRED << "[!] Nomor urut tidak valid.\n" << Color::RESET;
    }
}

// ─── Sorting & Search ─────────────────────────────────────────────────────────

void sortingMenu(Kamar* k, int n) {
    for (int i = 0; i < n - 1; i++) {
        int m = i;
        for (int j = i + 1; j < n; j++) { if (k[j].tipe > k[m].tipe) m = j; }
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
    if (cin.peek() == '\n') cin.ignore();
    string nama;
    cout << "\n";
    cout << Color::BBLUE << Color::BOLD;
    cout << "  ┌─────────────────────────────────────┐\n";
    cout << "  │     🔍 PENCARIAN DATA TAMU           │\n";
    cout << "  └─────────────────────────────────────┘\n";
    cout << Color::RESET;
    cout << "  " << Color::BCYAN << "Masukkan nama lengkap: " << Color::RESET;
    getline(cin, nama);

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
        cout << Color::BGREEN << "  ║ " << Color::RESET << Color::BWHITE << "Nama      : " << Color::BYELLOW << left << setw(27) << rList[index].nama              << Color::BGREEN << " ║\n" << Color::RESET;
        cout << Color::BGREEN << "  ║ " << Color::RESET << Color::BWHITE << "NIK/KTP   : " << Color::BWHITE  << left << setw(27) << rList[index].data.ktp           << Color::BGREEN << " ║\n" << Color::RESET;
        cout << Color::BGREEN << "  ║ " << Color::RESET << Color::BWHITE << "No. HP    : " << Color::BWHITE  << left << setw(27) << rList[index].data.noHP           << Color::BGREEN << " ║\n" << Color::RESET;
        cout << Color::BGREEN << "  ║ " << Color::RESET << Color::BWHITE << "Check-in  : " << Color::BYELLOW << left << setw(27) << rList[index].tanggal             << Color::BGREEN << " ║\n" << Color::RESET;
        cout << Color::BGREEN << "  ║ " << Color::RESET << Color::BWHITE << "No. Kamar : " << Color::BYELLOW << left << setw(27) << rList[index].noKamar             << Color::BGREEN << " ║\n" << Color::RESET;
        cout << Color::BGREEN << "  ║ " << Color::RESET << Color::BWHITE << "Total     : " << Color::BGREEN  << "Rp " << left << setw(24) << rList[index].totalHarga << Color::BGREEN << " ║\n" << Color::RESET;
        cout << Color::BGREEN << Color::BOLD << "  ╚══════════════════════════════════════════╝\n" << Color::RESET;
    } else {
        cout << "\n  " << Color::BRED << Color::BOLD << "[!] Nama '" << nama << "' tidak ditemukan.\n" << Color::RESET;
        cout << "  " << Color::BYELLOW << "[i] Pastikan penulisan nama sudah benar.\n" << Color::RESET;
    }
}

void cariKamar(Kamar* kList, int* nK) {
    int no;
    cout << "\n";
    cout << Color::BBLUE << Color::BOLD;
    cout << "  ┌─────────────────────────────────────┐\n";
    cout << "  │     🔎 CEK KETERSEDIAAN KAMAR        │\n";
    cout << "  └─────────────────────────────────────┘\n";
    cout << Color::RESET;
    cout << "  " << Color::BCYAN << "Nomor kamar yang dicari: " << Color::RESET;
    if (!(cin >> no)) {
        cin.clear();
        cin.ignore(1000, '\n');
        cout << "\n  " << Color::BRED << Color::BOLD << "[ERROR] Input nomor kamar tidak valid!\n" << Color::RESET;
        return;
    }
    cin.ignore(1000, '\n');

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
        cout << Color::BCYAN << "  ║ " << Color::RESET << Color::BWHITE << "Nomor Kamar : " << Color::BYELLOW << left << setw(25) << kList[hasil].noKamar << Color::BCYAN << " ║\n" << Color::RESET;
        cout << Color::BCYAN << "  ║ " << Color::RESET << Color::BWHITE << "Tipe Kamar  : " << Color::BWHITE  << left << setw(25) << kList[hasil].tipe     << Color::BCYAN << " ║\n" << Color::RESET;
        cout << Color::BCYAN << "  ║ " << Color::RESET << Color::BWHITE << "Harga/Malam : " << Color::BYELLOW << "Rp " << left << setw(22) << kList[hasil].harga << Color::BCYAN << " ║\n" << Color::RESET;
        cout << Color::BCYAN << "  ║ " << Color::RESET << Color::BWHITE << "Status      : " << statusColor << Color::BOLD << left << setw(25) << statusStr   << Color::BCYAN << " ║\n" << Color::RESET;
        cout << Color::BCYAN << Color::BOLD << "  ╚══════════════════════════════════════════╝\n" << Color::RESET;
    } else {
        cout << "\n  " << Color::BRED << Color::BOLD << "[!] Nomor kamar '" << no << "' tidak terdaftar.\n" << Color::RESET;
        cout << "  " << Color::BYELLOW << "[i] Silakan cek daftar kamar pada menu utama.\n" << Color::RESET;
    }
}

// ─── Registrasi & Login ───────────────────────────────────────────────────────

// Registrasi USER BIASA — dari menu awal (tidak bisa daftar admin)
void registrasi() {
    if (jumlahUser >= 10) {
        cout << "\n  " << Color::BRED << "[!] Kapasitas akun penuh.\n" << Color::RESET;
        return;
    }
    cout << "\n";
    cout << Color::BMAGENTA << Color::BOLD;
    cout << "  ┌─────────────────────────────────────┐\n";
    cout << "  │        DAFTAR AKUN BARU (USER)       │\n";
    cout << "  └─────────────────────────────────────┘\n";
    cout << Color::RESET;

    string uname, pass;
    // Username
    do {
        cout << "  " << Color::BCYAN << "Username (4-20 karakter, a-z/0-9/_): " << Color::RESET;
        cin >> uname;
        if (!validasiUsername(uname))
            cout << "  " << Color::BRED << "[!] Username tidak valid.\n" << Color::RESET;
        else {
            // Cek duplikat
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

    // Password
    do {
        cout << "  " << Color::BCYAN << "Password (min. 6 karakter)          : " << Color::RESET;
        cin >> pass;
        if (!validasiPassword(pass))
            cout << "  " << Color::BRED << "[!] Password minimal 6 karakter.\n" << Color::RESET;
    } while (!validasiPassword(pass));

    akun[jumlahUser].username = uname;
    akun[jumlahUser].nim      = pass;
    akun[jumlahUser].role     = 0;  // selalu user biasa
    jumlahUser++;
    cout << "  " << Color::BGREEN << Color::BOLD << "[BERHASIL] Akun user berhasil dibuat!\n" << Color::RESET;
    cin.ignore(1000, '\n');
}

// Registrasi ADMIN — hanya dipanggil dari menu admin
void registrasiAdmin() {
    if (jumlahUser >= 10) {
        cout << "\n  " << Color::BRED << "[!] Kapasitas akun penuh.\n" << Color::RESET;
        return;
    }
    cout << "\n";
    cout << Color::BYELLOW << Color::BOLD;
    cout << "  ┌─────────────────────────────────────┐\n";
    cout << "  │        TAMBAH AKUN ADMIN BARU        │\n";
    cout << "  └─────────────────────────────────────┘\n";
    cout << Color::RESET;

    string uname, pass;
    do {
        cout << "  " << Color::BCYAN << "Username (4-20 karakter, a-z/0-9/_): " << Color::RESET;
        cin >> uname;
        if (!validasiUsername(uname))
            cout << "  " << Color::BRED << "[!] Username tidak valid.\n" << Color::RESET;
        else {
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
        cout << "  " << Color::BCYAN << "Password (min. 6 karakter)          : " << Color::RESET;
        cin >> pass;
        if (!validasiPassword(pass))
            cout << "  " << Color::BRED << "[!] Password minimal 6 karakter.\n" << Color::RESET;
    } while (!validasiPassword(pass));

    akun[jumlahUser].username = uname;
    akun[jumlahUser].nim      = pass;
    akun[jumlahUser].role     = 1;  // admin
    jumlahUser++;
    cout << "  " << Color::BGREEN << Color::BOLD << "[BERHASIL] Akun admin '" << uname << "' berhasil ditambahkan!\n" << Color::RESET;
    cin.ignore(1000, '\n');
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
    cout << "  │          LOGIN KE SISTEM             │\n";
    cout << "  └─────────────────────────────────────┘\n";
    cout << Color::RESET;
    cout << "  " << Color::BCYAN << "Username : " << Color::RESET; cin >> u;
    cout << "  " << Color::BCYAN << "Password : " << Color::RESET; cin >> p;
    cin.ignore(1000, '\n');
    for (int i = 0; i < jumlahUser; i++) {
        if (akun[i].username == u && akun[i].nim == p) {
            usernameOut = u;
            roleOut     = akun[i].role;
            return 1;
        }
    }
    return 0;
}

// ─── Main ─────────────────────────────────────────────────────────────────────

int main() {
    enableANSI();

    // Akun default
    akun[0] = {"admin", "admin123", 1};
    akun[1] = {"user",  "user123",  0};
    jumlahUser = 2;

    int pilihAwal;
    bool programJalan = true;

    while (programJalan) {
        string usernameAktif = "";
        int roleAktif  = 0;
        int statusLogin = 0;
        int percobaan  = 0;

        while (statusLogin == 0) {
            tampilWelcome();
            cout << Color::BWHITE << Color::BOLD;
            cout << "  ┌─────────────────────────────────────────────────────┐\n";
            cout << "  │  Percobaan Login : [" << percobaan+1 << "/3]                          │\n";
            cout << "  ├─────────────────────────────────────────────────────┤\n";
            cout << "  │  [1] Login ke Sistem                                │\n";
            cout << "  │  [2] Registrasi Akun User Baru                      │\n";
            cout << "  │  [3] Keluar dari Aplikasi                           │\n";
            cout << "  └─────────────────────────────────────────────────────┘\n";
            cout << Color::RESET;
            cout << "  " << Color::BWHITE << Color::BOLD << "Pilih Opsi (1-3): " << Color::RESET;

            if (!(cin >> pilihAwal)) {
                cin.clear();
                cin.ignore(1000, '\n');
                cout << "\n  " << Color::BRED << "[!] Input tidak valid, masukkan angka 1-3.\n" << Color::RESET;
                continue;
            }
            cin.ignore(1000, '\n');

            if (pilihAwal == 1) {
                if (login(usernameAktif, roleAktif) == 1) {
                    statusLogin = 1;
                    percobaan = 0;
                    cout << "\n  " << Color::BGREEN << Color::BOLD << "✔  Login berhasil! Selamat datang, " << usernameAktif << "!\n" << Color::RESET;
                } else {
                    percobaan++;
                    cout << "\n  " << Color::BRED << Color::BOLD << "[!] Gagal! Username/Password salah. (" << percobaan << "/3)\n" << Color::RESET;
                    if (percobaan >= 3) {
                        cout << "  " << Color::BRED << Color::BOLD << "[!] Akun terkunci karena salah 3x. Program berhenti.\n" << Color::RESET;
                        return 0;
                    }
                }
            } else if (pilihAwal == 2) {
                registrasi();  // hanya buat akun user biasa
            } else if (pilihAwal == 3) {
                programJalan = false;
                cout << "\n  " << Color::BMAGENTA << Color::BOLD << "Terima kasih telah menggunakan HMS Grand Rizuna Estate. Sampai jumpa!\n" << Color::RESET;
                break;
            } else {
                cout << "\n  " << Color::BRED << "[!] Pilihan tidak valid.\n" << Color::RESET;
            }
        }

        if (!programJalan) break;

        // ── Menu Admin ──
        if (roleAktif == 1) {
            while (statusLogin == 1) {
                int pilih;
                tampilMenuAdmin(usernameAktif);
                if (!(cin >> pilih)) {
                    cin.clear();
                    cin.ignore(1000, '\n');
                    cout << "\n  " << Color::BRED << "[!] Input tidak valid.\n" << Color::RESET;
                    continue;
                }
                cin.ignore(1000, '\n');
                switch (pilih) {
                    case 1: create(kamarList, dataReservasi, &jumlahKamar, &jumlahReservasi, usernameAktif); break;
                    case 2: read(dataReservasi, &jumlahReservasi); break;
                    case 3: update(dataReservasi, &jumlahReservasi, kamarList, &jumlahKamar); break;
                    case 4: hapus(dataReservasi, &jumlahReservasi, kamarList, &jumlahKamar); break;
                    case 5: {
                        int s;
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
                        if (!(cin >> s)) {
                            cin.clear();
                            cin.ignore(1000, '\n');
                            cout << "  " << Color::BRED << "[!] Input tidak valid.\n" << Color::RESET;
                            break;
                        }
                        cin.ignore(1000, '\n');
                        if      (s == 1) { sortingMenu(kamarList, jumlahKamar);        cout << "  " << Color::BGREEN << "[INFO] Diurutkan berdasarkan Tipe.\n"   << Color::RESET; }
                        else if (s == 2) { sortKamarByNumber(kamarList, jumlahKamar);  cout << "  " << Color::BGREEN << "[INFO] Diurutkan berdasarkan Nomor.\n"  << Color::RESET; }
                        else if (s == 3) { insertionSortHarga(kamarList, jumlahKamar); cout << "  " << Color::BGREEN << "[INFO] Diurutkan berdasarkan Harga.\n"  << Color::RESET; }
                        else             { cout << "  " << Color::BRED << "[!] Pilihan tidak valid.\n" << Color::RESET; }
                        tampilKamar(kamarList, &jumlahKamar);
                        break;
                    }
                    case 6:  cariNama(dataReservasi, &jumlahReservasi); break;
                    case 7:  cariKamar(kamarList, &jumlahKamar); break;
                    case 8: {
                        string t;
                        cout << "  " << Color::BCYAN << "Masukkan tipe kamar: " << Color::RESET;
                        getline(cin, t);
                        tampilKamar(kamarList, jumlahKamar, t);
                        break;
                    }
                    case 9:  registrasiAdmin(); break;
                    case 10:
                        statusLogin = 0;
                        cout << "\n  " << Color::BYELLOW << Color::BOLD << "[INFO] Logout berhasil. Sampai jumpa, " << usernameAktif << "!\n" << Color::RESET;
                        break;
                    default:
                        cout << "\n  " << Color::BRED << "[!] Pilihan tidak tersedia.\n" << Color::RESET;
                }
            }
        }

        // ── Menu User ──
        else {
            while (statusLogin == 1) {
                int pilih;
                tampilMenuUser(usernameAktif);
                if (!(cin >> pilih)) {
                    cin.clear();
                    cin.ignore(1000, '\n');
                    cout << "\n  " << Color::BRED << "[!] Input tidak valid.\n" << Color::RESET;
                    continue;
                }
                cin.ignore(1000, '\n');
                switch (pilih) {
                    case 1: create(kamarList, dataReservasi, &jumlahKamar, &jumlahReservasi, usernameAktif); break;
                    case 2: readUser(dataReservasi, &jumlahReservasi, usernameAktif); break;
                    case 3: cariKamar(kamarList, &jumlahKamar); break;
                    case 4: {
                        string t;
                        cout << "  " << Color::BCYAN << "Masukkan tipe kamar: " << Color::RESET;
                        getline(cin, t);
                        tampilKamar(kamarList, jumlahKamar, t);
                        break;
                    }
                    case 5:
                        statusLogin = 0;
                        cout << "\n  " << Color::BYELLOW << Color::BOLD << "[INFO] Logout berhasil. Sampai jumpa, " << usernameAktif << "!\n" << Color::RESET;
                        break;
                    default:
                        cout << "\n  " << Color::BRED << "[!] Pilihan tidak tersedia.\n" << Color::RESET;
                }
            }
        }
    }

    return 0;
}