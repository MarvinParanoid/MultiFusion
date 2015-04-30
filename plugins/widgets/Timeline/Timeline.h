#ifndef __TIMELINE_H__
#define __TIMELINE_H__

#include <QWidget>
#include <QDockWidget>
#include <QMainWindow>
#include <QMenuBar>
#include <QLayout>
#include <QPainter>
#include <QToolButton>
#include <QMessageBox>
#include <QMouseEvent>
#include <QLabel>
#include <QPixmap>
#include <QSpinBox>
#include <QBitmap>
#include <QTimer>

#include "./../../../pluginTool/Plugin.h"
#include "./../../../pluginTool/InterfacePlugin.h"
#include "./../../../interfaces/TimelineInterface.h"
#include "./../../../interfaces/MainWindowInterface.h"
#include "./../../../interfaces/PaintWidgetInterface.h"

#define STEP 10
#define DISPLACEMENT 10

class Frame: QFrame
{
	Q_OBJECT

	public:
		Frame(QWidget* parent):QFrame(parent)
		{
			setStyleSheet("background-color: rgba(0, 0, 255, 125);");
		}

		~Frame(){};

	protected:
		int position;

	friend class LinesDraw;
};

class NumberDraw: public QFrame
{
	Q_OBJECT

	public:

		NumberDraw(QWidget* parent, int cFr):QFrame(parent), countFrames(cFr)
		{
			setMinimumHeight(15);
			setMaximumHeight(15);
			setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Minimum);
		}

		void paintEvent(QPaintEvent *event)
		{
			QPainter* p = new QPainter(this);
			p->setBrush( QBrush(Qt::black) );
			p->setPen(QPen(Qt::black, 1, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin));

			int x;

			for(int i=0; i<countFrames; i++)
			{
				x = (STEP * i)+DISPLACEMENT;

				int j = 0;
				int dec = i;
				while (dec >= 10)
				{
					dec /=10;
					j++;
				}

				if(i%10 == 0 || i%10 == 5)
				{
					p->drawText(x - (j+1)*3, 10, QString::number(i));
				}
			}
		}

		virtual void setLength(int value)
		{
			countFrames = value;
			update();
		}
	private:
		int countFrames;

};

class LinesDraw: public QFrame
{
	Q_OBJECT

	signals:
		void frameAdded(int pos);
		void frameRemoved(int pos);
		void addEnabled(bool value);
		void removeEnabled(bool value);
		void posChanged(int value, bool played);
		void lengthChanged(int value);

	public:
		LinesDraw(QWidget* parent, int cP, int f):QFrame(parent), countPos(cP), fps(f), timer(this)
		{
			setMinimumHeight(40);
			setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Minimum);
			cursorPos = 0;
			maxFrame = 0;
			startPosX = -1;

			mainVLayout = new QVBoxLayout( this );
			mainVLayout->addStretch();
			mainVLayout->setSpacing(0);
			mainVLayout->setMargin(0);

			cursorF = new QFrame(this);
			cursorF->setStyleSheet("background-color: rgba(255, 93, 29, 125);");

