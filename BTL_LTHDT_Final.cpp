/*
    BAI TAP LON: QUAN LY KY TUC XA - LAP TRINH HUONG DOI TUONG (C++)

    Cau truc chuong trinh:
      PHAN 1. Khai bao thu vien
      PHAN 2. Ham ho tro nhap du lieu va kiem tra dau vao
      PHAN 3. Lop truu tuong Nguoi
      PHAN 4. Lop SinhVien (ke thua Nguoi)
      PHAN 5. Lop Phong (quan ly danh sach sinh vien)
      PHAN 6. Lop QuanLyKTX (quan ly toan bo ky tuc xa)
      PHAN 7. Ham main va menu chuong trinh

    Cac diem da sua : 
      - nhapInt: khong chap nhan '2abc', '2.5' la so nguyen hop le.
      - nhapDouble: khong chap nhan '500000abc', NaN hoac Infinity.
      - nhapChuoi: khong chap nhan chuoi rong/toan dau cach;
        tu dong bo dau cach thua o hai dau chuoi.

    Chuc nang moi:
      - Sua thong tin sinh vien theo ma sinh vien (khong sua ma dinh danh).
      - Xoa sinh vien theo ma, co buoc xac nhan truoc khi xoa.
      - Cap nhat menu: muc 7 va 8.

    Chu y: Du lieu chi ton tai trong bo nho khi chuong trinh dang chay.
*/

// ============================================================================
// PHAN 1. CAC THU VIEN SU DUNG
// ============================================================================
#include <algorithm>  // std::find_if: tim sinh vien/phong theo dieu kien.
#include <cmath>       // std::isfinite: kiem tra so thuc huu han.
#include <cstdlib>     // std::exit: ket thuc khi khong con du lieu nhap.
#include <iostream>    // std::cin, std::cout, std::istream, std::ostream.
#include <sstream>     // std::istringstream: phan tich mot dong van ban.
#include <string>      // std::string, std::getline.
#include <utility>     // std::move: chuyen doi tuong vao vector.
#include <vector>      // std::vector: danh sach co kich thuoc linh hoat.

// ============================================================================
// PHAN 2. HAM HO TRO NHAP DU LIEU
// Muc dich: kiem tra du lieu ngay khi nhap, tranh dua du lieu sai vao cac lop.
// Cach lam moi: doc TRON mot dong truoc, sau do moi phan tich du lieu.
// Vi the khong can ham boDong() nhu phien ban cu.
// ============================================================================

// 2.1. Nhap so nguyen nam trong doan [minVal, maxVal].
int nhapInt(std::istream& is, const std::string& thongBao, int minVal, int maxVal) {
    std::string dong;

    while (true) {
        std::cout << thongBao;
        if (!std::getline(is, dong)) std::exit(0); // EOF: khong con du lieu de nhap.

        // Dua ca dong vao luong phan tich; khong bo qua ky tu du o cuoi.
        std::istringstream boPhanTich(dong);
        int x = 0;
        char kyTuThua = '\0';

        // Dieu kien hop le:
        // 1. Doc duoc mot so nguyen.
        // 2. Khong con ky tu khac khoang trang phia sau so.
        // 3. Gia tri thuoc khoang duoc cho phep.
        // Vi du: " 2 " hop le; "2abc", "2.5" khong hop le.
        if ((boPhanTich >> x) && !(boPhanTich >> kyTuThua) &&
            x >= minVal && x <= maxVal) {
            return x;
        }

        std::cout << "=> Gia tri khong hop le, nhap lai!\n";
    }
}

// 2.2. Nhap so thuc nam trong doan [minVal, maxVal].
double nhapDouble(std::istream& is, const std::string& thongBao,
                 double minVal, double maxVal) {
    std::string dong;

    while (true) {
        std::cout << thongBao;
        if (!std::getline(is, dong)) std::exit(0);

        std::istringstream boPhanTich(dong);
        double x = 0.0;
        char kyTuThua = '\0';

        // Chap nhan so thuc hop le (vi du 1500000, 1500000.5, 1.5e6).
        // Khong chap nhan ky tu du ("500000abc") hoac gia tri vo han/NaN.
        if ((boPhanTich >> x) && !(boPhanTich >> kyTuThua) &&
            std::isfinite(x) && x >= minVal && x <= maxVal) {
            return x;
        }

        std::cout << "=> Gia tri khong hop le, nhap lai!\n";
    }
}

