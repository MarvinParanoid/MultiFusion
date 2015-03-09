#ifndef __GFRAMES_PROPERTIES_H__
#define __GFRAMES_PROPERTIES_H__

#include <QDataStream>
#include <QVector>
#include <QBrush>
#include <QPen>
#include <QtAlgorithms>

class GProperties
{
	public:
		bool operator==( const GProperties &p ) const
		{
			if( p.visible != visible )
				return false;

			if( p.alpha != alpha )
				return false;

			if( p.points != points )
				return false;

			if( p.position != position )
				return false;

			if( p.blocked != blocked )
				return false;

			return true;
		}

		bool operator!=( const GProperties &p ) const
		{
			return ( !( (*this) == p ) );
		}

		GProperties &operator=( const GProperties &p )
		{
			visible = p.visible;
			alpha = p.alpha;
			points = p.points;
			position = p.position;
			blocked = p.blocked;
			isTransform = p.isTransform;
			return ( *this );
		}

		void save( QDataStream &stream ) const
		{
			stream << visible;
			stream << blocked;
			stream << alpha;
			stream << position;
			stream << points;
			stream << isTransform;
		}

		void load( QDataStream &stream )
		{
			points.clear();

			stream >> visible;
			stream >> blocked;
			stream >> alpha;
			stream >> position;
			stream >> points;
			stream >> isTransform;
		}

				GProperties()
				{
				}

				GProperties( const GProperties &p )
				{
					(*this) = p;
				}

		~GProperties()
		{
		}

		bool 			visible;
		int				alpha;
		int				position;
		bool			blocked;
		bool			isTransform;
		QPolygonF 		points;
};

template < class T > class GAbstractFramesProperties
{
	public:
		int countFrames() const
		{
			return _properties.size();
		}

		bool insert( const T &p, const int before )
		{
			int count = countFrames();

			if( ( before < 0 ) || ( before > count ) )
				return false;

			_properties.insert( before, p );
			return true;
		}

		void append(  const T &p )
		{
			_properties.append(p);
		}

		bool remove( const int frame )
		{
			int count = countFrames();

			if( ( frame < 0 ) || ( frame >= count ) )
				return false;

			_properties.remove( frame );
			return true;
		}

		const T &properties( const int frame ) const
		{
			return _properties.at( frame );
		}

		bool setProperties( const T &p, const int frame )
		{
			int count = countFrames();

			if( ( frame < 0 ) || ( frame >= count ) )
				return false;

			if( p == _properties[ frame ] )
				return true;

			_properties[ frame ] = p;
			return true;
		}

		void save( QDataStream &stream ) const
		{
			int count = _properties.size();

			stream << count;
			for( int i = 0; i < count; i++ )
				_properties[i].save( stream );
		}

		void load( QDataStream &stream )
		{
			_properties.clear();

			int count = 0;
			stream >> count;
			for( int i = 0; i < count; i++ )
			{
				T property;
				property.load( stream );
				_properties.append( property );
			}
		}

	protected:
		const T &operator[]( const int i ) const
		{
			return _properties[i];
		}

		T &operator[]( const int i )
		{
			return _properties[i];
		}

	private:
		QVector< T > _properties;

		friend class GVectorFigure;
};

typedef GAbstractFramesProperties< GProperties > GFramesProperties;

#endif /* __GFRAMES_PROPERTIES_H__ */
