//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//ArkCommandList11.h
//--------------------------------------------------------------------------------
#ifndef __TConfiguration_h__
#define __TConfiguration_h__
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
namespace Arkeng
{
	template <class T>
	class TConfiguration
	{
	public:
		TConfiguration();
		virtual ~TConfiguration();

		void SetConfiguration( const T& config );
		void ApplyConfiguration( );
		const T& GetConfiguration( );

	private:
		T m_CurrentConfig;
		T m_NextConfig;
	};

#include "TConfiguration.inl"
};
//--------------------------------------------------------------------------------
#endif // TConfiguration_h
