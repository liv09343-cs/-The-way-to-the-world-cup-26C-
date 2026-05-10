#ifndef RESULTWINDOW_H
#define RESULTWINDOW_H

#include <QDialog>
#include <QString>

namespace Ui {
class ResultWindow;
}

class ResultWindow : public QDialog
{
    Q_OBJECT

public:
    explicit ResultWindow(QWidget *parent = nullptr);
    ~ResultWindow();

    void setResult(const QString &title, const QString &message, const QString &teamName);

private slots:
    void on_okButton_clicked();

signals:
    void okClicked();

private:
    Ui::ResultWindow *ui;
};

#endif // RESULTWINDOW_H