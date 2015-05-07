#include "PWE.h"
#include "GPropertiesObj.h"
#include "PaintWidget.h"
#include <QDebug>

PaintWidgetEditor::PaintWidgetEditor(PaintWidget &paintWidget):
					_paintWidget( paintWidget ),
					updateViews( true )
{
	ui.setupUi( this );

	colorBox =  new ColorBox( this, _paintWidget.viewportColor );
	QVBoxLayout *layout = new QVBoxLayout( ui.brushTab );
	QWidget *brushEditor = GET_EDITOR( paintWidget.painter.background );
	if(brushEditor!=0)
	{
		brushEditor->setParent( ui.brushTab );
		layout->addWidget( brushEditor );
	}

	colorBox->setParent( ui.windowBgOptions );
	ui.hboxLayout->addWidget( colorBox );

	connect( colorBox, SIGNAL( colorChanged() ), this, SLOT( onColorChanged() ) );
    connect( ui.colorFillBg, SIGNAL( clicked( bool ) ), this, SLOT( onColorFillBgButton( bool ) ) );
    connect( ui.transparencyFillBg, SIGNAL( clicked( bool ) ), this, SLOT( onTransparencyFillBgButton( bool ) ) );
    connect( ui.sizeHintStyle, SIGNAL( clicked( bool ) ), this, SLOT( onSizeHintStyle( bool ) ) );
    connect( ui.fullWindowStyle, SIGNAL( clicked( bool ) ), this, SLOT( onFullWindowStyle( bool ) ) );
    connect( ui.FixedSizeStyle, SIGNAL( clicked( bool ) ), this, SLOT( onFixedSizeStyle( bool ) ) );
    connect( ui.viewportWidth, SIGNAL( activated( const QString& ) ), this, SLOT( onViewportWidthChanged( const QString& ) ) );
    connect( ui.viewportWidth, SIGNAL( editTextChanged( const QString& ) ), this, SLOT( onViewportWidthEdited( const QString& ) ) );
    connect( ui.viewportHeight, SIGNAL( activated( const QString& ) ), this, SLOT( onViewportHeightChanged( const QString& ) ) );
    connect( ui.viewportHeight, SIGNAL( editTextChanged( const QString& ) ), this, SLOT( onViewportHeightEdited( const QString& ) ) );

	syncWithPainter();
}

PaintWidgetEditor::~PaintWidgetEditor()
{
}

void PaintWidgetEditor::onBackgroundChanged( QWidget *from )
{
	if( from == this )
		return;

	syncWithPainter();
	colorBox->repaint();
}

void PaintWidgetEditor::onColorChanged()
{
	_paintWidget.setViewportColor( _paintWidget.viewportColor );
	updateBackgroundViews();
}

void PaintWidgetEditor::onColorFillBgButton( bool checked )
{
	_paintWidget.setViewportColor( _paintWidget.viewportColor );
	colorBox->setEnabled( true );
	updateBackgroundViews();
}

void PaintWidgetEditor::onTransparencyFillBgButton( bool checked )
{
	_paintWidget.doViewportTransparent();
	colorBox->setEnabled( false );
	updateBackgroundViews();
}

void PaintWidgetEditor::onSizeHintStyle( bool checked )
{
	_paintWidget.setViewportType( PaintWidget::hintViewport );
	ui.viewportWidth->setEnabled( true );
	ui.viewportHeight->setEnabled( true );
	updateBackgroundViews();
}

void PaintWidgetEditor::onFullWindowStyle( bool checked )
{
	_paintWidget.setViewportType( PaintWidget::resizableViewport );
	ui.viewportWidth->setEnabled( false );
	ui.viewportHeight->setEnabled( false );
	updateBackgroundViews();
}

void PaintWidgetEditor::onFixedSizeStyle( bool checked )
{
	_paintWidget.setViewportType( PaintWidget::fixedViewport );
	ui.viewportWidth->setEnabled( true );
	ui.viewportHeight->setEnabled( true );
	updateBackgroundViews();
}