// 2.3. Nhap chuoi khong rong; loai bo khoang trang thua o dau/cuoi.
std::string nhapChuoi(std::istream& is, const std::string& thongBao) {
    std::string s;
    const std::string khoangTrang = " \t\r\n\v\f";

    while (true) {
        std::cout << thongBao;
        if (!std::getline(is, s)) std::exit(0);

        // Tim ky tu dau tien KHONG phai khoang trang.
        const std::string::size_type batDau = s.find_first_not_of(khoangTrang);
        if (batDau != std::string::npos) {
            // Tim ky tu cuoi cung KHONG phai khoang trang.
            const std::string::size_type ketThuc = s.find_last_not_of(khoangTrang);
            return s.substr(batDau, ketThuc - batDau + 1);
        }

        // Truong hop "" hoac "   " deu phai nhap lai.
        std::cout << "=> Khong duoc de trong, nhap lai!\n";
    }
}

// ============================================================================
// PHAN 3. LOP Nguoi - LOP CO SO TRUU TUONG
// - Chua thong tin chung: ten, tuoi.
// - Hai ham thuan ao (= 0) yeu cau lop con phai tu dinh nghia nhap/xuat.
// - operator >> va << cho phep dung cu phap quen thuoc: cin >> sv, cout << sv.
// ============================================================================
class Nguoi {
protected:
    // protected: lop SinhVien ke thua co the su dung truc tiep hai thuoc tinh.
    std::string ten;
    int tuoi{0};

public:
    virtual ~Nguoi() = default; // Huy ao, phu hop cho lop co so da hinh.

    // Hai ham thuan ao: lop Nguoi khong the khoi tao truc tiep.
    virtual void doNhap(std::istream& is) = 0;
    virtual void doXuat(std::ostream& os) const = 0;

    // Nap chong toan tu nhap; goi ham ao doNhap cua doi tuong thuc te.
    friend std::istream& operator>>(std::istream& is, Nguoi& ng) {
        ng.doNhap(is);
        return is;
    }

    // Nap chong toan tu xuat; goi ham ao doXuat cua doi tuong thuc te.
    friend std::ostream& operator<<(std::ostream& os, const Nguoi& ng) {
        ng.doXuat(os);
        return os;
    }
};

// ============================================================================
// PHAN 4. LOP SinhVien - KE THUA TU Nguoi
// - Du lieu rieng: ma sinh vien, gioi tinh, que quan.
// - Ghi de (override) hai ham doNhap / doXuat tu lop co so.
// ============================================================================
class SinhVien : public Nguoi {
private:
    std::string maSV;
    std::string gioiTinh;
    std::string queQuan;

public:
    // Nhap thong tin mot sinh vien; su dung cac ham nhap da kiem tra o PHAN 2.
    void doNhap(std::istream& is) override {
        maSV = nhapChuoi(is, "Nhap ma sinh vien: ");
        ten = nhapChuoi(is, "Nhap ho ten: ");
        tuoi = nhapInt(is, "Nhap tuoi: ", 1, 120);
        gioiTinh = nhapChuoi(is, "Nhap gioi tinh: ");
        queQuan = nhapChuoi(is, "Nhap que quan: ");
    }

    // In thong tin mot sinh vien ra luong xuat.
    void doXuat(std::ostream& os) const override {
        os << "  Ma SV     : " << maSV << "\n"
           << "  Ho ten    : " << ten << "\n"
           << "  Tuoi      : " << tuoi << "\n"
           << "  Gioi tinh : " << gioiTinh << "\n"
           << "  Que quan  : " << queQuan;
    }

