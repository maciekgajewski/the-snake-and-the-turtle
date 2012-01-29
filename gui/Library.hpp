#ifndef TURTLE_LIBRARY_HPP
#define TURTLE_LIBRARY_HPP

#include <QMetaType>
#include <QIcon>
#include <QString>
#include <QFileInfo>
#include <QMap>


namespace Turtle {

class LibraryItem
{
public:

    LibraryItem()
    { }

    LibraryItem(const QString& path, const QString& name, const QString& description, const QIcon& icon)
        : _path(path), _name(name), _description(description), _icon(icon)
    { }

    QString filePath() const { return _path; }
    QString description() const { return _description; }
    QIcon icon() const { return _icon; }
    QString name() const { return _name; }

    bool isNull() const { return _path.isNull(); }

private:

    QString _path;
    QString _name;
    QString _description;
    QIcon _icon;
};


class Library
{
public:
    Library();

    void load(const QString& path);

    QStringList categories() const { return _data.keys(); }
    QStringList itemsInCategory(const QString& category) const { return _data[category].keys(); }
    LibraryItem item(const QString& category, const QString& item) const { return _data[category][item]; }

private:

    void loadCategory(const QFileInfo& info);
    LibraryItem loadItem(const QFileInfo& info);

    typedef QMap<QString, LibraryItem> ItemDict;
    typedef QMap<QString, ItemDict> CategoryDict;

    CategoryDict _data;
};

} // namespace Turtle

Q_DECLARE_METATYPE(Turtle::LibraryItem)

#endif // TURTLE_LIBRARY_HPP
