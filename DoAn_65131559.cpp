#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include <conio.h>
#include <windows.h>

// --- DINH NGHIA MAU SAC ---
#define M_XANHLA    10
#define M_XANHDUONG 11 // Cyan
#define M_DO        12
#define M_VANG      14
#define M_TRANG     15
#define M_XAM       8

// Do rong man hinh console gia dinh de can giua
#define CONSOLE_WIDTH 100 

// --- 1. CAU TRUC DU LIEU ---
typedef struct {
    int maNV;
    char taiKhoan[50];
    char matKhau[50];
    char hoTen[100];
    char chucVu[20];
} NhanVien;

typedef struct {
    int maSP;
    char tenMon[100];
    long giaTien;
    int trangThai;
    int maDM;
} SanPham;

typedef struct {
    int maDM;
    char tenDM[100];
} DanhMuc;

typedef struct {
    int maDon;
    char ngayTao[30];
    long tongTien;
    int trangThai;
    int maNV;
} DonHang;

typedef struct {
    int maSP;
    char tenMon[100];
    int tongSoLuongBan;
} ThongKeSP;

typedef struct {
    int maSP;
    char tenMon[100];
    int soLuong;
    long giaTien;
    char ghiChu[100];
} ChiTietTam;

// --- 2. HAM TIEN ICH GIAO DIEN & MAU SAC ---

void setMau(int colorCode) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, colorCode);
}

void inGiua(const char* str, int color) {
    int len = strlen(str);
    int padding = (CONSOLE_WIDTH - len) / 2;
    if(padding < 0) padding = 0;
    for(int i = 0; i < padding; i++) printf(" ");
    setMau(color);
    printf("%s\n", str);
    setMau(M_TRANG);
}

void inDongKe(int doDai, int color) {
    int padding = (CONSOLE_WIDTH - doDai) / 2;
    if(padding < 0) padding = 0;
    for(int i = 0; i < padding; i++) printf(" ");
    setMau(color);
    for(int i = 0; i < doDai; i++) printf("=");
    printf("\n");
    setMau(M_TRANG);
}

// [DA SUA] Logo moi, can doi hon
void hienThiLogo() {
    printf("\n");
    setMau(M_XANHLA);
    
    // Them 'const' vao truoc char* de sua loi
    const char* logo[] = {
        "  _   _    ___    ____       _    ",
        " | \\ | |  / _ \\  |  _ \\     / \\   ",
        " |  \\| | | | | | | |_) |   / _ \\  ",
        " | . ` | | |_| | |  _ <   / ___ \\ ",
        " |_| \\_|  \\___/  |_| \\_\\ /_/   \\_\\"
    };
    
    for(int i=0; i<5; i++) {
        int padding = (CONSOLE_WIDTH - strlen(logo[i])) / 2;
        for(int s=0; s<padding; s++) printf(" ");
        printf("%s\n", logo[i]);
    }
    
    printf("\n");
    setMau(M_TRANG);
}
// --- CAC HAM XU LY DU LIEU CO BAN ---

void nhapMatKhau(char *matKhau) {
    char ch; int i = 0;
    while (1) {
        ch = _getch();
        if (ch == 13) { matKhau[i] = '\0'; break; }
        else if (ch == 8) { if (i > 0) { i--; printf("\b \b"); } }
        else { matKhau[i] = ch; i++; printf("*"); }
    }
}

void layThoiGian(char *buffer) {
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    sprintf(buffer, "%02d/%02d/%04d %02d:%02d:%02d", tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900, tm.tm_hour, tm.tm_min, tm.tm_sec);
}

void xoaXuongDong(char *str) {
    size_t len = strlen(str);
    if (len > 0 && str[len - 1] == '\n') str[len - 1] = '\0';
}

void toLowerCase(char *str) {
    for (int i = 0; str[i]; i++) str[i] = tolower(str[i]);
}

void damBaoXuongDong(const char *tenFile) {
    FILE *f = fopen(tenFile, "rb"); if (!f) return; 
    fseek(f, 0, SEEK_END); long size = ftell(f);
    if (size > 0) {
        fseek(f, -1, SEEK_END); char c = fgetc(f); fclose(f); 
        if (c != '\n') { FILE *fa = fopen(tenFile, "a"); if (fa) fprintf(fa, "\n"); if(fa) fclose(fa); }
    } else { fclose(f); }
}

int layIdChiTietMoi() {
    FILE *f = fopen("ChiTietDonHang.txt", "r"); if (!f) return 1; 
    int maxID = 0; char line[256];
    while (fgets(line, sizeof(line), f)) {
        char *token = strtok(line, "|");
        if (token) { int cur = atoi(token); if (cur > maxID) maxID = cur; }
    }
    fclose(f); return maxID + 1;
}

