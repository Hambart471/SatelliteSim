#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>

// Макросы QT_BEGIN_NAMESPACE/QT_END_NAMESPACE упрощают, если Qt собран в собственном неймспейсе
// (обычно это просто namespace Ui { … })
QT_BEGIN_NAMESPACE
// Здесь мы объявляем вперёд (forward declaration) класс Ui::MainWindow, который генерируется из
// mainwindow.ui инструментом uic
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    // автоматически свяжется с сигналом clicked() кнопки startButton, если в Qt Designer вы
    // задали имя объекта startButton и создали слот с таким именем
    void on_startButton_clicked();
    // слот на сигнал connected() от QTcpSocket, срабатывает, когда соединение по TCP установлено
    void onSocketConnected();
    // слот на сигнал errorOccurred(...), который передаёт причину ошибки сетевого соединения
    void onSocketError(QAbstractSocket::SocketError);

private:
    // указатель на сгенерированный класс, который содержит все виджеты из mainwindow.ui
    Ui::MainWindow *ui;
    // указатель на объект TCP-сокета, через который мы будем отправлять/принимать данные
    QTcpSocket *socket;

    // Вспомогательные методы
    // Приватный метод, который добавляет строку msg в окно логов с таймштампом
    void logMessage(const QString &msg);
    // проверяет, в норме ли температура и заряд, и возвращает строку "Норма" или "Авария", а при
    // ненорме дополнительно вызывает logMessage
    QString checkState(double temp, double charge);
    // вычисляет и возвращает время (в секундах) до полного заряда батареи по формуле
    // (100 − currentCharge) / 0.37
    double calcChargeTime(double currentCharge);
};

#endif // MAINWINDOW_H
