#ifndef TURTLE_LIBRARYBROWSER_HPP
#define TURTLE_LIBRARYBROWSER_HPP

#include "Library.hpp"

#include <QWidget>

class QTreeWidgetItem;

namespace Turtle {

namespace Ui {
    class LibraryBrowser;
}

class LibraryBrowser : public QWidget
{
    Q_OBJECT
    
public:
    explicit LibraryBrowser(QWidget *parent = 0);
    ~LibraryBrowser();

Q_SIGNALS:

    void fileSelected(const QString& path);


private Q_SLOTS:

    void on_documentView_itemClicked(QTreeWidgetItem *item, int column);

    void on_loadButton_clicked();

private:
    Ui::LibraryBrowser *ui;

    Library _library;
    QString _path; // path of currenlty sleected item
};


} // namespace Turtle
#endif // TURTLE_LIBRARYBROWSER_HPP