			QPixmap pixmap(":/timeline/images/cursor.png");
			cursorL = new QLabel(this);
			cursorL->setPixmap(pixmap);
			cursorL->setMask(pixmap.mask());
            //connect(this, SIGNAL(frameAdded(int)), this->parentWidget(), SIGNAL(frameAdded(int)));
			connect( &timer, SIGNAL( timeout() ), this, SLOT( play() ) );
		}

		~LinesDraw(){};

		void paintEvent(QPaintEvent *event)
		{
			for(int i = 0; i < frames.size(); i++)
			{
				frames[i]->setGeometry(STEP*frames[i]->position + DISPLACEMENT - 3, 0, 6, this->height()-18);
			}
			cursorL->setGeometry(STEP*cursorPos + 2, this->height() - 18, 24, 20);
			cursorF->setGeometry(STEP*cursorPos + DISPLACEMENT - 3, 0, 6, this->height()-18);
			QPainter* p = new QPainter(this);
			p->setBrush( QBrush(Qt::black) );
			p->setPen(QPen(Qt::black, 1, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin));

			int x;
			int height;
			p->drawLine(0, 0, this->width(), 0);
			for(int i=0; i<countPos; i++)
			{
				x = (STEP * i)+DISPLACEMENT;
				switch(i%10)
				{
					case 0: height = DECADE_H;break;
					case 5: height = FIFTH_H;break;
					default: height = STANDART_H;break;
				}

				p->drawLine(x, 0, x, height);
			}
			p->drawLine(0, this->height()-1, this->width(), this->height()-1);
			p->drawLine(0, this->height()-18, this->width(), this->height()-18);
		}

		virtual void addFrame(int pos)
		{
			Frame* f = new Frame(this);
			f->setParent(this);
			f->position = pos;
			f->setStyleSheet("background-color: rgba(0, 0, 255, 125);");
			frames.append(f);
			layout()->addWidget(f);
			update();
		}

		virtual void clear()
		{
			for(int i=0; i<frames.size(); i++)
				delete frames[i];
			frames.clear();
		}

		virtual int currentPos()
		{
			return cursorPos;
		}

		virtual void setMaxFrame(int max)
		{
			maxFrame = max;
		}

		virtual int getMaxFrame()
		{
			return maxFrame;
		}

		virtual int getLenght()
		{
			return countPos;
		}

		int getFps() {
			return fps;
		}

	public slots:

		virtual void setLenght(int value)
		{
			if(maxFrame > value)
				QMessageBox::warning(this,"Warning:",tr("You can't set length smallest than maximum frame!"));
			else
			{
				countPos = value;
				emit lengthChanged(value);
				update();
			}
		}

		virtual void setFps(int value)
		{
			fps = value;
		}

		virtual void play()
		{
			if(cursorPos < maxFrame)
			{
				cursorPos++;
				this->setEnabled(false);
				emit posChanged(cursorPos, true);
				update();
				timer.start( 1000 / fps );
			}
			else
			{
				cursorPos=0;
				this->setEnabled(true);
				emit posChanged(cursorPos, false);
				update();
				timer.stop();
			}
		}

		virtual void stop()
		{
			cursorPos=0;
			this->setEnabled(true);
			emit posChanged(cursorPos, false);
			update();
			timer.stop();
		}

		virtual void pause()
		{
			update();
			timer.stop();
		}

		virtual void removeFrame()
		{
			int i = isHaveFrame();
			if(i != -1)
			{
				if(i == 0)
				{
					QMessageBox::warning(this,tr("You can't delete 0 frame!!!"), tr("You can't delete 0 frame!!!"));
					return;
				}
				delete frames[i];
				frames.remove(i);
			}
			emit addEnabled(true);
			emit removeEnabled(false);
			update();
			emit frameRemoved(cursorPos);
		}

		virtual void addFrame()
		{
			Frame* f = new Frame(this);
			f->setParent(this);
			f->position = cursorPos;
			f->setStyleSheet("background-color: rgba(0, 0, 255, 125);");
			frames.append(f);
			layout()->addWidget(f);
			if(maxFrame < cursorPos)
				maxFrame = cursorPos;

			emit addEnabled(false);
			emit removeEnabled(true);
			update();
			emit frameAdded(cursorPos);

		}

	protected:

		virtual void mouseMoveEvent( QMouseEvent *event )
		{
			if((event->buttons() & Qt::LeftButton) && startPosX!=-1)
			{
				int distance = (event->pos().x() - startPosX);
				int Pos = (event->pos().x() - DISPLACEMENT)/STEP;
				if(distance > 0)
				{
					if(Pos < countPos)
					{
						cursorPos = Pos;
						startPosX = event->pos().x();
					}
				}
				else
				{
					if(Pos >= 0)
					{
						cursorPos = Pos;
						startPosX = event->pos().x();
					}
				}
				update();
			}
		}

		int isHaveFrame()
		{
			for(int i=0; i< frames.size(); i++)
			{
				if(frames[i]->position == cursorPos)
					return i;
			}
			return -1;
		}

		virtual void mouseReleaseEvent( QMouseEvent *event )
		{
			int Pos = (event->pos().x() - DISPLACEMENT)/STEP;
			if(Pos >= 0 && Pos < countPos && Pos != countPos)
			{
				cursorPos = Pos;
				update();
				emit posChanged(cursorPos, false);
				if(isHaveFrame() != -1)
				{
					emit addEnabled(false);
					emit removeEnabled(true);
				}
				else
				{
					emit addEnabled(true);
					emit removeEnabled(false);
				}

				if(cursorPos == 0)
				{
					emit removeEnabled(false);
				}
			}

		}

		virtual void mousePressEvent( QMouseEvent *event )
		{
			QPoint p = event->pos();
			if(cursorL->geometry().contains(p))
				startPosX = p.x();
			else
				startPosX = -1;
		}

	private:
		int countPos;
		int cursorPos;
		int startPosX;
		int maxFrame;
		int fps;
		QTimer timer;
		QVBoxLayout *mainVLayout;
		QFrame* cursorF;
		QLabel* cursorL;
		QVector <Frame*> frames;
		const static int DECADE_H = 15;
		const static int FIFTH_H = 10;
		const static int STANDART_H = 5;

	friend class Timeline;

};

