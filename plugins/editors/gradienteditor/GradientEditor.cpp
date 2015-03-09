#include "GradientEditor.h"

void GradientEditor::createPlugin(QObject *parent, QString idParent,plugin::PluginsManager *manager)
{
	connect( this, SIGNAL( setActive( QString ) ), getBeep(), SIGNAL( setActive( QString ) ) );
	manager->addPlugins(this, "GradientEditor");
}

QString GradientEditor::getName()const
{
	return "GradientEditor";
}

QGradient GradientEditor::gradient() const
{
	return range->gradient();
}

void GradientEditor::setGradient( const QGradient &g )
{
	if( ( !forceUpdate ) && ( gradient() == g ) )
		return;

	bool oldValueOfHandleSignals = handleSignals;
	handleSignals = false;
	range->setGradient( g );
	stops->setStops( g.stops() );
	handleSignals = oldValueOfHandleSignals;

	switch( g.type() )
	{
		case QGradient::LinearGradient:
			ui.linearTypeButton->setChecked( true );
			break;

		case QGradient::RadialGradient:
			ui.radialTypeButton->setChecked( true );
			break;

		default:
			ui.conicalTypeButton->setChecked( true );
			break;
	}

	syncSpread( g );
	if( handleSignals )
		emit gradientChanged( gradient() );
}

GradientEditor::GradientEditor( plugin::PluginsManager *_manager )
{
	range = new GradientRangeEditor( this );
	stops = new GradientStopsEditor( 0 );

	ui.setupUi( this );
	ui.vboxLayout->addWidget( range );
	ui.vboxLayout->addWidget( ui.gradientOptions );

	QHBoxLayout *layout = new QHBoxLayout( ui.gradientColorsBox );
	stops->setParent( ui.gradientColorsBox );
	layout->addWidget( stops );

	onStopsChanged( stops->stops() );
	forceUpdate = true;
	setGradient( range->gradient() );
	forceUpdate = false;

	connect( stops, SIGNAL( stopsChanged( const QGradientStops & ) ),
			this, SLOT( onStopsChanged( const QGradientStops & ) ) );

	connect( range, SIGNAL( startPointChanged( const QPointF & ) ),
			this, SLOT( onStartStopPointChanged( const QPointF & ) ) );

	connect( range, SIGNAL( endPointChanged( const QPointF & ) ),
			this, SLOT( onStartStopPointChanged( const QPointF & ) ) );

	connect( ui.linearTypeButton, SIGNAL( clicked( bool ) ),
			this, SLOT( onLinearGradientSelected( bool ) ) );

	connect( ui.radialTypeButton, SIGNAL( clicked( bool ) ),
			this, SLOT( onRadialGradientSelected( bool ) ) );

	connect( ui.conicalTypeButton, SIGNAL( clicked( bool ) ),
			this, SLOT( onConicalGradientSelected( bool ) ) );

	connect( ui.padSpreadButton, SIGNAL( clicked( bool ) ),
			this, SLOT( onPadSpreadSelected( bool ) ) );

	connect( ui.repeatSpreadButton, SIGNAL( clicked( bool ) ),
			this, SLOT( onRepeatSpreadSelected( bool ) ) );

	connect( ui.reflectSpreadButton, SIGNAL( clicked( bool ) ),
			this, SLOT( onReflectSpreadSelected( bool ) ) );

	handleSignals = true;
}

GradientEditor::~GradientEditor()
{
	delete range;
	delete stops;
}

void GradientEditor::syncSpread(  const QGradient &g )
{
	switch( g.spread() )
	{
		case QGradient::PadSpread:
			ui.padSpreadButton->setChecked( true );
			break;

		case QGradient::RepeatSpread:
			ui.repeatSpreadButton->setChecked( true );
			break;

		default:
			ui.reflectSpreadButton->setChecked( true );
			break;
	}
}

void GradientEditor::onStopsChanged( const QGradientStops &s )
{
	if( !handleSignals ) return;
	emit setActive(getName());

	handleSignals = false;
	qreal rad = range->radius();
	QGradient g = range->gradient();
	g.setStops( s );
	range->setGradient( g );
	range->setRadius( rad );
	handleSignals = true;
	emit gradientChanged( g );
}

void GradientEditor::onLinearGradientSelected( bool checked )
{
	if( !handleSignals ) return;
	emit setActive(getName());

	handleSignals = false;
	qreal rad = range->radius();
	QGradient g = range->gradient();
	QGradient newGradient = QLinearGradient( range->startPoint(), range->endPoint() );
	newGradient.setStops( stops->stops() );
	newGradient.setSpread( g.spread() );
	range->setGradient( newGradient );
	range->setRadius( rad );
	syncSpread( newGradient );
	handleSignals = true;
	emit gradientChanged( newGradient );
}

void GradientEditor::onRadialGradientSelected( bool checked )
{
	if( !handleSignals ) return;
	emit setActive(getName());

	handleSignals = false;
	qreal rad = range->radius();
	QGradient g = range->gradient();
	QGradient newGradient = QRadialGradient(
		range->startPoint(), range->radius(), range->endPoint() );
	newGradient.setStops( stops->stops() );
	newGradient.setSpread( g.spread() );
	range->setGradient( newGradient );
	range->setRadius( rad );
	syncSpread( newGradient );
	handleSignals = true;
	emit gradientChanged( newGradient );
}

void GradientEditor::onConicalGradientSelected( bool checked )
{
	if( !handleSignals ) return;
	emit setActive(getName());

	handleSignals = false;
	qreal rad = range->radius();
	QGradient g = range->gradient();
	QGradient newGradient = QConicalGradient( range->startPoint(), range->angle() );
	newGradient.setStops( stops->stops() );
	range->setGradient( newGradient );
	newGradient.setSpread( g.spread() );
	range->setRadius( rad );
	syncSpread( newGradient );
	handleSignals = true;
	emit gradientChanged( newGradient );
}

void GradientEditor::onPadSpreadSelected( bool checked )
{
	if( !handleSignals ) return;
	emit setActive(getName());

	handleSignals = false;
	qreal rad = range->radius();
	QGradient g = range->gradient();
	g.setSpread( QGradient::PadSpread );
	range->setGradient( g );
	range->setRadius( rad );
	handleSignals = true;
	emit gradientChanged( g );
}

void GradientEditor::onRepeatSpreadSelected( bool checked )
{
	if( !handleSignals ) return;
	emit setActive(getName());

	handleSignals = false;
	qreal rad = range->radius();
	QGradient g = range->gradient();
	g.setSpread( QGradient::RepeatSpread );
	range->setGradient( g );
	range->setRadius( rad );
	handleSignals = true;
	emit gradientChanged( g );
}

void GradientEditor::onReflectSpreadSelected( bool checked )
{
	if( !handleSignals ) return;
	emit setActive(getName());

	handleSignals = false;
	qreal rad = range->radius();
	QGradient g = range->gradient();
	g.setSpread( QGradient::ReflectSpread );
	range->setGradient( g );
	range->setRadius( rad );
	handleSignals = true;
	emit gradientChanged( g );
}

void GradientEditor::onStartStopPointChanged( const QPointF &p )
{
	if( !handleSignals ) return;
	emit setActive(getName());

	emit gradientChanged( gradient() );
}


EXPORT_PLUGIN( GradientEditor, GradientEditorInterface )
