#include "mainwindow.h"
#include "table.h"

#include <QtWidgets>

MainWindow::MainWindow(QWidget *parent, QString path)
    : QMainWindow(parent)
{
    setWindowTitle("Viatore di vie");
    QWidget *center = new QWidget(this);
    QVBoxLayout *centerLayout = new QVBoxLayout(center);
    center->setLayout(centerLayout);

    setCentralWidget(center);

    /* ------------------------------------------------------------ */

    QWidget *alto = new QWidget(this);
    QHBoxLayout *altoLayout = new QHBoxLayout(alto);
    alto->setLayout(altoLayout);

    const QString liguria = "Liguria";
    const QStringList liguriaProvince = {"GE", "IM", "SP", "SV"};
    altoLayout->addWidget(regioneWidget(alto, liguria, liguriaProvince, checkboxes));

    const QString piemonte = "Piemonte";
    const QStringList piemonteProvince = {"AL", "AT", "BI", "CN", "NO", "TO", "VB", "VC"};
    altoLayout->addWidget(regioneWidget(alto, piemonte, piemonteProvince, checkboxes));

    const QString lombardia = "Lombardia";
    const QStringList lombardiaProvince = {"BG", "BS", "CO", "CR", "LC", "LO", "MB", "MI", "MN", "PV", "SO", "VA"};
    altoLayout->addWidget(regioneWidget(alto, lombardia, lombardiaProvince, checkboxes));

    centerLayout->addWidget(alto);

    /* ------------------------------------------------------------ */

    QStringList allProvince;
    allProvince.append(liguriaProvince);
    allProvince.append(piemonteProvince);
    allProvince.append(lombardiaProvince);

    vieAll = readFiles(path, allProvince);

    /* ------------------------------------------------------------ */

    QTableView *basso = new QTableView(this);
    TheProxy *proxy = new TheProxy(this);
    model = new TheModel(this);

    proxy->setSourceModel(model);
    proxy->setDynamicSortFilter(true);
    basso->setModel(proxy);
    basso->setSortingEnabled(true);
    basso->sortByColumn(0, Qt::AscendingOrder);

    basso->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);
    basso->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    basso->horizontalHeader()->setSectionsMovable(true);
    //basso->horizontalHeader()->setSectionsClickable(true);

    centerLayout->addWidget(basso);

    /* ------------------------------------------------------------ */

    QLabel *copyright = new QLabel("Dati della tabella © OpenStreetMap contributors", this);

    centerLayout->addWidget(copyright);

    resize(800, 800);
}

QGroupBox *MainWindow::regioneWidget(QWidget *parent, const QString &regione, const QStringList &province, QList<QCheckBox *> &cbs)
{
    QGroupBox *regionatore = new QGroupBox(regione, parent);
    QVBoxLayout *layout = new QVBoxLayout(regionatore);

    regionatore->setLayout(layout);
    layout->setAlignment(Qt::AlignTop);

    for(QString provincia : province) {
        QCheckBox *cb = new QCheckBox(provincia, regionatore);
        connect(cb, SIGNAL(stateChanged(int)), this, SLOT(checkboxChanged()));
        layout->addWidget(cb);
        cbs.append(cb);
    }

    return regionatore;
}

QHash<QString, QList<Via> > MainWindow::readFiles(QString &path, QStringList &province)
{
    QHash<QString, QList<Via> > result;

    for(QString &provincia : province) {
        QList<Via> list;
        QFile file(path + "/vie_" + provincia + ".csv");
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            qDebug() << "Failed to open " + file.fileName() << Qt::endl;
            continue;
        }

        while (!file.atEnd()) {
            QByteArray line = file.readLine();
            QStringList parts = QString(line).trimmed().split(',');
            Via via(parts.at(0), parts.at(1), parts.at(2), parts.at(3));
            list.append(via);
        }
        result.insert(provincia, list);
    }

    return result;
}

void MainWindow::clearVie()
{
    vieFiltered.clear();
    vieNames.clear();
    vieDuplicates.clear();
}

void MainWindow::addPotentialDuplicate(Via &via)
{
    if(vieNames.contains(via.getName())) {
        vieDuplicates.append(via.getName());
    } else {
        vieNames.append(via.getName());
    }
}

void MainWindow::searchDuplicates(QString provincia)
{
    for(Via via : vieAll.value(provincia)) {
        addPotentialDuplicate(via);
    }
}

void MainWindow::checkboxChanged()
{
    clearVie();

    for(QCheckBox* cb : checkboxes) {
        if(cb->isChecked()) {
            QString provincia = cb->text();
            vieFiltered.append(vieAll.value(provincia));
            searchDuplicates(provincia);
        }
    }

    model->setVie(vieFiltered, vieDuplicates);
}

MainWindow::~MainWindow()
{
}