class Timeline:public QWidget, public TimelineInterface, public InterfacePlugin
{
	Q_OBJECT
	Q_INTERFACES( TimelineInterface )
	Q_INTERFACES( InterfacePlugin )

	signals:
		void frameChanged( int frame );
		void setActive(QString);

	public:
		virtual void createPlugin(QObject *parent, QString idParent,plugin::PluginsManager *manager)
		{
			mainWin = MAINWINDOW(parent);
			if(mainWin!=0)
			{
				painter = PAINTWIDGETINTERFACE(mainWin->getPaintWidget());
				timelineWindow = new QDockWidget(mainWin);
				timelineWindow->setWindowTitle( tr( "Timeline" ) );
				this->setParent( timelineWindow );
				timelineWindow->setWidget(this);
				mainWin->addDockWidget( Qt::BottomDockWidgetArea, timelineWindow );

				connect( painter, SIGNAL( frameChanged( qreal ) ), this, SLOT( onStateChanged() ) );
				connect( painter, SIGNAL( allLayersChanged() ), this, SLOT( onStateChanged( ) ) );
				connect( painter, SIGNAL( layerSelected() ), this, SLOT( onStateChanged( ) ) );
				connect(lines, SIGNAL( frameAdded(int) ), this, SLOT( onAddPressed(int) ) );
				connect(lines, SIGNAL( frameRemoved(int) ), this, SLOT( onRemovePressed(int) ) );
				connect(lines, SIGNAL( posChanged(int,bool) ), this, SLOT( onSetFrame(int,bool) ) );

				initTimeline();

				connect( this, SIGNAL( setActive( QString ) ), getBeep(), SIGNAL( setActive( QString ) ) );

				manager->addPlugins(this, "Timeline");
			}

		}

		virtual QString getName()const
		{
			return "Timeline";
		}

		virtual int getFps() {
			return lines->getFps();
		}

		virtual QFrame* addSeparator()
		{
			QFrame* verticalLine = new QFrame(this);
	        verticalLine->setFrameShape(QFrame::VLine);
	        verticalLine->setFrameShadow(QFrame::Raised);
	        verticalLine->setMaximumHeight(24);
	        return verticalLine;
		}

