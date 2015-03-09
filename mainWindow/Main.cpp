#include <QApplication>
#include <QtGui>
#include <QDir>
#include <QWidget>
#include <QBoxLayout>
#include <QTranslator>
#include <QStyleFactory>
#include "MainWindow.h"

// ----------------------------------------------------------------------
int main( int argC, char** argV )
{
    QApplication app( argC, argV );

	QTranslator *translator = 0;

    QStringList files = QDir( "./lang" ).entryList( QDir::Files );
    files.sort();
    int countFiles = files.size();

    for( int i = 0; i < countFiles; i++ )
    {
		translator = new QTranslator( &app );
		QString fileName = "./lang/" + files[i];

		if( !translator->load( fileName ) )
		{
			delete translator;
			continue;
		}

        app.installTranslator( translator );
    }
    app.addLibraryPath( "./plugins" );

    QApplication::setStyle( QStyleFactory::create("Skulpture") );
    QFile file( "./style/style.qss" );
    file.open( QFile::ReadOnly );
    QString styleSheet = QLatin1String( file.readAll() );
    app.setStyleSheet( styleSheet );
    mainWindow main;
	main.showMaximized();

    return app.exec();
}
