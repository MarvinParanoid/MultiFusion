#ifndef __G_UI_UTILS_H__
#define __G_UI_UTILS_H__

#include <QWidget>
#include <QDialog>
#include <QSlider>
#include <QLCDNumber>
#include <QDialogButtonBox>

#include "PaintWidget.h"

/**
*	@file guiutils.h В данном файле описаны несколько вспомогательных классов, реализующих некоторые
*			специфичные для GUI функции.
*/

/**	@example SimpleFrameView Простой пример использования класса FrameView.
*
*		Предположим, что у нас уже есть готовый обькт класса PaintWidget с именем <B>editor</B>.
*	И мы хотим (не так важно зачем) отобразить кадр с номером 3.56. Отобразить мы это хотим
*	в виде некоторого элемента управления в главном окне приложения. Обьект главного окна,
*	предположим, имеет имя MainWindow. Для этого мы должны написать следующее:
*
*	@code
*
*	#include "guiutils.h"
*
*	// ... Тут некоторый код вашей программы
*
*	...
*
*	FrameView view( editor, &MainWindow );
*
*	view.setFrame( 3.56 );
*	view.show();
*
*	// И тут, возможно, тоже.
*
*	...
*
*	@endcode
*
*		После этого будет создан новый элемент управления, который и будет показывать
*	всё время один и тот же необходимый кадр.
*
*	@warning Будте внимательны, обьект класса PaintWidget не должен быть уничтожен
*		раньше обьекта FrameView, отображающего из PaintWidget-а анимацию.
*/

/**
*		Этот класс реализует виджет, позволяющий отобразить любой из кадров анимации, которую
*	содержит в данный момент обьект класса PaintWidget. Причём, не обязательно это должен быть
*	основной кадр. Это может быть и любой промежуточный кадр.
*/
class FrameView:public QWidget
{
	Q_OBJECT

	public:

		/**
		*		Возвращает кадр, который в данный момент отображается (т.е. позиция в анимационном ролике).
		*	Т.к. кадр может быть любой, то тип возвращаемого значения - qreal.
		*
		*	@return отображаемый в данный момент кадр.
		*/
		virtual qreal frame() const
		{
			return _frame;
		}

		/**
		*	Устанавливает текущий показываемый кадр.
		*
		*	@param frame устанавливаемый для отображения кадр.
		*/
		virtual void setFrame( qreal frame )
		{
			_frame = frame;
			update();
		}

				/**
				*	Конструктор.
				*
				*	@param _paintWidget это тот самый PaintWidget, который содержит отображаемую
				*		обьктом данного класса анимацию.
				*
				*	@param parent родительский для данного виджет.
				*/
				FrameView( PaintWidget *_paintWidget, QWidget *parent = 0 ):
					QWidget( parent ), paintWidget( _paintWidget ), _frame( 0.0 )
				{
					setMinimumSize( QSize( 128, 128 ) );
				}

		/**
		*	Деструктор.
		*/
		virtual ~FrameView()
		{
		}

	protected:
		virtual void paintEvent( QPaintEvent *event )
		{
			QPainter p( this );

			if( paintWidget != 0 )
				paintWidget->paintFrameTo( p, rect(), _frame );
		}

	private:
		PaintWidget *paintWidget;
		qreal _frame;
};

/**
*	@example FrameSelectionDialogExample Простой пример использования класса FrameSelectionDialog.
*
*	@code
*
*	#include "guiutils.h"
*
*	// Некоторый ваш код
*
*	...
*
*	// А тут вам просто необходимо спросить пользователя о том, какой кадр он бы хотел...
*	// Предположим, удалить.
*
*	qreal selectedFrame = FrameSelectionDialog::selectFrame( editor, // editor - это обьект класса
*																	// PaintWidget, содержащий анимацию,
*																	// из которой мы удаляем кадр.
*					tr( "Пожалуйста, выберите кадр для удаления" ), // Заголовок окна, в котором пользователь
*																	// будет выбирать кадр.
*					true,	// Так как удалять можно только основные кадры, то так мы диалогу выбора
*							// кадров и говорим - разрешить пользователю выбирать только основные кадры.
*					&MainWindow ); // Окно нашего приложения. Не обязательно оно должно быть главным, и,
*								// если мы хотим сделать диалог не модальным, мы вообще не должны указывать
*								// этот параметр.
*
*	if( selectedFrame != -1 ) // Проверяем, не нажал ли пользоваель кнопку "отмена".
*	{
*		// И тут мы должны удалить кадр из анимации.
*		editor->setFrame( selectedFrame );
*		editor->deleteCurrentFrame();
*	}
*
*	// Вероятно, снова ваш код.
*
*	...
*
*	@endcode
*/