    // SUA thong tin sinh vien (giu nguyen maSV - ma dinh danh duy nhat).
    // Dung cac ham nhap da kiem tra de tranh luu du lieu sai.
    // Sau khi sua, vector van giu nguyen doi tuong va vi tri cua sinh vien.
    void suaThongTin(std::istream& is) {
        ten = nhapChuoi(is, "Nhap ho ten moi: ");
        tuoi = nhapInt(is, "Nhap tuoi moi: ", 1, 120);
        gioiTinh = nhapChuoi(is, "Nhap gioi tinh moi: ");
        queQuan = nhapChuoi(is, "Nhap que quan moi: ");
    }

    // Kiem tra ma trung; getter cho phep lop khac doc ma ma khong sua truc tiep.
    bool coMa(const std::string& ma) const { return maSV == ma; }
    const std::string& getMaSV() const { return maSV; }
};

// ============================================================================
// PHAN 5. LOP Phong - QUAN LY MOT PHONG
// - Luu ma phong, loai phong, suc chua, don gia va danh sach sinh vien.
// - Tu kiem tra tinh trang con cho, trung ma sinh vien trong phong.
// - Ho tro tim kiem va tinh tong tien cua mot phong.
// ============================================================================
class Phong {
private:
    std::string maPhong;
    std::string loaiPhong;
    size_t soLuongToiDa{0};
    double giaTien{0.0};
    std::vector<SinhVien> dsSinhVien; // Vector tu quan ly vung nho cho danh sach.

public:
    // Nap chong >>: nhap thong tin chung cua phong (chua nhap sinh vien).
    friend std::istream& operator>>(std::istream& is, Phong& p) {
        p.maPhong = nhapChuoi(is, "Nhap ma phong: ");
        p.loaiPhong = nhapChuoi(is, "Nhap loai phong: ");
        p.soLuongToiDa = nhapInt(is, "Nhap so luong toi da: ", 1, 1000);
        p.giaTien = nhapDouble(is, "Nhap gia tien moi sinh vien: ", 0.0, 1e12);
        return is;
    }

    // Nap chong <<: in thong tin phong va tat ca sinh vien dang o.
    friend std::ostream& operator<<(std::ostream& os, const Phong& p) {
        os << "\n========================================\n"
           << "Ma phong      : " << p.maPhong << "\n"
           << "Loai phong    : " << p.loaiPhong << "\n"
           << "So sinh vien  : " << p.dsSinhVien.size() << "/" << p.soLuongToiDa << "\n"
           << "Gia tien/nguoi: " << p.giaTien << " VND\n"
           << "----------------------------------------\n";

        size_t idx = 1;
        for (const auto& sv : p.dsSinhVien) {
            os << "Sinh vien thu " << idx++ << ":\n" << sv << "\n\n";
        }
        return os;
    }

    // Hai dieu kien co ban khi them phong / them sinh vien.
    bool coMa(const std::string& ma) const { return maPhong == ma; }
    bool conCho() const { return dsSinhVien.size() < soLuongToiDa; }

    // Tim xem ma sinh vien da ton tai TRONG phong nay hay chua.
    bool coSinhVien(const std::string& ma) const {
        for (const auto& sv : dsSinhVien) {
            if (sv.coMa(ma)) return true;
        }
        return false;
    }

    // Chi them khi phong con cho va ma sinh vien chua nam trong phong.
    bool themSinhVien(const SinhVien& sv) {
        if (!conCho() || coSinhVien(sv.getMaSV())) return false;
        dsSinhVien.push_back(sv);
        return true;
    }

    // SUA thong tin sinh vien trong phong theo maSV.
    // Tim trong vector, sau do sua TRUC TIEP doi tuong tim thay.
    // Tra ve false neu phong nay khong chua sinh vien co ma can sua.
    bool suaSinhVien(const std::string& ma) {
        auto it = std::find_if(dsSinhVien.begin(), dsSinhVien.end(),
                               [&](const SinhVien& sv) { return sv.coMa(ma); });
        if (it == dsSinhVien.end()) return false;

        std::cout << "\n=> Thong tin sinh vien hien tai:\n" << *it << "\n";
        it->suaThongTin(std::cin);
        std::cout << "=> Sua thong tin sinh vien thanh cong!\n";
        return true;
    }

