#include "LibraryBrowser.hpp"
#include "gui/ui_LibraryBrowser.h"

#include "Directories.hpp"

#include <QTreeWidgetItem>

namespace Turtle {

LibraryBrowser::LibraryBrowser(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LibraryBrowser)
{
    ui->setupUi(this);

    // load library
    _library.load(Directories::getExamplesDirectory());

    // populate widget
    QList<QString> categories = _library._data.keys();
//    QStringList categories = _library.categories();
    Q_FOREACH(const QString& category, categories)
    {

        QTreeWidgetItem* categoryItem = new QTreeWidgetItem(ui->documentView, QStringList() << category);

        //Q_FOREACH(const QString& libItemName, _library.itemsInCategory(category))
        Q_FOREACH(const QString& libItemName, _library._data[category].keys())
        {
            //LibraryItem libItem = _library.item(category, libItemName);
            const LibraryItem& libItem = _library._data[category][libItemName];

            QTreeWidgetItem* libItemItem = new QTreeWidgetItem(categoryItem);
            libItemItem->setText(0, libItem.name());
            libItemItem->setIcon(0, libItem.icon());
            libItemItem->setData(0, Qt::UserRole, QVariant::fromValue(libItem));
        }
    }
}

LibraryBrowser::~LibraryBrowser()
{
    delete ui;
}

void LibraryBrowser::on_documentView_itemClicked(QTreeWidgetItem *item, int column)
{
    QVariant data = item->data(0, Qt::UserRole);
    if (!data.isNull())
    {
        LibraryItem libItem = data.value<LibraryItem>();
        ui->pictureLabel->setPixmap(libItem.icon().pixmap(QSize(200,200)));
        ui->descriptionField->setPlainText(libItem.description());

        _path = libItem.filePath();
        ui->loadButton->setEnabled(true);
    }
}

void LibraryBrowser::on_loadButton_clicked()
{
    Q_EMIT fileSelected(_path);
    close();
}

} // namespace Turtle



