/**
* PluginDialog.h
*
*  Created on: 12.05.2009
*      Author: Kukushkin Alexey
*/

#ifndef PLUGINDIALOG_H_
#define PLUGINDIALOG_H_

#include <QDialog>
#include <QScrollArea>
#include <QDialogButtonBox>
#include <QLayout>
#include <QLabel>
#include <QPushButton>
#include <QWidget>
#include <QtXml>
#include <QtXmlPatterns>
#include <QMessageBox>

class PushButton:public QPushButton
{
	Q_OBJECT
	signals:
		void OnClickedButton(QPushButton * bt, QDomElement element);

	public:

		PushButton( QWidget *parent, QDomElement element):QPushButton( parent )
			,elem(element)
		{
			connect( this, SIGNAL( clicked( bool ) ),
					this, SLOT( onClicked() ) );
		}

	virtual ~PushButton(){};

	private slots:

		void onClicked()
		{
			emit OnClickedButton( this, elem );
		}

	private:
		QDomElement elem;
};

class PluginDialog: public QDialog
{
	Q_OBJECT

	public slots:
		void OnClickedButton(QPushButton * bt, QDomElement element)
		{
			if(element.attribute("included")=="true")
			{
				element.attributeNode("included").setValue("false");
				bt->setText(tr("Disabled"));
			}
			else
			{
				element.attributeNode("included").setValue("true");
				bt->setText(tr("Enabled"));
			}
		}

	public:

		bool writeXml(const QString &fileName,QDomDocument* pluginsDom)
		{
			QFile file(fileName);
			if (!file.open(QFile::WriteOnly | QFile::Text)) {

				return false;
			}

			int Indent = 4;
			QTextStream out(&file);
			pluginsDom->save(out,Indent);

			file.close();
			if (file.error()) {
				return false;
			}
			return true;
		}

		void addPluginsToDialog(QDomDocument* pluginsDom)
		{
			QFrame* frame;
			QHBoxLayout* hLayout;
			QSize newSize;
			QLabel* label;
			PushButton *turnOffOn;
			QWidget* widget = new QWidget(this);

			scrollArea->setFixedHeight(200);
			scrollArea->setWidget(widget);

			QVBoxLayout* vLayout= new QVBoxLayout( widget );

			QDomNodeList plugs = pluginsDom->elementsByTagName("plugin");

			for(int i=0; i<plugs.count(); i++)
			{
				QDomElement elm = plugs.item(i).toElement();

				hLayout= new QHBoxLayout();
				vLayout->addLayout(hLayout);

				label = new QLabel(this);
				label->setMinimumHeight(20);
				label->setText(elm.firstChildElement("Id").text());

				turnOffOn = new PushButton(this, elm);

				if(elm.attribute("included")=="true")
				{
					turnOffOn->setText(tr("Enabled"));
				}
				else
				{
					turnOffOn->setText(tr("Disabled"));
				}

				connect(turnOffOn, SIGNAL( OnClickedButton(QPushButton*, QDomElement)),
						this, SLOT( OnClickedButton(QPushButton*, QDomElement) ));

				hLayout->addWidget(label);
				hLayout->addWidget(turnOffOn);

				frame = new QFrame(this);
				frame->setFrameShape(QFrame::HLine);
				frame->setFrameShadow(QFrame::Sunken);
				vLayout->addWidget(frame);

				if( newSize.width() < (label->width() + turnOffOn->width()) )
					newSize.setWidth( label->width()  + turnOffOn->width() );

				newSize.setHeight(newSize.height() + label->height());

			}

			newSize += QSize( 52, 12 );
			scrollArea->setFixedWidth(newSize.width() + 20);
			widget->resize( newSize );


		}

        static void EnabledDialog( QWidget* _parent = 0, QDomDocument* pluginsDom = 0)
		{
			PluginDialog plugins(_parent);
			QDomDocument plDom = pluginsDom->cloneNode().toDocument();
			plugins.addPluginsToDialog(&plDom);
			if(plugins.exec() == Accepted)
			{
				plugins.writeXml("./plugins.xml",&plDom);
			}
		}

	private:

		PluginDialog(QWidget* _parent):QDialog(_parent),buttons( QDialogButtonBox::Ok, Qt::Horizontal, this )
		{

			this->setWindowTitle(tr("Plugins..."));

			scrollArea = new QScrollArea( this );
			main = new QVBoxLayout( this );
			QPalette pal = scrollArea->palette();
			pal.setBrush( scrollArea->backgroundRole(), QColor( 255, 255, 255 ) );
			scrollArea->setAutoFillBackground( true );
			scrollArea->setPalette( pal );
	        scrollArea->setAutoFillBackground(true);
	        scrollArea->setFrameShape(QFrame::StyledPanel);
	        scrollArea->setWidgetResizable(true);
			main->addWidget(scrollArea);
			main->addWidget(&buttons);

			connect( &buttons, SIGNAL( accepted() ), this, SLOT( accept() ) );
			connect( &buttons, SIGNAL( rejected() ), this, SLOT( reject() ) );


		}
		virtual ~PluginDialog(){};

		QVBoxLayout* main;
		QScrollArea *scrollArea;
		QDialogButtonBox buttons;

};

#endif /* PLUGINDIALOG_H_ */
