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
        setWindowTitle("Digital Signature Form");

        // Labels
        QLabel *labelChoose = new QLabel("1. Chọn 2 số ngẫu nhiên");
        QLabel *labelP = new QLabel("Số p");
        QLabel *labelQ = new QLabel("Số q");
        QLabel *labelPrivateKey = new QLabel("Khóa bí mật");
        QLabel *labelPublicKey = new QLabel("Khóa công khai");
        QLabel *labelSignText = new QLabel("Ký văn bản");
        QLabel *labelText = new QLabel("Văn bản");
        QLabel *labelSignature = new QLabel("Chữ ký");
        QLabel *labelVerify = new QLabel("Xác nhận");

        // Input fields
        editP = new QLineEdit;
        editQ = new QLineEdit;
        editPrivateKey = new QLineEdit;
        editPublicKey = new QLineEdit;
        editText = new QLineEdit;
        editSignResult = new QLineEdit;
        editVerifyText = new QLineEdit;
        editVerifySignature = new QLineEdit;
        editVerifyResult = new QLineEdit;

        // Buttons
        QPushButton *btnRandom = new QPushButton("Ngẫu nhiên");
        QPushButton *btnGenerateKey = new QPushButton("Sinh khóa");
        QPushButton *btnSign = new QPushButton("Ký văn bản");
        QPushButton *btnVerify = new QPushButton("Xác nhận");
        QPushButton *btnLoadFile = new QPushButton("Tải văn bản");
        QPushButton *btnSaveFile = new QPushButton("Lưu chữ ký");
        QPushButton *btnLoadSignature = new QPushButton("Tải chữ ký");
        QPushButton *btnLoadVerifyFile = new QPushButton("Tải văn bản kiểm tra");

        // Layout setup
        QGridLayout *layout = new QGridLayout;
        layout->addWidget(labelChoose, 0, 0, 1, 2);
        layout->addWidget(labelP, 1, 0);
        layout->addWidget(editP, 1, 1);
        layout->addWidget(labelQ, 1, 2);
        layout->addWidget(editQ, 1, 3);
        layout->addWidget(labelPrivateKey, 2, 0);
        layout->addWidget(editPrivateKey, 2, 1);
        layout->addWidget(labelPublicKey, 2, 2);
        layout->addWidget(editPublicKey, 2, 3);
        layout->addWidget(btnRandom, 3, 0, 1, 2);
        layout->addWidget(btnGenerateKey, 3, 2, 1, 2);

        layout->addWidget(labelSignText, 4, 0, 1, 4);
        layout->addWidget(labelText, 5, 0);
        layout->addWidget(editText, 5, 1, 1, 2);
        layout->addWidget(btnLoadFile, 5, 3);
        layout->addWidget(labelSignature, 6, 0);
        layout->addWidget(editSignResult, 6, 1, 1, 2);
        layout->addWidget(btnSign, 6, 3);
        layout->addWidget(btnSaveFile, 7, 3);

        layout->addWidget(labelVerify, 8, 0, 1, 4);
        layout->addWidget(new QLabel("Văn bản kiểm tra"), 9, 0);
        layout->addWidget(editVerifyText, 9, 1, 1, 2);
        layout->addWidget(btnLoadVerifyFile, 9, 3);
        layout->addWidget(new QLabel("Chữ ký kiểm tra"), 10, 0);
        layout->addWidget(editVerifySignature, 10, 1, 1, 2);
        layout->addWidget(btnLoadSignature, 10, 3);
        layout->addWidget(new QLabel("Kết quả kiểm tra"), 11, 0);
        layout->addWidget(editVerifyResult, 11, 1, 1, 3);
        layout->addWidget(btnVerify, 12, 3);

        setLayout(layout);

        // Connect signals and slots
        connect(btnRandom, &QPushButton::clicked, this, &MainWindow::generateRandomNumbers);
        connect(btnGenerateKey, &QPushButton::clicked, this, &MainWindow::generateKeys);
        connect(btnLoadFile, &QPushButton::clicked, this, &MainWindow::loadFile);
        connect(btnSign, &QPushButton::clicked, this, &MainWindow::signText);
        connect(btnSaveFile, &QPushButton::clicked, this, &MainWindow::saveSignature);
        connect(btnLoadSignature, &QPushButton::clicked, this, &MainWindow::loadSignature);
        connect(btnLoadVerifyFile, &QPushButton::clicked, this, &MainWindow::loadVerifyFile);
        connect(btnVerify, &QPushButton::clicked, this, &MainWindow::verifyText);
    }

