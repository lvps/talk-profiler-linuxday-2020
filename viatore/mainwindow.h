#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "table.h"
#include <QMainWindow>
#include <QtWidgets>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent, QString path);
    ~MainWindow();

private slots:
    void checkboxChanged();

private:
    QGroupBox *regioneWidget(QWidget *parent, const QString &regione, const QStringList &province, QList<QCheckBox *> &cbs);
    QHash<QString, QList<Via> > readFiles(QString &path, QStringList &province);
    void clearVie();
    void searchDuplicates(QString provincia);
    void addPotentialDuplicate(Via &via);

    QList<QCheckBox *> checkboxes;
    TheModel *model;

    QList<Via> vieFiltered;
    QList<QString> vieNames;
    QList<QString> vieDuplicates;
    QHash<QString, QList<Via> > vieAll;
};
#endif // MAINWINDOW_H