		Timeline( plugin::PluginsManager *manager )
		{
			int fps = 24;
			countPos = 101;
			QVBoxLayout *mainVLayout = new QVBoxLayout( this );
			QHBoxLayout *toolHLayout = new QHBoxLayout( this );
			QHBoxLayout *timeHLayout = new QHBoxLayout( this );
            QHBoxLayout *buttonHLayout = new QHBoxLayout( this );

            mainVLayout->addLayout(toolHLayout);
            mainVLayout->addLayout(timeHLayout);
            mainVLayout->addLayout(buttonHLayout);

			QScrollArea *scrollArea = new QScrollArea( this );
			scrollArea->setAlignment(Qt::AlignHCenter|Qt::AlignCenter);
			scrollArea->setWidgetResizable(true);
			scrollArea->setFrameShape(QFrame::Box);
			scrollArea->setFrameShadow(QFrame::Plain);
			scrollArea->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
			scrollArea->setMinimumHeight(75);
			timeHLayout->addWidget(scrollArea);


			fr = new QFrame(this);
			lines = new LinesDraw(this, countPos, fps);
			numbers = new NumberDraw(this, countPos);
			QVBoxLayout *VLayout = new QVBoxLayout( fr );

			numbers->setStyleSheet("background-color: qlineargradient(spread:reflect, x1:0.5, y1:0, x2:0.5, y2:0.5, "
					"stop:0 rgba(83, 79, 129, 255), stop:0.0734463 rgba(123, 120, 158, 255), stop:0.152542 rgba(146, 144, 176, 255), "
					"stop:0.420455 rgba(195, 193, 211, 255), "
					"stop:0.676136 rgba(220, 219, 230, 255), stop:0.920455 rgba(230, 229, 237, 255));");
			lines->setStyleSheet("background-color: rgb(228, 243, 220);");

			connect(lines, SIGNAL(addEnabled(bool)), this, SLOT(addEnabled(bool)));
			connect(lines, SIGNAL(removeEnabled(bool)), this, SLOT(removeEnabled(bool)));


			VLayout->addWidget(numbers);
			VLayout->addWidget(lines);
			VLayout->setMargin(0);
			VLayout->setSpacing(0);
			fr->setMinimumWidth( STEP*countPos + DISPLACEMENT*2 );
			fr->setMinimumHeight(40);

			scrollArea->setWidget( fr );
			mainVLayout->setContentsMargins(10,0,10,5);
			mainVLayout->setSpacing(4);
			toolHLayout->setMargin(0);

	        toolHLayout->addWidget(addSeparator());
	        toolHLayout->addSpacing(3);

			addFrame.setIcon( QIcon( ":/timeline/images/addFrame.png" ) );
			addFrame.setToolTip( tr( "Add frame" ) );
			addFrame.setAutoRaise(true);
			connect( &addFrame, SIGNAL( clicked() ), lines, SLOT( addFrame() ) );
			toolHLayout->addWidget(&addFrame);

			removeFrame.setIcon( QIcon( ":/timeline/images/removeFrame.png" ) );
			removeFrame.setToolTip( tr( "Remove frame" ) );
			removeFrame.setAutoRaise(true);
			connect( &removeFrame, SIGNAL( clicked() ), lines, SLOT( removeFrame() ) );
			toolHLayout->addWidget(&removeFrame);

			addFrame.setEnabled(false);
			removeFrame.setEnabled(true);

	        toolHLayout->addSpacing(3);
	        toolHLayout->addWidget(addSeparator());
	        toolHLayout->addSpacing(3);

			QIcon icon;
			icon.addPixmap(QPixmap(QString::fromUtf8(":/timeline/images/play.png")), QIcon::Normal, QIcon::Off);
			icon.addPixmap(QPixmap(QString::fromUtf8(":/timeline/images/play_check.png")), QIcon::Active, QIcon::On);
			play.setMinimumSize(24,24);
			play.setCheckable(true);
			play.setStyleSheet("border: 0;");
			play.setIconSize(play.minimumSize());
			play.setIcon(icon);
			play.setToolTip( tr( "Play" ) );
			play.setAutoRaise(true);
			play.setAutoExclusive(true);
			connect( &play, SIGNAL( clicked() ), lines, SLOT( play()) );
			toolHLayout->addWidget(&play);

			icon.addPixmap(QPixmap(QString::fromUtf8(":/timeline/images/stop.png")), QIcon::Normal, QIcon::Off);
			icon.addPixmap(QPixmap(QString::fromUtf8(":/timeline/images/stop_check.png")), QIcon::Active, QIcon::On);
			stop.setMinimumSize(24,24);
			stop.setCheckable(true);
			stop.setStyleSheet("border: 0;");
			stop.setIconSize(play.minimumSize());
			stop.setIcon( icon );
			stop.setToolTip( tr( "Stop" ) );
			stop.setAutoRaise(true);
			stop.setAutoExclusive(true);
			stop.setChecked(true);
			connect( &stop, SIGNAL( clicked() ), lines, SLOT( stop() ) );
			toolHLayout->addWidget(&stop);

		    icon.addPixmap(QPixmap(QString::fromUtf8(":/timeline/images/pause.png")), QIcon::Normal, QIcon::Off);
		    icon.addPixmap(QPixmap(QString::fromUtf8(":/timeline/images/pause_check.png")), QIcon::Active, QIcon::On);
			pause.setMinimumSize(24,24);
			pause.setCheckable(true);
			pause.setStyleSheet("border: 0;");
			pause.setIconSize(play.minimumSize());
			pause.setIcon(icon);
			pause.setToolTip( tr( "Pause" ) );
			pause.setAutoRaise(true);
			pause.setAutoExclusive(true);
			connect( &pause, SIGNAL( clicked() ), lines, SLOT( pause() ) );
			toolHLayout->addWidget(&pause);

	        toolHLayout->addSpacing(3);
	        toolHLayout->addWidget(addSeparator());
			toolHLayout->addStretch();

			QLabel* label = new QLabel(this);
			label->setText(tr("Length timeline:"));
			QLabel* label1 = new QLabel(this);
			label1->setText(tr("fps:"));

			countFrms = new QSpinBox(this);
			fpsWidget = new QSpinBox(this);
			countFrms->setRange(countPos, 2001);
			fpsWidget->setRange(1, 40);
			fpsWidget->setValue(fps);

			connect( fpsWidget, SIGNAL( valueChanged(int) ), lines, SLOT( setFps(int) ) );
			connect( fpsWidget, SIGNAL( valueChanged(int) ), this, SLOT ( changeFps() ) );

			connect( countFrms, SIGNAL( valueChanged(int) ), lines, SLOT( setLenght(int) ) );
			connect( lines, SIGNAL( lengthChanged(int) ), this, SLOT( onLengthChanged(int) ) );

			toolHLayout->addWidget(label1);
			toolHLayout->addWidget(fpsWidget);
			toolHLayout->addSpacing(10);
			toolHLayout->addWidget(label);
			toolHLayout->addWidget(countFrms);
		}

