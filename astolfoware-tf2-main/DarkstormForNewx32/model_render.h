#pragma once
#include "SDKClasses.h"
#include "modelinfo.h"
class IVModelRender
{
public:
	virtual int	DrawModel(int flags,
		DWORD* pRenderable,
		DWORD instance,
		int entity_index,
		const DWORD* model,
		Vector const& origin,
		Vector const& angles,
		int skin,
		int body,
		int hitboxset,
		const matrix3x4* modelToWorld = NULL,
		const matrix3x4* pLightingOffset = NULL) = 0;

	virtual void ForcedMaterialOverride(IMaterial* newMaterial, OverrideType_t nOverrideType = OVERRIDE_NORMAL) = 0;
	virtual void SetViewTarget(const DWORD* pStudioHdr, int nBodyIndex, const Vector& target) = 0;
	virtual DWORD CreateInstance(DWORD* pRenderable, DWORD* pCache = NULL) = 0;
	virtual void DestroyInstance(DWORD handle) = 0;
	virtual void SetStaticLighting(DWORD handle, DWORD* pHandle) = 0;
	virtual DWORD GetStaticLighting(DWORD handle) = 0;
	virtual bool ChangeInstance(DWORD handle, DWORD* pRenderable) = 0;
	virtual void AddDecal(DWORD handle, Ray_t const& ray, Vector const& decalUp, int decalIndex, int body, bool noPokeThru, int maxLODToDecal) = 0;
	virtual void RemoveAllDecals(DWORD handle) = 0;
	virtual void RemoveAllDecalsFromAllModels() = 0;
	virtual matrix3x4* DrawModelShadowSetup(DWORD* pRenderable, int body, int skin, DWORD* pInfo, matrix3x4* pCustomBoneToWorld = NULL) = 0;
	virtual void DrawModelShadow(DWORD* pRenderable, const DWORD info, matrix3x4* pCustomBoneToWorld = NULL) = 0;
	virtual bool RecomputeStaticLighting(DWORD handle) = 0;
	virtual void ReleaseAllStaticPropColorData(void) = 0;
	virtual void RestoreAllStaticPropColorData(void) = 0;
	virtual int	DrawModelEx(DWORD pInfo) = 0;
	virtual int	DrawModelExStaticProp(DWORD pInfo) = 0;
	virtual bool DrawModelSetup(ModelRenderInfo_t& pInfo, DrawModelState_t* pState, matrix3x4* pCustomBoneToWorld, matrix3x4** ppBoneToWorldOut) = 0;
	virtual void DrawModelExecute(const DrawModelState_t& state, const ModelRenderInfo_t& pInfo, matrix3x4* pCustomBoneToWorld = NULL) = 0;
	virtual void SetupLighting(const Vector& vecCenter) = 0;
	virtual int DrawStaticPropArrayFast(DWORD* pProps, int count, bool bShadowDepth) = 0;
	virtual void SuppressEngineLighting(bool bSuppress) = 0;
	virtual void SetupColorMeshes(int nTotalVerts) = 0;
	virtual void AddColoredDecal(DWORD handle, Ray_t const& ray, Vector const& decalUp, int decalIndex, int body, Color_t cColor, bool noPokeThru, int maxLODToDecal) = 0;
	virtual void GetMaterialOverride(IMaterial** ppOutForcedMaterial, DWORD* pOutOverrideType) = 0;
};


