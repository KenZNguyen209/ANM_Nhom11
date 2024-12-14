#include <QApplication>
#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QGridLayout>
#include <QFileDialog>
#include <QMessageBox>
#include <QFile>
#include <QTextStream>
#include <random>
#include <cmath>
#include <QString>
#include <QDebug>

class MainWindow : public QWidget {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr) {
        setWindowTitle("Mẫu Chữ Ký Số");

        // Nhãn
        QLabel *nhanChon = new QLabel("1. Chọn 2 số ngẫu nhiên");
        QLabel *nhanP = new QLabel("Số p");
        QLabel *nhanQ = new QLabel("Số q");
        QLabel *nhanKhoaBiMat = new QLabel("Khóa bí mật");
        QLabel *nhanKhoaCongKhai = new QLabel("Khóa công khai");
        QLabel *nhanKyVanBan = new QLabel("Ký văn bản");
        QLabel *nhanVanBan = new QLabel("Văn bản");
        QLabel *nhanKetQua = new QLabel("Kết quả");
        QLabel *nhanXacNhan = new QLabel("Xác nhận");

        // Các trường nhập
        editP = new QLineEdit;
        editQ = new QLineEdit;
        editKhoaBiMat = new QLineEdit;
        editKhoaCongKhai = new QLineEdit;
        editVanBan = new QLineEdit;
        editKetQuaKy = new QLineEdit;
        editVanBanXacNhan = new QLineEdit;
        editKetQuaXacNhan = new QLineEdit;

        // Các nút bấm
        QPushButton *nutNgauNhien = new QPushButton("Ngẫu nhiên");
        QPushButton *nutSinhKhoa = new QPushButton("Sinh khóa");
        QPushButton *nutKy = new QPushButton("Ký văn bản");
        QPushButton *nutXacNhan = new QPushButton("Xác nhận");
        QPushButton *nutTaiFile = new QPushButton("Tải văn bản");
        QPushButton *nutLuuFile = new QPushButton("Lưu văn bản");
        QPushButton *nutTaiVaKiemTra = new QPushButton("Tải và kiểm tra");

        // Thiết lập giao diện
        QGridLayout *layout = new QGridLayout;
        layout->addWidget(nhanChon, 0, 0, 1, 2);
        layout->addWidget(nhanP, 1, 0);
        layout->addWidget(editP, 1, 1);
        layout->addWidget(nhanQ, 1, 2);
        layout->addWidget(editQ, 1, 3);
        layout->addWidget(nhanKhoaBiMat, 2, 0);
        layout->addWidget(editKhoaBiMat, 2, 1);
        layout->addWidget(nhanKhoaCongKhai, 2, 2);
        layout->addWidget(editKhoaCongKhai, 2, 3);
        layout->addWidget(nutNgauNhien, 3, 0, 1, 2);
        layout->addWidget(nutSinhKhoa, 3, 2, 1, 2);

        layout->addWidget(nhanKyVanBan, 4, 0, 1, 4);
        layout->addWidget(nhanVanBan, 5, 0);
        layout->addWidget(editVanBan, 5, 1, 1, 2);
        layout->addWidget(nutTaiFile, 5, 3);
        layout->addWidget(nhanKetQua, 6, 0);
        layout->addWidget(editKetQuaKy, 6, 1, 1, 3);
        layout->addWidget(nutKy, 7, 3);
        layout->addWidget(nutLuuFile, 8, 3);

        layout->addWidget(nhanXacNhan, 9, 0, 1, 4);
        layout->addWidget(editVanBanXacNhan, 10, 1, 1, 3);
        layout->addWidget(nutTaiVaKiemTra, 10, 0);
        layout->addWidget(nhanKetQua, 11, 0);
        layout->addWidget(editKetQuaXacNhan, 11, 1, 1, 3);
        layout->addWidget(nutXacNhan, 12, 3);

        setLayout(layout);

        // Kết nối tín hiệu và khe cắm
        connect(nutNgauNhien, &QPushButton::clicked, this, &MainWindow::taoSoNgauNhien);
        connect(nutSinhKhoa, &QPushButton::clicked, this, &MainWindow::sinhKhoa);
        connect(nutTaiFile, &QPushButton::clicked, this, &MainWindow::taiFile);
        connect(nutKy, &QPushButton::clicked, this, &MainWindow::kyVanBan);
        connect(nutLuuFile, &QPushButton::clicked, this, &MainWindow::luuVanBanKy);
        connect(nutTaiVaKiemTra, &QPushButton::clicked, this, &MainWindow::taiVaKiemTraFile);
        connect(nutXacNhan, &QPushButton::clicked, this, &MainWindow::xacNhanVanBan);
    }

