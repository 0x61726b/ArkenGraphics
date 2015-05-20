//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//TriangleIndices.h
//--------------------------------------------------------------------------------
#ifndef __TriangleIndices_h__
#define __TriangleIndices_h__
//--------------------------------------------------------------------------------
namespace Arkeng
{
	class TriangleIndices
	{
	public:
		TriangleIndices();
		TriangleIndices( unsigned int P1, unsigned int P2, unsigned int P3 );
		~TriangleIndices();

		TriangleIndices& operator= ( const TriangleIndices& Triangle );

		// member access
		unsigned int P1( ) const;
		unsigned int& P1( );
		unsigned int P2( ) const;
		unsigned int& P2( );
		unsigned int P3( ) const;
		unsigned int& P3( );

		unsigned int MaterialID;

		void swapP1P2( );
		void swapP2P3( );
		void swapP3P1( );

	protected:
		unsigned int m_uiIndices[3];
	};
};
//--------------------------------------------------------------------------------
#endif // TriangleIndices_h
