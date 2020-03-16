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

    // connect all signals to slots
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
        // set combo box item to Choose the language
        ui->comboBoxFirstLang->setCurrentIndex(0);
        ui->comboBoxSecondLang->setCurrentIndex(0);

        // clear text
        ui->textBrowserFirstLang->setText("");
        ui->textBrowserSecondLang->setText("");
    } else if (action == ui->actionExit)
        QApplication::quit();
    else {
        qDebug() << "Invalid action!\n";

        QApplication::exit(EXIT_FAILURE);
    }
}

void MainWindow::translate() {
    // if there are the same languages on both combo boxes, then print warning and refresh
    if ((ui->comboBoxFirstLang->currentText() == ui->comboBoxSecondLang->currentText()) &&
        (ui->comboBoxFirstLang->currentIndex() && ui->comboBoxSecondLang->currentIndex())) {
        ui->statusBar->showMessage("It is pointless to translate from " + ui->comboBoxFirstLang->currentText() +
                                   " to " + ui->comboBoxSecondLang->currentText(), 5000);

        ui->actionRefresh->trigger();

        return;
    }

    // if the user didn't choose language, print the warning and refresh
    if (ui->comboBoxFirstLang->currentIndex() == 0 || ui->comboBoxSecondLang->currentIndex() == 0) {
        ui->statusBar->showMessage("You're not able to translate " + ui->comboBoxFirstLang->itemText(0), 5000);

        ui->actionRefresh->trigger();

        return;
    }

    // cast and parse all the text for link
    QString translatedText = parseText(ui->textBrowserFirstLang->toPlainText());
    QString langFrom = ui->comboBoxFirstLang->currentText() == "English" ? "en" : "ru";
    QString langTo = ui->comboBoxSecondLang->currentText() == "English" ? "en" : "ru";

    // print result
    ui->textBrowserSecondLang->setText(translation(translatedText, langFrom, langTo));
}

QString MainWindow::translation(const QString &keyword, const QString &from, const QString &to) {
    // set the link (set yours API key in mainwindow.h)
    QString url = QString("https://www.googleapis.com/language/translate/v2?key=%0&source=%1&target=%2&q=%3")
            .arg(API_KEY).arg(from).arg(to).arg(keyword);

    QNetworkAccessManager manager;
    QNetworkRequest request(url);
    QNetworkReply *reply = manager.get(request);

    // get reply
    do
        QCoreApplication::processEvents(QEventLoop::ExcludeUserInputEvents);
    while (!reply->isFinished());

    // convert to string
    QString translation(reply->readAll());
    reply->close();

    // free memory
    delete reply;

    return translation;
}

QString MainWindow::parseText(QString text) {
    // change all the spaces on pluses for link
    for (auto &ch : text) {
        if (ch == ' ')
            ch = '+';
    }

    return text;
}