void tachNgayThang(char *dateStr, int *d, int *m, int *y) {
    sscanf(dateStr, "%d/%d/%d", d, m, y);
}

int KiemTraNgayHopLe(int d, int m, int y) {
    if (y < 2000 || y > 3000) return 0;
    if (m < 1 || m > 12) return 0;
    int ngayTrongThang[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if ((y % 4 == 0 && y % 100 != 0) || (y % 400 == 0)) ngayTrongThang[2] = 29;
    if (d < 1 || d > ngayTrongThang[m]) return 0;
    return 1;
}

int nhapSoNguyen(const char *msg, int min, int max) {
    int value, status;
    int padding = (CONSOLE_WIDTH - 60) / 2;
    while (1) {
        for(int i=0; i<padding; i++) printf(" ");
        setMau(M_XANHLA); printf("%s", msg); setMau(M_TRANG);
        status = scanf("%d", &value);
        if (status != 1) {
            for(int i=0; i<padding; i++) printf(" ");
            setMau(M_DO); printf(">> Loi: Vui long chi nhap so!\n"); setMau(M_TRANG);
            while(getchar() != '\n'); 
        } else if (value < min || value > max) {
            for(int i=0; i<padding; i++) printf(" ");
            setMau(M_DO); printf(">> Loi: Gia tri phai tu %d - %d!\n", min, max); setMau(M_TRANG);
        } else {
            char c = getchar(); if(c != '\n') while(getchar() != '\n'); 
            return value;
        }
    }
}

// --- 3. CAC CHUC NANG HE THONG ---

void xemMenuPhanLoai() {
    FILE *fDM = fopen("DanhMuc.txt", "r");
    if (!fDM) { setMau(M_DO); inGiua("Loi: Khong tim thay file DanhMuc.txt", M_DO); system("pause"); return; }
    
    inGiua("--- DANH MUC DO UONG ---", M_VANG);
    char line[256];
    while (fgets(line, sizeof(line), fDM)) {
        int id; char ten[100];
        if (sscanf(line, "%d|%[^\n]", &id, ten) == 2) {
            xoaXuongDong(ten); char buf[100]; sprintf(buf, "[%d]. %s", id, ten); inGiua(buf, M_TRANG);
        }
    }
    fclose(fDM);
    
    int chonDM = nhapSoNguyen(">> Chon so danh muc (0 de quay lai): ", 0, 100);
    if (chonDM == 0) return;
    FILE *fSP = fopen("SanPham.txt", "r"); if (!fSP) return;
    
    system("cls"); hienThiLogo();
    char tieuDe[50]; sprintf(tieuDe, "--- MENU (DM: %d) ---", chonDM); inGiua(tieuDe, M_VANG);

    int tableWidth = 66; int pad = (CONSOLE_WIDTH - tableWidth) / 2;
    for(int i=0; i<pad; i++) printf(" "); setMau(M_XANHDUONG); printf("+-----+------------------------------+---------------+------------+\n");
    for(int i=0; i<pad; i++) printf(" "); setMau(M_VANG);      printf("|  ID | Ten Mon                      | Gia           | Trang Thai |\n");
    for(int i=0; i<pad; i++) printf(" "); setMau(M_XANHDUONG); printf("+-----+------------------------------+---------------+------------+\n"); setMau(M_TRANG);

    while (fgets(line, sizeof(line), fSP)) {
        SanPham sp;
        if (sscanf(line, "%d|%[^|]|%ld|%d|%d", &sp.maSP, sp.tenMon, &sp.giaTien, &sp.trangThai, &sp.maDM) == 5) {
            if (sp.maDM == chonDM) {
                for(int i=0; i<pad; i++) printf(" ");
                printf("| %-3d | %-28s | %-13ld | ", sp.maSP, sp.tenMon, sp.giaTien);
                if(sp.trangThai) { setMau(M_XANHLA); printf("%-10s", "Con"); } else { setMau(M_DO); printf("%-10s", "HET"); }
                setMau(M_TRANG); printf(" |\n");
            }
        }
    }
    for(int i=0; i<pad; i++) printf(" "); setMau(M_XANHDUONG); printf("+-----+------------------------------+---------------+------------+\n"); setMau(M_TRANG);
    fclose(fSP); printf("\n"); system("pause");
}

SanPham timVaChonMon() {
    SanPham spRong; spRong.maSP = 0;
    char tuKhoa[50];
    int pad = (CONSOLE_WIDTH - 60) / 2;
    
    SanPham ketQua[50]; 
    int count = 0;

    // --- [SUA] VONG LAP DE YEU CAU NHAP LAI NEU KHONG TIM THAY ---
    while (1) {
        count = 0; // Reset so luong tim thay moi lan lap
        
        // Xoa dong thong bao cu hoac dong nhap cu (Meo hien thi)
        printf("\r"); for(int i=0; i<CONSOLE_WIDTH; i++) printf(" "); printf("\r");
        
        for(int i=0; i<pad; i++) printf(" "); 
        printf("Nhap ten mon can tim (0 de huy): ");
        
        // Xoa bo dem truoc khi nhap de tranh troi lenh
        fflush(stdin); 
        scanf("%[^\n]", tuKhoa); 
        
        // Neu nguoi dung nhap 0 thi huy bo
        if (strcmp(tuKhoa, "0") == 0) return spRong;

        char tuKhoaGoc[50]; strcpy(tuKhoaGoc, tuKhoa); toLowerCase(tuKhoa);
        
        FILE *fSP = fopen("SanPham.txt", "r"); 
        if (!fSP) return spRong;

        char line[256];
        while (fgets(line, sizeof(line), fSP)) {
            SanPham sp;
            if (sscanf(line, "%d|%[^|]|%ld|%d|%d", &sp.maSP, sp.tenMon, &sp.giaTien, &sp.trangThai, &sp.maDM) == 5) {
                char tenLower[100]; strcpy(tenLower, sp.tenMon); toLowerCase(tenLower);
                if (strstr(tenLower, tuKhoa) != NULL) {
                    ketQua[count] = sp; count++;
                }
            }
        }
        fclose(fSP);

        if (count > 0) {
            break; // Tim thay it nhat 1 mon -> Thoat vong lap nhap de xuong phan chon
        } else {
            // --- [SUA] HIEN THI THONG BAO VA CHO 1.5 GIAY ---
            printf("\n");
            inGiua("Khong tim thay ten mon ma ban tim kiem, vui long nhap lai!", M_DO);
            Sleep(1500); // Dung 1.5 giay
            
            // Xoa dong thong bao loi de man hinh gon gang hon khi nhap lai
            printf("\r"); for(int i=0; i<CONSOLE_WIDTH; i++) printf(" "); printf("\r"); // Xoa dong hien tai
            printf("\x1b[A"); // Len 1 dong (ANSI Code) - Tren mot so console cu co the khong hoat dong nhung khong gay loi
            printf("\r"); for(int i=0; i<CONSOLE_WIDTH; i++) printf(" "); printf("\r"); // Xoa dong tren
            // -----------------------------------------------
        }
    }
    // -------------------------------------------------------------

    inGiua("--- KET QUA TIM KIEM ---", M_VANG);
    int tableWidth = 65; int tPad = (CONSOLE_WIDTH - tableWidth) / 2;
    for(int i=0; i<tPad; i++) printf(" "); setMau(M_XANHDUONG); printf("-----------------------------------------------------------------\n");
    for(int i=0; i<tPad; i++) printf(" "); setMau(M_VANG); printf("%-4s %-30s %-15s %s\n", "STT", "Ten Mon", "Gia", "Trang Thai");
    for(int i=0; i<tPad; i++) printf(" "); setMau(M_XANHDUONG); printf("-----------------------------------------------------------------\n"); setMau(M_TRANG);

    for (int i = 0; i < count; i++) {
        for(int k=0; k<tPad; k++) printf(" ");
        printf("%-4d %-30s %-15ld %s\n", i+1, ketQua[i].tenMon, ketQua[i].giaTien, (ketQua[i].trangThai ? "Con" : "HET"));
    }

    int stt = nhapSoNguyen(">> Chon STT mon (0 de huy): ", 0, count);
    if (stt == 0) return spRong; 
    return ketQua[stt - 1];
}

void taoDonHang(NhanVien nv) {
    ChiTietTam gioHang[100]; int soLuongMonTrongGio = 0; long tongTienTamTinh = 0;
    int maDonMoi = 0; FILE *fDH = fopen("DonHang.txt", "r"); char line[256]; int maxID = 5000;
    if (fDH) { while (fgets(line, sizeof(line), fDH)) { int tempID; char *token = strtok(line, "|"); if(token) { tempID = atoi(token); if (tempID > maxID) maxID = tempID; } } fclose(fDH); }
    maDonMoi = maxID + 1;

    while (1) {
        system("cls"); hienThiLogo();
        inGiua("=== TAO DON HANG MOI ===", M_XANHDUONG);
        char maDonStr[50]; sprintf(maDonStr, "Ma Don: %d", maDonMoi); inGiua(maDonStr, M_VANG);
        
        int tableWidth = 88; int tPad = (CONSOLE_WIDTH - tableWidth) / 2;
        printf("\n");
        for(int i=0; i<tPad; i++) printf(" "); setMau(M_XANHLA); printf("+----+------------------------------+-----+---------------+----------------------+\n");
        for(int i=0; i<tPad; i++) printf(" "); setMau(M_VANG);   printf("|STT | Ten Mon                      | SL  | Thanh Tien    | Ghi Chu              |\n");
        for(int i=0; i<tPad; i++) printf(" "); setMau(M_XANHLA); printf("+----+------------------------------+-----+---------------+----------------------+\n"); setMau(M_TRANG);
        
        if (soLuongMonTrongGio == 0) { 
            for(int i=0; i<tPad; i++) printf(" "); 
            printf("|                  (Gio hang trong)                                              |\n"); 
        } 
        else {
            for(int i = 0; i < soLuongMonTrongGio; i++) {
                for(int k=0; k<tPad; k++) printf(" ");
                char ghiChuHienThi[30];
                if(strcmp(gioHang[i].ghiChu, "None") == 0) strcpy(ghiChuHienThi, ""); 
                else strcpy(ghiChuHienThi, gioHang[i].ghiChu);

                printf("| %-2d | %-28s | %-3d | %-13ld | %-20.20s |\n", 
                    i+1, gioHang[i].tenMon, gioHang[i].soLuong, gioHang[i].giaTien * gioHang[i].soLuong, ghiChuHienThi);
            }
        }
        for(int i=0; i<tPad; i++) printf(" "); setMau(M_XANHLA); printf("+----+------------------------------+-----+---------------+----------------------+\n");
        
        char tongTienStr[100]; sprintf(tongTienStr, "TONG TIEN: %ld VND", tongTienTamTinh); inGiua(tongTienStr, M_VANG); printf("\n");

        inGiua("[1]. Them mon    [2]. Xoa mon    [3]. THANH TOAN    [0]. Huy don", M_TRANG);
        int chon = nhapSoNguyen(">> Chon thao tac: ", 0, 3);
        
        if (chon == 0) { inGiua(">> Da huy don hang.", M_DO); system("pause"); return; }
        
        // --- CHUC NANG THANH TOAN ---
        if (chon == 3) { 
            if (soLuongMonTrongGio == 0) { inGiua("Gio hang dang trong!", M_DO); system("pause"); continue; }
            char timeNow[30]; layThoiGian(timeNow);
            damBaoXuongDong("DonHang.txt"); fDH = fopen("DonHang.txt", "a"); if (fDH) { fprintf(fDH, "%d|%s|%ld|1|%d\n", maDonMoi, timeNow, tongTienTamTinh, nv.maNV); fclose(fDH); }
            damBaoXuongDong("ChiTietDonHang.txt"); FILE *fCT = fopen("ChiTietDonHang.txt", "a");
            if (fCT) { int currentDetailID = layIdChiTietMoi(); for(int i = 0; i < soLuongMonTrongGio; i++) { fprintf(fCT, "%d|%d|%d|%d|%ld|%s\n", currentDetailID + i, maDonMoi, gioHang[i].maSP, gioHang[i].soLuong, gioHang[i].giaTien, gioHang[i].ghiChu); } fclose(fCT); }
            inGiua("THANH TOAN THANH CONG!", M_XANHLA); system("pause"); break; 
        }

        // --- CHUC NANG XOA MON ---
        if (chon == 2) {
             if (soLuongMonTrongGio == 0) { inGiua("Gio hang trong, khong co gi de xoa!", M_DO); system("pause"); continue; }
             while (1) { 
                 int sttXoa = nhapSoNguyen(">> Nhap STT muon xoa (0 de thoat): ", 0, soLuongMonTrongGio);
                 if (sttXoa == 0) break; 
                 int idx = sttXoa - 1;
                 printf("\n");
                 inGiua("Ban co chac chan xoa mon nay ra khoi don hang hien tai khong?", M_VANG);
                 inGiua("[ 1 ] Dong y xoa       [ 0 ] Khong (Chon lai)", M_TRANG);
                 int xacNhan = nhapSoNguyen(">> Xac nhan: ", 0, 1);
                 if (xacNhan == 1) {
                     tongTienTamTinh -= (gioHang[idx].giaTien * gioHang[idx].soLuong);
                     for(int k=idx; k<soLuongMonTrongGio-1; k++) gioHang[k] = gioHang[k+1];
                     soLuongMonTrongGio--;
                     inGiua(">> Da xoa thanh cong!", M_XANHLA);
                     Sleep(800); 
                     break; 
                 } else { inGiua(">> Da huy xoa. Vui long chon lai.", M_XANHLA); continue; }
             }
        }

        // --- CHUC NANG THEM MON ---
        if (chon == 1) {
            SanPham sp = timVaChonMon(); if (sp.maSP == 0) continue;
            if (sp.trangThai == 0) { inGiua("San pham HET HANG!", M_DO); system("pause"); continue; }
            int sl = nhapSoNguyen(">> Nhap so luong: ", 1, 100);
            
            char ghiChu[100]; 
            int tPad = (CONSOLE_WIDTH - 60) / 2;
            for(int i=0; i<tPad; i++) printf(" "); 
            setMau(M_XANHLA); printf(">> Ghi chu (Enter neu khong): "); setMau(M_TRANG);  
            fgets(ghiChu, sizeof(ghiChu), stdin); xoaXuongDong(ghiChu); if (strlen(ghiChu) == 0) strcpy(ghiChu, "None");

            int foundIdx = -1; for(int i=0; i<soLuongMonTrongGio; i++) { if(gioHang[i].maSP == sp.maSP && strcmp(gioHang[i].ghiChu, ghiChu) == 0) { foundIdx = i; break; } }
            if(foundIdx != -1) { gioHang[foundIdx].soLuong += sl; } else { if(soLuongMonTrongGio >= 100) { inGiua("Gio hang day!", M_DO); continue; } gioHang[soLuongMonTrongGio].maSP = sp.maSP; strcpy(gioHang[soLuongMonTrongGio].tenMon, sp.tenMon); gioHang[soLuongMonTrongGio].giaTien = sp.giaTien; gioHang[soLuongMonTrongGio].soLuong = sl; strcpy(gioHang[soLuongMonTrongGio].ghiChu, ghiChu); soLuongMonTrongGio++; }
            tongTienTamTinh += (sp.giaTien * sl);

            // --- [MOI THEM] HIEN THI THONG BAO DA THEM ---
            printf("\n");
            char msg[100];
            // Luu y: Viet khong dau de tranh loi font tren console C co ban
            sprintf(msg, ">> Da them: %s (x%d) vao hoa don!", sp.tenMon, sl);
            inGiua(msg, M_XANHLA); // Hien thi mau xanh la cay
            Sleep(1000); // Dung 1 giay de nguoi dung kip doc
            // ---------------------------------------------
        }
    }
}

// --- 4. CAC HAM BAO CAO ---

void baoCaoDoanhThuCoBan() {
    FILE *f = fopen("DonHang.txt", "r"); if (!f) { inGiua("Loi file!", M_DO); return; }
    system("cls"); inGiua("--- BAO CAO DOANH THU ---", M_VANG);
    inGiua("1. Theo NGAY", M_TRANG); inGiua("2. Theo NHAN VIEN", M_TRANG); inGiua("3. TAT CA", M_TRANG); inGiua("0. Quay lai", M_TRANG);
    int loaiBaoCao = nhapSoNguyen(">> Chon: ", 0, 3);
    if (loaiBaoCao == 0) { fclose(f); return; }
    char ngayCanTim[30] = ""; int maNVCanTim = 0;
    if (loaiBaoCao == 1) {
        int d, m, y, hopLe = 0;
        do { int tPad = (CONSOLE_WIDTH - 40) / 2; for(int i=0; i<tPad; i++) printf(" "); printf("Nhap ngay (dd/mm/yyyy): "); if (scanf("%d/%d/%d", &d, &m, &y) == 3) { if (KiemTraNgayHopLe(d, m, y)) { hopLe = 1; while(getchar() != '\n'); } } else while(getchar() != '\n'); } while (!hopLe);
        sprintf(ngayCanTim, "%02d/%02d/%d", d, m, y);
    } else if (loaiBaoCao == 2) { maNVCanTim = nhapSoNguyen("Nhap Ma NV: ", 1, 9999); }

    int tableWidth = 72; int pad = (CONSOLE_WIDTH - tableWidth) / 2; printf("\n");
    for(int i=0; i<pad; i++) printf(" "); setMau(M_XANHDUONG); printf("+------------+-------------------------+------------+-----------------+\n");
    for(int i=0; i<pad; i++) printf(" "); setMau(M_VANG);      printf("| %-10s | %-23s | %-10s | %-15s |\n", "Ma Don", "Thoi Gian", "NV Ban", "Tong Tien");
    for(int i=0; i<pad; i++) printf(" "); setMau(M_XANHDUONG); printf("+------------+-------------------------+------------+-----------------+\n"); setMau(M_TRANG);

    char line[256]; long tongDoanhThu = 0; int foundCount = 0;
    while (fgets(line, sizeof(line), f)) {
        DonHang dh;
        if (sscanf(line, "%d|%[^|]|%ld|%d|%d", &dh.maDon, dh.ngayTao, &dh.tongTien, &dh.trangThai, &dh.maNV) == 5) {
            if (dh.trangThai == 1) {
                int thoaManDK = 0;
                if (loaiBaoCao == 3) thoaManDK = 1; 
                else if (loaiBaoCao == 1 && strstr(dh.ngayTao, ngayCanTim) != NULL) thoaManDK = 1;
                else if (loaiBaoCao == 2 && dh.maNV == maNVCanTim) thoaManDK = 1;
                if (thoaManDK) { for(int i=0; i<pad; i++) printf(" "); printf("| %-10d | %-23s | %-10d | %-15ld |\n", dh.maDon, dh.ngayTao, dh.maNV, dh.tongTien); tongDoanhThu += dh.tongTien; foundCount++; }
            }
        }
    }
    fclose(f);
    for(int i=0; i<pad; i++) printf(" "); setMau(M_XANHDUONG); printf("+------------+-------------------------+------------+-----------------+\n");
    char ketQua[100]; sprintf(ketQua, "TONG DOANH THU (%d don): %ld VND", foundCount, tongDoanhThu); inGiua(ketQua, M_VANG); system("pause");
}

void baoCaoDoanhThuTheoThoiGian() {
    FILE *f = fopen("DonHang.txt", "r"); if (!f) return;
    system("cls"); inGiua("--- BAO CAO THOI GIAN ---", M_VANG);
    inGiua("1. Theo THANG", M_TRANG); inGiua("2. Theo QUY", M_TRANG); inGiua("3. Theo NAM", M_TRANG); inGiua("0. Quay lai", M_TRANG);
    int loaiTG = nhapSoNguyen(">> Chon: ", 0, 3);
    if (loaiTG == 0) { fclose(f); return; }
    int nhapThang = 0, nhapNam = 0, nhapQuy = 0;
    if (loaiTG == 1) { nhapThang = nhapSoNguyen("Nhap Thang (1-12): ", 1, 12); nhapNam = nhapSoNguyen("Nhap Nam: ", 2000, 3000); }
    else if (loaiTG == 2) { nhapQuy = nhapSoNguyen("Nhap Quy (1-4): ", 1, 4); nhapNam = nhapSoNguyen("Nhap Nam: ", 2000, 3000); }
    else if (loaiTG == 3) { nhapNam = nhapSoNguyen("Nhap Nam: ", 2000, 3000); }

    int tableWidth = 56; int pad = (CONSOLE_WIDTH - tableWidth) / 2; printf("\n");
    for(int i=0; i<pad; i++) printf(" "); setMau(M_XANHDUONG); printf("+------------+----------------------+-------------------+\n");
    for(int i=0; i<pad; i++) printf(" "); setMau(M_VANG);      printf("| %-10s | %-20s | %-17s |\n", "Ma Don", "Ngay Tao", "Doanh Thu");
    for(int i=0; i<pad; i++) printf(" "); setMau(M_XANHDUONG); printf("+------------+----------------------+-------------------+\n"); setMau(M_TRANG);
    char line[256]; long tongDoanhThu = 0; int count = 0;
    while (fgets(line, sizeof(line), f)) {
        DonHang dh;
        if (sscanf(line, "%d|%[^|]|%ld|%d|%d", &dh.maDon, dh.ngayTao, &dh.tongTien, &dh.trangThai, &dh.maNV) == 5) {
            if (dh.trangThai == 1) {
                int d, m, y; tachNgayThang(dh.ngayTao, &d, &m, &y); int thoaMan = 0;
                if (loaiTG == 1) { if (m == nhapThang && y == nhapNam) thoaMan = 1; }
                else if (loaiTG == 2) { int quy = (m - 1) / 3 + 1; if (quy == nhapQuy && y == nhapNam) thoaMan = 1; }
                else if (loaiTG == 3) { if (y == nhapNam) thoaMan = 1; }
                if (thoaMan) { for(int i=0; i<pad; i++) printf(" "); printf("| %-10d | %-20s | %-17ld |\n", dh.maDon, dh.ngayTao, dh.tongTien); tongDoanhThu += dh.tongTien; count++; }
            }
        }
    }
    fclose(f);
    for(int i=0; i<pad; i++) printf(" "); setMau(M_XANHDUONG); printf("+------------+----------------------+-------------------+\n");
    char ketQua[100]; sprintf(ketQua, "TONG: %ld VND (%d don)", tongDoanhThu, count); inGiua(ketQua, M_VANG); system("pause");
}

// [DA KHOI PHUC VA CHINH SUA GIAO DIEN]
void baoCaoMonYeuThich() {
    system("cls"); inGiua("--- PHAN TICH MON AN ---", M_VANG);
    int nhapThang = nhapSoNguyen("Nhap Thang (1-12): ", 1, 12);
    int nhapNam = nhapSoNguyen("Nhap Nam: ", 2000, 3000);

    ThongKeSP dsThongKe[100]; int soLuongSP = 0;
    FILE *fSP = fopen("SanPham.txt", "r"); if(!fSP) return;
    char lineSP[256];
    while(fgets(lineSP, sizeof(lineSP), fSP)) {
        SanPham sp; if(sscanf(lineSP, "%d|%[^|]", &sp.maSP, sp.tenMon) == 2) { dsThongKe[soLuongSP].maSP = sp.maSP; strcpy(dsThongKe[soLuongSP].tenMon, sp.tenMon); dsThongKe[soLuongSP].tongSoLuongBan = 0; soLuongSP++; }
    } fclose(fSP);

    int maDonHopLe[1000]; int soDonHopLe = 0;
    FILE *fDH = fopen("DonHang.txt", "r"); char lineDH[256];
    while(fgets(lineDH, sizeof(lineDH), fDH)) {
        DonHang dh; if(sscanf(lineDH, "%d|%[^|]|%ld|%d", &dh.maDon, dh.ngayTao, &dh.tongTien, &dh.trangThai) == 4) { if(dh.trangThai == 1) { int d, m, y; tachNgayThang(dh.ngayTao, &d, &m, &y); if(m == nhapThang && y == nhapNam) maDonHopLe[soDonHopLe++] = dh.maDon; } }
    } fclose(fDH);

    if(soDonHopLe == 0) { inGiua("Khong co du lieu thang nay.", M_DO); system("pause"); return; }

    FILE *fCT = fopen("ChiTietDonHang.txt", "r"); char lineCT[256];
    while(fgets(lineCT, sizeof(lineCT), fCT)) {
        char lineCopy[256]; strcpy(lineCopy, lineCT);
        char *token = strtok(lineCopy, "|"); if(!token) continue; token = strtok(NULL, "|"); if(!token) continue; int md = atoi(token);
        token = strtok(NULL, "|"); if(!token) continue; int msp = atoi(token); token = strtok(NULL, "|"); if(!token) continue; int sl = atoi(token);
        int valid = 0; for(int k=0; k<soDonHopLe; k++) { if(maDonHopLe[k] == md) { valid = 1; break; } }
        if(valid) { for(int j=0; j<soLuongSP; j++) { if(dsThongKe[j].maSP == msp) { dsThongKe[j].tongSoLuongBan += sl; break; } } }
    } fclose(fCT);

    for(int i=0; i<soLuongSP-1; i++) { for(int j=i+1; j<soLuongSP; j++) { if(dsThongKe[i].tongSoLuongBan < dsThongKe[j].tongSoLuongBan) { ThongKeSP temp = dsThongKe[i]; dsThongKe[i] = dsThongKe[j]; dsThongKe[j] = temp; } } }

    // VE BANG KET QUA
    int tableWidth = 50; int pad = (CONSOLE_WIDTH - tableWidth) / 2; printf("\n");
    for(int i=0; i<pad; i++) printf(" "); setMau(M_XANHDUONG); printf("+----------------------------------+------------+\n");
    for(int i=0; i<pad; i++) printf(" "); setMau(M_VANG);      printf("| Ten Mon                          | So Luong   |\n");
    for(int i=0; i<pad; i++) printf(" "); setMau(M_XANHDUONG); printf("+----------------------------------+------------+\n"); setMau(M_TRANG);
    
    inGiua(">>> TOP BAN CHAY <<<", M_XANHLA);
    int countShow = 0;
    for(int i=0; i<soLuongSP; i++) {
        if(dsThongKe[i].tongSoLuongBan > 0) {
            for(int k=0; k<pad; k++) printf(" ");
            printf("| %-32s | %-10d |\n", dsThongKe[i].tenMon, dsThongKe[i].tongSoLuongBan);
            countShow++; if(countShow >= 3) break;
        }
    }
    if(countShow == 0) { for(int k=0; k<pad; k++) printf(" "); printf("| (Chua co du lieu)                |            |\n"); }
    for(int i=0; i<pad; i++) printf(" "); setMau(M_XANHDUONG); printf("+----------------------------------+------------+\n"); setMau(M_TRANG);

    printf("\n");
    inGiua(">>> TOP BAN CHAM <<<", M_DO);
    for(int i=0; i<pad; i++) printf(" "); setMau(M_XANHDUONG); printf("+----------------------------------+------------+\n"); setMau(M_TRANG);
    int countBad = 0;
    for(int i=soLuongSP-1; i>=0; i--) {
        for(int k=0; k<pad; k++) printf(" ");
        printf("| %-32s | %-10d |\n", dsThongKe[i].tenMon, dsThongKe[i].tongSoLuongBan);
        countBad++; if(countBad >= 3) break;
    }
    for(int i=0; i<pad; i++) printf(" "); setMau(M_XANHDUONG); printf("+----------------------------------+------------+\n");
    system("pause");
}

void menuBaoCao() {
    int chon;
    do {
        system("cls"); hienThiLogo();
        inDongKe(50, M_XANHDUONG); inGiua("HE THONG BAO CAO & THONG KE", M_VANG); inDongKe(50, M_XANHDUONG); printf("\n");
        inGiua("[ 1 ] Bao cao doanh thu (Ngay/NV/Tat ca)   ", M_TRANG);
        inGiua("[ 2 ] Bao cao thoi gian (Thang/Quy/Nam)    ", M_TRANG);
        inGiua("[ 3 ] Phan tich mon an (Yeu thich/It mua)  ", M_TRANG);
        inGiua("[ 0 ] Quay lai Menu Chinh                  ", M_TRANG); printf("\n");
        chon = nhapSoNguyen(">> Chon chuc nang: ", 0, 3);
        switch (chon) {
            case 1: baoCaoDoanhThuCoBan(); break;
            case 2: baoCaoDoanhThuTheoThoiGian(); break;
            case 3: baoCaoMonYeuThich(); break; // Da them lai call
            case 0: break;
            default: inGiua("Chon sai!", M_DO);
        }
    } while (chon != 0);
}

void menuChinh(NhanVien nv) {
    int chon;
    do {
        system("cls"); hienThiLogo();
        char chao[100]; sprintf(chao, "Xin chao: %s (%s)", nv.hoTen, nv.chucVu); inGiua(chao, M_XANHLA);
        int width = 50;
        inDongKe(width, M_XANHDUONG);
        inGiua("[ 1 ]   THUC DON DO UONG          ", M_TRANG);
        inGiua("[ 2 ]   TAO DON HANG (BAN HANG)   ", M_TRANG);
        inGiua("[ 3 ]   BAO CAO & THONG KE        ", M_TRANG);
        inGiua("[ 0 ]   DANG XUAT                 ", M_DO);
        inDongKe(width, M_XANHDUONG);
        chon = nhapSoNguyen(">> Chon chuc nang: ", 0, 3);
        switch (chon) {
            case 1: xemMenuPhanLoai(); break;
            case 2: taoDonHang(nv); break;
            case 3: menuBaoCao(); break;
            case 0: break;
            default: inGiua("Chon sai!", M_DO); system("pause");
        }
    } while (chon != 0);
}

int main() {
    system("color 07"); // Reset mau mac dinh
    NhanVien currentUser; char tk[50], mk[50]; int loggedIn = 0;
    while (!loggedIn) {
        system("cls"); hienThiLogo();
        inDongKe(40, M_XANHDUONG); inGiua("DANG NHAP HE THONG", M_VANG); inDongKe(40, M_XANHDUONG); printf("\n");
        int pad = (CONSOLE_WIDTH - 30) / 2;
        for(int i=0; i<pad; i++) printf(" "); printf("Tai khoan: "); scanf("%s", tk);
        for(int i=0; i<pad; i++) printf(" "); printf("Mat khau: "); nhapMatKhau(mk);
        FILE *f = fopen("NhanVien.txt", "r"); if (!f) { inGiua("\nLoi: Thieu file NhanVien.txt", M_DO); return 1; }
        char line[256]; int found = 0;
        while (fgets(line, sizeof(line), f)) {
            NhanVien nv;
            if (sscanf(line, "%d|%[^|]|%[^|]|%[^|]|%s", &nv.maNV, nv.taiKhoan, nv.matKhau, nv.hoTen, nv.chucVu) == 5) {
                xoaXuongDong(nv.chucVu); if (strcmp(tk, nv.taiKhoan) == 0 && strcmp(mk, nv.matKhau) == 0) { currentUser = nv; found = 1; break; }
            }
        } fclose(f);
        if (found) { loggedIn = 1; printf("\n\n"); inGiua("DANG NHAP THANH CONG!", M_XANHLA); Sleep(1000); } 
        else { printf("\n\n"); inGiua("Sai tai khoan hoac mat khau!", M_DO); system("pause"); }
    }
    menuChinh(currentUser);
    return 0;
}
