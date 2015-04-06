//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//ArkCommandList11.h
//--------------------------------------------------------------------------------
using namespace Arkeng;
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
template <class T>
TConfiguration<T>::TConfiguration()
{
}
//--------------------------------------------------------------------------------
template <class T>
TConfiguration<T>::~TConfiguration()
{
}
//--------------------------------------------------------------------------------
template <class T>
void TConfiguration<T>::SetConfiguration(const T& config)
{
	m_NextConfig = config;
}
//--------------------------------------------------------------------------------
template <class T>
void TConfiguration<T>::ApplyConfiguration( )
{
	m_CurrentConfig = m_NextConfig;
}
//--------------------------------------------------------------------------------
template <class T>
const T& TConfiguration<T>::GetConfiguration( )
{
	return( m_CurrentConfig );
}
//--------------------------------------------------------------------------------