private:
    bool isPrime(int num) {
        if (num <= 1) return false;
        for (int i = 2; i <= std::sqrt(num); ++i) {
            if (num % i == 0) return false;
        }
        return true;
    }

    int gcd(int a, int b) {
        while (b != 0) {
            int temp = b;
            b = a % b;
            a = temp;
        }
        return a;
    }

    int modExp(int base, int exp, int mod) {
        int result = 1;
        while (exp > 0) {
            if (exp % 2 == 1) {
                result = (result * base) % mod;
            }
            base = (base * base) % mod;
            exp /= 2;
        }
        return result;
    }

    int modInverse(int a, int m) {
        a = a % m;
        for (int x = 1; x < m; x++) {
            if ((a * x) % m == 1) {
                return x;
            }
        }
        return -1;
    }

    void generateRandomNumbers() {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dist(2, 100);

        int p, q;
        do {
            p = dist(gen);
        } while (!isPrime(p));

        do {
            q = dist(gen);
        } while (!isPrime(q) || q == p);

        editP->setText(QString::number(p));
        editQ->setText(QString::number(q));
    }

    void generateKeys() {
        if (editP->text().isEmpty() || editQ->text().isEmpty()) {
            QMessageBox::warning(this, "Lỗi", "Vui lòng nhập số p và q!");
            return;
        }
        int p = editP->text().toInt();
        int q = editQ->text().toInt();

        if (!isPrime(p) || !isPrime(q)) {
            QMessageBox::warning(this, "Lỗi", "Cả hai số p và q phải là số nguyên tố!");
            return;
        }

        int n = p * q;
        int phi = (p - 1) * (q - 1);

        int e;
        do {
            e = 3 + rand() % (phi - 3);
        } while (gcd(e, phi) != 1);

        int d = modInverse(e, phi);
        if (d == -1) {
            QMessageBox::warning(this, "Lỗi", "Không thể tạo khóa!");
            return;
        }

        editPrivateKey->setText(QString::number(d));
        editPublicKey->setText(QString::number(e));
        nValue = n; // Store n for further operations
    }

    void loadFile() {
        QString fileName = QFileDialog::getOpenFileName(this, "Mở tệp văn bản", "", "Text Files (*.txt);;All Files (*)");
        if (!fileName.isEmpty()) {
            QFile file(fileName);
            if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
                editText->setText(file.readAll());
                file.close();
            } else {
                QMessageBox::warning(this, "Lỗi", "Không thể mở tệp!");
            }
        }
    }

    void loadVerifyFile() {
        QString fileName = QFileDialog::getOpenFileName(this, "Mở tệp văn bản kiểm tra", "", "Text Files (*.txt);;All Files (*)");
        if (!fileName.isEmpty()) {
            QFile file(fileName);
            if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
                editVerifyText->setText(file.readAll());
                file.close();
            } else {
                QMessageBox::warning(this, "Lỗi", "Không thể mở tệp!");
            }
        }
    }

    void signText() {
        if (editText->text().isEmpty() || editPrivateKey->text().isEmpty()) {
            QMessageBox::warning(this, "Lỗi", "Vui lòng nhập văn bản và khóa bí mật!");
            return;
        }

        QString inputText = editText->text();
        int d = editPrivateKey->text().toInt();
        QString signedText;
        for (QChar c : inputText) {
            int m = c.unicode();
            int s = modExp(m, d, nValue);
            signedText.append(QString::number(s) + " ");
        }
        editSignResult->setText(signedText.trimmed());
    }

    void saveSignature() {
        QString fileName = QFileDialog::getSaveFileName(this, "Lưu chữ ký", "", "Text Files (*.txt);;All Files (*)");
        if (!fileName.isEmpty()) {
            QFile file(fileName);
            if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
                QTextStream out(&file);
                out << editSignResult->text();
                file.close();
                QMessageBox::information(this, "Thành công", "Chữ ký đã được lưu!");
            } else {
                QMessageBox::warning(this, "Lỗi", "Không thể lưu tệp!");
            }
        }
    }

    void loadSignature() {
        QString fileName = QFileDialog::getOpenFileName(this, "Mở chữ ký", "", "Text Files (*.txt);;All Files (*)");
        if (!fileName.isEmpty()) {
            QFile file(fileName);
            if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
                editVerifySignature->setText(file.readAll());
                file.close();
            } else {
                QMessageBox::warning(this, "Lỗi", "Không thể mở tệp chữ ký!");
            }
        }
    }

    void verifyText() {
        if (editVerifyText->text().isEmpty() || editVerifySignature->text().isEmpty() || editPublicKey->text().isEmpty()) {
            QMessageBox::warning(this, "Lỗi", "Vui lòng nhập văn bản, chữ ký và khóa công khai!");
            return;
        }

        QStringList signedValues = editVerifySignature->text().split(" ");
        int e = editPublicKey->text().toInt();
        QString verifiedText;

        for (const QString &value : signedValues) {
            int s = value.toInt();
            int m = modExp(s, e, nValue);
            verifiedText.append(QChar(m));
        }

        if (verifiedText == editVerifyText->text()) {
            editVerifyResult->setText("Hợp lệ");
        } else {
            editVerifyResult->setText("Không hợp lệ");
        }
    }

private:
    QLineEdit *editP;
    QLineEdit *editQ;
    QLineEdit *editPrivateKey;
    QLineEdit *editPublicKey;
    QLineEdit *editText;
    QLineEdit *editSignResult;
    QLineEdit *editVerifyText;
    QLineEdit *editVerifySignature;
    QLineEdit *editVerifyResult;

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
