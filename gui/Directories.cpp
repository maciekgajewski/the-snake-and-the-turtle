#include "Directories.hpp"

#include <QSettings>
#include <QCoreApplication>

namespace Turtle {
namespace Directories {

static QString examples;

QString getExamplesDirectory()
{
    if (!examples.isNull())
        return examples;

    // look in the settings
    QSettings settings(QSettings::SystemScope, "TheSnakeAndTheTurtle", "gui");
    QVariant v = settings.value("examplespath");
    if (!v.isNull())
    {
        examples = v.toString();
        return examples;
    }

    // fall back to binary path

    examples = QCoreApplication::applicationDirPath() + "/examples";
    settings.setValue("examplespath", examples);
    return examples;
}

}
} // namespace Turtle
