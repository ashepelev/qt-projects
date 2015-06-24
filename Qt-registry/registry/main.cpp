#include "mainwindow.h"
#include "registrymodel.h"
#include <QApplication>

#include <QTreeView>
#include <QSplitter>
#include <QTranslator>
#include <QMessageBox>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    bool bTranslationLoaded = false;
    QTranslator translator;
    const char
    *
    aszTranslationDirs[] = {".", "../ translations ", ":/ translations ", 0};
    for (const char** ppszDir = aszTranslationDirs; *ppszDir != 0; ++ ppszDir)
    {
        bTranslationLoaded = translator.load("registry_ru", *ppszDir);
        if (bTranslationLoaded)
            break;
    }
    if (bTranslationLoaded)
        a.installTranslator(&translator);
    else
        QMessageBox::warning(0, " Translation ", "Not loaded ");
    MainWindow w;
    w.show();

    return a.exec();
}
