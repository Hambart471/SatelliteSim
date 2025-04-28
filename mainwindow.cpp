#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDateTime>
#include <QHostAddress>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , socket(new QTcpSocket(this))
{
    ui->setupUi(this);

    // Подключаем слоты для TCP
    connect(socket, &QTcpSocket::connected, this, &MainWindow::onSocketConnected);
    connect(socket,
            &QTcpSocket::errorOccurred,
            this,
            &MainWindow::onSocketError);
}

MainWindow::~MainWindow()
{
    delete ui;
}

// Слот: нажали кнопку «Начать симуляцию»
void MainWindow::on_startButton_clicked()
{
    // Считываем данные из полей
    double temp = ui->tempEdit->text().toDouble();
    double charge = ui->chargeEdit->text().toDouble();
    double speed = ui->speedEdit->text().toDouble();
    double x = ui->xEdit->text().toDouble();
    double y = ui->yEdit->text().toDouble();
    double z = ui->zEdit->text().toDouble();

    ui->resultEdit->clear();

    // Проверяем состояние
    QString state = checkState(temp, charge);
    logMessage(QString("Состояние спутника: %1").arg(state));

    // Вычисляем время до полной зарядки
    double secondsToFull = calcChargeTime(charge);
    logMessage(QString("Время до полной зарядки: %1 секунд").arg(secondsToFull));

    // Формируем строку для отправки по TCP
    QString data = QString("\n\tSTATE: %1\n\tPOS: %2,%3,%4\n\tSPD: %5\n\tTEMP: %6\n\tCHG: %7")
                       .arg(state)
                       .arg(x).arg(y).arg(z)
                       .arg(speed)
                       .arg(temp)
                       .arg(charge);
    logMessage(QString("Подготовка к отправке: %1").arg(data));

    // Подключаемся к локальному приёмнику (127.0.0.1:12345)
    socket->connectToHost(QHostAddress::LocalHost, 12345);
}

// Слот: успешно подключились
void MainWindow::onSocketConnected()
{
    QString data = ui->resultEdit->toPlainText().split('\n').last(); // последний лог как пример
    socket->write(data.toUtf8());
    socket->flush();
    logMessage("Данные отправлены по TCP");
    socket->disconnectFromHost();
}

// Слот: ошибка сокета
void MainWindow::onSocketError(QAbstractSocket::SocketError)
{
    logMessage(QString("Ошибка TCP: %1").arg(socket->errorString()));
}

// Логирование в поле resultEdit
void MainWindow::logMessage(const QString &msg)
{
    QString time = QDateTime::currentDateTime().toString("hh:mm:ss");
    ui->resultEdit->appendPlainText(QString("[%1]\t%2").arg(time).arg(msg));
}

// Проверяем нормальность параметров
QString MainWindow::checkState(double temp, double charge)
{
    bool tempOk = (temp >= -300 && temp <= 3000);
    bool chargeOk = (charge >= 20 && charge <= 100);
    if (!tempOk)
        logMessage("! Предупреждение: температура вне нормы");
    if (!chargeOk)
        logMessage("! Предупреждение: уровень заряда критический");
    return (tempOk && chargeOk) ? "Норма" : "Авария";
}

// Расчёт времени до полной зарядки (сек)
double MainWindow::calcChargeTime(double currentCharge)
{
    const double rate = 0.37; // % в секунду
    return (100.0 - currentCharge) / rate;
}