void PaintWidgetEditor::onViewportWidthChanged( const QString &text )
{
	if( ( !updateViews ) || ( text.isNull() ) ) return;

	updateViews = false;
	bool isInt = false;
	int val = text.toInt( &isInt );
	if( ( !isInt ) || ( val < 1 ) || ( val > 4096 ) )
		ui.viewportWidth->lineEdit()->setText( "1" );
	updateViews = true;
}

void PaintWidgetEditor::onViewportWidthEdited( const QString &text )
{
	if( ( !updateViews ) || ( text.isNull() ) ) return;

	updateViews = false;
	bool isInt = false;
	int val = text.toInt( &isInt );
	if( ( !isInt ) || ( val < 1 ) || ( val > 4096 ) )
	{
		ui.viewportWidth->lineEdit()->setText( "1" );
		val = 1;
	}

    _paintWidget.setViewportFixedSize( QSize( val, ui.viewportHeight->currentText().toInt( &isInt ) ) );
	updateViews = true;
	updateBackgroundViews();
}

void PaintWidgetEditor::onViewportHeightChanged( const QString &text )
{
	if( ( !updateViews ) || ( text.isNull() ) ) return;

	updateViews = false;
	bool isInt = false;
	int val = text.toInt( &isInt );
	if( ( !isInt ) || ( val < 1 ) || ( val > 4096 ) )
		ui.viewportHeight->lineEdit()->setText( "1" );
	updateViews = true;
}

void PaintWidgetEditor::onViewportHeightEdited( const QString &text )
{
	if( ( !updateViews ) || ( text.isNull() ) ) return;

	updateViews = false;
	bool isInt = false;
	int val = text.toInt( &isInt );

	if( ( !isInt ) || ( val < 1 ) || ( val > 4096 ) )
	{
		ui.viewportHeight->lineEdit()->setText( "1" );
		val = 1;
	}

	_paintWidget.setViewportFixedSize( QSize(ui.viewportWidth->currentText().toInt( &isInt ), val ) );
	updateViews = true;
	updateBackgroundViews();
}

void PaintWidgetEditor::destroy()
{
	delete this;
}

void PaintWidgetEditor::updateBackgroundViews()
{
	if( updateViews )
		_paintWidget.updateAllViews( this );
}

void PaintWidgetEditor::syncWithPainter()
{
	if( _paintWidget.transparent )
	{
		ui.colorFillBg->setChecked( false );
		ui.transparencyFillBg->setChecked( true );
		colorBox->setEnabled( false );
	}
	else
	{
		ui.colorFillBg->setChecked( true );
		ui.transparencyFillBg->setChecked( false );
		colorBox->setEnabled( true );
	}

	switch( _paintWidget.viewportType() )
	{
		case PaintWidget::hintViewport:
			ui.sizeHintStyle->setChecked( true );
			ui.viewportWidth->setEnabled( true );
			ui.viewportHeight->setEnabled( true );
			break;

		case PaintWidget::fixedViewport:
			ui.FixedSizeStyle->setChecked( true );
			ui.viewportWidth->setEnabled( true );
			ui.viewportHeight->setEnabled( true );
			break;

		case PaintWidget::resizableViewport:
			ui.fullWindowStyle->setChecked( true );
			ui.viewportWidth->setEnabled( false );
			ui.viewportHeight->setEnabled( false );
			break;
	}

	QSize viewport = _paintWidget.viewportFixedSize();
	ui.viewportWidth->setEditText( QString( "%1" ).arg( viewport.width() ) );
	ui.viewportHeight->setEditText( QString( "%1" ).arg( viewport.height() ) );
}

QObject* PaintWidgetEditor::getColorBox()
{
	return this->colorBox;
}

void PaintWidgetEditor::setColorBox(QObject clb)
{

}

QObject* PaintWidgetEditor::getPaintWidget()
{
	return &_paintWidget;
}

void PaintWidgetEditor::setPaintWidget(QObject pw)
{

}

bool PaintWidgetEditor::getUpdateViews()
{
	return this->updateViews;
}

void PaintWidgetEditor::setUpdateViews(bool uv)
{

}

Ui::viewportPropsForm PaintWidgetEditor::getUI()
{
	return this->ui;
}