    // XOA sinh vien khoi vector. erase() lam giam so luong sinh vien
    // nen thong ke va tong tien phong se tu cap nhat theo du lieu moi.
    bool xoaSinhVien(const std::string& ma) {
        auto it = std::find_if(dsSinhVien.begin(), dsSinhVien.end(),
                               [&](const SinhVien& sv) { return sv.coMa(ma); });
        if (it == dsSinhVien.end()) return false;

        dsSinhVien.erase(it);
        return true;
    }

    // Tim ma sinh vien; neu co thi in thong tin va tra ve true.
    bool timSinhVien(const std::string& ma) const {
        auto it = std::find_if(dsSinhVien.begin(), dsSinhVien.end(),
                               [&](const SinhVien& sv) { return sv.coMa(ma); });
        if (it == dsSinhVien.end()) return false;

        std::cout << "\n=> Tim thay tai phong [" << maPhong << "]:\n" << *it << "\n";
        return true;
    }

    // Tien phong = don gia moi sinh vien * so sinh vien hien tai.
    double tinhTien() const { return giaTien * static_cast<double>(dsSinhVien.size()); }
    size_t soSinhVien() const { return dsSinhVien.size(); }
    const std::string& getMaPhong() const { return maPhong; }
};

// ============================================================================
// PHAN 6. LOP QuanLyKTX - QUAN LY TOAN BO PHONG VA NGHIEP VU
// - Co vector chua tat ca phong.
// - Kiem tra trung ma sinh vien o quy mo TOAN KTX, khong chi trong mot phong.
// - Cung cap cac thao tac tuong ung voi menu chinh.
// ============================================================================
class QuanLyKTX {
private:
    std::vector<Phong> dsPhong;

    // Kiem tra sinh vien da o mot phong bat ky trong KTX hay chua.
    bool tonTaiSinhVien(const std::string& ma) const {
        for (const auto& ph : dsPhong) {
            if (ph.coSinhVien(ma)) return true;
        }
        return false;
    }

public:
    // (1) Them phong: nhap phong, chan ma phong bi trung, sau do luu vao vector.
    void themPhong() {
        Phong p;
        std::cin >> p;

        for (const auto& ph : dsPhong) {
            if (ph.coMa(p.getMaPhong())) {
                std::cout << "=> Ma phong da ton tai!\n";
                return;
            }
        }

        dsPhong.push_back(std::move(p));
        std::cout << "=> Them phong thanh cong!\n";
    }

    // (2) Them sinh vien: kiem tra co phong, dung ma phong, phong con cho,
    //     va khong trung ma sinh vien trong toan KTX.
    void themSinhVien() {
        if (dsPhong.empty()) {
            std::cout << "=> Chua co phong nao trong he thong!\n";
            return;
        }

        std::string ma = nhapChuoi(std::cin, "Nhap ma phong: ");

        auto it = std::find_if(dsPhong.begin(), dsPhong.end(),
                               [&](const Phong& p) { return p.coMa(ma); });
        if (it == dsPhong.end()) {
            std::cout << "=> Khong tim thay phong voi ma da nhap!\n";
            return;
        }

        if (!it->conCho()) {
            std::cout << "=> Phong da day!\n";
            return;
        }

        SinhVien sv;
        std::cin >> sv;

        if (tonTaiSinhVien(sv.getMaSV())) {
            std::cout << "=> Ma sinh vien da ton tai trong KTX!\n";
            return;
        }

        it->themSinhVien(sv);
        std::cout << "=> Them sinh vien thanh cong!\n";
    }

    // (3) Liet ke tat ca phong; moi phong tu in danh sach sinh vien cua no.
    void xuatDanhSach() const {
        if (dsPhong.empty()) {
            std::cout << "=> Chua co phong nao trong danh sach!\n";
            return;
        }
        for (const auto& ph : dsPhong) {
            std::cout << ph;
        }
    }

    // (4) Tong tien phong: cong ket qua tinhTien() cua tung phong.
    // Luu y: day la so tien tinh theo don gia, chua co theo doi da thu hay chua.
    void tinhTongTien() const {
        double tong = 0.0;
        for (const auto& ph : dsPhong) {
            tong += ph.tinhTien();
        }
        std::cout << "=> Tong thu tien phong toan KTX: " << tong << " VND\n";
    }

