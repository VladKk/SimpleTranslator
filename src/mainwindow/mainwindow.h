#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
Q_OBJECT;

public:
    explicit MainWindow(QWidget *parent = nullptr);

    ~MainWindow() final;

private slots:

    void actions();

    void translate();

private:
    Ui::MainWindow *ui;

    bool checkComboBox();

    static QString translation(const QString &keyword, const QString &from, const QString &to);
};

#endif //MAINWINDOW_H