/**
*	Данный класс реализует диалог выбора кадра.
*
*/
class FrameSelectionDialog:public QDialog
{
	Q_OBJECT

	public:
		/**
		*		Единственный доступный публично метод данного класса. Он-то и занимается показом окна,
		*	в котором пользователь может выбрать некоторый кадр анимации.
		*
		*	@param paintWidget обькт класса PaintWidget, содержащий анимацию, кадр из которой мы вибираем.
		*	@param title заголовок диалогового окна, которое будет показано.
		*	@param _onlyIntactFrames если данный парметр установить в true, то тогда пользователь
		*				сможет выбирать только базовые кадры анимации. В противном случае, он
		*				сможет выбирать любые кадры. По умолчанию данный параметр принимает значение false,
		*				т.е. пользователь сможет выбирать любые кадры.
		*
		*	@param parent родительское окно для показываемого приложения. Если вы хотите показать
		*			диалог немодально, вы можете не указывать даный параметр.
		*
		*	@return выбранный пользователем кадр, или -1, если пользователь отменил ввод.
		*
		*/
		static qreal selectFrame( PaintWidget *paintWidget, const QString title,
				bool _onlyIntactFrames = false, QWidget *parent = 0 )
		{
			if( paintWidget == 0 )
				return -1;

			FrameSelectionDialog dialog( paintWidget, _onlyIntactFrames, parent );

			dialog.setFrame( paintWidget->frame() );
			dialog.setWindowTitle( title );
			if( dialog.exec() == Accepted )
				return dialog.frame();

			return -1;
		}

	private slots:
		void onFrameChanged( int value )
		{
			qreal frame = value;

			if( !onlyIntactFrames )
				frame /= 100.0;

			frameView.setFrame( frame );
			frameNumber.display( frame );
		}

	private:
		bool onlyIntactFrames;
		FrameView frameView;
		QSlider	position;
		QLCDNumber frameNumber;
		QDialogButtonBox buttons;

		virtual qreal frame() const
		{
			return frameView.frame();
		}

		virtual void setFrame( qreal frame )
		{
			if( onlyIntactFrames )
				frame = int( frame );

			frameView.setFrame( frame );
			frameNumber.display( frame );
			position.setValue( (int)frame );
		}

				FrameSelectionDialog( PaintWidget *paintWidget,
					bool _onlyIntactFrames = false, QWidget *parent = 0 ):QDialog( parent ),
						onlyIntactFrames( _onlyIntactFrames ),
						frameView( paintWidget, this ),
						position( Qt::Horizontal, this ), frameNumber( this ),
						buttons( QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
							Qt::Horizontal, this )
				{
					QVBoxLayout *l = new QVBoxLayout( this );
					l->addWidget( &frameView );

					QHBoxLayout *l2 = new QHBoxLayout;
					l->addLayout( l2 );
					l2->addWidget( &frameNumber );
					l2->addWidget( &position );

					l->setStretchFactor( &frameView, 1 );

					l->addWidget( &buttons );

					position.setMinimum( 0 );
					if( _onlyIntactFrames )
						position.setMaximum( paintWidget->countFrames() - 1 );
					else
						position.setMaximum( ( paintWidget->countFrames() - 1 ) * 100 );

					if( position.maximum() < 0 )
						position.setMaximum( 0 );

					connect( &buttons, SIGNAL( accepted() ), this, SLOT( accept() ) );
					connect( &buttons, SIGNAL( rejected() ), this, SLOT( reject() ) );
					connect( &position, SIGNAL( valueChanged ( int ) ),
						this, SLOT( onFrameChanged( int ) ) );

					resize( 350, 400 );
				}

		virtual ~FrameSelectionDialog()
		{
		}
};

#endif /* __G_UI_UTILS_H__ */
