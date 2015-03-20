//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//ArkConstantBuffer11.h
//--------------------------------------------------------------------------------
#ifndef __ArkConstantBuffer11_h__
#define __ArkConstantBuffer11_h__
//--------------------------------------------------------------------------------
#include "ArkBuffer11.h"
//--------------------------------------------------------------------------------
namespace Arkeng
{
	struct ConstantBufferMapping
	{
		ArkRenderParameter11*		pParameter;
		unsigned int				offset;
		unsigned int				size;
		D3D_SHADER_VARIABLE_CLASS	varclass;
		unsigned int				elements;
		unsigned int				valueID;
	};

	class ArkConstantBuffer11 : public ArkBuffer11
	{
	public:
		ArkConstantBuffer11(Microsoft::WRL::ComPtr<ID3D11Buffer> pBuffer);
		virtual ~ArkConstantBuffer11();

		virtual ResourceType				GetType();

		void						AddMapping(ConstantBufferMapping& mapping);
		void						EmptyMappings();
		void						EvaluateMappings(PipelineManager* pPipeline,IParameterManager* pParamManager);
		bool						ContainsMapping(int index,const ConstantBufferMapping& mapping);

		void						SetAutoUpdate(bool enable);
		bool						GetAutoUpdate();

	protected:
		bool									m_bAutoUpdate;
		std::vector< ConstantBufferMapping >	m_Mappings;

		friend ArkRenderer11;
	};
};
//--------------------------------------------------------------------------------
#endif
