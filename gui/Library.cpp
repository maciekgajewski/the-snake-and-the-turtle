#include "Library.hpp"

#include <QDir>
#include <QRegExp>

namespace Turtle {

static const QString PREFIX("tdemo_");

Library::Library()
{
}

void Library::load(const QString &path)
{
    QDir root(path);

    _data.clear();

    QFileInfoList categories = root.entryInfoList(QDir::Dirs, QDir::Name);

    Q_FOREACH(const QFileInfo& info, categories)
    {
        if (info.baseName().startsWith(PREFIX))
        {
            loadCategory(info);
        }
    }
}

void Library::loadCategory(const QFileInfo &info)
{
    Q_ASSERT(info.isDir());
    Q_ASSERT(info.baseName().startsWith(PREFIX));

    QString name = info.baseName().right(info.baseName().length() - PREFIX.length());

    QDir d(info.filePath());

    QFileInfoList pythonFiles = d.entryInfoList(QStringList() << "*.py", QDir::Files, QDir::Name);

    ItemDict dict;
    Q_FOREACH(const QFileInfo& i, pythonFiles)
    {
        if (i.baseName().startsWith(PREFIX))
        {
            LibraryItem item = loadItem(i);
            dict.insert(item.name(), item);
        }
    }

    _data.insert(name, dict);
}

LibraryItem Library::loadItem(const QFileInfo &info)
{

    // path
    QString path = info.absoluteFilePath();

    // name
    QString name = info.baseName().right(info.baseName().length() - PREFIX.length());

    // descritpion
    QString description;
    QRegExp descriptionRegexp("\"\"\"(.+)\"\"\"");

    QFile file(path);
    file.open(QFile::ReadOnly);
    QString content = QString::fromUtf8(file.readAll());

    if (content.indexOf(descriptionRegexp) != -1)
    {
        description = descriptionRegexp.cap(1);
    }

    // image
    // TODO

    return LibraryItem(path, name, description, QIcon());
}

} // namespace Turtle