private:
    bool laSoNguyenTo(int num) {
        if (num <= 1) return false;
        for (int i = 2; i <= std::sqrt(num); ++i) {
            if (num % i == 0) return false;
        }
        return true;
    }

    int ucln(int a, int b) {
        while (b != 0) {
            int temp = b;
            b = a % b;
            a = temp;
        }
        return a;
    }

    int moDoiSo(int base, int exp, int mod) {
        int result = 1;
        base = base % mod;
        while (exp > 0) {
            if (exp % 2 == 1) {
                result = (result * base) % mod;
            }
            exp = exp >> 1;
            base = (base * base) % mod;
        }
        return result;
    }

    int nghichDaoMod(int a, int m) {
        a = a % m;
        for (int x = 1; x < m; x++) {
            if ((a * x) % m == 1) {
                return x;
            }
        }
        return -1;
    }

    void taoSoNgauNhien() {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dist(2, 100);

        int p, q;
        do {
            p = dist(gen);
        } while (!laSoNguyenTo(p));

        do {
            q = dist(gen);
        } while (!laSoNguyenTo(q) || q == p);

        editP->setText(QString::number(p));
        editQ->setText(QString::number(q));
    }

    void sinhKhoa() {
        if (editP->text().isEmpty() || editQ->text().isEmpty()) {
            QMessageBox::warning(this, "Lỗi", "Vui lòng nhập số p và q!");
            return;
        }
        int p = editP->text().toInt();
        int q = editQ->text().toInt();

        if (!laSoNguyenTo(p) || !laSoNguyenTo(q)) {
            QMessageBox::warning(this, "Lỗi", "Cả hai số p và q phải là số nguyên tố!");
            return;
        }

        int n = p * q;
        int phi = (p - 1) * (q - 1);

        int e;
        do {
            e = 3 + rand() % (phi - 3);
        } while (ucln(e, phi) != 1);

        int d = nghichDaoMod(e, phi);
        if (d == -1) {
            QMessageBox::warning(this, "Lỗi", "Không thể tạo khóa!");
            return;
        }

        editKhoaBiMat->setText(QString::number(d));
        editKhoaCongKhai->setText(QString::number(e));
        nValue = n; // Lưu n cho các phép toán sau
    }

    void taiFile() {
        QString fileName = QFileDialog::getOpenFileName(this, "Mở tệp văn bản", "", "Text Files (*.txt);;All Files (*)");
        if (!fileName.isEmpty()) {
            QFile file(fileName);
            if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
                editVanBan->setText(file.readAll());
                file.close();
            } else {
                QMessageBox::warning(this, "Lỗi", "Không thể mở tệp!");
            }
        }
    }

    void kyVanBan() {
        if (editVanBan->text().isEmpty() || editKhoaBiMat->text().isEmpty()) {
            QMessageBox::warning(this, "Lỗi", "Vui lòng nhập văn bản và khóa bí mật!");
            return;
        }

        QString inputText = editVanBan->text();
        int d = editKhoaBiMat->text().toInt();
        QString signedText;
        for (QChar c : inputText) {
            int m = c.unicode();
            int s = moDoiSo(m, d, nValue);
            signedText.append(QString::number(s) + " ");
        }
        editKetQuaKy->setText(signedText.trimmed());
    }

    void luuVanBanKy() {
        QString fileName = QFileDialog::getSaveFileName(this, "Lưu tệp ký", "", "Text Files (*.txt);;All Files (*)");
        if (!fileName.isEmpty()) {
            QFile file(fileName);
            if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
                QTextStream out(&file);
                out << editKetQuaKy->text();
                file.close();
                QMessageBox::information(this, "Thành công", "Tệp ký đã được lưu!");
            } else {
                QMessageBox::warning(this, "Lỗi", "Không thể lưu tệp!");
            }
        }
    }

    void taiVaKiemTraFile() {
        QString fileName = QFileDialog::getOpenFileName(this, "Mở tệp ký", "", "Text Files (*.txt);;All Files (*)");
        if (!fileName.isEmpty()) {
            QFile file(fileName);
            if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
                QString signedText = file.readAll();
                file.close();

                editVanBanXacNhan->setText(signedText);
                xacNhanVanBan();
            } else {
                QMessageBox::warning(this, "Lỗi", "Không thể mở tệp!");
            }
        }
    }

    void xacNhanVanBan() {
        if (editVanBanXacNhan->text().isEmpty() || editKhoaCongKhai->text().isEmpty()) {
            QMessageBox::warning(this, "Lỗi", "Vui lòng nhập văn bản và khóa công khai!");
            return;
        }

        QStringList signedValues = editVanBanXacNhan->text().split(" ");
        int e = editKhoaCongKhai->text().toInt();
        QString verifiedText;

        for (const QString &value : signedValues) {
            int s = value.toInt();
            int m = moDoiSo(s, e, nValue);
            verifiedText.append(QChar(m));
        }

        if (verifiedText == editVanBan->text()) {
            editKetQuaXacNhan->setText("Hợp lệ");
        } else {
            editKetQuaXacNhan->setText("Không hợp lệ");
        }
    }

private:
    QLineEdit *editP;
    QLineEdit *editQ;
    QLineEdit *editKhoaBiMat;
    QLineEdit *editKhoaCongKhai;
    QLineEdit *editVanBan;
    QLineEdit *editKetQuaKy;
    QLineEdit *editVanBanXacNhan;
    QLineEdit *editKetQuaXacNhan;

    int nValue;
};

#include "main.moc"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    MainWindow window;
    window.resize(600, 500);
    window.show();

    return app.exec();
}
