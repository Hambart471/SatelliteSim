#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // Стиль: тёмная тема, синий акцент
    qApp->setStyleSheet(R"(
        QWidget {
            background-color: #111111;
            color: #EEEEEE;
            font-family: Roboto, Arial, sans-serif;
            font-size: 12pt;
        }
        QPushButton {
            background-color: #005288;
            color: #FFFFFF;
            border-radius: 5px;
            padding: 6px 12px;
        }
        QPushButton:hover {
            background-color: #0069C0;
        }
        QLineEdit, QTextEdit {
            background-color: #1A1A1A;
            border: 1px solid #333333;
            border-radius: 3px;
        }
    )");

    MainWindow w;
    w.show();
    return a.exec();
}