    // (5) Thong ke tong so phong va tong so sinh vien dang o.
    void thongKe() const {
        size_t tongSV = 0;
        std::cout << "=> Tong so phong: " << dsPhong.size() << "\n";
        for (const auto& ph : dsPhong) {
            tongSV += ph.soSinhVien();
        }
        std::cout << "=> Tong so sinh vien dang o: " << tongSV << "\n";
    }

    // (6) Tim sinh vien theo ma: duyet qua cac phong cho toi khi tim thay.
    void timKiem() const {
        std::string ma = nhapChuoi(std::cin, "Nhap ma sinh vien can tim: ");

        bool timThay = false;
        for (size_t i = 0; i < dsPhong.size(); i++) {
            if (dsPhong[i].timSinhVien(ma)) {
                timThay = true;
                break;
            }
        }
        if (!timThay) {
            std::cout << "=> Khong tim thay sinh vien!\n";
        }
    }

    // (7) SUA sinh vien trong TOAN KTX: nhap ma, tim phong dang o,
    //     sau do goi Phong::suaSinhVien(). MaSV duoc giu nguyen.
    void suaSinhVien() {
        std::string ma = nhapChuoi(std::cin, "Nhap ma sinh vien can sua: ");
        for (auto& ph : dsPhong) {
            if (ph.suaSinhVien(ma)) return;
        }
        std::cout << "=> Khong tim thay sinh vien can sua!\n";
    }

    // (8) XOA sinh vien: tim tren toan KTX, hien thong tin, hoi xac nhan.
    //     Khong xoa khi nhap n/N hoac bat ky noi dung nao khac y/Y.
    void xoaSinhVien() {
        std::string ma = nhapChuoi(std::cin, "Nhap ma sinh vien can xoa: ");
        for (auto& ph : dsPhong) {
            if (!ph.coSinhVien(ma)) continue;

            // Hien thong tin va phong hien tai de tranh xoa nham.
            ph.timSinhVien(ma);
            std::string xacNhan = nhapChuoi(std::cin, "Xac nhan xoa sinh vien? (y/n): ");
            if (xacNhan != "y" && xacNhan != "Y") {
                std::cout << "=> Da huy thao tac xoa!\n";
                return;
            }

            if (ph.xoaSinhVien(ma)) {
                std::cout << "=> Xoa sinh vien thanh cong!\n";
            }
            return;
        }
        std::cout << "=> Khong tim thay sinh vien can xoa!\n";
    }
};

// ============================================================================
// PHAN 7. HAM main() - DIEM BAT DAU CHUONG TRINH
// - Tao mot doi tuong QuanLyKTX de luu du lieu trong suot phien chay.
// - Hien menu, nhap lua chon [0..8] va goi chuc nang tuong ung.
// - Vong lap ket thuc khi nguoi dung chon 0.
// ============================================================================
int main() {
    QuanLyKTX ql;
    int chon = -1;

    do {
        std::cout << "\n============ QUAN LY KTX ============\n"
                  << "1. Them phong\n"
                  << "2. Them sinh vien vao phong\n"
                  << "3. Xuat danh sach ktx\n"
                  << "4. Tinh tong tien phong\n"
                  << "5. Thong ke ktx\n"
                  << "6. Tim kiem sinh vien\n"
                  << "7. Sua thong tin sinh vien\n"
                  << "8. Xoa sinh vien\n"
                  << "0. Thoat\n"
                  << "-------------------------------------\n";

        chon = nhapInt(std::cin, "Nhap lua chon: ", 0, 8);

        // switch chia nhanh theo gia tri nguoi dung vua chon.
        switch (chon) {
            case 1: ql.themPhong(); break;
            case 2: ql.themSinhVien(); break;
            case 3: ql.xuatDanhSach(); break;
            case 4: ql.tinhTongTien(); break;
            case 5: ql.thongKe(); break;
            case 6: ql.timKiem(); break;
            case 7: ql.suaSinhVien(); break;
            case 8: ql.xoaSinhVien(); break;
            case 0: std::cout << "=> Da thoat!\n"; break;
        }
    } while (chon != 0);

    return 0;
}