		virtual ~Timeline()
		{
		}

		virtual void initTimeline()
		{
			emit setActive(getName());

			lines->clear();
			int max = painter->maxFrame();
			lines->setMaxFrame(max);
			if(max > countPos)
			{
				countPos = max+1;
				lines->setLenght(countPos);
			}

			int countFrames = painter->countFrames();
			for(int i=0; i<countFrames; i++)
				lines->addFrame(painter->getPositionFrame(i));
			if(lines->isHaveFrame() != -1)
			{
				removeFrame.setEnabled(true);
				addFrame.setEnabled(false);
			}
			else
			{
				removeFrame.setEnabled(false);
				addFrame.setEnabled(true);
			}
		}

	private slots:
		void changeFps() {
			emit setActive(getName());
		}

		void addEnabled(bool value)
		{
			addFrame.setEnabled(value);
		}

		void removeEnabled(bool value)
		{
			removeFrame.setEnabled(value);
		}

		void onPlayPressed()
		{
			emit setActive(getName());
		}

		void onPausePressed()
		{
			emit setActive(getName());
		}

		void onStopPressed()
		{
			emit setActive(getName());
		}


		void onRemovePressed(int frame)
		{
			emit setActive(getName());
			painter->deleteFrame(frame);
		}

		void onSetFrame(int pos, bool played)
		{
			emit setActive(getName());
			if(pos == 0)
				stop.setChecked(true);
			painter->setFrame(pos, played);
		}

		void onLengthChanged(int value)
		{
			emit setActive(getName());
			countPos = value;
			fr->setMinimumWidth( STEP*countPos + DISPLACEMENT*2 );
			numbers->setLength(value);
		}

		void onAddPressed(int frame)
		{
			emit setActive(getName());
			painter->addFrame(frame);
		}

		void onStateChanged()
		{
			initTimeline();
		}
	private:

		MainWindowInterface* mainWin;
		PaintWidgetInterface* painter;
		QFrame* fr;
		QDockWidget *timelineWindow;
		QToolButton addFrame;
		QToolButton removeFrame;
		QToolButton play;
		QToolButton stop;
		QToolButton pause;
		QSpinBox* countFrms;
		QSpinBox* fpsWidget;
		int countPos;
		NumberDraw* numbers;
		LinesDraw* lines;
};

#endif /* __TIMELINE_H__ */
