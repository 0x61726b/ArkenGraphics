//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//ArkGeometryGenerator11.cpp
//--------------------------------------------------------------------------------
#include "Pch.h"
#include "ArkGeometryGenerator11.h"
#include "VertexElement11.h"
//--------------------------------------------------------------------------------
using namespace Arkeng;
//--------------------------------------------------------------------------------
ArkGeometryGenerator11::ArkGeometryGenerator11()
{
}
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
void ArkGeometryGenerator11::GenerateFullScreenQuad( GeometryPtr pGeometry )
{
	/*if ( pGeometry == NULL ) {
		EventManager::Get()->ProcessEvent( EvtErrorMessagePtr( new EvtErrorMessage( std::wstring( 
			L"Attempted to generate a full-screen quad on null geometry object!" ) ) ) );

		return;
	}*/

	VertexElement11* pClipPositions = new VertexElement11( 4, 4 );
	pClipPositions->m_SemanticName = "POSITION";
	pClipPositions->m_uiSemanticIndex = 0;
	pClipPositions->m_Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	pClipPositions->m_uiInputSlot = 0;
	pClipPositions->m_uiAlignedByteOffset = 0;
	pClipPositions->m_InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	pClipPositions->m_uiInstanceDataStepRate = 0;

	VertexElement11* pTexCoords = new VertexElement11( 2, 4 );
	pTexCoords->m_SemanticName = "TEXCOORDS";
	pTexCoords->m_uiSemanticIndex = 0;
	pTexCoords->m_Format = DXGI_FORMAT_R32G32_FLOAT;
	pTexCoords->m_uiInputSlot = 0;
	pTexCoords->m_uiAlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	pTexCoords->m_InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	pTexCoords->m_uiInstanceDataStepRate = 0;

	XMFLOAT4* pClipPos = pClipPositions->Get4f( 0 );
	XMFLOAT2* pTex = pTexCoords->Get2f( 0 );

	// Set the clip positions of each vertex in the near clipping plane
	pClipPos[0] = XMFLOAT4( -1.0f, 1.0f, 0.0f, 1.0f );		// Upper left
	pClipPos[1] = XMFLOAT4( -1.0f, -1.0f, 0.0f, 1.0f );		// Lower left
	pClipPos[2] = XMFLOAT4( 1.0f, 1.0f, 0.0f, 1.0f );		// Upper right
	pClipPos[3] = XMFLOAT4( 1.0f, -1.0f, 0.0f, 1.0f );		// Lower right

	pTex[0] = XMFLOAT2( 0.0f, 0.0f );
	pTex[1] = XMFLOAT2( 0.0f, 1.0f );
	pTex[2] = XMFLOAT2( 1.0f, 0.0f );
	pTex[3] = XMFLOAT2( 1.0f, 1.0f );

	TriangleIndices face;

	pGeometry->AddElement( pClipPositions );
	pGeometry->AddElement( pTexCoords );

	face = TriangleIndices( 0, 2, 1 );
	pGeometry->AddFace( face );

	face = TriangleIndices( 1, 2, 3 );
	pGeometry->AddFace( face );
}
//--------------------------------------------------------------------------------
void ArkGeometryGenerator11::GenerateTexturedPlane( GeometryPtr pGeometry, int SizeX, int SizeY )
{
	/*if ( pGeometry == NULL ) {
		EventManager::Get()->ProcessEvent( EvtErrorMessagePtr( new EvtErrorMessage( std::wstring( 
			L"Attempted to generate a textured plane on null geometry object!" ) ) ) );

		return;
	}*/

	VertexElement11* pPositions = new VertexElement11( 3, SizeX * SizeY );
	pPositions->m_SemanticName = "POSITION";
	pPositions->m_uiSemanticIndex = 0;
	pPositions->m_Format = DXGI_FORMAT_R32G32B32_FLOAT;
	pPositions->m_uiInputSlot = 0;
	pPositions->m_uiAlignedByteOffset = 0;
	pPositions->m_InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	pPositions->m_uiInstanceDataStepRate = 0;

	VertexElement11* pTexCoords = new VertexElement11( 2, SizeX * SizeY );
	pTexCoords->m_SemanticName = "TEXCOORDS";
	pTexCoords->m_uiSemanticIndex = 0;
	pTexCoords->m_Format = DXGI_FORMAT_R32G32_FLOAT;
	pTexCoords->m_uiInputSlot = 0;
	pTexCoords->m_uiAlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	pTexCoords->m_InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	pTexCoords->m_uiInstanceDataStepRate = 0;

	XMFLOAT3* pPos = pPositions->Get3f( 0 );
	XMFLOAT2* pTex = pTexCoords->Get2f( 0 );

	// Set the locations and texture coordinates first.
	for ( int y = 0; y < SizeY; y++ )
	{
		for ( int x = 0; x < SizeX; x++ )
		{
			pPos[y*SizeX+x] = XMFLOAT3( (float)x, 0.0f, (float)y );		// Upper left
			pTex[y*SizeX+x] = XMFLOAT2( (float)x, (float)y );
		}
	}

	// Generate the triangle faces of the plane.
	for ( int j = 0; j < SizeY-1; j++ )
	{
		for ( int i = 0; i < SizeX-1; i++ )
		{
			TriangleIndices f1;
			TriangleIndices f2;

			f1.P1() = j*SizeX + i;
			f1.P2() = (j*SizeX + i) + SizeX;
			f1.P3() = (j*SizeX + i) + 1;

			f2.P1() = (j*SizeX + i) + 1;
			f2.P2() = (j*SizeX + i) + SizeX;
			f2.P3() = (j*SizeX + i) + SizeX + 1;

			pGeometry->AddFace( f1 );
			pGeometry->AddFace( f2 );
		}
	}

	// Add the vertex elements to the geometry object.
	pGeometry->AddElement( pPositions );
	pGeometry->AddElement( pTexCoords );
}
//--------------------------------------------------------------------------------
void ArkGeometryGenerator11::GenerateAxisGeometry( GeometryPtr pGeometry )
{
	/*if ( pGeometry == NULL ) {
		EventManager::Get()->ProcessEvent( EvtErrorMessagePtr( new EvtErrorMessage( std::wstring( 
			L"Attempted to generate an axis on null geometry object!" ) ) ) );

		return;
	}*/

	// The axis geometry requires to have at positions plus colors.

	VertexElement11* pPositions = new VertexElement11( 3, 5*3 );
	pPositions->m_SemanticName = "POSITION";
	pPositions->m_uiSemanticIndex = 0;
	pPositions->m_Format = DXGI_FORMAT_R32G32B32_FLOAT;
	pPositions->m_uiInputSlot = 0;
	pPositions->m_uiAlignedByteOffset = 0;
	pPositions->m_InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	pPositions->m_uiInstanceDataStepRate = 0;

	VertexElement11* pColors = new VertexElement11( 4, 5*3 );
	pColors->m_SemanticName = "COLOR";
	pColors->m_uiSemanticIndex = 0;
	pColors->m_Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	pColors->m_uiInputSlot = 0;
	pColors->m_uiAlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	pColors->m_InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	pColors->m_uiInstanceDataStepRate = 0;

	XMFLOAT3* pPos = pPositions->Get3f( 0 );
	XMFLOAT4* pCols = pColors->Get4f( 0 );

	float fThickness = 0.05f;
	float fLength = 3.0f;

	XMFLOAT4 XBase = XMFLOAT4( 1.0f, 0.0f, 0.0f, 1.0f );
	XMFLOAT4 XEnd  = XMFLOAT4( 1.0f, 0.0f, 0.0f, 1.0f );
	XMFLOAT4 YBase = XMFLOAT4( 0.0f, 1.0f, 0.0f, 1.0f );
	XMFLOAT4 YEnd  = XMFLOAT4( 0.0f, 1.0f, 0.0f, 1.0f );
	XMFLOAT4 ZBase = XMFLOAT4( 0.0f, 0.0f, 1.0f, 1.0f );
	XMFLOAT4 ZEnd  = XMFLOAT4( 0.0f, 0.0f, 1.0f, 1.0f );


	// Create the X-Axis
	pPos[0] = XMFLOAT3( 0.0f, fThickness, fThickness );
	pCols[0] = XBase;
	pPos[1] = XMFLOAT3( 0.0f, -fThickness, fThickness );
	pCols[1] = XBase;
	pPos[2] = XMFLOAT3( 0.0f, -fThickness, -fThickness );
	pCols[2] = XBase;
	pPos[3] = XMFLOAT3( 0.0f, fThickness, -fThickness );
	pCols[3] = XBase;
	pPos[4] = XMFLOAT3( fLength, 0.0f, 0.0f );
	pCols[4] = XEnd;

	// Next is the Y-Axis.
	pPos[5] = XMFLOAT3( fThickness, 0.0f, fThickness );
	pCols[5] = YBase;
	pPos[6] = XMFLOAT3( -fThickness, 0.0f, fThickness );
	pCols[6] = YBase;
	pPos[7] = XMFLOAT3( -fThickness, 0.0f, -fThickness );
	pCols[7] = YBase;
	pPos[8] = XMFLOAT3( fThickness, 0.0f, -fThickness );
	pCols[8] = YBase;
	pPos[9] = XMFLOAT3( 0.0f, fLength, 0.0f );
	pCols[9] = YEnd;

	// Next is the Z-Axis.
	pPos[10] = XMFLOAT3( fThickness, fThickness, 0.0f );
	pCols[10] = ZBase;
	pPos[11] = XMFLOAT3( -fThickness, fThickness, 0.0f );
	pCols[11] = ZBase;
	pPos[12] = XMFLOAT3( -fThickness, -fThickness, 0.0f );
	pCols[12] = ZBase;
	pPos[13] = XMFLOAT3( fThickness, -fThickness, 0.0f );
	pCols[13] = ZBase;
	pPos[14] = XMFLOAT3( 0.0f, 0.0f, fLength );
	pCols[14] = ZEnd;

	// Generate and add each line segment.
	TriangleIndices tri;
		
	tri.P1() = 0;
	tri.P2() = 1;
	tri.P3() = 4;
	pGeometry->AddFace( tri );

	tri.P1() = 1;
	tri.P2() = 2;
	tri.P3() = 4;
	pGeometry->AddFace( tri );

	tri.P1() = 2;
	tri.P2() = 3;
	tri.P3() = 4;
	pGeometry->AddFace( tri );

	tri.P1() = 3;
	tri.P2() = 0;
	tri.P3() = 4;
	pGeometry->AddFace( tri );


	tri.P1() = 5;
	tri.P2() = 6;
	tri.P3() = 9;
	pGeometry->AddFace( tri );

	tri.P1() = 6;
	tri.P2() = 7;
	tri.P3() = 9;
	pGeometry->AddFace( tri );

	tri.P1() = 7;
	tri.P2() = 8;
	tri.P3() = 9;
	pGeometry->AddFace( tri );

	tri.P1() = 8;
	tri.P2() = 5;
	tri.P3() = 9;
	pGeometry->AddFace( tri );


	tri.P1() = 10;
	tri.P2() = 11;
	tri.P3() = 14;
	pGeometry->AddFace( tri );

	tri.P1() = 11;
	tri.P2() = 12;
	tri.P3() = 14;
	pGeometry->AddFace( tri );

	tri.P1() = 12;
	tri.P2() = 13;
	tri.P3() = 14;
	pGeometry->AddFace( tri );

	tri.P1() = 13;
	tri.P2() = 10;
	tri.P3() = 14;
	pGeometry->AddFace( tri );

	// Add the vertex elements to the geometry object.
	pGeometry->AddElement( pPositions );
	pGeometry->AddElement( pColors );
}
//--------------------------------------------------------------------------------
void ArkGeometryGenerator11::GenerateSphere( GeometryPtr pGeometry, unsigned int URes, 
                                            unsigned int VRes, float Radius )
{
    _ASSERT( VRes >= 3 );
    _ASSERT( URes >= 4 );
    _ASSERT( Radius > 0.0f );

	/*if ( pGeometry == NULL ) {
		EventManager::Get()->ProcessEvent( EvtErrorMessagePtr( new EvtErrorMessage( std::wstring( 
			L"Attempted to generate a sphere on null geometry object!" ) ) ) );

		return;
	}*/


    const unsigned int NumVertexRings = VRes - 2;
    const unsigned int NumVerts = NumVertexRings * URes + 2;
    const unsigned int NumTriangleRings = VRes - 1;
    const unsigned int NumTriangles = ( NumTriangleRings + 1 ) * URes * 2;
    const unsigned int NumIndices = NumTriangles * 3;  

    VertexElement11* pPositions = new VertexElement11( 3, NumVerts );
    pPositions->m_SemanticName = "POSITION";
    pPositions->m_uiSemanticIndex = 0;
    pPositions->m_Format = DXGI_FORMAT_R32G32B32_FLOAT;
    pPositions->m_uiInputSlot = 0;
    pPositions->m_uiAlignedByteOffset = 0;
    pPositions->m_InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
    pPositions->m_uiInstanceDataStepRate = 0;

    // Calculate all of the vertex positions
    XMFLOAT3* pVerts = pPositions->Get3f( 0 );
    int currVert = 0;

    // First vertex will be at the top pole
    pVerts[currVert++] = XMFLOAT3( 0.0f, Radius, 0.0f );

    // Add in the vertical rings of vertices
    for ( unsigned int v = 1; v <= NumVertexRings; ++v )
    {
        for ( unsigned int u = 0; u < URes; ++u )
        {
            float uAngle = u / static_cast<float>( URes ) * 3.14159f * 2.0f;
            float vAngle = v / static_cast<float>( VRes - 1 ) * 3.14159f;

            float x = sinf( vAngle ) * cosf( uAngle ) * Radius;
            float y = cosf( vAngle ) * Radius;
            float z = -sinf( vAngle ) * sinf( uAngle ) * Radius;
            pVerts[currVert++] = XMFLOAT3( x, y, z );
        }
    }

    // Last vertex is the bottom pole
    pVerts[currVert++] = XMFLOAT3( 0.0f, -Radius, 0.0f );
    _ASSERT( currVert == NumVerts );

    pGeometry->AddElement( pPositions );

    // Now we'll add the triangles
    TriangleIndices face;

    // Top ring first
    for ( unsigned int u = 0; u < URes; ++u )
    {
        const unsigned int currentU = u;
        const unsigned int nextU = ( u + 1 ) % URes;
        face = TriangleIndices( 0, u + 1, nextU + 1 );
        pGeometry->AddFace( face );
    }

    // Now the middle rings
    for ( unsigned int v = 1; v < VRes - 2; ++v )
    {
        const unsigned int top = 1 + ( ( v - 1 ) * URes );
        const unsigned int bottom = top + URes;
        for ( unsigned int u = 0; u < URes; ++u )
        {                
            const unsigned int currentU = u;
            const unsigned int nextU = ( u + 1 ) % URes;
            const unsigned int currTop = top + currentU;
            const unsigned int nextTop = top + nextU;
            const unsigned int currBottom = bottom + currentU;
            const unsigned int nextBottom = bottom + nextU;

			_ASSERT( currTop <= NumVerts );
			_ASSERT( currBottom <= NumVerts );
			_ASSERT( nextBottom <= NumVerts );
			_ASSERT( nextTop <= NumVerts );


            face = TriangleIndices( currTop, currBottom, nextBottom );
            pGeometry->AddFace( face );

            face = TriangleIndices( nextBottom, nextTop, currTop);
            pGeometry->AddFace( face );
        }
    }

    // Now the bottom ring
    const unsigned int top = 1 + ( ( NumVertexRings - 1 ) * URes );
    const unsigned int bottom = NumVerts - 1;
    for ( unsigned int u = 0; u < URes; ++u )
    {
        const unsigned int currentU = u;
        const unsigned int nextU = ( u + 1 ) % URes;
        const unsigned int currTop = top + currentU;
        const unsigned int nextTop = top + nextU;

		_ASSERT( currTop <= NumVerts );
		_ASSERT( bottom <= NumVerts );
		_ASSERT( nextTop <= NumVerts );

        face = TriangleIndices( currTop, bottom, nextTop );
        pGeometry->AddFace( face );
    }
}
//--------------------------------------------------------------------------------
void ArkGeometryGenerator11::GenerateCone( GeometryPtr pGeometry, unsigned int URes, 
                                        unsigned int VRes, float Radius, float Height )
{
    _ASSERT( VRes >= 1 );
    _ASSERT( URes >= 4 );
    _ASSERT( Radius > 0.0f );
    _ASSERT( Height > 0.0f );

	/*if ( pGeometry == NULL ) {
		EventManager::Get()->ProcessEvent( EvtErrorMessagePtr( new EvtErrorMessage( std::wstring( 
			L"Attempted to generate a cone on null geometry object!" ) ) ) );

		return;
	}*/

    const unsigned int NumVertexRings = VRes;
    const unsigned int NumVerts = NumVertexRings * URes + 2;
    const unsigned int NumTriangleRings = VRes - 1;
    const unsigned int NumTriangles = ( ( NumTriangleRings + 1 )* URes * 2 );
    const unsigned int NumIndices = NumTriangles * 3;

    VertexElement11* pPositions = new VertexElement11( 3, NumVerts );
    pPositions->m_SemanticName = "POSITION";
    pPositions->m_uiSemanticIndex = 0;
    pPositions->m_Format = DXGI_FORMAT_R32G32B32_FLOAT;
    pPositions->m_uiInputSlot = 0;
    pPositions->m_uiAlignedByteOffset = 0;
    pPositions->m_InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
    pPositions->m_uiInstanceDataStepRate = 0;

    // Calculate all of the vertex positions
    XMFLOAT3* pVerts = pPositions->Get3f( 0 );
    int currVert = 0;

    // First vertex will be at the top pole
    pVerts[currVert++] = XMFLOAT3( 0.0f, 0.0f, 0.0f );

    // Add in the rings of vertices
    for ( unsigned int v = 1; v <= NumVertexRings; ++v )
    {
        for ( unsigned int u = 0; u < URes; ++u )
        {
            float uAngle = u / static_cast<float>( URes ) * 3.14159f * 2.0f;                

            float heightScale = v / static_cast<float>( NumVertexRings );
            float x = cosf( uAngle ) * Radius * heightScale;
            float y = sinf( uAngle ) * Radius * heightScale;
            float z = heightScale * Height;
            pVerts[currVert++] = XMFLOAT3( x, y, z );
        }
    }

    // First vertex will be the center of the circle
    pVerts[currVert++] = XMFLOAT3( 0.0f, 0.0f, Height );
        
    _ASSERT( currVert == NumVerts );
    pGeometry->AddElement( pPositions );

    // Now we'll add the triangles
    TriangleIndices face;

    // Top ring first
    for ( unsigned int u = 0; u < URes; ++u )
    {
        const unsigned int currentU = u;
        const unsigned int nextU = ( u + 1 ) % URes;
        face = TriangleIndices( 0, nextU + 1, u + 1 );
        pGeometry->AddFace( face );
    }

    // Now the other rings
    for ( unsigned int v = 1; v < VRes; ++v )
    {
        const unsigned int top = 1 + ( ( v - 1 ) * URes );
        const unsigned int bottom = top + URes;
        for ( unsigned int u = 0; u < URes; ++u )
        {                
            const unsigned int currentU = u;
            const unsigned int nextU = ( u + 1 ) % URes;
            const unsigned int currTop = top + currentU;
            const unsigned int nextTop = top + nextU;
            const unsigned int currBottom = bottom + currentU;
            const unsigned int nextBottom = bottom + nextU;

            face = TriangleIndices( currTop, nextTop, nextBottom );
            pGeometry->AddFace( face );

            face = TriangleIndices( nextBottom, currBottom, currTop);
            pGeometry->AddFace( face );
        }
    }

    // And now the bottom
    const unsigned int top = 1 + ( ( VRes - 1 ) * URes );
    const unsigned int center = NumVerts - 1;
    for ( unsigned int u = 0; u < URes; ++u )
    {
        const unsigned int currentU = u;
        const unsigned int nextU = ( u + 1 ) % URes;
        const unsigned int currTop = top + currentU;
        const unsigned int nextTop = top + nextU;

        face = TriangleIndices( nextTop, center, currTop );
        pGeometry->AddFace( face );
    }
}
//--------------------------------------------------------------------------------