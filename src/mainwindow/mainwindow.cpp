#include <QDebug>
#include <QMessageBox>
#include <QNetworkAccessManager>
#include <QNetworkReply>

#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
        QMainWindow(parent),
        ui(new Ui::MainWindow) {
    setWindowIcon(QIcon(":icons/simpletranslator.ico"));

    ui->setupUi(this);

    connect(ui->actionAbout_developer, SIGNAL(triggered()), this, SLOT(actions()));
    connect(ui->actionAbout_program, SIGNAL(triggered()), this, SLOT(actions()));
    connect(ui->actionExit, SIGNAL(triggered()), this, SLOT(actions()));
    connect(ui->actionRefresh, SIGNAL(triggered()), this, SLOT(actions()));

    connect(ui->buttonTranslate, SIGNAL(clicked()), this, SLOT(translate()));
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::actions() {
    auto *action = (QAction *) sender();

    if (action == ui->actionAbout_program) {
        QMessageBox::information(this, "SimpleTranslator",
                                 "This is my first Qt project\n"
                                 "Tried to use everything I know\n"
                                 "May have a lot of bugs and mistakes\n"
                                 "Used technologies:\n"
                                 "C++, Qt, CMake");
    } else if (action == ui->actionAbout_developer) {
        QMessageBox::information(this, "SimpleTranslator",
                                 "Find me in the Web!\n"
                                 "Telegram: @vlad_is_real\n"
                                 "GMail: vladislav.kolyadenko@gmail.com\n"
                                 "Instagram: @ncks_gwc");
    } else if (action == ui->actionRefresh) {
        ui->comboBoxFirstLang->setCurrentIndex(0);
        ui->comboBoxSecondLang->setCurrentIndex(0);

        ui->textBrowserFirstLang->setText("Enter text here");
        ui->textBrowserSecondLang->setText("Translation");
    } else if (action == ui->actionExit)
        QApplication::quit();
    else {
        qDebug() << "Invalid action!\n";

        QApplication::exit(EXIT_FAILURE);
    }
}

void MainWindow::translate() {
    if (checkComboBox()) {
        ui->statusBar->showMessage("It is pointless to translate from " + ui->comboBoxFirstLang->currentText() +
                                   " to " + ui->comboBoxSecondLang->currentText(), 5000);

        ui->comboBoxFirstLang->setCurrentIndex(0);
        ui->comboBoxSecondLang->setCurrentIndex(0);

        ui->textBrowserFirstLang->setText("Enter text here");
        ui->textBrowserSecondLang->setText("Translation");

        return;
    }

    ui->textBrowserSecondLang->setText(
            translation(ui->textBrowserFirstLang->toPlainText(), ui->comboBoxFirstLang->currentText(),
                        ui->comboBoxSecondLang->currentText()));
}

bool MainWindow::checkComboBox() {
    return (ui->comboBoxFirstLang->currentText() == ui->comboBoxSecondLang->currentText()) &&
           (ui->comboBoxFirstLang->currentIndex() && ui->comboBoxSecondLang->currentIndex());
}

QString MainWindow::translation(const QString &keyword, const QString &from, const QString &to) {
    QString url = QString(
            "http://translate.google.com/translate_a/t?client=t&text=%0&hl=%1&sl=%2&tl=%1&multires=1&prev=enter&oc=2&ssel=0&tsel=0&uptl=%1&sc=1").arg(
            keyword).arg(to).arg(from);

    QNetworkAccessManager manager;
    QNetworkRequest request(url);
    QNetworkReply *reply = manager.get(request);

    do
        QCoreApplication::processEvents(QEventLoop::ExcludeUserInputEvents);
    while (!reply->isFinished());

    QString translation(reply->readAll());
    reply->close();

    delete reply;

    translation = translation.replace("[[[\"", "");
    translation = translation.mid(0, translation.indexOf(",\"") - 1);

    return translation;
}